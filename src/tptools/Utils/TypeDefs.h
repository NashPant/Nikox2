#pragma once

#include "Operators.h"

#include <ostream>
#include <type_traits>

/**
* @brief Macro used to implement a strong typedef. Strong typedef guarantees that two types are distinguished even though the
*        share the same underlying implementation. Typedef does not create a new type.  TYPEDEF(T, D) creates a new type
*        named D that operates as a type T.
*/
#define TYPEDEF(T, D)																					   \
struct D final : TP::operators::totally_ordered<D>                                                       \
{                                                                                                          \
    D(const T& t) noexcept(std::is_nothrow_copy_constructible<T>()) : t_(t) {}                             \
    D() noexcept(std::is_nothrow_default_constructible<T>()) = default;                                    \
    D& operator=(const T& rhs) noexcept(std::is_nothrow_copy_assignable<T>()) {t_ = rhs; return *this;}    \
    constexpr operator const T&() const {return t_;}                                                       \
    constexpr const T& underlying() const {return t_;}                                                     \
	                                                                                                       \
    [[nodiscard]] friend bool operator==( const D& x, const D& y) noexcept { return x.t_==y.t_; }          \
    [[nodiscard]] friend bool operator<( const D& x, const D& y) noexcept { return x.t_<y.t_; }            \
	                                                                                                       \
    template <typename Char, typename Traits> friend std::basic_ostream<Char, Traits>&                     \
      operator <<(std::basic_ostream<Char, Traits>& stream, const D& val) {  return stream << val.t_; }    \
private:                                                                                                   \
    T t_;                                                                                                  \
};

#define TYPEDEF_CUSTOM_CSTR(T, D, F)																	   \
struct D final : TP::operators::totally_ordered<D>                                                       \
{                                                                                                          \
    D(const T& t) : t_(F(t)) {};																	       \
    D() noexcept(std::is_nothrow_default_constructible<T>()) = default;                                    \
	D& operator=(const T& rhs) noexcept(std::is_nothrow_copy_assignable<T>()) {t_ = rhs; return *this;}    \
    constexpr operator const T&() const {return t_;}                                                       \
    constexpr const T& underlying() const {return t_;}                                                     \
	                                                                                                       \
    [[nodiscard]] friend bool operator==( const D& x, const D& y) noexcept { return x.t_==y.t_; }          \
    [[nodiscard]] friend bool operator<( const D& x, const D& y) noexcept { return x.t_<y.t_; }            \
	                                                                                                       \
    template <typename Char, typename Traits> friend std::basic_ostream<Char, Traits>&                     \
      operator <<(std::basic_ostream<Char, Traits>& stream, const D& val) {  return stream << val.t_; }    \
private:                                                                                                   \
    T t_;                                                                                                  \
};
