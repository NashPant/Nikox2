#pragma once
#include "Operators.h"
#include <memory>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <optional>


namespace TP
{
	namespace utils
	{
		template <typename T> class NotNull;

		// Useful traits
		template <typename T> struct IsNotNull : std::false_type {};
		template <typename T> struct IsNotNull<NotNull<T>> : std::true_type {};

		// Equivalent to Rust NonNull ptr, i.e. restricts a pointer or smart pointer to only hold non-null values.
		template <class PtrT>
		class NotNull :
			TP::operators::totally_ordered<NotNull<PtrT>>,
			TP::operators::totally_ordered<NotNull<PtrT>, PtrT>,
			TP::operators::totally_ordered<NotNull<PtrT>, typename std::add_pointer<typename std::pointer_traits<PtrT>::element_type>::type>
		{
		public:
			static_assert(std::is_assignable<PtrT&, std::nullptr_t>::value, "T cannot be assigned nullptr.");
			using ElementType = typename std::pointer_traits<PtrT>::element_type;
			using PtrType = typename std::add_pointer<typename std::pointer_traits<PtrT>::element_type>::type;

			NotNull(NotNull&& other) = default;
			NotNull& operator=(NotNull&& other) = default;
			NotNull(const NotNull& other) noexcept(std::is_nothrow_copy_constructible<PtrT>::value) = default;
			NotNull& operator=(const NotNull& other) noexcept(std::is_nothrow_copy_assignable<PtrT>::value) = default;

			template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, PtrT>>>
			explicit NotNull(U&& u)
				: ptr_(std::forward<U>(u))
			{
				if (!ptr_)
					throw std::runtime_error("NotNull::cstr: Cannot build a NotNullPtr from nullptr value");
			}

			template <typename U, typename = std::enable_if_t<std::is_constructible_v<PtrT, U*>>>
			explicit NotNull(U* u)
				: ptr_(u)
			{
				if (!ptr_)
					throw std::runtime_error("NotNull::cstr: Cannot build a NotNullPtr from nullptr value");
			}

			template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, PtrT>>>
			NotNull(const NotNull<U>& other) : NotNull(other.asNullable())
			{
			}

			template <typename = std::enable_if_t<std::negation_v<std::is_same<std::nullptr_t, PtrT>>>>
			explicit NotNull(PtrT ptr)
				: ptr_(std::move(ptr))
			{
				if (!ptr_)
					throw std::runtime_error("NotNull::cstr: Cannot build a NotNullPtr from nullptr value");
			}

			ElementType& operator*()  const noexcept { return *ptr_; }
			PtrType operator->() const noexcept { return &*ptr_; }
			[[nodiscard]] PtrType get() const noexcept { return &*ptr_; }

			// get access to underlying ptr
			[[nodiscard]] PtrT const& asNullable() const & { return ptr_; }
			PtrT && asNullable() && { return std::move(ptr_); }

			[[nodiscard]] friend bool operator==(const NotNull<PtrT>& x, const NotNull<PtrT>& y) noexcept { return x.ptr_ == y.ptr_; }
			[[nodiscard]] friend bool operator<(const NotNull<PtrT>& x, const NotNull<PtrT>& y) noexcept { return x.ptr_ < y.ptr_; }
			[[nodiscard]] friend bool operator==(const NotNull<PtrT>& x, const PtrT& y) noexcept { return x.ptr_ == y; }
			[[nodiscard]] friend bool operator<(const NotNull<PtrT>& x, const PtrT& y) noexcept { return x.ptr_ < y; }
			[[nodiscard]] friend bool operator==(const NotNull<PtrT>& x, const PtrType& y) noexcept { return x.get() == y; }
			[[nodiscard]] friend bool operator<(const NotNull<PtrT>& x, const PtrType& y) noexcept { return x.get() < y; }

			// prevents compilation when someone attempts to assign a null pointer constant
			NotNull(std::nullptr_t) = delete;
			NotNull& operator=(std::nullptr_t) = delete;

			// ptr is not null so should not need to convert have bool operator
			operator bool() const = delete;

			// pointers only point to single objects
			NotNull& operator++() = delete;
			NotNull& operator--() = delete;
			NotNull operator++(int) = delete;
			NotNull operator--(int) = delete;
			NotNull& operator+=(std::ptrdiff_t) = delete;
			NotNull& operator-=(std::ptrdiff_t) = delete;
			void operator[](std::ptrdiff_t) const = delete;

		private:
			PtrT ptr_;
		};

		template <typename T> using NotNullUniquePtr = NotNull<std::unique_ptr<T>>;
		template <typename T> using NotNullSharedPtr = NotNull<std::shared_ptr<T>>;

		template <typename T, typename... Args>
		[[nodiscard]] NotNullUniquePtr<T> makeNotNullUnique(Args &&... args)
		{
			const auto uniquePtr = std::make_unique<T>(std::forward<Args>(args)...);
			return NotNullUniquePtr<T>(std::move(uniquePtr));
		}

		template <typename T, typename... Args>
		[[nodiscard]] NotNullSharedPtr<T> makeNotNullShared(Args&&... args)
		{
			auto sharedPtr = std::make_shared<T>(std::forward<Args>(args)...);
			return NotNullSharedPtr<T>(std::move(sharedPtr));
		}

		template <class T>
		auto makeNotNull(T&& t)
		{
			return NotNull<std::remove_cv_t<std::remove_reference_t<T>>>{std::forward<T>(t)};
		}

		template <class T>
		std::ostream& operator<<(std::ostream& os, const NotNull<T>& val)
		{
			os << val.get();
			return os;
		}

		template<class T1, class T2>
		[[nodiscard]] bool operator==(const NotNull<T1>& left, const NotNull<T2>& right) noexcept
		{
			return (left.get() == right.get());
		}

		template<class T1, class T2>
		[[nodiscard]] bool operator!=(const NotNull<T1>& left, const NotNull<T2>& right) noexcept
		{
			return (left.get() != right.get());
		}

		template<class T1, class T2>
		[[nodiscard]] bool operator<(const NotNull<T1>& left, const NotNull<T2>& right) noexcept
		{
			return (left.get() < right.get());
		}

		template<class T1, class T2>
		[[nodiscard]] bool operator>=(const NotNull<T1>& left, const NotNull<T2>& right) noexcept
		{
			return (left.get() >= right.get());
		}

		template<class T1, class T2>
		[[nodiscard]] bool operator>(const NotNull<T1>& left, const NotNull<T2>& right) noexcept
		{
			return (left.get() > right.get());
		}

		template<class T1, class T2>
		[[nodiscard]] bool operator<=(const NotNull<T1>& left, const NotNull<T2>& right) noexcept
		{
			return (left.get() <= right.get());
		}
	}
}


template <typename T> using Ptr = std::shared_ptr<T>;
template <typename T> using CPtr = std::shared_ptr<const T>;

template<typename T, typename... Ts> auto makeCptr(Ts&&... args)
{
	using DecayedT = std::decay_t<T>;
	return std::make_shared<const DecayedT>(std::forward<Ts>(args)...);
}

#define FWD_DECLARE_SHAREDPTR(T)\
class T;                     \
using T##ConstPtr = std::shared_ptr<const T>;


#define FWD_DECLARE_NOTNULLSHARED(T)\
class T;                     \
using T##ConstPtr = TP::utils::NotNullSharedPtr<const T>;

#define FWD_DECLARE_NOTNULLUNIQUE(T)\
class T;                     \
using T##ConstPtr = TP::utils::NotNullUniquePtr<const T>;

