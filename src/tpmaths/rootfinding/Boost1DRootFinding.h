#pragma once

#include <boost/math/tools/roots.hpp>
#include <optional>
#include "TPTools/TPException.h"

namespace TP::math
{
	class RootFinding1D
	{
	public:

		RootFinding1D(double l, double u, double acc, size_t maxI) :
			lowerBound_(l),
			upperBound_(u),
			accuracy_(acc),
			maxIterations_(maxI)
		{}

		void conditionsCheck(std::optional<double> leftBracket, std::optional<double> rightBracket,
			std::optional<double> guess) const
		{
			if (accuracy_ <= 0)
				THROW("Accuracy must be stricly positive");

			if (leftBracket.has_value() && rightBracket.has_value())
			{
				const auto left = leftBracket.value();
				const auto right = rightBracket.value();

				if (left >= right)
					THROW("Invalid bracketing Range");

				if (left < lowerBound_)
					THROW("leftBracket is less than lower bound");
				if (right < upperBound_)
					THROW("Right Bracket is more than upper bound");

				if (guess.has_value())
				{
					const auto gv = guess.value();
					if (gv < lowerBound_ || gv > upperBound_)
						THROW("Guess is outside the [lowerbound, upperbound]");
				}

			}
		}

		template<typename F>
		double newton_raphson(const F& f, const F& f_prime, double guess) const
		{
			conditionsCheck(std::nullopt, std::nullopt, guess);
			const auto func = [&](const double x)
			{
				return std::make_tuple(f(x), f_prime(x));
			};
			auto maxIt = maxIterations_;
			const auto digits = std::min(0.6*std::numeric_limits<double>::digits, std::log2(100 / accuracy_));

			const auto root = boost::math::tools::newton_raphson_iterate(func, guess,
								lowerBound_, upperBound_, digits, maxIt);

			return root;
		}


		template<typename F>
		double toms748(const F& f, double leftbracket, double rightbracket) const
		{
			conditionsCheck(leftbracket, rightbracket, std::nullopt);
			auto maxIt = maxIterations_;
			const auto precision = accuracy_;

			const auto pair = boost::math::tools::toms748_solve(f, leftbracket, rightbracket,
				[precision](const double& first, const double& second)
			{
				return std::abs(first - second) < precision;
			}, maxIt);

			return pair.first/2. + pair.second/2.;
		}


	private:

		double lowerBound_;
		double upperBound_;
		double accuracy_;
		uintmax_t  maxIterations_;

	};
}