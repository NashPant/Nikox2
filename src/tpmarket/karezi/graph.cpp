
#include <algorithm>
#include <iostream> 
#include <deque>
#include <stdexcept> 
#include <string>
#include <vector>
#include "graph.h"

namespace TP::market::karezi
{
	/* Topologically sort g using Kahn's algorithm
	* c.f., Kahn, A. B., Communications of the ACM, 5, 11, 558-562 (1962)
	* */
	std::vector<size_t> topological_sort(const Graph& g)
	{
		size_t n = g.vertices.size();
		std::vector<size_t> in_degs(n), ret;
		std::deque<size_t> q;
		ret.reserve(n);

		for (size_t v = 0; v < n; v++)
			if (!(in_degs[v] = g.vertices[v].in_deg))
				q.push_back(v);

		while (!q.empty()) {
			size_t v = q.front();
			ret.push_back(v);
			q.pop_front();
			for (size_t w : g.vertices[v].arcs)
				if (!--in_degs[w])
					q.push_back(w);
		}
		if (ret.size() != n)
			throw std::invalid_argument("Graph contains at least one cycle");
		return ret;
	}
	/* Visit all successors of vertex v in topological order */
	std::vector<size_t> visit_succ(Graph& g, const std::vector<size_t>& t_sort, size_t v)
	{
		std::vector<size_t>::const_iterator p = std::find(t_sort.cbegin(), t_sort.cend(), v);
		std::vector<size_t> ret;

		if (p != t_sort.cend()) {
			for (size_t w: g.vertices[*p].arcs)
				g.vertices[w].mark = true;
			++p;
			ret.reserve(t_sort.cend() - p); /* worst case: all vertices after v in t_sort are successors of v */
		}
		for (; p != t_sort.cend(); ++p) {
			Vertex& u = g.vertices[*p];

			if (u.mark) {
				ret.push_back(*p);
				for (size_t w : u.arcs)
					g.vertices[w].mark = true;
				u.mark = false;
			}
		}
		return ret;
	}
	/* Print graph */
	void print_graph(const Graph& g)
	{
		for (size_t v = 0; v < g.vertices.size(); v++) {
			const Vertex& p = g.vertices[v];
			std::cout << v << " (in_deg: " << p.in_deg << ", mark: " << p.mark << ", addr: "
				<< static_cast<size_t>(p.addr.first) << ", " << p.addr.second << "); ";

			for (size_t w : p.arcs)
				std::cout << v << " -> " << w << ' ';
			std::cout << std::endl;
		}
	}
	/* Test graph functions */
	void test_graph(void)
	{
		Graph g(8);
		g.new_arc(0, 1);
		g.new_arc(1, 2);
		g.new_arc(1, 7);
		g.new_arc(3, 5);
		g.new_arc(4, 5);
		g.new_arc(5, 7);
		print_graph(g);

		std::vector<size_t> t_sort = topological_sort(g);
		std::cout << "Topological sort: ";
		for (size_t v : t_sort)
			std::cout << v << ' ';

		std::cout << std::endl;

		for (size_t v = 0; v < t_sort.size(); v++) {
			std::vector<size_t> succ = visit_succ(g, t_sort, t_sort[v]);
			std::cout << "Successors " << t_sort[v] << ": ";

			for (size_t w : succ)
				std::cout << w << ' ';
			std::cout << std::endl;
		}
	}
}