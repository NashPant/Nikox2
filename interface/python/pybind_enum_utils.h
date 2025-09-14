
#pragma once
#include <magic_enum/magic_enum.hpp>
//#include "TPTools/TPException.h"

namespace TP::utils
{
	namespace py = pybind11;

	template <typename Enum>
	void expose_enum(py::module& m, const std::string& enum_name)
	{
		py::enum_<Enum> pyenum(m, enum_name.c_str());
		for (Enum value:  magic_enum::enum_values<Enum>())
		{
			auto name = magic_enum::enum_name(value);
			pyenum.value(name.data(), value);
		}


		pyenum.export_values();
		pyenum.def_static("__new__", [](py::object, const std::string& name)
			{
				auto opt = magic_enum::enum_cast<Enum>(name);
				//TP_REQUIRE(!!opt) ("The enum-name" + name + " is not valid.");
				return *opt;
			});
	}

}
			// namespace utils