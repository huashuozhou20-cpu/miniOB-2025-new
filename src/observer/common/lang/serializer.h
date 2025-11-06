#pragma once

#include <vector>
#include <cstring>
#include <cstdint>
#include "common/lang/span.h"

namespace common {

class Serializer {
public:
    using BufferType = std::vector<char>;

    Serializer() = default;
    ~Serializer() = default;

    int write_int32(int32_t value) {
        const char* data = reinterpret_cast<const char*>(&value);
        buffer_.insert(buffer_.end(), data, data + sizeof(value));
        return 0;
    }

    int write(span<const char> data) {
        buffer_.insert(buffer_.end(), data.data(), data.data() + data.size());
        return 0;
    }

    BufferType& data() {
        return buffer_;
    }

    const BufferType& data() const {
        return buffer_;
    }

private:
    BufferType buffer_;
};

class Deserializer {
public:
    Deserializer(const char* data, size_t size) 
        : data_(data), size_(size), pos_(0) {}

    int read_int32(int32_t& value) {
        if (pos_ + sizeof(int32_t) > size_) {
            return -1;
        }
        std::memcpy(&value, data_ + pos_, sizeof(int32_t));
        pos_ += sizeof(int32_t);
        return 0;
    }

    int read(span<char> buffer) {
        size_t bytes_to_read = buffer.size();
        if (pos_ + bytes_to_read > size_) {
            bytes_to_read = size_ - pos_;
        }
        if (bytes_to_read > 0) {
            std::memcpy(buffer.data(), data_ + pos_, bytes_to_read);
            pos_ += bytes_to_read;
        }
        return bytes_to_read == buffer.size() ? 0 : -1;
    }

    size_t remain() const {
        return size_ > pos_ ? size_ - pos_ : 0;
    }

private:
    const char* data_;
    size_t size_;
    size_t pos_;
};

} // namespace common

