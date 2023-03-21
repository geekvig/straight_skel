#pragma once
#include "definitions.h"
#define PI 3.14159265


// REDUNDANT FILE FLUFF 
// DO NOT TAKE A LOOK

std::pair<double, double> calc_bisector_rad(Edge* e1, Edge* e2);

using coord = std::pair<double, double>;
double cross(coord a, coord b);
double dot(coord a, coord b);
std::pair<double, double> get_intersection(coord p1, coord v1, coord p2, coord v2);

std::pair<double, double> calc_bisector_rad2(Edge* e1, Edge* e2);

std::pair<double, double> real_calc_bisector(Vertex* prev, Vertex* curr, Vertex* next);


// ********* bisector calculations ************************ //


std::pair<double, double> normalize(double x, double y) {
	double len = sqrt(x * x + y * y);
	return { x / len, y / len };
}

std::pair<double, double> calc_bisector_rad(Edge* e1, Edge* e2) {
	std::pair<double, double> ret;
	Vertex* curr = e1->v2;
	Vertex* prev = e1->v1;
	Vertex* next = e2->v2;

	auto n1 = normalize(curr->pos.x - prev->pos.x, curr->pos.y - prev->pos.y);
	auto n2 = normalize(next->pos.x - curr->pos.x, next->pos.y - curr->pos.y);

	double angleIn = std::acos(n1.first);
	if (n1.second < 0) angleIn = 2 * PI - angleIn;

	double angleOut = std::acos(n2.first);
	if (n2.second < 0) angleOut = 2 * PI - angleOut;

	auto rotate = angleIn - angleOut;
	if (rotate < 0) rotate = rotate + 2 * PI;

	if (rotate > PI) {
		n2.first *= -1;
		n2.second *= -1;
	}
	else {
		n1.first *= -1;
		n1.second *= -1;
	}

	ret.first = n1.first + n2.first;
	ret.second = n1.second + n2.second;
	return ret;
}


double cross(coord a, coord b) {
	return a.first * b.second - b.first * a.second;
}

double dot(coord a, coord b) {
	return a.first * b.first + a.second * b.second;
}

std::pair<double, double> get_intersection(coord p1, coord v1, coord p2, coord v2) {
	double denom = cross(v1, v2);
	if (denom < 0.00001) return p1;

	double s = (p2.first * v2.second + p1.second * v2.first - p2.second * v2.first - p1.first * v2.second);
	return { p1.first + s * v1.first, p1.second + s * v1.second };
}

std::pair<double, double> calc_bisector_rad2(Edge* e1, Edge* e2) {
	std::pair<double, double> ret;
	Vertex* curr = e1->v2;
	Vertex* prev = e1->v1;
	Vertex* next = e2->v2;

	std::pair<double, double> p = { curr->pos.x, curr->pos.y };
	std::pair<double, double> v1 = { curr->pos.x - prev->pos.x, curr->pos.y - prev->pos.y };
	auto n1 = normalize(v1.second, -1 * v1.first);
	auto n2 = normalize(next->pos.y - curr->pos.y, -1 * (next->pos.x - curr->pos.x));

	ret.first = n1.first + n2.first;
	ret.second = n1.second + n2.second;

	std::pair<double, double> temp = { prev->pos.x + n1.first, prev->pos.y + n1.second };

	auto i = get_intersection(p, ret, temp, v1);

	return { i.first - p.first, i.second - p.second };
}

std::pair<double, double> real_calc_bisector(Vertex* prev, Vertex* curr, Vertex* next) {
	std::pair<double, double> bisect;
	auto v1 = normalize(curr->pos.x - prev->pos.x, curr->pos.y - prev->pos.y);
	auto v2 = normalize(next->pos.x - curr->pos.x, next->pos.y - curr->pos.y);

	bisect.first = v1.first + v2.first;
	bisect.second = v1.second + v2.second;
	auto temp = abs(dot(v1, bisect));
	bisect.first = bisect.first / temp;
	bisect.second = bisect.second / temp;

	return { -1 * bisect.second, bisect.first };
}
