#pragma once
#ifndef POLYGON_H
#define POLYGON_H

#include <unordered_set>
#include "CLL.h"
#include <vector>
#include <utility>
#include <queue>
#include <iostream>

struct Intersection_point {
	CllNode<Vertex>* Va;
	CllNode<Vertex>* Vb;
	Point p;
	double dist = 0; //distance of this intersection point from the corresponding line - key in priority queue

	Intersection_point(CllNode<Vertex>* Va, CllNode<Vertex>* Vb, Point p, double dist) : Va(Va), Vb(Vb), p(p), dist(dist) {}
	
	Intersection_point(const Intersection_point& other) {
		Va = other.Va;
		Vb = other.Vb;
		p = other.p;
		dist = other.dist;
	}
	
	Intersection_point& operator=(const Intersection_point& other) {
		Va = other.Va;
		Vb = other.Vb;
		p = other.p;
		dist = other.dist;
		return *this;
	}
};

std::ostream& operator<<(std::ostream& os, const Intersection_point& ip);

class Polygon {

private:

	struct comp {
		bool operator () (const Intersection_point& a, const Intersection_point& b) {
			return a.dist > b.dist;
		}
	};

public:
	CLL<CllNode<Vertex>> LAV; // circular linked list of vertices (LAV)
	std::unordered_set<CllNode<Vertex>*> vertices; // total set of vertices
	std::unordered_set<Edge*> edges; // total set of edges
	std::priority_queue<Intersection_point, std::vector<Intersection_point>, comp> pQ;
	std::vector<std::vector<Point>> straight_skeleton;

public:
	void add_vertex(Vertex* V);
	void add_edge(Edge* E);
	void init_edges();
	
	void init_polygon(const std::vector<std::vector<double>>& F);
	void init_polygon(const std::vector<std::vector<double>>& F, std::vector<std::vector<double>>& ang_F);

	void compute_bisector_intersections();
	
	void compute_angle_bisector();
	void compute_angle_bisector_workaround(const std::vector<std::vector<double>>& ang_F);

	void compute_straight_skeleton();

};


#endif