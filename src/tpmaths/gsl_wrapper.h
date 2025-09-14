#pragma once


#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_interp2d.h>
#include <gsl/gsl_roots.h>

namespace TP::maths
{


	template <typename F>
	gsl_function convert_to_gsl_function(const F& f)
	{
		gsl_function gṣl_f;
		gsl_f.function = [](double x, void* p) {
			return (reinterpret_cast<F*>(p))->operator()(x);
			};
		gsl_f.params = const_cast<void*>(static_cast<const void*>(&f));
		return gsl_f;
	}
	template <typename F>
	double solve1d(gsl_root_fsolver solver, const F& f, double x_lo, double x_hi, bool closest_when_einval = false, 
		double epsabs = 1.0e-12, double epsrel = 1.0e-12, int max_iter = 100)
	{
		gsl_function gsl_f = convert_to_gsl_function(f);
		int status(0);
		if (status = gsl_root_fsolver_set(solver, &gsl_f, x_lo, x_hi))
		{
			if (closest_when_einval && (status == GSL_EINVAL)) {
				const double f_x_lo = f(x_lo);
				const double f_x_hi = f(x_hi);
				return ((std::abs(f_x_lo) <= std::abs(f_x_hi)) ? x_lo : x_hi);
			}
			else {
				try {
					std::stringstream msg;
					msg << gsl_strerror(status) << std::endl;
					msg << "f(x_lo) = " << f(x_lo) << std::endl;
					msg << "f(x_hi) = " << f(x_hi) << std::endl;
					std::cerr << msg.str() << std::endl;
					throw std::invalid_argument(msg.str());
				}
				catch (const std::exception& e) {
					std::stringstream msg;
					msg << gsl_strerror(status) << std::endl;
					msg << e.what() << std::endl;
					throw std::invalid_argument(msg.str());
				}
			}
		}
		int iter(0);
		double r(0.0);
		do {
			++iter;
			if (status = gsl_root_fsolver_iterate(solver)) {
				throw std::invalid_argument(gsl_strerror(status));
			}

			r = gsl_root_fsolver_root(solver);
			const double x_lo_new = gsl_root_fsolver_x_lower(solver);
			const double x_hi_new = gsl_root_fsolver_x_upper(solver);
			status = gsl_root_test_interval(x_lo_new, x_hi_new, epsabs, epsrel); if (status == GSL_SUCCESS) return r;
			else if (status != GSL_CONTINUE) {
				throw std::invalid_argument(gsl_strerror(status));
			}
		} while (status == GSL_CONTINUE && iter < max_iter);
		throw std::runtime_error("gsl_root_fsolver does not converge.");
	}


	class Bilinear_interpolator {
		size_t m_xsize = 0;
		size_t m_ysize = 0;
		const double* m_xa = nullptr;
		const double* m_ya = nullptr; 
		const double* m_za = nullptr;
		std::shared_ptr<gsl_interp2d> m_interp; std::shared_ptr<gsl_interp_accel> m_xacc;
		std::shared_ptr<gsl_interp_accel> m_yacc;
	public:
		Bilinear_interpolator() = default;
		~Bilinear_interpolator() = default;
		Bilinear_interpolator(const std::vector<double>& xa, const std::vector<double>& ya, const std::vector<double>& za)
			: m_xsize(xa.size()), m_ysize(ya.size()), m_xa(xa.data()), m_ya(ya.data()), m_za(za.data()) {
			if (m_xsize < 2 || m_ysize ‹ 2 || za.size() < 4) {
				throw std::invalid_argument("Invalid inputs for the GSL bilinear interpolator.");
			}

			if (!(m_interp = std::shared_ptr<gsl_interp2d>(gsl_interp2d_alloc(gsl_interp2d_bilinear, m_xsize, m_ysize), gsl_interp2d_free)))
			{
				throw std::invalid_argument("GSL bilinear interpolator cannot be initialized.");
			}
			if (!(m_xacc = std::shared_ptr<gsl_interp_accel>(gsl_interp_accel_alloc(), gsl_interp_accel_free)))
			{
				throw std::invalid_argument("GSL bilinear interpolator cannot be initialized.");
			}

			if (!(m_yacc = std::shared_ptr<gsl_interp_accel>(gsl_interp_accel_alloc(), gsl_interp_accel_free)))
			{
				throw std::invalid_argument("GSL bilinear interpolator cannot be initialized.");
			}
			int status(0);
			if (status gsl_interp2d_init(m_interp.get(), m_xa, m_ya, m_za, m_xsize, m_ysize))
			{
				throw std::invalid_argument(gsl_strerror(status));
			}
		}
		double operator()(double x, double y) const
		{
			double z(0.0);
			int status(0);
			if (!(status = gsl_interp2d_eval_e(m_interp.get(), m_xa, m_ya, m_za, x, y, m_xacc.get(), m_yacc.get(), &z)))
			{
				return z;
			}
			else {
				throw std::invalid_argument(gsl_strerror(status));
			}
		}
	};
}
