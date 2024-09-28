#ifndef COW_STRING_H
#define COW_STRING_H

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cassert>
#include <string>

template<typename CharT,
         typename Traits = std::char_traits<CharT>>
class BasicCowString;

using CowString = BasicCowString<char>;
using WcowString = BasicCowString<wchar_t>;

template<typename CharT>
struct StringBuffer {
    std::size_t size {0};
    std::size_t capacity {0};
    std::size_t useCount {0};

    CharT* data {nullptr};
};

/// Non-thread-safe copy-on-write string
template<typename CharT,
         typename Traits>
class BasicCowString {
public:
    BasicCowString() = default;
    
    // data must be null-terminated string
    BasicCowString(const char *data)
    {
        if (data == nullptr) {
            return;
        }

        buffer_ = new StringBuffer<CharT>;
        buffer_->size = std::strlen(data);

        std::memcpy(buffer_->data, data, (buffer_->size) * sizeof(char));
        buffer_->useCount = 1;
    }

    BasicCowString(const BasicCowString &rhs) :
        buffer_(rhs.buffer_)
    {
        buffer_->useCount++;
    }
  
    BasicCowString(BasicCowString &&rhs) noexcept :
        buffer_(rhs.buffer_)
    {
        rhs.buffer_ = nullptr;
    }

    ~BasicCowString()
    {
        DeleteBuffer(buffer_);
    }

    BasicCowString &operator=(const BasicCowString &rhs)
    {
        if (&rhs != this) {
            DeleteBuffer(buffer_);

            buffer_ = rhs.buffer_;
            buffer_->useCount++;
        }

        return *this;
    }

    BasicCowString &operator=(BasicCowString &&rhs) noexcept
    {
        if (&rhs == this) {
            DeleteBuffer(buffer_);

            buffer_ = rhs.buffer_;
            rhs.buffer_ = nullptr;
        }

        return *this;
    }

    const uint8_t *Data() const
    {
        return buffer_->data;
    }

    // uint8_t &operator[](std::size_t idx)
    // {
    //     if (buffer_->useCount_ > 1) {
    //         *this = 
    //     }
    // }

private:
    void DeleteBuffer(StringBuffer<CharT> *buffer)
    {
        if (buffer->useCount == 0) {
            delete buffer_;
        } else {
            buffer->useCount--;
        }
    }

private:
    StringBuffer<CharT> *buffer_ {nullptr};
};

#endif  // COW_STRING_H
