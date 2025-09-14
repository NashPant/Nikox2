

#pragma once
//#include "datetime.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cassert>
#include <iterator>
#include <numeric>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
#include <Eigen/Core> 
#include <Eigen/Dense>

namespace TP::tools
{

	template<typename T> int size(const T& vec)
	{
		return static_cast<int>(vec.size());
	}
	template <typename T> bool empty(const T& vec)
	{
		return static_cast<int>(vec.size()) == 0;
	}
	template <typename T> decltype(auto) front(T & vec) 
	{ 
		return *vec.begin(); 
	} 
	template <typename T> decltype(auto) back(T& vec)
	{
		return *(std::prev(vec.end()));
	}

	double values_like(const double&, double x);
	Eigen::VectorXd values_like(const Eigen::VectorXd & vec, double x); 
	Eigen::ArrayXd values_like(const Eigen::ArrayXd & vec, double x);
	template<typename T, typename V>

	std::vector<T> values_like(const V& v, const T& t)
	{
		std::vector<T> vec(size(v), t);
		return vec;
	}
	inline double value_of(double x) { return x; }

	template <class Derived>
	inline auto value_of(const Eigen::DenseBase<Derived>&x) { return x.derived().eval(); }

	template<typename Vec> void sort(Vec& array)
	{
		std::sort(array.data(), array.data() + array.size());
	}

	double cwiseQuotient(double x, double y);
	Eigen::VectorXd
			cwiseQuotient(const Eigen::VectorXd & x, const Eigen::VectorXd & y);

	double cwiseProduct(double x, double y);
	Eigen::VectorXd
		cwiseProduct(const Eigen::VectorXd & x, const Eigen::VectorXd & y);
	double cwiseMax(double a, double b);
	Eigen::VectorXd cwiseMax(const Eigen::VectorXd & a, const Eigen::VectorXd & b); 
	Eigen::VectorXd cwiseMax(double a, const Eigen::VectorXd & b);
	Eigen::VectorXd cwiseMax(const Eigen::VectorXd & a, double b);
	double cwiseMin(double a, double b);
	Eigen::VectorXd cwiseMin(const Eigen::VectorXd & a, const Eigen::VectorXd & b); 
	Eigen::VectorXd cwiseMin(double a, const Eigen::VectorXd & b);
	Eigen::VectorXd cwiseMin(const Eigen::VectorXd & a, double b);
			


	template<typename T> bool is_increasing(const T& vec)
	{
		if (TP::tools::empty(vec))
			return true;
		using Type = decltype(*vec.begin());
		const auto iter = std::adjacent_find(vec.begin(), vec.end(), std::greater<Type>());
		return iter == vec.end();
	}

	template <typename T> bool is_decreasing(const T& vec)
	{
		if (TP::tools::empty(vec))
			return true;

		using Type = decltype(*vec.begin());
		const auto iter =
			std::adjacent_find(vec.begin(), vec.end(), std::less<Type>());
		return iter == vec.end();
	}
	template<typename T> bool is_strictly_increasing(const T& vec)
	{
		if (TP::tools::empty(vec))
			return true;
		using Type = decltype(*vec.begin());
		const auto iter =
			std::adjacent_find(vec.begin(), vec.end(), std::greater_equal<Type>());
		return iter == vec.end();
	}
	template <typename T> bool is_strictly_decreasing(const T& vec)
	{
		if (TP::tools::empty(vec))
		{
			return true;
		}
		using Type = decltype(*vec.begin());
		const auto iter =
			std::adjacent_find(vec.begin(), vec.end(), std::less_equal<Type>());
		return iter == vec.end();
	}
	template<typename T> bool is_finite(const T& vec)
	{
		return std::all_of(
			vec.begin(), vec.end(), [](const auto& x) { return std::isfinite(x); });
	}
	template <typename T> T pad_vector(const T& vals, size_t left, size_t right)
	{
		if (empty(vals))
			return {};
		T padded(vals.size() + left + right);
		const auto iter = std::fill_n(padded.begin(), left, front(vals)); 
		const auto iter2 = std::copy(vals.begin(), vals.end(), iter);
		std::fill_n(iter2, right, back(vals));
		return padded;

	}
	template <typename Container, typename = void> struct container_index_type
	{
		using type = size_t;
	};
	template <typename Container> struct container_index_type<Container, std::void_t<typename Container::Index>>
	{
		using type = typename Container::Index;
	};

	template <typename Container>
	using IndexType = typename container_index_type<Container>::type;
	// Helper: For std::vector
	template <typename T>
	const T& get_element(const std::vector<T>&container, size_t i)
	{
		return container[i];
	}
	// Helper: Overload for Eigen dense types (e.g. VectorXd, ArrayXd)
	template <typename Derived>
	auto get_element(
		const Eigen::DenseBase<Derived>& container, typename Derived::Index i) -> decltype(container.coeff(i))
	{
		return container.coeff(i);
	}
}