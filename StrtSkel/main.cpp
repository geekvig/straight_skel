#include <iostream>
#include "Polygon.h"
#include <vector>


void print(CLL<CllNode<Vertex>>* t) {
	CllNode<Vertex>* temp = t->begin();
	for (int i = 0; i < t->Size(); i++) {
		std::cout << std::endl << " **************** " << std::endl;
		std::cout << temp->v->pos.x << " " << temp->v->pos.y << std::endl;
		std::cout << " Line 1 " << std::endl;
		temp->v->e1->line.Print();
		std::cout << std::endl;
		std::cout << " Line 2 " << std::endl;
		temp->v->e2->line.Print();
		std::cout << std::endl << " bisector " << std::endl;
		temp->v->bisector_line.Print();
	    //std::cout << std::endl;
		std::cout << std::endl << std::endl;
		//std::cout << std::endl << " **************** " << std::endl;
		temp = temp->next;
	}
}


int main() {

	//std::vector<std::vector<double>> F{ {2.0, 4.0}, {1.0, 1.0}, {4.0, 0.0}, {5.0, 2.0} };
	//std::vector<std::vector<double>> F{ {3.5, 5}, {2.0, 4.0}, {1.0, 1.0}, {4.0, 0.0}, {5.0, 2.0} };
	//std::vector<std::vector<double>> F { {0.0, 1.0}, {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0} };
	//std::vector<std::vector<double>> F{ {0.5, 2.0}, {0.0, 1.0}, {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0} };
	//std::vector<std::vector<double>> F{ {0.0, 2.0}, {-3.0, 0.5}, {0.0, -1.0}, {1.0, -0.5}, {2.0, 1.0} };
	//std::vector<std::vector<double>> F { {-0.1, 0.9}, {0.1, -0.1}, {1.1, 0.1}, {0.9, 1.1} };
	//std::vector<std::vector<double>> F{ {0.0, 0.0}, {0.5, -1.0}, {1.0, 0.0}, {0.5, 1.0} };
	//std::vector<std::vector<double>> F{ {0.0, 0.0}, {1.0, -1.0}, {2.0, -1.0}, {3.0, 0.0}, {2.0, 1.0}, {1.0, 1.0} };
	std::vector<std::vector<double>> F{ {0.0, 0.0}, {1.0, -1.0}, {2.0, -1.0}, {2.5, -0.5}, {3.0, 0.0}, {2.0, 1.0}, {1.0, 1.0} };

	//std::vector<std::vector<double>> ang_F{ {2.3, 3.1}, {1.9, 1.4}, {3.6, 0.92}, {4.0, 1.7} };
	//std::vector<std::vector<double>> ang_F{ {0.71, 0.29}, {0.71, 0.71}, {0.29, 0.71}, {0.29, 0.29} };
	//std::vector<std::vector<double>> ang_F{ {0.5, 1.0}, {0.97, 0.77}, {0.71, 0.71}, {0.29, 0.71}, {0.027, 0.77} };

	Polygon p;
	p.init_polygon(F);

	std::cout << std::endl << " Printing straight skeleton " << std::endl;
	for (auto& pts : p.straight_skeleton) {
		for (auto& pt : pts)
			std::cout << pt << std::endl;
		std::cout << std::endl;
	}

	//print(&p.LAV);

}
