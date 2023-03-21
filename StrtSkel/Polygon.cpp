#include "Polygon.h"
#include <iostream>
#include "definitions.h"
#include <utility>


std::ostream& operator<<(std::ostream& os, const Intersection_point& ip) {
	os << "point = " << ip.p << " dist = " << ip.dist << " " << std::endl;
	os << "Va = " << ip.Va->v->pos << "  Vb = " << ip.Vb->v->pos << std::endl;
	return os;
}

void Polygon::add_vertex(Vertex* V) {
	CllNode<Vertex>* pV = new CllNode<Vertex>(V);
	LAV.push_back(pV);
	vertices.insert(pV);
}

void Polygon::add_edge(Edge* E) {
	edges.insert(E);
}

void Polygon::compute_angle_bisector() {
	auto temp = this->LAV.begin();
	std::cout << std::endl << " Printing Bisectors " << std::endl << std::endl;
	for (int i = 0; i < this->LAV.Size(); i++) {
		std::cout << temp->v->pos.x << " " << temp->v->pos.y << std::endl;
		temp->v->bisector_line = calc_bisector(temp->v->e1, temp->v->e2);
		//temp->v->bisector_line = find_bisector(temp->v->e1->line, temp->v->e2->line, temp->v->pos);
		temp->v->bisector_line.Print();
		std::cout << std::endl;
		temp = temp->next;
	}
}


void Polygon::compute_angle_bisector_workaround(const std::vector<std::vector<double>>& ang_F) {
	auto temp = this->LAV.begin();
	for (int i = 0; i < this->LAV.Size(); i++) {
		Vertex* bisect_v = new Vertex(ang_F[i][0], ang_F[i][1]);
		temp->v->bisector_line = Line(temp->v, bisect_v);
		temp = temp->next;
	}
}


void Polygon::init_edges() {
	CllNode<Vertex>* temp = LAV.begin();
	for (int i = 0; i < LAV.Size(); i++) {
		//always go counter clockwise - just a convention
		Edge* e = new Edge(temp->v, temp->next->v);
		temp->v->e2 = e;
		temp->next->v->e1 = e;
		this->add_edge(e);
		std::cout << temp->v->pos.x << " " << temp->v->pos.y << std::endl;
		std::cout << temp->next->v->pos.x << " " << temp->next->v->pos.y << std::endl;
		e->line.Print(); std::cout << std::endl;
		temp = temp->next;
	}
	return;
}

void Polygon::init_polygon(const std::vector<std::vector<double>>& F) {
	for (const std::vector<double>& f : F) {
		Vertex* v = new Vertex(f[0], f[1]);
		add_vertex(v);
	}
	init_edges();
	compute_angle_bisector();
	compute_bisector_intersections();
	compute_straight_skeleton();

	return;
}


void Polygon::init_polygon(const std::vector<std::vector<double>>& F, std::vector<std::vector<double>>& ang_F) {
	for (const std::vector<double>& f : F) {
		Vertex* v = new Vertex(f[0], f[1]);
		add_vertex(v);
	}
	init_edges();
	compute_angle_bisector_workaround(ang_F);
	compute_bisector_intersections();
	compute_straight_skeleton();

	return;
}



void Polygon::compute_bisector_intersections() {
	CllNode<Vertex>* temp = LAV.begin();
	for (int i = 0; i < LAV.Size(); i++) {
		//calculate the point of intersection of 2 bisectors of vertices v(i-1) and v(i)
		Point I = bisector_line_intersections(temp->prev->v->bisector_line, temp->v->bisector_line);
		//find the distance of this point of intersection to the line
		double dist = point_line_dist(temp->v->e1->line, I);
		auto Int_pt = Intersection_point(temp->prev, temp, I, dist);
		pQ.push(Int_pt);
		temp = temp->next;
	}
}


//Core Logic
void Polygon::compute_straight_skeleton() {

	while (!pQ.empty()) {
		//
		Intersection_point curr = pQ.top(); 
		pQ.pop();
		if (curr.Va->v->visited && curr.Vb->v->visited) continue;
		curr.Va->v->visited = true;
		curr.Vb->v->visited = true;
		
		// check if we have reached the end (last triangulation)
		if (curr.Va->prev->prev == curr.Vb) {
			straight_skeleton.push_back({ curr.p, curr.Va->v->pos });
			straight_skeleton.push_back({ curr.p, curr.Vb->v->pos });
			straight_skeleton.push_back({ curr.p, curr.Va->prev->v->pos });
			return;
		}

		//push the result
		straight_skeleton.push_back({ curr.p, curr.Va->v->pos });
		straight_skeleton.push_back({ curr.p, curr.Vb->v->pos });

		//create a new vertex
		Vertex* v = new Vertex(curr.p.x, curr.p.y);

		//hook the correct edges
		v->e1 = curr.Va->v->e1;
		v->e2 = curr.Vb->v->e2;

		curr.Va->v->e1->v2 = v;
		curr.Vb->v->e2->v1 = v;

		//insert it into the SLAV (LAV here) & hook up the connections;
		CllNode<Vertex>* cv = new CllNode<Vertex>(v);
		cv->next = curr.Vb->next;
		curr.Vb->next->prev = cv;
		cv->prev = curr.Va->prev;
		curr.Va->prev->next = cv;

		//push it into the set of vertices
		vertices.insert(cv);

		//remove the 2 nodes from the SLAV
		//curr.Va->next = nullptr; curr.Va->prev = nullptr;
		//curr.Vb->next = nullptr; curr.Vb->prev = nullptr;

		//calculator its bisector;
		v->bisector_line = calc_bisector(v->e1, v->e2);

		Point p1 = bisector_line_intersections(v->e1->v1->bisector_line, v->bisector_line);
		double dist_p1 =  point_line_dist(v->e1->line, p1);
		auto Int_pt1 = Intersection_point(cv->prev, cv, p1, dist_p1);

		Point p2 = bisector_line_intersections(v->bisector_line, v->e2->v2->bisector_line);
		double dist_p2 = point_line_dist(v->e2->line, p2);
		auto Int_pt2 = Intersection_point(cv, cv->next, p2, dist_p2);

		//Push if its greater than an epsilon
		if (abs(curr.p.x - Int_pt1.p.x) > 0.01 && abs(curr.p.y - Int_pt1.p.y) > 0.01 && Int_pt1.dist <= pQ.top().dist) 
			pQ.push(Int_pt1);
		
		if (abs(curr.p.x - Int_pt2.p.x) > 0.01 && abs(curr.p.y - Int_pt2.p.x) > 0.01 && Int_pt2.dist <= pQ.top().dist) 
			pQ.push(Int_pt2);

	}
}