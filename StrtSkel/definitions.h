#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <math.h>
#include <utility>
#include <iostream>

struct Line; // defined later
struct Vertex; 
struct Edge;

//cartesian coordinate
struct Point {
	double x = 0.0;
	double y = 0.0;
	Point(){}
	Point(double x, double y) : x(x), y(y) {};
	Point(const Point& p) {
		x = p.x;
		y = p.y;
	}
	Point& operator=(const Point& p) {
		x = p.x;
		y = p.y;
		return *this;
	}

	Point operator + (Point p) { return { x + p.x, y + p.y }; }
	Point operator - (Point p) { return { x - p.x, y - p.y }; }
    Point operator * (double d) { return { x * d, y * d }; }
	Point operator / (double d) { return { x / d, y / d }; } 

	friend std::ostream& operator<<(std::ostream& os, Point p);

};


// generic equation of a line
struct Line {

	//direction vector v 
	Point v;
	double constant = 0;

	//slope equation y = mx + d 
	double m = 0; // slope 
	double d = 0; // intercept

	// standard form ax + by + c = 0
	double a = 0;
	double b = 0;
	double c = 0;

	Line() {}

	Line(const Line& l) {
		a = l.a;
		b = l.b;
		c = l.c;

		m = l.m;
		d = l.d;
		
		v = l.v;
		constant = l.constant;
	}

	Line& operator=(const Line& l) {
		a = l.a;
		b = l.b;
		c = l.c;

		m = l.m;
		d = l.d;

		v = l.v;
		constant = l.constant;

		return *this;
	}

	Line(Line&& l) noexcept {
		a = l.a;
		b = l.b;
		c = l.c;

		m = l.m;
		d = l.d;

		v = l.v;
		constant = l.constant;
	}

	Line& operator=(Line&& l) noexcept {
		a = l.a;
		b = l.b;
		c = l.c;

		m = l.m;
		d = l.d;

		v = l.v;
		constant = l. constant;

		return *this;
	}

	Line(Vertex* v1, Vertex* v2);
	Line(Point v, double c) : v(v), c(c) {}
	//Line(Point p, Point q) : v(q - p), c(cross(v, p)) {}

	void Print();

};

struct Vertex {
	Point pos;
	Edge* e1 = nullptr;
	Edge* e2 = nullptr;
	std::pair<double, double> bisector_p;
	Line bisector_line; // the bisector line passing through the vertex
	bool visited = false;

	Vertex (){}
	Vertex (double x, double y) : pos(x, y) { }
	Vertex(const Vertex& v) {
		pos = v.pos;
		e1 = v.e1;
		e2 = v.e2;
		bisector_line = v.bisector_line;
	}
};

struct Edge {
	Vertex* v1 = nullptr;
	Vertex* v2 = nullptr;
	Line line;
	Edge() {}
	Edge(Vertex* v1, Vertex* v2) : v1(v1), v2(v2) {
		line = Line(v1, v2);
	}
};


double dot(Point v, Point w);
double cross(Point v, Point w);

double sq(Point p);
double abs(Point p);


// ****** functions to calc bisectors ****** //
Line calc_bisector_main(Line l1, Line l2, bool interior);
Line find_bisector(Line l1, Line l2, Point p);
bool obtuse_calc(Edge* e1, Edge* e2);
Line calc_bisector(Edge* e1, Edge* e2);
// ****************************************** //

Point bisector_line_intersections(const Line& l1, const Line& l2);
double point_line_dist(const Line& l, Point p);



#endif