#pragma once

#include <functional>

namespace TP {
    template <typename T>
    void hash_combine(std::size_t& seed, const T& value) noexcept {
        std::hash<T> hasher;
        seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
}