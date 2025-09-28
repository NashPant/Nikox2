
#pragma once
#include <memory>
#include <vector>
#include <map>
#include <unordered_set>
#include "curve_enums.h"
#include <tpdatetime/Date.h>
#include <tptools/Utils/NotNull.h>
#include <tptools/serialisation/include.h>
#include "curve_utils.h"
#include <tpmarketdata/marketcurves/discount_factors.h>
//
namespace TP::curves
{
	using TP::date_time::Date;


	enum class InterpolationType
	{
		FlatForward,
		FlatBSplines3,
		FlatBSplines4, // order 4
		FlatBSplines5, // order 5
	};

	struct DiscountCurveConfig
	{
		InterpolationType interp_type = InterpolationType::FlatForward;
		Date cutoff_date{};
		bool use_dfs_input = true;
	};

	FWD_DECLARE_SHAREDPTR(DiscountCurve)

		class DiscountCurve {
		public:

			DiscountCurve(Date base_date)
				: _base_date(base_date)
			{
			}
			DiscountCurve() = default;

			virtual ~DiscountCurve() = default;

			virtual double	discount_factor(Date start_date, Date end_date) const
			{
				return discount(end_date) / discount(start_date);
			}

			virtual double	discount(Date date) const = 0;

			virtual Date base_date() const
			{
				return _base_date;
			}



		protected:
			Date _base_date;
	};



	// helper function to create a discount curve 

	CPtr<DiscountCurve> create_market_discount_curve(const std::vector<Date>& dates,
		const std::vector<double>& values,
		const DiscountCurveConfig& config = DiscountCurveConfig{});


	class SpreadDiscountCurve : public DiscountCurve {
	public:
		virtual ~SpreadDiscountCurve() = default;


		SpreadDiscountCurve(
			CPtr<DiscountCurve> base_curve, CPtr<DiscountCurve> spread_curve)
			: _base_curve(base_curve)
			, _spread_curve(spread_curve)
		{
			// use the base curves base date (allows for simple spread curves) 
			_base_date = _base_curve->base_date();
		}
		virtual double discount(Date date) const override
		{
			// apply the spread on top of the base curve
			return _base_curve->discount(date) * _spread_curve->discount(date);
		}

		CPtr<DiscountCurve> base_curve() const { return _base_curve; }
		CPtr<DiscountCurve> spread_curve() const { return _spread_curve; }

	private:
		CPtr<DiscountCurve> _base_curve;
		CPtr<DiscountCurve> _spread_curve;
	};


	inline CPtr<DiscountCurve> create_spread_curve_impl(CPtr<DiscountCurve> base_curve,
		CPtr<DiscountCurve> spread_curve)
	{
		return std::make_shared<SpreadDiscountCurve>(base_curve, spread_curve);
	}
	// helper function with purpose to work in python
	// not great but working around "safety" until we tie down the interface inline CPtr<::market::DiscountCurve> create_market_spread_curve(
	//CPtr<::market::DiscountCurve> base_curve, CPtr<::market::DiscountCurve> spread_curve)
	//return std::make_shared<SpreadDiscountCurve>(base_curve, spread_curve);
	// 
	// a wrapper around maps of curves that store dates and discount factors in a // has-table
	class CachedDiscountCurve : public DiscountCurve {
	public:
		virtual ~CachedDiscountCurve() = default;

		CachedDiscountCurve(CPtr<DiscountCurve> underlying_curve,
			std::unordered_set<Date> dates);


		CachedDiscountCurve(
			CPtr<DiscountCurve> underlying_curve,
			std::vector<Date> dates)
			: CachedDiscountCurve(
				underlying_curve,
				std::unordered_set<Date>(dates.begin(), dates.end()))
		{
		}

		double discount(Date date) const override;
	private:
		CPtr<DiscountCurve> _underlying_curve;
		std::unordered_set<Date> _dates;
		std::unordered_map<Date, double> _cached_discounts;
	};


	class ShiftedDiscountCurve : public DiscountCurve {
		CPtr<DiscountCurve> _underlying_curve;
		CurveRollType _curve_roll_type;
	public:
		ShiftedDiscountCurve(
			CPtr<DiscountCurve> underlying_curve,
			Date new_base_date,
			CurveRollType curve_roll_type);
		double discount(Date date) const override;
	};
	// TODO: this is a very bad choice of name. Sounds like a generic zero curve. 
	// 
	class ZeroDiscountCurve : public DiscountCurve {
	public:
		double discount_factor(Date, Date) const override
		{
			return 1.0;
		};
		double discount(Date) const override { return 1.0; };
		PROPERTIES(ZeroDiscountCurve);
	};



	class FlatDiscountCurve : public DiscountCurve {
		double _rate;
	public:
		FlatDiscountCurve() = default;
		explicit FlatDiscountCurve(double rate, Date base_date)
			: _rate(rate)
		{
			_base_date = base_date;
		}

		double rate() const { return _rate; }
		double discount_factor(Date start, Date end) const override
		{
			return std::exp(-_rate * curve_year_fraction(end, start));
		};


		double discount(Date date) const override
		{
			return std::exp(-_rate * curve_year_fraction(date, _base_date));
		};
		PROPERTIES(FlatDiscountCurve, _rate, _base_date);
	};



	class FlatFwdDiscountCurve : public DiscountCurve {
	public:
		FlatFwdDiscountCurve() = default;
		FlatFwdDiscountCurve(const market_data::DiscountFactors& mkt_data) :
			FlatFwdDiscountCurve(mkt_data.dates(), mkt_data.dfs())
		{
		}

		FlatFwdDiscountCurve(
			std::vector<Date> dates, std::vector<double> dfs);

		FlatFwdDiscountCurve(
			Date as_of_date,
			std::vector<Date> dates,
			std::vector<double> dfs);

		// useful for fast fitting (maybe this shouldn't be here?)
		FlatFwdDiscountCurve(
			std::vector<Date> dates,
			std::vector<double> log_dfs,
			std::vector<double> year_fracs);


		double discount_factor(Date start, Date end) const override;
		double discount(Date end) const override;

		const auto& dates() const { return _dates; }
		const auto& dfs() const { return _dfs; }
		const auto& log_dfs() const { return _log_dfs; }
		const auto& year_fractions() const { return _year_fractions; }
	private:
		double log_discount_factor(Date date) const;
		std::vector<Date> _dates;
		std::vector<double> _dfs;
		std::vector<double> _log_dfs;
		std::vector<double> _year_fractions;

		PROPERTIES(FlatFwdDiscountCurve,
			_dates, _dfs,
			_log_dfs,
			_year_fractions,
			_base_date);
	};

	class FlatFwdFitterDiscountCurve : public DiscountCurve {
	public:
		FlatFwdFitterDiscountCurve() = default;
		FlatFwdFitterDiscountCurve(
			std::vector<Date> dates, std::vector<double> flat_fwds);
		// useful for fast fitting (maybe this shouldn't be here?)
		FlatFwdFitterDiscountCurve(
			std::vector<Date> dates, std::vector<double> flat_fwds, std::vector<double> year_fracs);


		double discount_factor(Date start, Date end) const override;
		double discount(Date end) const override;

		// compute list of discount factors (should be fast if input is sorted) 
		std::vector<double> discount_factors(std::vector<Date> dates) const;
		const auto& dates() const { return _dates; }
		const auto& dfs() const { return _dfs; }
		const auto& log_dfs() const { return _log_dfs; }
		const auto& flat_fwds() const { return _flat_fwds; }
		const auto& year_fractions() const { return _year_fractions; }


	private:
		double log_discount_factor(Date date) const;
		std::vector<Date> _dates;
		std::vector<double> _flat_fwds;
		std::vector<double> _year_fractions;
		std::vector<double> _dfs;
		std::vector<double> _log_dfs;
		PROPERTIES(
			FlatFwdFitterDiscountCurve,
			_dates, _dfs,
			_log_dfs,
			_flat_fwds,
			_year_fractions);
	};

		template <size_t order>
		std::array<math::Boundary<double>, order - 1> get_boundaries()
		{
			static_assert (order >= 1, "Order may not be zero.");
			if constexpr (order == 2)
				return { math::Boundary<double> { math::Node::FIRST, 2, 0.0 } };
			else if constexpr (order == 3)
				return { math::Boundary<double> { math::Node::FIRST, 3, 0.0 },
				math::Boundary<double> { math::Node::LAST, 3, 0.0 } };
			else if constexpr (order == 4)
				return { math::Boundary<double> { math: : Node::FIRST, 3, 0.0},
				math::Boundary<double> { math::Node::LAST, 3, 0.0 },
				math::Boundary<double> { math: : Node::LAST, 4, 0.0 } };
			else if constexpr (order == 5)
				return { math::Boundary<double> { math: : Node::FIRST, 3, 0.0 },
				math: : Boundary<double> { math::Node::FIRST, 4, 0.0},
				math::Boundary<double> { math::Node::LAST, 3, 0.0 },
				math::Boundary<double> { math: : Node::LAST, 4, 0.0 } };
			else
				return {};
		};

	//std::shared_ptr<const DiscountCurve> create_discount_curve(
	//	const std::vector<Date>& dates,
	//	const std::vector<double>& values,
	//	const DiscountCurveConfig& config = DiscountCurveConfig{});

	//std::shared_ptr<const DiscountCurve> create_spread_curve(
	//	std::shared_ptr<const DiscountCurve> base_curve, std::shared_ptr<const DiscountCurve> spread_curve);

	// namespace market
}
// namespace QMA