
#include <Eigen/Dense> 
#include <bspline/Core.h> 
#include <vector>
#include "basis_spline.h"
namespace TP::maths

{
	Eigen::ArrayXXd
		build_basis_matrix_with_linear_extrapolation(
			const Eigen::ArrayXd& xs,
			double x0,
			double xN,
			const Eigen::ArrayXXd& S_basis,
			const Eigen::ArrayXXd& S_basis_outer,
			const Eigen::ArrayXXd& dS_basis_outer)
	{
		const auto n_bases = S_basis_outer.cols();
		// values
		const Eigen::ArrayXd S_basis_x0 = S_basis_outer.row(0);
		const Eigen::ArrayXd S_basis_xN = S_basis_outer.row(1);
		// derivatives
		const Eigen::ArrayXd dS_basis_x0 = dS_basis_outer.row(0);
		const Eigen::ArrayXd dS_basis_xN = dS_basis_outer.row(1);
		// Build the design matrix M considering linear extrapolation 
		Eigen::ArrayXXd M(xs.size(), n_bases);
		for (auto i = 0; i < xs.size(); ++i)
		{
			const double x = xs[i];
			if (x < x0)
			{
				// Linear extrapolation on the left
				M.row(i) = S_basis_x0 + (x - x0) * dS_basis_x0;
			}
			else if (x > xN)
			{
				// Linear extrapolation on the right
				M.row(i) = S_basis_xN + (x - xN) * dS_basis_xN;
			}
			else
			{
				// Within the knot span 
				M.row(1) = S_basis.row(i);
			}
		}
		return M;
	}
}
