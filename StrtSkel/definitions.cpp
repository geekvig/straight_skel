#include "definitions.h"
#include <iostream>
#include <math.h>
#include <cmath>

#define PI 3.14159265

// find the equation of line segment (edge)given 2 vertices of polygon 
Line::Line(Vertex* v1, Vertex* v2)
{

	v = v1->pos - v2->pos;
	constant = -1 * cross(v, v1->pos);
	double x1 = v1->pos.x;
	double x2 = v2->pos.x;

	double y1 = v1->pos.y;
	double y2 = v2->pos.y;

	if (x2 - x1 == 0)
		m = std::numeric_limits<double>::max();
	else
		m = (double)(y2 - y1) / (double)(x2 - x1);

	if (m == std::numeric_limits<double>::max()) d = 0;
	else
		d = double(y2) - (double)(m * x2);


	//computing the corresponding standard form  ax + by + c = 0

	if (m != std::numeric_limits<double>::max()) {
		a = y2 - y1;
		b = -1 * (x2 - x1);
		c = d * (x2 - x1);
	}
	else if (m == 0) {
		a = 0;
		b = 1;
		c = -y1;
	}
	else {
		a = 1;
		b = 0;
		c = -x1;
	}

}

void Line::Print() {

	//std::cout << "slope form:  mx + d = y " << std::endl;
	//std::cout << m << "x" << " + " << d << std::endl;
	std::cout << "standard form: ax + by + c = 0" << std::endl;
	std::cout << a << "x" << " + " << b << "y" << " + " << c << std::endl;
}

Point bisector_line_intersections(const Line& l1, const Line& l2) {
	Point p;
	auto a1 = l1.a; auto b1 = l1.b; auto c1 = l1.c;
	auto a2 = l2.a; auto b2 = l2.b; auto c2 = l2.c;
	p.x = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
	p.y = (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1);
	return p;
}

double point_line_dist(const Line& l, Point p) {
	double ret;
	auto a = l.a; auto b = l.b; auto c = l.c;
	auto x0 = p.x; auto y0 = p.y;
	return abs(a * x0 + b * y0 + c) / sqrt(pow(a, 2) + pow(b, 2));
}


// ********************** //

std::ostream& operator<<(std::ostream& os, Point p) {
	return os << p.x << ", " << p.y;
}


double dot  (Point v, Point w) { return v.x * w.x + v.y * w.y; }
double cross(Point v, Point w) { return v.x * w.y - v.y * w.x; }

double sq(Point p) { return p.x * p.x + p.y * p.y; }
double abs(Point p) { return sqrt(sq(p)); }


Line calc_bisector_main(Line l1, Line l2, bool interior) {
	//if (cross(l1.v, l2.v) != 0) // l1 and l2 cannot be parallel!
	double sign = interior ? 1 : -1;
	return { l2.v / abs(l2.v) + l1.v / abs(l1.v) * sign,
	l2.c / abs(l2.v) + l1.c / abs(l1.v) * sign };
}


Line find_bisector(Line l1, Line l2, Point p) {
	// edge AB = l1 
	// edge BC = l2
	l1.a /= -l1.b;
	l1.c /= -l1.b;
	l1.b = 1.0;
	l2.a /= -l2.b;
	l2.c /= -l2.b;
	l2.b = 1.0;

	double m1 = -l1.a / l1.b;
	double m2 = -l2.a / l2.b;
	double theta = atan(abs((m1 - m2) / (1 + m1 * m2)));
	std::cout << "m1 = " << m1 << std::endl;
	std::cout << "m2 = " << m2 << std::endl;
	std::cout << "theta = " << theta << std::endl;
	double m_bisector = -1 / tan(theta / 2);
	double d_bisector = p.y - (m_bisector * p.x);

	Line ret;
	ret.m = m_bisector;
	ret.d = d_bisector;

	// y = m_bisector * x + d_bisector;
	// ax + by + c = 0
	ret.a = m_bisector;
	ret.b = -1;
	ret.c = d_bisector;

	return ret;
}


bool obtuse_calc(Edge* e1, Edge* e2) {
	bool obtuse = false;
	Vertex* curr = e1->v2; // B
	Vertex* prev = e1->v1; // A 
	Vertex* next = e2->v2; // C 
	//find angle BAC
	Point BA = { prev->pos.x - curr->pos.x, prev->pos.y - curr->pos.y };
	Point BC = { next->pos.x - curr->pos.x, next->pos.y - curr->pos.y };

	double r = dot(BA, BC);
	if (r < 0) {
		obtuse = true;
	}
	return obtuse;
}

double cross_check(Edge* e1, Edge* e2) {
	Vertex* curr = e1->v2; // B
	Vertex* prev = e1->v1; // A 
	Vertex* next = e2->v2; // C 
	double ret;
	Point BA = { prev->pos.x - curr->pos.x, prev->pos.y - curr->pos.y };
	Point BC = { next->pos.x - curr->pos.x, next->pos.y - curr->pos.y };
	ret = cross(BA, BC);
	return ret;
}

Line calc_bisector(Edge* e1, Edge* e2) {
	Line& l1 = e1->line;
	Line& l2 = e2->line;
	Line ret;
	// check if the angle between 2 edges internal to the polygon is obtuse 
	bool is_obtuse = obtuse_calc(e1, e2);
	auto a1 = l1.a; auto b1 = l1.b; auto c1 = l1.c;
	auto a2 = l2.a; auto b2 = l2.b; auto c2 = l2.c;

	auto m1 = (-1 * a1) / b1; auto d1 = (-1 * c1) / b1;
	auto m2 = (-1 * a2) / b2; auto d2 = (-1 * c2) / b2;

	
	//parallel lines
	if (m1 == m2) {
		auto d = d2 > d1 ? d1 + abs(d2 - d1) : d2 + abs(d2 - d1);
		ret.a = -1 * m1;
		ret.b = 1.0;
		ret.c = -1 * d;
		return ret;
	}

	// If a1.a2 + b1.b2 > 0, 
	// then the bisector corresponding to “ + “ symbol gives the obtuse angle bisector 
	// and the bisector corresponding to “ - " is the bisector of the acute angle between the lines
	bool obtuse_check = (a1 * a2 + b1 * b2) > 0;

	double numerator = std::sqrt(pow(a1, 2.0) + pow(b1, 2.0));
	double denominator = std::sqrt(pow(a2, 2.0) + pow(b2, 2.0));
	// handle edge cases 
	//if (denominator == 0) 

	

	/*
	if (a1 * a2 + b1 * b2 == 0) {
		// check the cross product;
		bool r = cross_check(e1, e2);
		if (r < 0) {
			ret.a = (a1 - k * a2);
			ret.b = (b1 - k * b2);
			ret.c = (c1 - k * c2);
		}
		else {
			ret.a = (a1 + k * a2);
			ret.b = (b1 + k * b2);
			ret.c = (c1 + k * c2);
		}
		return ret;
	}
	*/

	double k = double(numerator / denominator);
	if ((obtuse_check && is_obtuse) || (!obtuse_check && !is_obtuse)) {
		ret.a = (a1 - k * a2);
		ret.b = (b1 - k * b2);
		ret.c = (c1 - k * c2);
	}
	else {
		ret.a = (a1 + k * a2);
		ret.b = (b1 + k * b2);
		ret.c = (c1 + k * c2);
	}
	return ret;
}