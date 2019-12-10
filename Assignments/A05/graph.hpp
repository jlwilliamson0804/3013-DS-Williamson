#pragma once
#include <iostream>
#include "json.hpp"
#include <stdlib.h>
#include <cmath>
#include "map_parts.hpp"

using namespace std;

using json = nlohmann::json;

struct Edge;

struct Node {
	//from 
	City *C;
	vector<Edge > Edges;
	bool visited;

	Node() {
		C = new City;
		visited = false;
	}
	Node(json j, int id) {
		C = new City(j, id);
		visited = false;
	}

};

struct Edge {
	Road R;
	Node *to;

	Edge(Node* n, int t, double d) {
		R.type = t;
		R.distance = d;
		to = n;

	}
};

class Graph {

};
