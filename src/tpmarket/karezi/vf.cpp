
#include <gsl/gsl_errno.h>
#include "vf.h"


static gsl_error_handler_t * old_handler = gsl_set_error_handler_off();
namespace TP::market::karezi
{
	/* Boost hash_combine */
	inline void hash_combine(size_t& seed, const size_t& v)
	{
		const size_t m = 0xe9846af9b1a615d;
		size_t x = seed + 0x9e3779b9 + v;
		x ^= x >> 32;
		x *= m;
		x ^= x >> 32;
		x *= m;
		x ^= x >> 28;
		seed = x;
	}
	/* Hash function for VFKey */
	size_t hash_VFKey(const VFKey& vf_key) noexcept
	{
		size_t seed = 0;
		for (const VFKeyPart& kp : vf_key.key)
			hash_combine(seed, std::hash<VFKeyPart>{}(kp));
		return seed;
	}
	std::ostream& operator<<(std::ostream& os, const VFKey& vf_key)
	{
		os << "(";
		for (size_t i = 0; i < vf_key.size(); ++i) {

			if (i > 0)
				os << ", ";
			if (std::holds_alternative<bool>(vf_key[i]))
				os << std::get<bool>(vf_key[i]);
			else if (std::holds_alternative<int>(vf_key[i]))
				os << std::get<int>(vf_key[i]);
			else if (std::holds_alternative<std::string>(vf_key[i]))
				os << std::get<std::string>(vf_key[i]);
			else
				throw std::runtime_error("Unsupported result type in operator<<(std::ostream &os, const VFKey &vf_key)");
		}
		os << ")";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, MktSvc svc)
	{
		os << static_cast<size_t>(svc);
		return os;

	}
	std::ostream& operator<<(std::ostream& os, const MarketAddr& addr)
	{
		os << static_cast<size_t>(addr.first) << ", " << addr.second;
		return os;
	}

}