//
// Created by igor on 14/06/2023.
//

#include "Model.h"

void Model::run(int argc, char **argv) {
    // SETUP
    if (argc != 3) {
        m_view->Log(std::string("Usage: ") + argv[0] +
                    " <castles.dat> <farms.dat>");
    }
    std::ifstream if_castles(argv[1]);
    std::ifstream if_farms(argv[2]);

    std::string castle;
    while (std::getline(if_castles, castle)) {
        addCastle(castle);
    }
    if_castles.close();
    std::string farm;
    while (std::getline(if_farms, farm)) {
        addFarm(farm);
    }
    if_farms.close();



}

void Model::go() {
    for (const auto &object : *Agent_list) {
        object->update();
    }
    time++;
}

void Model::addKnight(const std::string &name, const std::string &home) {
    auto it = findStructure(home);
    if (!it) {
        throw std::exception();
    }
    double x = it->getLocation().x;
    double y = it->getLocation().y;

    std::shared_ptr<Knight> knight =
        std::make_shared<Knight>(name, Point(x, y));
    Sim_object_list->push_back(knight);
    Agent_list->push_back(knight);
}

void Model::addPeasant(const std::string &name, Point position) {
    std::shared_ptr<Peasant> peasant =
        std::make_shared<Peasant>(name, position);
    Sim_object_list->push_back(peasant);
    Agent_list->push_back(peasant);
}

void Model::addThug(const std::string &name, Point position) {
    std::shared_ptr<Thug> thug = std::make_shared<Thug>(name, position);
    Sim_object_list->push_back(thug);
    Agent_list->push_back(thug);
}

std::shared_ptr<Structure> Model::findStructure(const std::string &name) {
    auto it = find_if(Structure_list->begin(), Structure_list->end(),
                      [&name](std::shared_ptr<Structure> &structure) {
                          return name == structure->getName();
                      });
    return *it;
}

void Model::status() {
    for (const auto &obj : *Sim_object_list) {
        obj->broadcast_current_state();
    }
}

void Model::course(const std::string &basicString, double theta, int i) {

    auto agent = findAgent(basicString);
    if (agent) {
        agent->setCourse(theta);
    } else {
        m_view->Log("No agent named " + basicString);
    }
}

void Model::position(const std::string &basicString, Point point, int i) {
    auto agent = findAgent(basicString);
    if (agent) {
        agent->setDestinationCoordinates(point);
    } else {
        m_view->Log("No agent named " + basicString);
    }
}

void Model::stop(const std::string &basicString) {
    auto agent = findAgent(basicString);
    if (agent) {
        agent->stop();
    } else {
        m_view->Log("No agent named " + basicString);
    }
}
void Model::destination(const std::string &basicString,
                        const std::string &castle) {
    auto agent = findAgent(basicString);
    auto castle_ = findStructure(castle);
    if (castle_ && castle_ && !agent) // castle or agent does not exist
    {
        std::shared_ptr<Knight> knight = std::dynamic_pointer_cast<Knight>(agent);
        knight->setOnPatrol(castle_, Structure_list);
    } else {
        m_view->Log("No structure named " + basicString);
    }
}

void Model::attack(const std::string &thug, const std::string &peasant) {
    // find if thug exists
    std::shared_ptr<Agent> thug_ = findAgent(thug);
    std::shared_ptr<Thug> thug1 = std::dynamic_pointer_cast<Thug>(thug_);
    if (!thug_ || !thug1) {
        // illegal argument - peasant does not exists
    }

    // find if peasant exists
    std::shared_ptr<Agent> peasant_ = findAgent(peasant);
    if (!peasant_) {
        // illegal argument - peasant does not exists
    }
    thug1->attack(std::dynamic_pointer_cast<Peasant>(peasant_), Agent_list);
}

std::shared_ptr<Agent> Model::findAgent(const std::string &a_name) {
    auto it = std::find_if(Agent_list->begin(), Agent_list->end(),
                           [&a_name](std::shared_ptr<Agent> agent) {
                               return agent->getName() == a_name;
                           });
    return *it;
}

void Model::badInput(const std::string &str) {
    std::string badInputMessage = std::string("Bad input: \"") + str + "\"";
    m_view->Log(badInputMessage);
}

Model &Model::Get() {
    static Model instance;
    return instance;
}

void Model::start_working(const std::string &peasant_name,
                          const std::string &farm_name,
                          const std::string &castle_name) {
    auto agent = findAgent(peasant_name);
    auto structure1 = findStructure(farm_name);
    auto structure2 = findStructure(castle_name);
    if (agent && structure1 && structure2) {
        std::shared_ptr<Peasant> peasant = std::dynamic_pointer_cast<Peasant>(agent);
        std::shared_ptr<Farm> farm = std::dynamic_pointer_cast<Farm>(structure1);
        std::shared_ptr<Castle> castle =
            std::dynamic_pointer_cast<Castle>(structure1);

        peasant->start_working(farm, castle);

    } else {
        m_view->Log("Invalid name");
    }
}
const std::shared_ptr<std::vector<std::shared_ptr<Sim_object>>> &
Model::getSimObjectList() const {
    return Sim_object_list;
}

void Model::makeDefault() { m_view->makeDefault(); }

void Model::setSizeView(int _size) { m_view->setSize(_size); }

void Model::setZoomView(double zoom) { m_view->setScale(zoom); }

void Model::setPanView(double x, double y) { m_view->setPan(x, y); }

void Model::show() { m_view->show(); }

void Model::attach(std::unique_ptr<View> someView) {
    if (!m_view) {
        m_view = std::move(someView);
    }
}

void Model::detach(std::unique_ptr<View> someView) {
    if (someView == m_view) {
        m_view.reset();
    }
}


void Model::addFarm(const std::string &line) {
    std::vector<std::string> &&separated = utils::split(line, ',');
    std::string &name = separated[0];
    std::string &point = separated[1];
    std::string &hay = separated[2];
    std::string &prod = separated[3];

    point = point.substr(2);                     // to cut off " ("
    point = point.substr(0, point.length() - 1); // to cut off ")"
    std::vector<std::string> &&point_coords = utils::split(point, ',');

    double x = std::stod(point_coords[0]);
    double y = std::stod(point_coords[1]);

    int nhay = std::stoi(hay);

    int production = std::stoi(prod);

    Structure_list->push_back(
        std::make_shared<Farm>(name, Point(x,y), nhay, production));
}
void Model::addCastle(const std::string &line) {
    std::vector<std::string> &&separated = utils::split(line, ',');
    std::string &name = separated[0];
    std::string &point = separated[1];
    std::string &hay = separated[2];

    point = point.substr(2);                     // to cut off " ("
    point = point.substr(0, point.length() - 1); // to cut off ")"
    std::vector<std::string> &&point_coords = utils::split(point, ',');

    double x = std::stod(point_coords[0]);
    double y = std::stod(point_coords[1]);

    int nhay = std::stoi(hay);

    Structure_list->push_back(
        std::make_shared<Castle>(name, Point(x,y), nhay));
}

void Model::log(std::string str) {
    m_view->Log(str);

}
