#pragma once


#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map> 
#include <memory> 
#include <sstream> 
#include <string> 
#include <set> 
#include <utility> 
#include <variant> 
#include <vector>
#include "tpdatetime/Date.h"

namespace TP::market::karezi
{
	using namespace date_time;

	/* VF key */
	using VFKeyPart = std::variant<bool, int, std::string>;
	struct VFKey;
	std::ostream& operator<<(std::ostream& os, const VFKey& vf_key);


	struct VFKey {
		std::vector<VFKeyPart> key;
		VFKey() = default;
		VFKey(const std::vector<VFKeyPart>& key_) : key(key_) {}
		const VFKeyPart& operator[](size_t i) const { return key.at(i); }
		size_t size() const { return key.size(); }
		const VFKeyPart& front() { return key.front(); }
		bool empty() const { return key.empty(); }
		friend bool operator<(const VFKey& lhs, const VFKey& rhs)
		{
			for (size_t i = 0; i < lhs.size() && i < rhs.size(); i++)
				if (lhs[i] != rhs[i])
					return lhs[i] < rhs[i];

			return lhs.size() < rhs.size();
		}


		friend bool operator==(const VFKey& lhs, const VFKey& rhs)
		{
			for (size_t i = 0; i < lhs.size() && i < rhs.size(); i++)
				if (lhs[i] != rhs[i])
					return false;
			return (lhs.size() == rhs.size());
		}
		std::string to_string() const
		{
			std::stringstream ret;
			ret << *this;
			return ret.str();
		}
	};
	template <typename... Args>
	VFKey make_VFKey(Args&&... args)
	{
		VFKey result;
		(result.key.emplace_back(std::forward<Args>(args)), ...);
		return result;
	}


	extern size_t hash_VFKey(const VFKey& vf_key) noexcept;
	/* Static env */
	enum class StcSvc {
		Calendar,
		DiscountCurveClass,
		CDSConventions,
		IndexCDSConventions,
		PoolConstituents,
		CreditEvents,
		TrancheParams,
		BondRef,
		BondModelInfo
	};
	using StaticAddr = std::pair<StcSvc, VFKey>;

	struct StaticObject {
		virtual ~StaticObject() = default;
	};
	using StaticVal = std::variant<bool, int, double, std::string, VFKey, std::shared_ptr<StaticObject> >;

	struct StaticEnv {
		virtual const StaticVal Value(const StaticAddr& addr) const = 0;
	};

	/* Time service */
	enum class TimeSvc {
		today,
		pv_date,
		roll_date
	};

	using TimeService = std::map<TimeSvc, Date>;
	/* Market env */
	enum class MktSvc {
		DiscountCurve,
		IRIndexCurve,
		IRNVolGrid,
		IRSVMAlphaGrid,
		IRSVMBetaGrid,
		IRSVMRhoGrid,
		IRSVMShiftGrid,
		IRSVMVol0bject,
		IRIndexFixings,
		FXSpotBase,
		FXSpot,
		CXFactor,
		DefaultCurve,
		IndexDefaultCurve,
		IndexConstituentDefaultCurve,
		Recovery,
		QuoteRecovery,
		IDRRecovery,
		IndexRecovery,
		CDSQuotes,
		IndexCDSQuotes,
		BondCleanPrice,
		BondCashflow,
		LoanCashflow,
		BondAuxMktInfo,
		TrancheParamsMod,
		CrdtIdxOptVols,
		SRTCleanPrice,
		EqSpot,
		EqOptListedPrice,
		FutPrice,
		FutOptPrice
	};

	std::ostream& operator<<(std::ostream& os, MktSvc svc);

	using MarketAddr = std::pair<MktSvc, VFKey>;

	std::ostream& operator<<(std::ostream& os, const MarketAddr& addr);

	using MarketDeps = std::set<MarketAddr>;



	struct MarketObject {
		virtual ~MarketObject() = default;
	};

	using MarketVal = std::variant<bool, int, double, std::string, std::shared_ptr<MarketObject> >;

	class MarketEnv {
		using MarketEnvMap = std::map<MarketAddr, MarketVal>;
		MarketEnvMap m_mkt_env;

	public:
		virtual const MarketVal& Value(const MarketAddr& addr) const
		{
			if (MarketEnvMap::const_iterator p = m_mkt_env.find(addr); p != m_mkt_env.cend())
				return p->second;
			else {
				std::stringstream msg;
				msg << "Market address not found: " << addr; throw std::invalid_argument(msg.str());
			}
		}
		virtual bool Contains(const MarketAddr& addr) const
		{
			return (m_mkt_env.find(addr) != m_mkt_env.cend());
		}

		virtual void Insert(const MarketAddr& addr, const MarketVal& val)
		{
			m_mkt_env[addr] = val;
		}

		virtual MarketDeps Intersect(const MarketDeps& addrs) const
		{
			MarketDeps ret;
			std::set_intersection(
				addrs.cbegin(), addrs.cend(), m_mkt_env.cbegin(), m_mkt_env.cend(), std::inserter(ret, ret.end()), IntersectCmp());
			return ret;
		}
	private:
		struct IntersectCmp {
			bool operator() (const MarketAddr& addr, const std::pair<MarketAddr, MarketVal>& map_pair) const
			{
				return addr < map_pair.first;
			}
			bool operator()(const std::pair<MarketAddr, MarketVal>& map_pair, const MarketAddr& addr) const
			{
				return map_pair.first < addr;
			}
		};
	};
	/* Return mkt_env object if it exists, otherwise mkt_base object */
	inline const MarketVal& mkt_obj(const std::shared_ptr<MarketEnv>& mkt_base, const std::shared_ptr<MarketEnv>& mkt_env, const MarketAddr& addr)
	{
		return (mkt_env->Contains(addr)) ? mkt_env->Value(addr) : mkt_base->Value(addr);
	}
	class MarketEnvCascade : public MarketEnv {
		std::shared_ptr<MarketEnv> m_mkt_base, m_mkt_env;
	public:
		MarketEnvCascade(const std::shared_ptr<MarketEnv>& mkt_base, const std::shared_ptr<MarketEnv>& mkt_env)
			: m_mkt_base(mkt_base),
			m_mkt_env(mkt_env)
		{
			if (mkt_base == nullptr) {
				throw std::invalid_argument("mkt_base cannot be nullptr in the ctor of MarketEnvCascade.");
			}
			if (mkt_env == nullptr) {
				throw std::invalid_argument("mkt_env cannot be nullptr in the ctor of MarketEnvCascade.");
			}
		}
	public:
		virtual const MarketVal& Value(const MarketAddr& addr) const {
			return mkt_obj(m_mkt_base, m_mkt_env, addr);
		}

		virtual bool Contains(const MarketAddr& addr) const {
			return (m_mkt_env->Contains(addr) || m_mkt_base->Contains(addr));
		};


		virtual void Insert(const MarketAddr& addr, const MarketVal& val) {
			throw std::invalid_argument("Insert is delibrately disallowed for cascaded market env, since it's supposed to be used as a base.");
		}
		virtual MarketDeps Intersect(const MarketDeps& addrs) const {
			MarketDeps ret;
			MarketDeps intersect_base = m_mkt_base->Intersect(addrs);
			MarketDeps intersect_env = m_mkt_env->Intersect(addrs);
			std::set_union(intersect_base.cbegin(), intersect_base.cend(), intersect_env.cbegin(), intersect_env.cend(),
				std::inserter(ret, ret.end()));
			return ret;
		}
	};
	using MarketErrs = std::map<MarketAddr, std::string>;
	struct MarketObjectBuilder {
		virtual MarketDeps MarketDepends(
			const std::vector<MarketAddr>& addrs, const std::shared_ptr<MarketErrs>& errs) const = 0;


		virtual void Build(
			const std::vector<MarketAddr>& addrs, const std::shared_ptr<MarketEnv>& mkt_base,
			const std::shared_ptr<MarketEnv>& mkt_env, const std::shared_ptr<MarketErrs>& errs) const = 0;
	};

	/* Model parameters */
	using ModelParamVal = std::variant<bool, int, double, std::string>;

	using ModelParams = std::map<std::string, ModelParamVal>;

	/* Model Manager */
	using MMValue = std::variant<bool, int64_t, double, std::string>;

	struct ModelManager {
		virtual size_t RowCount(void) const = 0;
		virtual size_t ColIdx(const std::string& col_name) const = 0;
		virtual MMValue Value(size_t r, size_t c) const = 0;
		virtual bool Contains(const std::string& col_name) const = 0;
	};



	/* Result object */
	enum class ResType {
		error,
		value,
		set1_cash,
		idr,
		implied_spread,
		ir_svm_geometry,
		eq_delta,
		eq_gamma,
		eq_vega,
		eq_volga,
		eq_vanna,
		fut_delta,
		fut_gamma,
		fut_vega_ln,
		ir_strike_vega_n
	};

	enum class ResAttrType {
		pos_idx,
		err_msg,
		mkt_idx,
		mkt_addr,
		currency,
		pay_date,
		clean_price,
		swap_annuity,
		swap_par_rate,
		swap_leg_type,
		strike_vol_ln,
		strike_vol_n,
		credit_key,
		expiry_idx,
		tenor_idx,
		atm_vol_n,
		fwd_premium_pv,
		underlying_swap_pv,
		underlying,
		strike,
		expiry,
		tenor
	};

	using ResCtrl = std::map<ResType, std::vector<ResAttrType> >;

	using ResAttrValue = std::variant<bool, int, int64_t, double, std::string, MarketAddr>;

	struct ResultAtom {
		double value;
		std::vector<std::pair<ResAttrType, ResAttrValue> > attrs;

		ResultAtom(double value_, const std::vector<std::pair<ResAttrType, ResAttrValue> >& attrs_) :
			value(value_), attrs(attrs_)
		{
		}
	};

	class Result {
		std::map<ResType, std::vector<ResultAtom> > m_atoms;
	public:
		void AddAtom(ResType res_type, double value, const std::vector<std::pair<ResAttrType, ResAttrValue> >& attrs)
		{
			m_atoms[res_type].push_back(ResultAtom(value, attrs));
		}

		size_t GetCount(ResType res_type) const
		{
			if (const auto p = m_atoms.find(res_type); p != m_atoms.cend())
				return p->second.size();
			else
				return 0;
		}
		const ResultAtom& GetAtom(ResType res_type, size_t i) const
		{
			if (const auto p = m_atoms.find(res_type); p != m_atoms.cend())
				if (i < p->second.size())
					return p->second[i];
				else
				{
					std::stringstream msg;
					msg << "Invalid atom index: " << i;
					throw std::invalid_argument(msg.str());
				}
			else {

				std::stringstream msg;
				msg << "ResType not found: " << static_cast<int>(res_type);
				throw std::invalid_argument(msg.str());
			}
		}
	};

	/* Valuation model */
	struct ValuationModel {
		virtual void SetTimeService(const TimeService& time_svc) = 0;

		virtual const ResCtrl SupportedResults(void) const = 0;

		virtual void SetResCtrl(const ResCtrl& res_ctrl) = 0;

		virtual MarketDeps MarketDepends(void) const = 0;

		virtual Result Evaluate(
			const std::shared_ptr<MarketEnv>& mkt_base, const std::vector<std::shared_ptr<MarketEnv>>& mkt_envs) const = 0;
	};

}