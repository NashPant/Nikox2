#pragma once

#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <utility>
#include "vf.h"
#include "graph.h"

namespace TP::market::karezi
{
	class MarketBuilder {
		std::map<MktSvc, std::shared_ptr<MarketObjectBuilder> > m_mob_cfg; /* MOB cobfiguration */
		mutable Graph m_g; /* market dependency graph */
		std::shared_ptr<MarketErrs> m_mkt_dep_errs; /* market dependency errors */
		std::map<MarketAddr, size_t> m_addr2v; /* market address -> vertex map */
		std::vector<size_t> m_t_sort; /* topological sort of m_g */
		std::vector<size_t>::const_iterator m_first; /* first non-root vertex in m_t_sort */
	public:
		MarketBuilder(
			const MarketDeps& mkt_deps,
			const std::map<MktSvc, std::shared_ptr<MarketObjectBuilder>>& mob_cfg
		);

		std::pair<std::shared_ptr<MarketEnv>, std::shared_ptr<MarketErrs>> BuildBaseEnv(void) const;
		std::pair<std::shared_ptr<MarketEnv>, std::shared_ptr<MarketErrs>> BuildEnv(
			const std::shared_ptr<MarketEnv>& mkt_base, const std::vector<MarketAddr>& addrs, const std::vector<MarketVal>& vals) const;

		MarketDeps Depends(const MarketDeps& addrs) const;

	private:
		const std::shared_ptr<MarketObjectBuilder> get_mob(MktSvc svc) const
		{
			const auto p = m_mob_cfg.find(svc);

			if (p == m_mob_cfg.cend()) {
				std::stringstream msg;
				msg << "MOB not found for svc: " << svc;
				throw(std::invalid_argument(msg.str()));
			}
			return p->second;
		}

		std::map<MarketAddr, size_t>::const_iterator find_addr(const MarketAddr& addr) const
		{
			std::map<MarketAddr, size_t>::const_iterator p = m_addr2v.find(addr);
			if (p == m_addr2v.cend()) {
				std::stringstream msg;
				msg << "Market address not found: " << addr;
				throw(std::invalid_argument(msg.str()));
			};

			return p;
		}
	};
}