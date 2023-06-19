//
// Created by igor on 14/06/2023.
//

#ifndef PROJECT_3_VIEW_H
#define PROJECT_3_VIEW_H

#include "Geometry.h"
#include "Sim_object.h"
#include <set>
#include <pair>

class View {
	int scale;	// number of kilometers in square side
	int size_x;	// size of terminal window
	int size_y;
	Point origin;
	std::set<Sim_object> objects;

	public:
	// getters setters
	std::pair<int, int> getSize();
	void setSize(int size);

	void update_location();

	void draw();
};


#endif //PROJECT_3_VIEW_H
