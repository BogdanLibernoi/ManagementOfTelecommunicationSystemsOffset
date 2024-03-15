#pragma once
#include<vector>

struct Edge {
	int from, to, weight;
};
typedef std::pair<int, int> Pair;

class Graph
{
public:
	std::vector<std::vector<Pair>> Edges;

	Graph(std::vector<Edge> const& edges, int n) 
	{
		Edges.resize(n);

		for (auto& edge : edges) {
			int from = edge.from;
			int to = edge.to;
			int weight = edge.weight;

			Edges[from].push_back(std::make_pair(to, weight));
		}
	}


};

