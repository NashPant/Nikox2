
#include <vector>
#include <tpdatetime/Date.h>


namespace TP::curves
{

	inline double curve_year_fraction(date_time::Date d1, date_time::Date d2)
	{
		//return std::chrono::duration<double, std::ratio<365, 1>>(d1 - d2).count();
		return (d1 - d2).count() / 365.0;
	}

	// helper function to prepare flat - b-spline curve segments
	inline std::vector<date_time::Date> divide_dates(std::vector<date_time::Date> dates,
		date_time::Date cutoff_date, bool dates_left = true,
		bool add_cutoff_date = true)
	{
		if (dates.empty() || (dates_left && cutoff_date <= dates.front()) || (!dates_left && cutoff_date >= dates.back()))
		{
			return std::vector<date_time::Date>();
		}
		// -- * ----* -----* ----* ----* ----* --
		//			    ^ cutoff_date
		// -- * ----* --* (left)
		//	(right)		*--* ----* ----* ----* --

		auto dt_it = std::upper_bound(dates.begin(), dates.end(), cutoff_date);
		if (dates_left)
		{
			std::vector<date_time::Date> left_dates(dates.begin(), dt_it);
			// add another date at the cutoff point and extrapolate flat
			if (add_cutoff_date && dt_it != dates.end() && left_dates.back() != cutoff_date) 
				left_dates.push_back(cutoff_date);
			return left_dates;
		}
		else
		{
			std::vector<date_time::Date> right_dates;
			if (add_cutoff_date && dt_it != dates.begin())
			{
				right_dates.push_back(cutoff_date);
			}
			std::copy(dt_it, dates.end(), std::back_inserter(right_dates));
			return right_dates;
		}
	}
		// end of divide_dates
}