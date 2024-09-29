#ifndef COW_STRING_H
#define COW_STRING_H

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cassert>
#include <string>
#include <memory>
#include <iostream>
#include <algorithm>

template<typename CharT, typename Traits = std::char_traits<CharT>>
class BasicCowString;

using CowString = BasicCowString<char>;
using WCowString = BasicCowString<wchar_t>;

template<typename CharT>
struct StringBuffer {
    std::size_t length {0};
    std::size_t capacity {0};
    std::size_t useCount {0};

    std::unique_ptr<CharT []> data;
};

/// Non-thread-safe copy-on-write string
template<typename CharT, typename Traits>
class BasicCowString {
public:    
    BasicCowString(const CharT *rawString)
    {
        if (rawString == nullptr) {
            return;
        }

        std::size_t length = Traits::length(rawString);
        std::size_t capacity = 1 + std::max(length * LENGTH_2_CAPACITY_MULTIPLIER, length + 1);

        auto bufferData = std::make_unique<CharT []>(capacity);
        std::memcpy(bufferData.get(), rawString, (length + 1) * sizeof(CharT));

        buffer_ = new StringBuffer<CharT>;
        buffer_->length = length;
        buffer_->capacity = capacity;
        buffer_->useCount = 1;
        buffer_->data = std::move(bufferData);
    }

    BasicCowString(const BasicCowString &rhs) :
        buffer_(rhs.buffer_)
    {
        buffer_->useCount++;
    }
  
    BasicCowString(BasicCowString &&rhs) :
        buffer_(rhs.buffer_)
    {
        rhs.buffer_ = nullptr;
    }

    ~BasicCowString()
    {
        DeleteBufferIfNeeded(buffer_);
    }

    BasicCowString &operator=(const BasicCowString &rhs)
    {
        if (&rhs == this) {
            return *this;
        }

        DeleteBufferIfNeeded(buffer_);

        buffer_ = rhs.buffer_;
        buffer_->useCount++;

        return *this;
    }

    BasicCowString &operator=(BasicCowString &&rhs)
    {
        if (&rhs == this) {
            return *this;
        }

        DeleteBufferIfNeeded(buffer_);

        buffer_ = rhs.buffer_;
        rhs.buffer_ = nullptr;

        return *this;
    }

    CharT &operator[](std::size_t idx)
    {
        if (buffer_->useCount > 1) {
            *this = Clone();
        }
        return buffer_->data[idx];
    }

    const CharT *Data() const
    {
        return buffer_->data.get();
    }

    std::size_t Length() const
    {
        return buffer_->length;
    }

private:
    BasicCowString() = default;

    void DeleteBufferIfNeeded(StringBuffer<CharT> *buffer) const
    {
        if (buffer == nullptr) {
            return;
        }

        buffer->useCount--;
        if (buffer->useCount == 0) {
            delete buffer_;       
        }
    }

    BasicCowString Clone() const
    {
        BasicCowString clone;

        clone.buffer_ = new StringBuffer<CharT>;
        clone.buffer_->length = buffer_->length;
        clone.buffer_->capacity = buffer_->capacity;
        clone.buffer_->useCount = 1;

        auto cloneData = std::make_unique<CharT []>(buffer_->capacity);
        std::memcpy(cloneData.get(), buffer_->data.get(), (buffer_->length + 1) * sizeof(CharT));
        clone.buffer_->data = std::move(cloneData);

        return clone;
    }

private:
    static constexpr std::size_t LENGTH_2_CAPACITY_MULTIPLIER = 1.2;

    StringBuffer<CharT> *buffer_ {nullptr};
};

#endif  // COW_STRING_H
