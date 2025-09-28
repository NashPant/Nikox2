
#include <iostream>
#include <list> 
#include <map> 
#include <stack> 
#include <vector>
#include <utility>
#include "graph.h"
#include "vf.h"
#include "mkt_builder.h"
#define DEBUG 0

namespace TP::market::karezi
{
	void dfs(Graph& g, Vertex* s, std::stack<std::list<size_t>::const_iterator>& P, MarketDeps& ret);

	MarketBuilder::MarketBuilder(
		const MarketDeps& mkt_deps,
		const std::map<MktSvc, std::shared_ptr<MarketObjectBuilder> >& mob_cfg)
		: m_mob_cfg(mob_cfg), m_mkt_dep_errs(std::make_shared<MarketErrs>())
	{
		size_t n = 0;
		/* Initialize graph and addr2v map with leaf vertices */
		m_g.vertices.resize(mkt_deps.size());
		for (auto& addr : mkt_deps) {
			m_g.vertices[n].addr = addr;
			m_addr2v[addr] = n;
#if DEBUG
			std::cout << addr << ": " << n << std::endl;
#endif
			n++;
		}

		/* Add all predecessors and arcs to graph and extend addr2v map */
		std::vector<MarketAddr> addrs(1);
		for (size_t v = 0; v < n; v++) {
			addrs[0] = m_g.vertices[v].addr;
			std::shared_ptr<MarketObjectBuilder> mob = get_mob(addrs[0].first);
			MarketDeps deps = mob->MarketDepends(addrs, m_mkt_dep_errs);
			for (auto& addr : deps) {

				if (auto p = m_addr2v.find(addr); p != m_addr2v.end()) {
					m_g.new_arc(p->second, v);
				}
				else {
					m_g.vertices.resize(n + 1);
					m_g.new_arc(n, v);
					m_g.vertices[n].addr = addr;
					m_addr2v[addr] = n;
					n++;
				}
			}
		}


		m_t_sort = topological_sort(m_g);

		for (m_first = m_t_sort.cbegin(); (m_first != m_t_sort.cend()) && !m_g.vertices[*m_first].in_deg; ++m_first);
	}
	std::pair<std::shared_ptr<MarketEnv>, std::shared_ptr<MarketErrs>> MarketBuilder::BuildBaseEnv(void) const
	{
		std::map<MktSvc, std::list<size_t> > svc_grp; std::vector<size_t>::const_iterator p;
		std::shared_ptr<MarketEnv> ret = std::make_shared<MarketEnv>();
		std::shared_ptr<MarketErrs> errs = std::make_shared<MarketErrs>();

		/* Root vertices (with in-degree = 0) may be calculated in any order
		* Hence, we group root vertices by svc in order to enable MOBs to minimize the number of database queries
		*/
		for (p = m_t_sort.cbegin(); p != m_first; ++p)
			svc_grp[m_g.vertices[*p].addr.first].push_back(*p);

		for (const auto& grp: svc_grp) {
			std::vector<MarketAddr> addrs;

			addrs.reserve(grp.second.size());

			for (size_t v: grp.second) {
				addrs.push_back(m_g.vertices[v].addr);
			}
			get_mob(grp.first)->Build(addrs, ret, ret, errs);
		}

		/* Calculate non-root vertices
		¦* We calculate one address at a time, however,
		* - A MOB is permitted to return additional addresses
		*
		We do not recalculate an address that is already present in the market env
		* This enables MOBS to calculate multiple addresses simultaneously if this is optimial, e.g.,
		* a rate curve MOB may calculate the DiscountCurve and IRIndexCurves in a given currency
		*/

		std::vector<MarketAddr> addrs(1);

		for (; p != m_t_sort.cend(); ++p) {
			addrs[0] = m_g.vertices[*p].addr;

			if (!ret->Contains(addrs[0]))
				get_mob(addrs[0].first)->Build(addrs, ret, ret, errs);
		}



		/* Add market dependency discovery errors */
		for (auto e = m_mkt_dep_errs->cbegin(); e != m_mkt_dep_errs->cend(); ++e)
			(*errs)[e->first] = e->second;
		return std::make_pair(ret, errs);
	}


	std::pair<std::shared_ptr<MarketEnv>, std::shared_ptr<MarketErrs>> MarketBuilder::BuildEnv(
		const std::shared_ptr<MarketEnv>& mkt_base, const std::vector<MarketAddr>& addrs, const std::vector<MarketVal>& vals) const
	{
		std::shared_ptr<MarketEnv> ret = std::make_shared<MarketEnv>();
		std::shared_ptr<MarketErrs> errs = std::make_shared<MarketErrs>();

		if (addrs.size() != vals.size())
			throw std::invalid_argument("addrs.size() != vals.size()");

		/* Initialize new mkt env with given values and mark their immediate successors */
		for (size_t i = 0; i < addrs.size(); i++) {
			size_t v = find_addr(addrs[i])->second;

			ret->Insert(addrs[i], vals[i]);
			for (size_t w : m_g.vertices[v].arcs)
				m_g.vertices[w].mark = true;
		}


		/* Calculate all successors of the initialized vertices */
		std::vector<MarketAddr> addr(1);
		for (std::vector<size_t>::const_iterator p = m_first; p != m_t_sort.cend(); ++p) {
			Vertex& v = m_g.vertices[*p];
			if (v.mark) {
				addr[0] = v.addr;

				if (!ret->Contains(addr[0]))
					get_mob(addr[0].first)->Build(addr, mkt_base, ret, errs);

				for (size_t w : v.arcs)
					m_g.vertices[w].mark = true;

				v.mark = false;
			}
		}
		/* Add market dependency discovery errors */
		for (auto e = m_mkt_dep_errs->cbegin(); e != m_mkt_dep_errs->cend(); ++e)
			(*errs)[e->first] = e->second;
		return std::make_pair(ret, errs);
	}




	MarketDeps MarketBuilder::Depends(const MarketDeps& addrs) const
	{
		std::stack<std::list<size_t>::const_iterator> P;
		MarketDeps ret;
		for (const MarketAddr& addr : addrs) {
			const auto p = find_addr(addr);
			Vertex* s = &m_g.vertices[p->second];

			if (!s->mark) {
				ret.insert(s->addr);
				dfs(m_g, s, P, ret);
			}
		}

		for (Vertex& v: m_g.vertices)
			v.mark = false;
		return ret;
	}

	/* Depth-first search
	* c.f., Implementation A in Tarjan, R.E. and U. Zwick, European Journal of Combinatorics, 119, 103815, (2024) */
	void dfs(Graph& g, Vertex* s, std::stack<std::list<size_t>::const_iterator>& P, MarketDeps& ret)
	{
		Vertex* v = s, * w = nullptr;
		std::list<size_t>::const_iterator a = v->rev_arcs.begin();

		v->mark = true; /* previsit(v) */

		while (true) {
			if (a != v->rev_arcs.end()) {
				w = &g.vertices[*a];

				if (!w->mark) {
					w->mark = true; /* previsit(w) */
					ret.insert(w->addr);
					/* FORWARD */
					P.push(a);
					v = w;
					a = v->rev_arcs.begin();
					continue;
				}
			}
			else {
				if (v == s)
					return;
				/* BACKWARD */
				a = P.top();
				P.pop();
				w = v;
				v = (P.empty()) ? s : &g.vertices[*(P.top())];

			}
			++a;
		}
	}
}

