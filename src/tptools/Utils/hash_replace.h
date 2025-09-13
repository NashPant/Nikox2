#pragma once

#include <functional>
// std and 3rd party includes
#include <concepts>
#include <functional>
#include <ranges>
#include "variant_utils.h"

namespace TP {
    template <typename T>
    void hash_combine(std::size_t& seed, const T& value) noexcept {
        std::hash<T> hasher;
        seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }



    template <typename... Args> size_t hash(const std::variant<Args...>& variant)
    {
        return visit(variant, [](const auto& v) { return hash(v); });
    }

    template <std::ranges::range V> size_t hash(const V& v)
    {
        size_t h = 0;
        for (const auto& e : v)
        {
            h = hash(h, e);
        }
        return h;
    }
    template<typename V> size_t hash(V&& v)
    {
        return std::hash<std::remove_cvref_t<V>>()(std::forward<V>(v));
    }

    template <typename V0, typename V1, typename... V>
    size_t hash(V0 v0, V1 v1, V&&... v)
    {
        struct       HashCombineWrapper
        {
            size_t hash_v{};
            HashCombineWrapper operator+(HashCombineWrapper w) const
            {
                size_t seed = hash_v;
                // boost hash_combine
                seed ^= w.hash_v + 0x9e3779b9 + (seed << 6U) + (seed >> 2U);
                return HashCombineWrapper{ seed };
            }
        };

        const HashCombineWrapper v0v1 =
            HashCombineWrapper{ hash(std::forward<V0>(v0)) } +
            HashCombineWrapper{ hash(std::forward<V1>(v1)) };

        const auto combined = (v0v1 + ... + HashCombineWrapper{ hash(std::forward<V>(v)) });
        return combined.hash_v;
        // namespace utils
    }

}