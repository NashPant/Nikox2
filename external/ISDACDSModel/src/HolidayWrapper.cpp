
#include "HolidayWrapper.h" 
#include "HolidayWrapper_C.h"


std::atomic<ISDAHoliday*> ISDAHoliday::pinstance{ nullptr };
ISDAHoliday::Cleanup ISDAHoliday::cleanup;
std::mutex ISDAHoliday::m_;


ISDAHoliday* ISDAHoliday::Instance() {
	if (pinstance ==nullptr) {
		std::lock_guard<std::mutex> lock(m_);
		if (pinstance == nullptr) {
			pinstance = new ISDAHoliday();
		}
	}
	return pinstance;
}

THolidayList* get_Holiday(const char* cal_name) {
	ISDAHoliday* holi_mappy = ISDAHoliday::Instance();
	return holi_mappy->get_Holiday(cal_name).get();
}