
#ifndef HOLIDAYWRAPPER_HPP 
#define HOLIDAYWRAPPER_HPP


#include <atomic> 
#include <mutex>
#include <stdexcept>
#include <map>
#include <string>
#include <memory>

#include <buscache.h>


class ISDAHoliday {
public:
	static ISDAHoliday* Instance();


	void add_holiday(const std::string& cal_name, const std::shared_ptr<THolidayList>& hol)
	{
		auto it = m_holi_mappy.find(cal_name);
		if (it == m_holi_mappy.end()) {
			m_holi_mappy.insert({ cal_name, hol });
		}

	}

	std::shared_ptr<THolidayList> get_Holiday(const std::string& cal_name) const {
		auto it = m_holi_mappy.find(cal_name);
		if (it != m_holi_mappy.end()) {
			return it->second;
		}
		else {
			return nullptr;
		}
	}

private:
	ISDAHoliday() {
		m_holi_mappy["NONE"] = std::shared_ptr<THolidayList>(JpmcdsHolidayListNewGeneral(NULL, JPMCDS_WEEKEND_SATURDAY | JPMCDS_WEEKEND_SUNDAY),
			[](THolidayList* ptr) {JpmcdsHolidayListDelete(ptr); });
		m_holi_mappy["NO_WEEKENDS"] = std::shared_ptr<THolidayList>(JpmcdsHolidayListNewGeneral(NULL, JPMCDS_WEEKEND_NO_WEEKENDS),
			[](THolidayList* ptr) {JpmcdsHolidayListDelete(ptr); });
	}

	~ISDAHoliday() {}

	struct Cleanup {
		~Cleanup() {
			if (ISDAHoliday::pinstance) {
				std::lock_guard<std::mutex> lock(m_);
				if (ISDAHoliday::pinstance) {
					delete ISDAHoliday::pinstance;
					ISDAHoliday::pinstance = nullptr;
				}
			}
		}
	};
	static std::atomic<ISDAHoliday*> pinstance;
	static Cleanup cleanup;
	static std::mutex m_;
	std::map<std::string, std::shared_ptr<THolidayList>> m_holi_mappy;
};

#endif /* HOLIDAYWRAPPER_HPP */