#if !defined TP_CUBICSOLVER_
#define TP_CUBICSOLVER_


#include "TPTools/TPException.h"
#include "TPMaths\Constants.h"

#include <cmath>
#include <vector>
#include <array>

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned short ushort;



namespace TPMaths
{

	template <typename T>
	typename T::return_t calc_roots(double a, double b, double c)
	{
		double Q = (a*a - 3.0*b) / 9.0;
		double R = (2.0*a*a*a - 9.0*a*b + 27.0*c) / 54.0;

		double sqrtQ = sqrt(Q);

		double R2 = R * R;
		double Q3 = Q * Q*Q;

		double discriminant = R2 - Q3;

		if (discriminant < 0)
		{
			//3 real roots (all unequal)
			double theta = acos(R / sqrt(Q3));

			double x1 = -2.0*sqrtQ * cos(theta / 3.0) - a / 3.0;

			double x2 = -2.0*sqrtQ * cos((theta + 2.0*Constants::Pi()) / 3.0) - a / 3.0;
			double x3 = -2.0*sqrtQ * cos((theta - 2.0*Constants::Pi()) / 3.0) - a / 3.0;

			return T::template pick<3>( { x1,x2,x3 } );
		}
		else if (abs(discriminant) < 1E-15)
		{
			//it means R2 = Q3 which implies A = B
			//3 real roots and al least two are equal
			double A = -pow(R, 1.0 / 3.0);
			double B = A;

			double x1 = (A + B) - a / 3.0;
			double x2 = -0.5*(A + B) - a / 3.0;
			//std::array<double, 2> v2 = { x1, x2 };
			//double v2[] = { x1,x2 };
			return T::template pick<2>({ x1,x2 });
			//return T::template pick<2>(v2);
		}
		else
		{
			//1 real root

			//double A = - Math.pow(R + Math.sqrt(discriminant), 1.0/3.0); //blows up!
			double psi = R + sqrt(discriminant);
			double A;
			if (psi >= 0.0) {
				A = -exp(1.0 / 3.0*log(psi));
			}
			else {
				A = exp(1.0 / 3.0*log(-psi));
			}

			double B;
			if (abs(A) < 1E-15) {
				B = 0;
			}
			else {
				B = Q / A;
			}

			double x1 = (A + B) - a / 3.0;
			return T::template pick<1>({ x1 });
		}
	}

	struct SmallestNonNegative {
		typedef double return_t;
		template <size_t N>
		static double pick(std::array<double, N> xs)
		{
			double r = -1;
			for (const auto&x : xs) {
				if (x >= 0 && (x < r || r < 0)) {
					r = x;
				}
			}
			if (r >= 0) {
				return r;
			}
			else
			{
				THROW("root is negative");
			}
		}
	};

	inline
	double calc_cubic_smallest_nonneg_root(double a, double b, double c)
	{
		return calc_roots<SmallestNonNegative>(a, b, c);
	}

}

#endif