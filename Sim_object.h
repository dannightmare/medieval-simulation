//
// Created by igor on 14/06/2023.
//

#ifndef PROJECT_3_SIM_OBJECT_H
#define PROJECT_3_SIM_OBJECT_H


class Sim_object {
	std::string name;
	std::pair<double,double> location;
	std::string state;

	protected:
	std::string getName() const noexcept;
	std::string getLocation() const noexcept;
	virtual void update();
	std::string broadcast_current_state() const noexcept;

};


#endif //PROJECT_3_SIM_OBJECT_H
