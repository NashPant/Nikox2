
#pragma once
#include <Eigen/Dense>
#include <bspline/Core.h>
// #include <bspline/interpolation/interpolation.h>
#include <concepts>
//#include <utils/arrays.h>
#include <vector>
namespace TP::maths
{


	enum class Node
	{
		FIRST, LAST
	};


	template<typename T> struct Boundary
	{
		Node node = Node::FIRST;
		size_t derivative = 1;
		T value = T(0.);
	};

	template <std::size_t order>
	concept ValidOrder =
		(order == 1 || order == 2 || order == 3 || order == 4 || order == 5);


	template <typename T, size_t order>
		requires ValidOrder<order>
	Eigen::Matrix<T, -1, -1> compute_spline_interp_design_matrix(
		const std::vector<bspline::Spline<T, order>>& splines,
		const std::vector<T>& xs,
		const std::array<Boundary<T>, order - 1>& boundaries)
	{
		const auto n_bases = splines.size();
		const auto n_conditions = xs.size() + order - 1;
		Eigen::Matrix<T, -1, -1> N(n_conditions, n_bases);
		int count_left = 0;
		int count_right = 0;
		for (auto const& bc: boundaries)
		{
			const size_t xrow = (bc.node == Node::FIRST ? 0 : xs.size() - 1);
			const size_t Nrow =
				(bc.node == Node::FIRST ? count_left
					: n_conditions - 1 - count_right);

			if (bc.node == Node::FIRST)
				count_left++;
			if (bc.node == Node::LAST)
				count_right++;

			// Lambda to apply any Dx<k> operator 
			auto apply_derivative = [&](auto op) {
				for (size_t j = 0; j < n_bases; ++j)
					N(Nrow, j) = (op * splines[j])(xs[xrow]);
				};

			switch (bc.derivative)
			{
			case 1:
				apply_derivative(bspline::operators::Dx<1> {});
				break;
			case 2:
				apply_derivative(bspline::operators::Dx<2> {});
				break;
			case 3:
				apply_derivative(bspline::operators::Dx<3> {});
				break;
			case 4:
				apply_derivative(bspline::operators::Dx<4>{});
				break;
			case 5:
				apply_derivative(bspline::operators::Dx<5> {});
				break;
			default:
				throw std::invalid_argument("Unsupported derivative order");
			}
		}

		for (int j = 0; j < n_bases; ++j)
		{
			for (int i = 0; i < xs.size(); ++i)
			{
				N(i + count_left, j) = splines[j](xs[i]);
			}
		}
		return N;
	}

	template<typename T, size_t order>
		requires ValidOrder<order>
	Eigen::Matrix<T, -1, 1> assemble_spline_targets(
		const std::vector<T>& ys,
		const std::array<Boundary<T>, order - 1>& boundaries)
	{
		const size_t n = ys.size();
		const size_t n_conditions = n + boundaries.size(); // == n + order - 1 
		Eigen::Matrix<T, -1, 1> b(n_conditions);

		size_t count_left = 0;
		size_t count_right = 0;
		for (auto const& bc : boundaries)
		{
			const size_t row =
				(bc.node == Node::FIRST ? count_left
					: n_conditions - 1 - count_right);
			if (bc.node == Node::FIRST)
				count_left++;
			if (bc.node == Node::LAST)
				count_right++;
			b(row) = bc.value;
		}

		for (size_t i = 0; i < n; ++i)
		{
			b(i + count_left) = ys[i];
		}
		return b;
	}


	template <typename SPLINE>
	Eigen::MatrixXd compute_spline_basis(
		const std::vector<SPLINE>& splines, const Eigen::ArrayXd& xs)
	{
		const auto n_bases = splines.size();
		// Evaluate basis functions at xi (may be zero outside knot span)
		Eigen::MatrixXd N(xs.size(), n_bases);
		for (int j = 0; j < n_bases; ++j)
		{
			for (int i = 0; i < xs.size(); ++i)
			{
				N(i, j) = splines[j](xs[i]);
			}
			return N;
		}
	}



	template <size_t order>
	auto compute_spline_basis_and_derivatives(
		const std::vector<bspline::Spline<double, order>>& splines,
		const Eigen::ArrayXd& xs)
	{
		const auto n_bases = splines.size();
		// Evaluate basis functions at xi (may be zero outside knot span)
		Eigen::ArrayXXd N(xs.size(), n_bases);
		Eigen::ArrayXXd N_d(xs.size(), n_bases);
		Eigen::ArrayXXd N_dd(xs.size(), n_bases);

		const auto dx = bspline::operators::Dx<1>{};
		const auto d2x = bspline::operators::Dx<2>{};
		for (int j = 0; j < n_bases; ++j)
		{
			const auto spline_1stderiv = dx * splines[j];
			const auto spline_2ndderiv = d2x * splines[j];
			for (int i = 0; i < xs.size(); ++i)
			{
				N(i, j) = splines[j](xs[i]);
				N_d(i, j) = spline_1stderiv(xs[i]);
				N_dd(i, j) = spline_2ndderiv(xs[i]);
			}
		}
		struct ValueAndDerivatives
		{
			Eigen::ArrayXXd N;
			Eigen::ArrayXXd N_d;
			Eigen::ArrayXXd N_dd;
		};



		return ValueAndDerivatives{ N, N_d, N_dd };
	}

	template <size_t order>
	auto compute_spline_basis_1st_derivatives(
		const std::vector<bspline::Spline<double, order>>& splines, const Eigen::ArrayXd& xs)
	{
		const auto n_bases = splines.size();
		// Evaluate basis functions at xi (may be zero outside knot span) 
		Eigen::ArrayXXd N_d(xs.size(), n_bases);
		const auto d1x = bspline::operators::Dx<1>{};
		for (int j = 0; j < n_bases; ++j)
		{
			const auto spline_deriv = d1x * splines[j];
			for (int i = 0; i < xs.size(); ++i)
			{
				N_d(i, j) = spline_deriv(xs[i]);
			}
		}
		return N_d;
	}

	template <size_t order>
	auto compute_spline_basis_2nd_derivatives(
		const std::vector<bspline::Spline<double, order>>& splines,
		const Eigen::ArrayXd& xs)
	{
		const auto n_bases = splines.size();
		// Evaluate basis functions at xi (may be zero outside knot span) 
		Eigen::ArrayXXd N_dd(xs.size(), n_bases);
		const auto d2x = bspline::operators::Dx<2>{};
		for (int j = 0; j < n_bases; ++j)
		{
			const auto spline_2ndderiv = d2x * splines[j];
			for (int i = 0; i < xs.size(); ++i)
			{
				N_dd(i, j) = spline_2ndderiv(xs[i]);
			}
			return N_dd;
		}

	}



	template <size_t order>
	auto compute_spline_basis_3rd_derivatives(
		const std::vector<bspline::Spline<double, order>>& splines, const Eigen::ArrayXd& xs)
	{
		const auto n_bases = splines.size();
		// Evaluate basis functions at xi (may be zero outside knot span) 
		Eigen::ArrayXXd N_ddd(xs.size(), n_bases);
		const auto d3x = bspline::operators::Dx<3>{};
		for (int j = 0; j < n_bases; ++j)
		{
			const auto spline_2ndderiv = d3x * splines[j];
			for (int i = 0; i < xs.size(); ++i)
			{
				N_ddd(i, j) = spline_2ndderiv(xs[i]);
			}
		}
		return N_ddd;
	}

	Eigen::ArrayXXd
		build_basis_matrix_with_linear_extrapolation(
			const Eigen::ArrayXd& xs,
			double x0,
			double XN,
			const Eigen::ArrayXXd& S_basis,
			const Eigen::ArrayXXd& S_basis_outer,
			const Eigen::ArrayXXd& dS_basis_outer);


	//inline auto clamped_3rd_order_spline_basis_functions(const Eigen::ArrayXd& ns)
	//{
	//	constexpr size_t SPLINE_ORDER = 3;
	//	std::vector<double> v_y(ns.data(), ns.data() + ns.size());
	//	std::vector<double> knots =
	//		utils::pad_vector(v_y, SPLINE_ORDER, SPLINE_ORDER);
	//	return bspline::generateBSplines<SPLINE_ORDER>(knots);
	//}


	//template <typename T, size_t order>
	//inline auto clamped_spline_basis_functions(const std::vector<T>& xs)
	//{
	//	static_assert(order >= 1, "Order may not be zero.");
	//	std::vector<double> v_xs(xs.data(), xs.data() + xs.size());
	//	std::vector<double> knots = utils::pad_vector(v_xs, order, order);
	//	return bspline::generateBSplines<order>(knots);
	//}


	//template <typename B, typename S>
	//auto build_symmetric_matrix(const B& b, const std::vector<S>& basis)
	//{
	//	const auto n_bases = basis.size();
	//	Eigen::MatrixXd M(n_bases, n_bases);
	//	for (auto i = 0; i < n_bases; ++i)
	//	{
	//		for (auto j = 0; j < n_bases; ++j)
	//		{
	//			const auto val = b(basis[i], basis[j]);
	//			M(i, j) = val;
	//			M(j, i) = val;
	//		}
	//	}
	//	return M;
	//}


	template <typename B, typename S>
	auto build_symmetric_matrix(
		const B& b, const std::vector<S>& basis, const std::vector<double>& weights)
	{
		// TODO: FIXME or remove
		const auto n_bases = basis.size();
		Eigen::MatrixXd M(n_bases, n_bases);
		for (auto i = 0; i < n_bases; ++i)
		{
			const auto wi = std::sqrt(weights[i]);
			for (auto j = 0; j < n_bases; ++j)
			{
				const auto wj = std::sqrt(weights[j]);
				const auto val = b(wi * basis[i], wj * basis[j]);
				M(i, j) = val;
				M(j, i) = val;
			}
		}
		return M;
	}

	template <size_t order>
	Eigen::MatrixXd build_smoothing_matrix(
		const std::vector<bspline::Spline<double, order>>& splines1, double smoothing_weight = 1.0)
	{
		const bspline::operators::Dx<2> d2x;
		const auto blf = bspline::integration::BilinearForm(d2x, d2x);
		return build_symmetric_matrix(blf, splines1) * smoothing_weight;
	}

	template <size_t order>
	Eigen::MatrixXd build_order3_smoothing_matrix(
		const std::vector<bspline::Spline<double, order>>& splines1, double smoothing_weight = 1.0)
	{
		const bspline::operators::Dx<3> d3x;
		const auto blf = bspline::integration::BilinearForm(d3x, d3x);
		return build_symmetric_matrix(blf, splines1) * smoothing_weight;
	}

	template <size_t order>
	Eigen::MatrixXd build_tension_matrix(
		const std::vector<bspline::Spline<double, order>>& splines1, double tension_weight = 1.0)
	{
		const bspline::operators::Dx<1> dx;
		const auto blf = bspline::integration::BilinearForm(dx, dx);
		return build_symmetric_matrix(blf, splines1) * tension_weight;
	}



	//template <size_t order>
	//Eigen::MatrixXd build_smoothing_matrix(
	//	const std::vector<bspline::Spline<double, order>>& basis_splines,
	//	const Eigen::ArrayXd& nodes,
	//	const std::vector<double>& smoothing_weights)
	//{
	//	// 1. Generate B-spline basis
	//	constexpr size_t weight_order = 1;
	//	auto weight_basis = clamped_spline_basis_functions<double, weight_order>(utils::to_std_vector(nodes));
	//	//
	//	//	bspline::generateBSplines<weight_order, double>(utils::to_std_vector(nodes));
	//	// 2. Fit weight function w(x)
	//	auto w_spline = bspline::linearCombination(
	//		smoothing_weights.begin(), smoothing_weights.end(), weight_basis.begin(), weight_basis.end());
	//	// 3. Second-derivative splines
	//	std::vector<bspline::Spline<double, order - 2>> d2;
	//	const bspline::operators::Dx<2> d2x;
	//	for (auto& bs : basis_splines)
	//		d2.push_back(d2x bs);
	//	// 4. Weighted derivatives
	//	std::vector<decltype(d2[0] * w_spline)> wd2;
	//	for (auto& bs2 : d2)
	//		wd2.push_back(bs2 * w_spline);
	//	// 5. Assemble smoothing matrix M 
	//	bspline::integration::BilinearForm<
	//		bspline::operators::IdentityOperator,
	//		bspline::operators::IdentityOperator>
	//		blf;

	//	const auto n = basis_splines.size();
	//	Eigen::MatrixXd M(n, n);
	//	for (int i = 0; i < n; ++i)
	//		for (int j = i; j < n; ++j)
	//		{
	//			double v = blf(wd2[i], d2[j]);
	//			M(i, j) = M(j, i) = v;
	//		}
	//	return M;
	//}

	//template <size_t order>
	//Eigen::MatrixXd build_order3_smoothing_matrix(
	//	const std::vector<bspline::Spline<double, order>>& basis_splines, const Eigen::ArrayXd& nodes,
	//	const std::vector<double>& smoothing_weights)
	//{
	//	// 1. Generate B-spline basis
	//	constexpr size_t weight_order = 1;
	//	auto weight_basis = clamped_spline_basis_functions<double, weight_order>(utils::to_std_vector(nodes));
	//	// bspline::generateBSplines<weight_order, double>(
	//	//	utils::to_std_vector(nodes));
	//	// 2. Fit weight function w(x)
	//	auto w_spline = bspline::linearCombination(
	//		smoothing_weights.begin(), smoothing_weights.end(), weight_basis.begin(),
	//		weight_basis.end());
	//	// 3. Second-derivative splines
	//	std::vector<bspline::Spline<double, order - 3>> d3; const bspline::operators::Dx<3> d3x;
	//	for (auto& bs : basis_splines)
	//		d3.push_back(d3x * bs);
	//	// 4. Weighted derivatives
	//	std::vector<decltype(d3[0] * w_spline)> wd3;
	//	for (auto& bs3 : d3)
	//		wd3.push_back(bs3 * w_spline);
	//	// 5. Assemble smoothing matrix M b
	//	bspline::integration::BilinearForm<
	//		bspline::operators::IdentityOperator, bspline::operators::IdentityOperator >
	//		blf;
	//	const auto n = basis_splines.size();
	//	Eigen::MatrixXd M(n, n);
	//	for (int i = 0; i < n; ++i)
	//		for (int j = i; j < n; ++j)
	//		{
	//			double v = blf(wd3[i], d3[j]);
	//			M(i, j) = M(j, i) = v;
	//		}
	//	return M;

	//}



	//template <size_t order>
	//Eigen::MatrixXd
	//	build_tension_matrix(
	//		const std::vector<bspline::Spline<double, order>>& basis_splines, const Eigen::ArrayXd& nodes,
	//		const std::vector<double>& smoothing_weights)
	//{
	//	// 1. Generate B-spline basis
	//	constexpr size_t weight_order = 1;
	//	auto weight_basis = clamped_spline_basis_functions<double, weight_order>(utils::to_std_vector(nodes));
	//	// bspline::generateBSplines<weight_order, double>(
	//	//	utils::to_std_vector(nodes));
	//	// 2. Fit weight function w(x)
	//	auto w_spline = bspline::linearCombination(
	//		smoothing_weights.begin(),
	//		smoothing_weights.end(),
	//		weight_basis.begin(),
	//		weight_basis.end());
	//	// 3. Second-derivative splines 
	//	std::vector<bspline::Spline<double, order - 1>> d1;
	//	const bspline::operators::Dx<1> d1x;
	//	for (auto& bs : basis_splines)
	//		d1.push_back(d1x * bs);
	//	// 4. Weighted derivatives
	//	std::vector<decltype(d1[0] * w_spline)> wd1;
	//	for (auto& bs1 : d1)
	//		wd1.push_back(bs1 * w_spline);
	//	// 5. Assemble smoothing matrix M 
	//	bspline::integration::BilinearForm<
	//		bspline::operators::IdentityOperator, bspline::operators::IdentityOperator >
	//		blf;

	//	const auto n = basis_splines.size(); Eigen::MatrixXd M(n, n);
	//	for (int i = 0; i < n; ++i)
	//		for (int j = i; j < n; ++j)
	//		{
	//			double v = blf(wd1[i], d1[j]);
	//			M(i, j) M(j, i) = v;
	//		}
	//	return M;
	//}

	template <size_t order>
	Eigen::MatrixXd build_smoothing_matrix(
		const std::vector<bspline::Spline<double, order>>& splines1, const std::vector<double>& smoothing_weights)
	{
		const bspline::operators::Dx<2> d2x;
		const auto blf = bspline::integration::BilinearForm(d2x, d2x);
		return build_symmetric_matrix(blf, splines1, smoothing_weights);
	}

	template <size_t order>
	Eigen::MatrixXd build_order3_smoothing_matrix(
		const std::vector<bspline::Spline<double, order>>& splines1, const std::vector<double>& smoothing_weights)
	{
		const bspline::operators::Dx<3> d3x;
		const auto blf = bspline::integration::BilinearForm(d3x, d3x);
		return build_symmetric_matrix(blf, splines1, smoothing_weights);
	}


	template <size_t order>
	Eigen::MatrixXd build_tension_matrix(
		const std::vector<bspline::Spline<double, order>>& splines1, const std::vector<double>& smoothing_weights)
	{

		const bspline::operators::Dx<1> dx;
		const auto blf = bspline::integration::BilinearForm(dx, dx);
		return build_symmetric_matrix(blf, splines1, smoothing_weights);
	}


}
