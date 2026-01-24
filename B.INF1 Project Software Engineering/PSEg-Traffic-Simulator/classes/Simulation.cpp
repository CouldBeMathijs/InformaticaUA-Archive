#include "Simulation.h"
#include "../DesignByContract.h"
#include "../ErrorMessages.h"
#include "../io/input/tinyxml.h"
#include "cmath"
#include "iostream"
#include <algorithm>
#include <vector>

Simulation::Simulation() : timeEachTick(0.0166),time(0){
    _initCheck = this;
    ENSURE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationEnsure("Simulation"));
}

bool Simulation::properlyInitialized() const {
    return _initCheck == this;
}

Simulation::~Simulation() {
    for (Road* road: roads) {
        delete road;
    }
    for (Intersection* intersection : intersections) {
        delete intersection;
    }

    intersections.clear();
    roads.clear();
}

void Simulation::runSec(double seconds) {
    REQUIRE(seconds >= 0, ErrorMessagesInstructions::mustBePositive("seconds"));
    unsigned int steps = ceil(seconds / timeEachTick);
    runTicks(steps);
}

void Simulation::runTicks(unsigned int steps) {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Simulation"));
  double startTime = time;
  for (unsigned int i = 0; i < steps; i++) {
    time += timeEachTick;

    // Update all roads
    for (Road* road : roads) {
      road->tick(time, timeEachTick);
    }
  }
  ENSURE(time >= startTime, ErrorMessagesInstructions::valueNotIncreased("time"));
}

const std::vector<Road*> &Simulation::getRoads() const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Simulation"));
  return roads;
}

Road* Simulation::findRoadWithName(const std::string& name) const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Simulation"));
    for (Road * road : roads) {
        if (road->getName() == name) {
            return road;
        }
    }
    return nullptr;
}

double Simulation::getTimePerTick() const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Simulation"));
  return timeEachTick;
}

double Simulation::getTime() const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Simulation"));
  return time;
}

std::vector<Intersection *> & Simulation::getIntersections() {
    return intersections;
}

void Simulation::addRoad(Road *road) {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Simulation"));
  REQUIRE(road != nullptr, ErrorMessagesInstructions::missingParameter("road"));
  REQUIRE(road->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
  roads.push_back(road);
  ENSURE(std::find(getRoads().begin(), getRoads().end(), road) != getRoads().end(), ErrorMessagesInstructions::notUpdatedSuccessfully("road"));
}

void Simulation::addIntersection(Intersection *intersection) {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Simulation"));
  REQUIRE(intersection != nullptr, ErrorMessagesInstructions::missingParameter("road"));
  REQUIRE(intersection->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Intersection"));
    intersections.push_back(intersection);
}
