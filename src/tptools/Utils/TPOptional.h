#pragma once
#include "Operators.h"

#include <functional>
#include <optional>
#include <type_traits>
#include <utility>
#include <variant>

namespace TP
{
	template<typename T>
	class Optional;

	namespace detail
	{
		template <typename U> struct OptTrait : std::false_type
		{
			static Optional<U> getOpt(U&& u) { return std::forward<U>(u); };
		};

		// ReSharper disable CppExplicitSpecializationInNonNamespaceScope
		template <> struct OptTrait<std::nullopt_t> : std::true_type
		{
			static auto getOpt(std::nullopt_t) { return std::nullopt; };
		};
		// ReSharper restore CppExplicitSpecializationInNonNamespaceScope

		template <typename U> struct OptTrait<std::optional<U>> : std::true_type
		{
			static Optional<U> getOpt(std::optional<U>&& u) { return Optional<U>(std::forward<std::optional<U>>(u)); };
		};

		template <typename U> struct OptTrait<Optional<U>> : std::true_type
		{
			static Optional<U> getOpt(Optional<U>&& u) { return std::forward<Optional<U>>(u); };
		};
	}

	/**
	 * \brief replacement for std::optional that tries to solve the problem of Monads in C++ using http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0798r3.html proposal
	 */
	template<typename T>
	class Optional final
		: TP::operators::totally_ordered<std::nullopt_t>,
		TP::operators::totally_ordered<Optional<T>>
	{
		template<typename T2> using RemoveCVR = std::remove_cv_t<std::remove_reference_t<T2>>;

		// see https://en.cppreference.com/w/cpp/utility/optional/optional (8) for explanation
		template<typename U> using AllowConversion = std::bool_constant<!std::disjunction_v<
			std::is_same<RemoveCVR<U>, std::optional<T>>,
			std::is_same<RemoveCVR<U>, Optional<T>>,
			std::is_same<RemoveCVR<U>, std::in_place_t>,
			std::negation<std::is_constructible<T, U&&>>
			>>;

		// see https://en.cppreference.com/w/cpp/utility/optional/optional (4, 5) for explanation
		template<typename U> using AllowUnwrapping = std::bool_constant<!std::disjunction_v<
			std::is_constructible<T, std::optional<U>&>,
			std::is_constructible<T, const std::optional<U>&>,
			std::is_constructible<T, std::optional<U>&&>,
			std::is_constructible<T, const std::optional<U>&&>,
			std::is_convertible<std::optional<U>&, T>,
			std::is_convertible<const std::optional<U>&, T>,
			std::is_convertible<std::optional<U>&&, T>,
			std::is_convertible<const std::optional<U>&&, T>,
			std::is_constructible<T, Optional<U>&>,
			std::is_constructible<T, const Optional<U>&>,
			std::is_constructible<T, Optional<U>&&>,
			std::is_constructible<T, const Optional<U>&&>,
			std::is_convertible<Optional<U>&, T>,
			std::is_convertible<const Optional<U>&, T>,
			std::is_convertible<Optional<U>&&, T>,
			std::is_convertible<const Optional<U>&&, T>
			>>;

		// see https://en.cppreference.com/w/cpp/utility/optional/operator%3D (4) for explanation
		template<typename U> using AllowAssignment = std::bool_constant<std::conjunction_v<
			std::negation<std::is_same<std::optional<T>, RemoveCVR<U>>>,
			std::negation<std::is_same<Optional<T>, RemoveCVR<U>>>,
			std::is_constructible<T, U>,
			std::is_assignable<T&, U>,
			std::disjunction<std::is_scalar<T>, std::is_same<T, std::decay_t<U>>>
			>>;

		// see https://en.cppreference.com/w/cpp/utility/optional/operator%3D (5-6) for explanation
		template<typename U>
		using AllowUnwrappingAssignment = std::bool_constant<!std::disjunction_v<
			AllowUnwrapping<U>,
			std::is_assignable<T&, std::optional<U>&>,
			std::is_assignable<T&, const std::optional<U>&>,
			std::is_assignable<T&, std::optional<U>&&>,
			std::is_assignable<T&, const std::optional<U>&&>,
			std::is_assignable<T&, Optional<U>&>,
			std::is_assignable<T&, const Optional<U>&>,
			std::is_assignable<T&, Optional<U>&&>,
			std::is_assignable<T&, const Optional<U>&&>
			>>;

		template <typename F>
		using IsOrElseInvokable = std::disjunction<
			std::is_invocable_r<void, F>,
			std::is_invocable_r<std::optional<T>, F>,
			std::is_invocable_r<Optional<T>, F>
		>;

		template <typename F>
		static constexpr bool isOrElseInvokable = IsOrElseInvokable<F>::value;

	public:
		using ValueType = typename std::optional<T>::value_type;

		// constructors ----------------

		constexpr Optional() noexcept = default;

		// ReSharper disable CppNonExplicitConvertingConstructor
		constexpr Optional(std::nullopt_t) noexcept : value_() {}
		constexpr Optional(const Optional& other) = default;
		constexpr Optional(Optional&& other) = default;

		template<typename... Types, typename = std::enable_if_t<std::is_constructible_v<T, Types...>>>
		constexpr explicit Optional(const std::in_place_t t, Types&&... args) : value_(t, std::forward<Types>(args)...) {}

		template<typename Elem, typename... Types, class = std::enable_if_t<std::is_constructible_v<T, std::initializer_list<Elem>&, Types...>>>
		constexpr explicit Optional(const std::in_place_t t, std::initializer_list<Elem> initList, Types&&... args) : value_(t, initList, std::forward<Types>(args)...) {}

		template<typename U = T, std::enable_if_t<std::conjunction_v<AllowConversion<U>, std::is_convertible<U&&, T>>, int> = 0>
		constexpr Optional(U&& right) // NOLINT
			: value_(std::forward<U>(right)) {
		}

		template<typename U = T, std::enable_if_t<std::conjunction_v<AllowConversion<U>, std::negation<std::is_convertible<U&&, T>>>, int> = 0>
		constexpr explicit Optional(U&& right) // NOLINT
			: value_(std::forward<U>(right)) {
		}

		template<typename U = T,
			std::enable_if_t<std::conjunction_v<AllowUnwrapping<U>, std::is_constructible<T, const U&>, std::is_convertible<const U&, T>>, int> = 0>
		Optional(const std::optional<U>& right) : value_(right) {}

		template<typename U = T,
			std::enable_if_t<std::conjunction_v<AllowUnwrapping<U>, std::is_constructible<T, const U&>, std::negation<std::is_convertible<const U&, T>>>, int> = 0>
		explicit Optional(const std::optional<U>& right) : value_(right) {}

		template<typename U = T, std::enable_if_t<std::conjunction_v<AllowUnwrapping<U>, std::is_constructible<T, U>, std::is_convertible<U, T>>, int> = 0>
		Optional(std::optional<U>&& right)  noexcept : value_(std::move(right))
		{
		}

		template<typename U = T, std::enable_if_t<std::conjunction_v<AllowUnwrapping<U>, std::is_constructible<T, U>, std::negation<std::is_convertible<U, T>>>, int> = 0>
		explicit Optional(std::optional<U>&& right) noexcept : value_(std::move(right))
		{
		}

		template<typename U, std::enable_if_t<std::conjunction_v<AllowUnwrapping<U>, std::negation<std::is_same<T, U>>, std::is_constructible<T, const U&>, std::is_convertible<const U&, T>>, int> = 0>
		Optional(const Optional<U>& right) : value_(right ? std::make_optional<T>(*right) : std::nullopt) {}

		template<typename U, std::enable_if_t<std::conjunction_v<AllowUnwrapping<U>, std::negation<std::is_same<T, U>>, std::is_constructible<T, const U&>, std::negation<std::is_convertible<const U&, T>>>, int> = 0>
		explicit Optional(const Optional<U>& right) : value_(right ? std::make_optional<T>(*right) : std::nullopt) {}

		template<typename U, std::enable_if_t<std::conjunction_v<AllowUnwrapping<U>, std::negation<std::is_same<T, U>>, std::is_constructible<T, U&&>, std::is_convertible<U&&, T>>, int> = 0>
		Optional(Optional<U>&& right) noexcept
		{
			if (right)
				value_ = std::move(*right);
		}

		template<typename U, std::enable_if_t<std::conjunction_v<AllowUnwrapping<U>, std::negation<std::is_same<T, U>>, std::is_constructible<T, U&&>, std::negation<std::is_convertible<U&&, T>>>, int> = 0>
		explicit Optional(Optional<U>&& right) noexcept
		{
			if (right)
				value_ = std::move(*right);
		}
		// ReSharper restore CppNonExplicitConvertingConstructor

		// assignments -----------------

		Optional& operator=(std::nullopt_t) noexcept { value_ = std::nullopt; return *this; }
		Optional& operator=(const Optional& other) = default;
		Optional& operator=(Optional&& other) = default;

		template<typename U = T, std::enable_if_t<AllowAssignment<U>::value, int> = 0>
		Optional& operator=(U&& right) { value_ = right; return (*this); }

		template<class U = T, std::enable_if_t<std::conjunction_v<AllowUnwrappingAssignment<U>, std::is_constructible<T, const U&>, std::is_assignable<T&, const U&>>, int> = 0>
		Optional& operator=(const std::optional<U>& right) { value_ = right; return (*this); }

		template<class U = T, std::enable_if_t<std::conjunction_v<AllowUnwrappingAssignment<U>, std::is_constructible<T, U>, std::is_assignable<T&, U>>, int> = 0>
		Optional& operator=(std::optional<U>&& right) { value_ = std::move(right); return (*this); }

		template<class U, std::enable_if_t<std::conjunction_v<AllowUnwrappingAssignment<U>, std::negation<std::is_same<T, U>>, std::is_constructible<T, const U&>, std::is_assignable<T&, const U&>>, int> = 0>
		Optional& operator=(const Optional<U>& right) { value_ = right.value_; return (*this); }

		template<class U, std::enable_if_t<std::conjunction_v<AllowUnwrappingAssignment<U>, std::negation<std::is_same<T, U>>, std::is_constructible<T, U>, std::is_assignable<T&, U>>, int> = 0>
		Optional& operator=(Optional<U>&& right) { value_ = std::move(right.value_); return (*this); }

		template<class... Types> T& emplace(Types&&... args) { return value_.emplace(std::forward<Types>(args)...); }

		void swap(Optional& right) noexcept(noexcept(std::swap(std::declval<std::optional<T>>(), std::declval<std::optional<T>>()))) { value_.swap(right.value_); }

		constexpr const T* operator->() const { return std::addressof(*value_); }

		constexpr T* operator->() { return std::addressof(*value_); }

		constexpr const T& operator*() const& { return *value_; }

		constexpr T& operator*()& { return *value_; }

		constexpr T&& operator*()&& { return std::move(*value_); }

		constexpr const T&& operator*() const&& { return std::move(*value_); }

		constexpr explicit operator bool() const noexcept { return value_.has_value(); }

		explicit constexpr operator const std::optional<T>& () const noexcept { return value_; }

		explicit constexpr operator std::optional<T> && () noexcept { return std::move(value_); }

		[[nodiscard]] constexpr bool hasValue() const noexcept { return value_.has_value(); }

		[[nodiscard]] constexpr const T& value() const& { return value_.value(); }

		constexpr T& value()& { return value_.value(); }

		constexpr T&& value()&& { return std::move(value_.value()); }

		[[nodiscard]] constexpr const T&& value() const&& { return std::move(value_.value()); }

		template<class T2, std::enable_if_t<std::is_convertible_v<T2, T>, int> = 0>
		constexpr T valueOr(T2&& right) const& { return value_.value_or(std::forward<T2>(right)); }

		template<class T2, std::enable_if_t<std::is_convertible_v<T2, T>, int> = 0>
		constexpr T valueOr(T2&& right)&& { return std::move(value_.value_or(std::forward<T2>(right))); }

		template<typename F, typename = std::enable_if_t<std::is_same_v<std::decay_t<std::invoke_result_t<F>>, T>>>
		constexpr T valueOr(F&& f) const&
		{
			return hasValue() ? value() : std::forward<F>(f)();
		}

		template<typename TException, class... TArgs, typename = std::enable_if_t<std::is_base_of_v<std::exception, TException>>>
		T valueOrThrow(TArgs&&... args) const&
		{
			static_assert(std::is_constructible_v<TException, TArgs...>, "No matching constructor to call for this exception");
			if (hasValue()) return value();
			throw TException(std::forward<TArgs>(args)...);
		}

		template <class F, typename = std::enable_if_t<std::is_invocable_v<F, T>>>
		constexpr auto andThen(F&& f)&
		{
			using R = std::decay_t<decltype(std::invoke(std::forward<F>(f), value()))>;
			static_assert(detail::OptTrait<R>(), "Function should return a specialization of optional");

			return hasValue() ? detail::OptTrait<R>::getOpt(std::invoke(std::forward<F>(f), value())) : std::nullopt;
		}

		template <class F, typename = std::enable_if_t<std::is_invocable_v<F, T>>>
		constexpr auto andThen(F&& f)&&
		{
			using R = std::decay_t<decltype(std::invoke(std::forward<F>(f), value()))>;
			static_assert(detail::OptTrait<R>(), "Function should return a specialization of optional");

			return hasValue() ? detail::OptTrait<R>::getOpt(std::invoke(std::forward<F>(f), std::move(value()))) : std::nullopt;
		}

		template <class F, typename = std::enable_if_t<std::is_invocable_v<F, T>>>
		constexpr auto andThen(F&& f) const&
		{
			using R = std::decay_t<decltype(std::invoke(std::forward<F>(f), value()))>;
			static_assert(detail::OptTrait<R>(), "Function should return a specialization of optional");

			return hasValue() ? detail::OptTrait<R>::getOpt(std::invoke(std::forward<F>(f), value())) : std::nullopt;
		}

		template <class F, typename = std::enable_if_t<std::is_invocable_v<F, T>>>
		constexpr auto andThen(F&& f) const&&
		{
			using R = std::decay_t<decltype(std::invoke(std::forward<F>(f), value()))>;
			static_assert(detail::OptTrait<R>(), "Function should return a specialization of optional");

			return hasValue() ? detail::OptTrait<R>::getOpt(std::invoke(std::forward<F>(f), std::move(value()))) : std::nullopt;
		}

		template <class F, typename = std::enable_if_t<std::is_invocable_v<F, T>>>
		constexpr auto transform(F&& f)&
		{
			using R = std::decay_t<decltype(std::invoke(std::forward<F>(f), value()))>;

			if constexpr (std::is_same_v<R, void>)
			{
				if (hasValue())
				{
					std::invoke(std::forward<F>(f), value());
					return Optional<std::monostate>(std::monostate());
				}
#
				return Optional<std::monostate>();
			}
			else
			{
				static_assert(!detail::OptTrait<R>(), "Function should not return a specialization of optional");
				return hasValue() ? Optional<R>(std::invoke(std::forward<F>(f), value())) : std::nullopt;
			}
		}

		template <class F, typename = std::enable_if_t<std::is_invocable_v<F, T>>>
		constexpr auto transform(F&& f)&&
		{
			using R = std::decay_t<decltype(std::invoke(std::forward<F>(f), value()))>;

			if constexpr (std::is_same_v<R, void>)
			{
				if (hasValue())
				{
					std::invoke(std::forward<F>(f), std::move(value()));
					return Optional<std::monostate>(std::monostate());
				}
#
				return Optional<std::monostate>();
			}
			else
			{
				static_assert(!detail::OptTrait<R>(), "Function should not return a specialization of optional");
				return hasValue() ? Optional<R>(std::invoke(std::forward<F>(f), std::move(value()))) : std::nullopt;
			}
		}

		template <class F, typename = std::enable_if_t<std::is_invocable_v<F, T>>>
		constexpr auto transform(F&& f) const&
		{
			using R = std::decay_t<decltype(std::invoke(std::forward<F>(f), value()))>;

			if constexpr (std::is_same_v<R, void>)
			{
				if (hasValue())
				{
					std::invoke(std::forward<F>(f), value());
					return Optional<std::monostate>(std::monostate());
				}
#
				return Optional<std::monostate>();
			}
			else
			{
				static_assert(!detail::OptTrait<R>(), "Function should not return a specialization of optional");
				return hasValue() ? Optional<R>(std::invoke(std::forward<F>(f), value())) : std::nullopt;
			}
		}

		template <class F, typename = std::enable_if_t<std::is_invocable_v<F, T>>>
		constexpr auto transform(F&& f) const&&
		{
			using R = std::decay_t<decltype(std::invoke(std::forward<F>(f), value()))>;

			if constexpr (std::is_same_v<R, void>)
			{
				if (hasValue())
				{
					std::invoke(std::forward<F>(f), std::move(value()));
					return Optional<std::monostate>(std::monostate());
				}
#
				return Optional<std::monostate>();
			}
			else
			{
				static_assert(!detail::OptTrait<R>(), "Function should not return a specialization of optional");
				return hasValue() ? Optional<R>(std::invoke(std::forward<F>(f), std::move(value()))) : std::nullopt;
			}
		}

		template <class F, typename = std::enable_if_t<isOrElseInvokable<F>>>
		constexpr Optional<T> orElse(F&& f)&
		{
			if (hasValue()) return *this;

			if constexpr (std::is_void_v<std::invoke_result_t<F>>)
			{
				std::forward<F>(f)();
				return std::nullopt;
			}
			else
			{
				return detail::OptTrait<RemoveCVR<std::invoke_result_t<F>>>::getOpt(std::forward<F>(f)());
			}
		}

		template <class F, typename = std::enable_if_t<isOrElseInvokable<F>>>
		constexpr Optional<T> orElse(F&& f)&&
		{
			if (hasValue()) return std::move(*this);

			if constexpr (std::is_void_v<std::invoke_result_t<F>>)
			{
				std::forward<F>(f)();
				return std::nullopt;
			}
			else
			{
				return detail::OptTrait<RemoveCVR<std::invoke_result_t<F>>>::getOpt(std::forward<F>(f)());
			}
		}

		template <class F, typename = std::enable_if_t<isOrElseInvokable<F>>>
		constexpr Optional<T> orElse(F&& f) const&
		{
			if (hasValue()) return *this;

			if constexpr (std::is_void_v<std::invoke_result_t<F>>)
			{
				std::forward<F>(f)();
				return std::nullopt;
			}
			else
			{
				return detail::OptTrait<RemoveCVR<std::invoke_result_t<F>>>::getOpt(std::forward<F>(f)());
			}
		}

		template <class F, typename = std::enable_if_t<isOrElseInvokable<F>>>
		constexpr Optional<T> orElse(F&& f) const&&
		{
			if (hasValue()) return std::move(*this);

			if constexpr (std::is_void_v<std::invoke_result_t<F>>)
			{
				std::forward<F>(f)();
				return std::nullopt;
			}
			else
			{
				return detail::OptTrait<RemoveCVR<std::invoke_result_t<F>>>::getOpt(std::forward<F>(f)());
			}
		}

		[[nodiscard]] friend bool operator==(const Optional<T>& x, const Optional<T>& y) noexcept { return x.value_ == y.value_; }
		[[nodiscard]] friend bool operator<(const Optional<T>& x, const Optional<T>& y) noexcept { return x.value_ < y.value_; }
		[[nodiscard]] friend bool operator==(const Optional<T>& x, const std::nullopt_t& y) noexcept { return x.value_ == y; }
		[[nodiscard]] friend bool operator<(const Optional<T>& x, const std::nullopt_t& y) noexcept { return x.value_ < y; }

	private:
		std::optional<T> value_;
	};

	template<class T>
	[[nodiscard]] constexpr Optional<std::decay_t<T>> makeOptional(T&& value)
	{
		return Optional<std::decay_t<T>>{ std::forward<T>(value) };
	}

	template<class T, class... Types>
	[[nodiscard]] constexpr Optional<T> makeOptional(Types&&... args)
	{
		return (Optional<T>{std::in_place, std::forward<Types>(args)...});
	}

	template<class T, class Elem, class... Types>
	[[nodiscard]] constexpr Optional<T> makeOptional(std::initializer_list<Elem> iList, Types&&... args)
	{
		return (Optional<T>{std::in_place, iList, std::forward<Types>(args)...});
	}

	template<class T>
	[[nodiscard]] constexpr Optional<std::decay_t<T>> fromStd(const std::optional<T>& value)
	{
		return Optional<std::decay_t<T>>{ value };
	}

	/*template<class F, class... Args, typename = std::enable_if_t<std::is_invocable_v<F, Args...>>>
	[[nodiscard]] constexpr auto whenAll(F&& f, Optional<Args>&&... args)
	{


		if(args.hasValue()...)
	}*/

}