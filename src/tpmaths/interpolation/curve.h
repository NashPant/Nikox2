
#pragma once
#include <bspline/operators/Derivative.h>
//#include <utils/enums.h>
#include "tptools/TPException.h"
#include <vector>
#include "basis_spline.h"
#include "tpmaths/math_enums.h"


namespace TP::maths
{
	template<typename T, size_t order> class BSCurve {
		std::vector<T> _xs;
		std::vector<T> _ys;
		std::array<Boundary<T>, order - 1> _boundaries;
		std::vector<bspline::Spline<T, order>> _basis;
		Eigen::Matrix<T, -1, 1> _coeffs;

		ExtrapolationType _extrap_lhs{ ExtrapolationType::LINEAR };
		ExtrapolationType _extrap_rhs{ ExtrapolationType::LINEAR };

		T _x0;
		T _xN;
		T _y0;
		T _yN;
		T _dvdx0;
		T _dvdxN;

		auto extrapolate(T slope, T x_val, T x_lim, T y_lim) const
		{
			return y_lim + slope * (x_val - x_lim);
		}

		auto interpolate(T x) const
		{
			T result = T(0.0);
			for (auto i = 0; i < _basis.size(); ++i)
				result += _coeffs(i) * _basis[i](x);
			return result;
		}

		Boundary<T> boundary(ExtrapolationType type, Node node) const
		{
			static_assert(order >= 1, "Order may not be zero.");
			if (type == ExtrapolationType::LINEAR)
				return Boundary<T> { node, 2, T(0) };
			return Boundary<T> { node, 1, T(0) };
		}


		std::array<Boundary<T>, order - 1> get_boundaries(
			ExtrapolationType lhs, ExtrapolationType rhs) const
		{
			static_assert(order >= 1, "Order may not be zero.");
			if constexpr (order == 2)
				return { boundary(lhs, Node::FIRST) };
			else if constexpr (order == 3)
				return { boundary(lhs, Node::FIRST), boundary(rhs, Node::LAST) };
			else if constexpr (order == 4)
				return { boundary(lhs, Node::FIRST),
				Boundary<T> { Node::FIRST, 3, T(0) },
				boundary(rhs, Node::LAST) };
			else if constexpr (order == 5)
				return { boundary(lhs, Node::FIRST),
				Boundary<T> { Node::FIRST, 3, T(0) },
				 boundary(rhs, Node::LAST),
				Boundary<T> { Node::LAST, 3, T(0) } };
			else
				return {};
		};

		auto basis_splines(const std::vector<T>& xs) const
		{
			return clamped_spline_basis_functions<T, order>(xs);
		}

		void compute_boundary_values()
		{
			_x0 = _xs.front();
			_xN = _xs.back();
			_y0 = _ys.front();
			_yN = _ys.back();
			_dvdx0 = T(0.0);
			_dvdxN = T(0.0);
			const auto dx = bspline::operators::Dx<1>{};

			if (_extrap_lhs != ExtrapolationType::CONSTANT)
				for (auto i = 0; i < _basis.size(); ++i)
					_dvdx0 += (dx * _basis[i])(_x0) * _coeffs[i];

			if (_extrap_rhs != ExtrapolationType::CONSTANT)
				for (auto i = 0; i < _basis.size(); ++i)
					_dvdxN += (dx * _basis[i])(_xN) * _coeffs[i];
		}

		auto basis_spline_coeffs() const
		{
			TP_REQUIRE(_xs.size() == _ys.size(), "Curve: xs and ys must have the same size.");
			TP_REQUIRE( _xs.size() >= order + 1, "Curve: xs must have at least order + points."); 
			TP_REQUIRE( std::is_sorted(_xs.begin(), _xs.end()), "Curve: xs must be sorted ascending");
			const auto M = compute_spline_interp_design_matrix<T, order>(
				_basis, _xs, _boundaries);
			const auto targets =
				assemble_spline_targets<T, order>(_ys, _boundaries);
			return M.colPivHouseholderQr().solve(targets).eval();
		}
	public:
		BSCurve(const std::vector<T>& xs,
			const std::vector<T>& ys,
			ExtrapolationType lhs = ExtrapolationType::LINEAR, 
			ExtrapolationType rhs = ExtrapolationType::LINEAR)
			: _xs(xs)
			, _ys(ys)
			, _boundaries(get_boundaries(lhs, rhs))
			, _basis(basis_splines(_xs))
			, _coeffs(basis_spline_coeffs())
			, _extrap_lhs(lhs) // allow a constant extrapolation override ,
			, _extrap_rhs(rhs)
		{
			// allow a constant extrapolation override
			compute_boundary_values();
		}
		BSCurve(
			const std::vector<T>& xs,
			const std::vector<T>& ys,
			const std::array<Boundary<T>, order - 1>& boundaries)
			: _xs(xs)
			, _ys(ys)
			, _boundaries(boundaries)
			, _basis(basis_splines(_xs))
			, _coeffs(basis_spline_coeffs())
		{
			compute_boundary_values();
		}

		BSCurve(
			std::vector<bspline::Spline<T, order>> basis,
			const std::vector<T>& xs, // todo - should be from basis
			const std::vector<T>& ys,
			const std::array<Boundary<T>, order - 1>& boundaries)
			: _xs(xs)
			, _ys(ys)
			, _boundaries(boundaries)
			, _basis(basis)
			, _coeffs(basis_spline_coeffs())
		{
			compute_boundary_values();
		}

		auto value(T x) const -> T
		{
			if (x < _xs.front())
			{
				return extrapolate(_dvdx0, x, _x0, _y0);
			}
			if (x > _xs.back())
			{
				return extrapolate(_dvdxN, x, _xN, _yN);
			}
			return interpolate(x);
		}
	};


	template <typename T, size_t order> class BSCurveGenerator {
		std::vector<T> _xs;
		std::vector<bspline::Spline<T, order>> _basis;

		auto basis_splines(const std::vector<T>& xs) const
		{
			return clamped_spline_basis_functions<T, order>(xs);
		}

	public:
		BSCurveGenerator(const std::vector<T>& xs)
			: _xs(xs)
			, _basis(basis_splines(_xs))
		{
		}
		auto generator_BSpline(
			const std::vector<T>& ys,
			const std::array<Boundary<T>, order -1>& boundaries) const
		{
			return BSCurve<T, order>(_basis, _xs, ys, boundaries);
		}
	};
}		// namespace math