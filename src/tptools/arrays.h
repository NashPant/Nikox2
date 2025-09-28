

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
	template <typename T> decltype(auto) front(T& vec)
	{
		return *vec.begin();
	}
	template <typename T> decltype(auto) back(T& vec)
	{
		return *(std::prev(vec.end()));
	}

	double values_like(const double&, double x);
	Eigen::VectorXd values_like(const Eigen::VectorXd& vec, double x);
	Eigen::ArrayXd values_like(const Eigen::ArrayXd& vec, double x);
	template<typename T, typename V>

	std::vector<T> values_like(const V& v, const T& t)
	{
		std::vector<T> vec(size(v), t);
		return vec;
	}
	inline double value_of(double x) { return x; }

	template <class Derived>
	inline auto value_of(const Eigen::DenseBase<Derived>& x) { return x.derived().eval(); }

	template<typename Vec> void sort(Vec& array)
	{
		std::sort(array.data(), array.data() + array.size());
	}

	double cwiseQuotient(double x, double y);
	Eigen::VectorXd
		cwiseQuotient(const Eigen::VectorXd& x, const Eigen::VectorXd& y);

	double cwiseProduct(double x, double y);
	Eigen::VectorXd
		cwiseProduct(const Eigen::VectorXd& x, const Eigen::VectorXd& y);
	double cwiseMax(double a, double b);
	Eigen::VectorXd cwiseMax(const Eigen::VectorXd& a, const Eigen::VectorXd& b);
	Eigen::VectorXd cwiseMax(double a, const Eigen::VectorXd& b);
	Eigen::VectorXd cwiseMax(const Eigen::VectorXd& a, double b);
	double cwiseMin(double a, double b);
	Eigen::VectorXd cwiseMin(const Eigen::VectorXd& a, const Eigen::VectorXd& b);
	Eigen::VectorXd cwiseMin(double a, const Eigen::VectorXd& b);
	Eigen::VectorXd cwiseMin(const Eigen::VectorXd& a, double b);



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
		if (TP::tools::empty(vals))
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
	const T& get_element(const std::vector<T>& container, size_t i)
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


	// Templated function to get the surrounding indices.
	template<typename Container>
	std::pair<IndexType<Container>, IndexType<Container>>
		get_surrounding_indices(const Container& times, double t, double tolerance = 0.0)
	{
		IndexType<Container> n = times.size();
		if (n == 0)
			return { 0, 0 };
		// If t is less than or equal to the first element, return {0,0}.
		if (t - tolerance <= get_element(times, 0))
			return { 0, 0 };
		//If t is greater than or equal to the last element, return {n-1,n-1}.
		if (t + tolerance >= get_element(times, n - 1))
			return { n - 1, n - 1 };
		// Binary search for the two indices such that times [10] < t < // times[hi].
		IndexType<Container> lo = 0, hi = n - 1;
		while (hi - lo > 1)
		{
			IndexType<Container> mid = lo + (hi - lo) / 2;
			if (get_element(times, mid) < t)
				lo = mid;
			else
				hi = mid;
		}

		// If t exactly equals the hi element, return hi for both.
		const double element_high = get_element(times, hi);
		const double element_low = get_element(times, lo);
		if (element_high - tolerance <= t && t <= element_high + tolerance)
		{
			return { hi, hi };
		}
		if (element_low - tolerance <= t && t <= element_low + tolerance)
		{
			return { lo, lo };
		}
		return { lo, hi };
	}

	Eigen::ArrayXd select_indices(
		const Eigen::Ref<const Eigen::ArrayXd>& array,
		const std::vector<int>& indices);


	std::vector<bool>
		select_indices(const std::vector<bool>& array, const std::vector<int>& indices);


	template <typename Container, typename V>
	int upper_bound_idx(const Container& c, const V& v)
	{
		const auto& iter = std::upper_bound(c.begin(), c.end(), v);
		return static_cast<int>(iter - c.begin());
	}

	template <typename Container, typename V>
	int lower_bound_idx(const Container& c, const V& v)
	{
		const auto& iter = std::lower_bound(c.begin(), c.end(), v);
		return static_cast<int>(iter - c.begin());
	}

	template <typename V, typename T>
	void assign(V& array, const std::vector<int>& indices, const T& value)
	{
		for (int idx = 0; idx < indices.size(); ++idx)
		{
			array[indices[idx]] = value;
		}
	}

	Eigen::ArrayXd to_eigen(const std::vector<double>& in);

	inline std::vector<double> to_std_vector(const Eigen::ArrayXd& in)
	{
		std::vector<double> vec(in.begin(), in.end());
		return vec;
	}

	template <typename T>
	std::vector<T> flatten(const std::vector<std::vector<T>>& vec_of_vecs)
	{
		std::vector<T> flat;
		size_t total_size = 0;
		for (const auto& v: vec_of_vecs)
		{
			total_size += v.size();
		}
		flat.reserve(total_size);
		for (const auto& v: vec_of_vecs)
		{
			flat.insert(flat.end(), v.begin(), v.end());
		}
		return flat;
	}

	inline auto flatten(std::vector<Eigen::VectorXd>& vec_of_vecs)
	{
		Eigen::Index totalSize = 0;
		for (const auto& v : vec_of_vecs)
		{
			totalSize += v.size();
		}
		Eigen::VectorXd flat(totalSize);
		Eigen::Index pos = 0;
		for (const auto& v : vec_of_vecs)
		{
			flat.segment(pos, v.size()) = v;
			pos += v.size();
		}

		return flat;
	}


	template<typename T> struct is_eigen_vector : std::false_type
	{
	};

	template<typename Scalar,
		int Rows,
		int Cols,
		int Options, int MaxRows,
		int MaxCols>
	struct is_eigen_vector<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>>
		: std::integral_constant<bool, (Rows == 1 || Cols == 1)>
	{
	};

	template<typename Scalar,
		int Rows,
		int Cols,
		int Options,
		int MaxRows,
		int MaxCols>
	struct  is_eigen_vector<
		Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>>
		: std::integral_constant<bool, (Rows == 1 || Cols == 1)>
	{
	};


}