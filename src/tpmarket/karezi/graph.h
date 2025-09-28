
#pragma once
#include <list>
#include <vector>
#include "vf.h"

namespace TP::market::karezi
{
	/* Graph */
	typedef struct Vertex {
		std::list<size_t> arcs;
		/* arcs from this vertex */
		std::list<size_t> rev_arcs; /* reverse arcs */
		size_t in_deg; /* in-degree */
		bool mark; /* true => vertex is marked */
		MarketAddr addr; /* market address */
		Vertex() : in_deg(0), mark(false) {}
	} Vertex;

	typedef struct Graph {
		std::vector<Vertex> vertices;
		Graph(size_t n = 0) : vertices(n) {}

		/* Create a new arc from vertex v to vertex w */
		void new_arc(size_t v, size_t w)
		{
			vertices[v].arcs.push_back(w); vertices[w].in_deg++;
			vertices[w].rev_arcs.push_back(v);
		}
	} Graph;
	/* Topologically sort g using Kahn's algorithm
	* c.f., Kahn, A. B., Communications of the ACM, 5, 11, 558-562 (1962)
	*/
	extern std::vector<size_t> topological_sort(const Graph& g);
	/* Visit all successors of vertex v in topological order */
	extern std::vector<size_t> visit_succ(Graph& g, const std::vector<size_t>& t_sort, size_t v);
	/* Print graph */
	extern void print_graph(const Graph& g);
	/* Test graph functions */
	extern void test_graph(void);
}
