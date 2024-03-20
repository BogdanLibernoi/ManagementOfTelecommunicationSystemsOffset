#pragma once
#include<vector>
#include <iostream>
#include <random>
#include <list>

typedef std::pair<int, int> Pair;

struct Edge {
	int from, to, weight;
};


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
	Graph(int n) 
	{
		Edges.resize(n);
	}

	void RandomCompletion(int NetSize)
	{
		Edges.resize(NetSize);
		int start = 1, end = 500;
		int randValue;

		int randEdges, randTo;

		for (int i = 0; i < NetSize; i++)
		{
			randValue = (rand() % (end - start + 1) + start);
			Edges[i].push_back(std::make_pair(i + 1, randValue));
			randValue = (rand() % (end - start + 1) + start);
			Edges[i].push_back(std::make_pair(i + 2, randValue));
		}	
	}


};

