#include "Road.h"
#include "../DesignByContract.h"
#include "../ErrorMessages.h"
#include "Intersection.h"
#include "Stop.h"
#include "Vehicle.h"
#include "VehicleGenerator.h"
#include <algorithm>
#include <iostream>
#include <limits>

Road::Road(unsigned int length, const std::string &name)
    : name(name), length(length), vehicleGenerator(nullptr) {

  _initCheck = this;
  REQUIRE(!name.empty(), ErrorMessagesInstructions::missingParameter("name"));

  ENSURE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationEnsure("Road"));
  ENSURE((int)getLength() == (int)length, ErrorMessagesInstructions::mismatchedComputedValue("length", "getLength"));
  ENSURE(getName() == name, ErrorMessagesInstructions::mismatchedComputedValue("name", "getName"));
  ENSURE(getVehicleGenerator() == nullptr, ErrorMessagesInstructions::mismatchedComputedValue("vehicleGenerator", "getVehicleGenerator"));
}

bool Road::properlyInitialized() const { return _initCheck == this; }

Road::~Road() {
  for (const Vehicle *vehicle : vehicles) delete vehicle;
  for (const Stop *traffic_light : stops) delete traffic_light;
  delete vehicleGenerator;
}

void Road::tick(double time, double timeEachTick) {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
  REQUIRE(time >= 0, ErrorMessagesInstructions::mustBePositive("time"));
  REQUIRE(timeEachTick >= 0, ErrorMessagesInstructions::mustBePositive("timeEachTick"));

  for (Stop *stop : stops) stop->update(time);

  std::vector<Vehicle *> toBeDeleted;

  for (size_t j = 0; j < vehicles.size(); j++) {
    Vehicle *vehicle = vehicles[j];
    vehicle->calculateAcceleration();

    if (!(dynamic_cast<FireTruck *>(vehicle) || dynamic_cast<Ambulance *>(vehicle) || dynamic_cast<Police *>(vehicle))) {
      vehicle->brakeBeforeLight();
    }

    if (Bus *bus = dynamic_cast<Bus *>(vehicle)) bus->brakeBeforeBusStop(time);

    vehicle->updateSpeedPosition(timeEachTick);
    vehicle->stopIfNeeded();

    bool transferred = tryTransferVehicle(vehicle, j);
    if (transferred) {
      vehicles.erase(std::remove(vehicles.begin(), vehicles.end(), nullptr), vehicles.end());
      j--;
    } else if (vehicle->getPosition() >= length) {
      toBeDeleted.push_back(vehicle);
    }
  }

  for (Vehicle *v : toBeDeleted) removeVehicle(v);

  if (vehicleGenerator) vehicleGenerator->tick(time);
}

bool Road::tryTransferVehicle(Vehicle *vehicle, size_t index) {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
  REQUIRE(vehicle != nullptr, ErrorMessagesInstructions::missingParameter("vehicle"));

  const RoadIntersectionBinding *closestBinding = vehicle->getClosestIntersectionBinding();

  if (!closestBinding) {
    closestBinding = findClosestIntersection(vehicle->getPosition());
    vehicle->setClosestIntersectionBinding(closestBinding);
  }

  if (closestBinding && !vehicle->getPlannedNextRoad()) {
    vehicle->setPlannedNextRoad(closestBinding->intersection->getRandomNextRoad());
  }

  if (vehicle->getPlannedNextRoad() && closestBinding) {
    if (vehicle->getPosition() - closestBinding->position > 0) {
      Road *nextRoad = vehicle->getPlannedNextRoad();
      double entryPos = 0;
      for (const auto &bind : nextRoad->getIntersectionBindings()) {
        if (bind.intersection == closestBinding->intersection) {
          entryPos = bind.position;
          break;
        }
      }

      vehicle->setPosition(entryPos);
      vehicle->setRoad(nextRoad);
      nextRoad->addVehicle(vehicle);
      vehicles[index] = nullptr;
      vehicle->setPlannedNextRoad(nullptr);
      vehicle->setClosestIntersectionBinding(nullptr);

      return true;
    }
  }
  return false;
}

const RoadIntersectionBinding *Road::findClosestIntersection(double position) const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));

  const RoadIntersectionBinding *closest = nullptr;
  double minDist = std::numeric_limits<double>::max();

  for (const auto &binding : intersectionBindings) {
    double dist = binding.position - position;
    if (dist > 0 && dist < minDist) {
      minDist = dist;
      closest = &binding;
    }
  }

  ENSURE(closest == nullptr || closest->position >= position, ErrorMessagesInstructions::invalidComparisonAsmallerthanB("vehiclePosition", "intersectionPosition"));
  return closest;
}

const std::string &Road::getName() const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
  ENSURE(!name.empty(), ErrorMessagesInstructions::notUpdatedSuccessfully("name"));
  return name;
}

unsigned int Road::getLength() const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
  ENSURE(length >= 0, ErrorMessagesInstructions::mustBePositive("length"));
  return length;
}

const std::vector<Vehicle *> &Road::getVehicles() const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
  return vehicles;
}

const std::vector<Stop *> &Road::getStops() const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
  return stops;
}

VehicleGenerator *Road::getVehicleGenerator() const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
  return vehicleGenerator;
}

void Road::addStop(Stop *stop) {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
  REQUIRE(stop != nullptr, ErrorMessagesInstructions::missingParameter("stop"));
  stops.push_back(stop);
  ENSURE(std::find(getStops().begin(), getStops().end(), stop) != getStops().end(), ErrorMessagesInstructions::notUpdatedSuccessfully("stop"));
}

void Road::addVehicle(Vehicle *vehicle) {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
  REQUIRE(vehicle != nullptr, ErrorMessagesInstructions::missingParameter("vehicle"));
  vehicles.push_back(vehicle);
  ENSURE(std::find(getVehicles().begin(), getVehicles().end(), vehicle) != getVehicles().end(), ErrorMessagesInstructions::notUpdatedSuccessfully("vehicle"));
}

void Road::setVehicleGenerator(VehicleGenerator *generator) {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
  vehicleGenerator = generator;
  ENSURE(getVehicleGenerator() == generator, ErrorMessagesInstructions::notUpdatedSuccessfully("vehicleGenerator"));
}

void Road::removeVehicle(const Vehicle *vehicle) {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
  REQUIRE(vehicle != nullptr, ErrorMessagesInstructions::missingParameter("vehicle"));
  auto it = std::find(vehicles.begin(), vehicles.end(), vehicle);
  vehicles.erase(it);
  delete vehicle;
  ENSURE(std::find(getVehicles().begin(), getVehicles().end(), vehicle) == getVehicles().end(), ErrorMessagesInstructions::notDeletedSuccessfully("vehicle"));
}

bool Road::hasVehicleGenerator() const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
  return vehicleGenerator != nullptr;
}

void Road::addIntersectionAt(Intersection *inter, double position) {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
  REQUIRE(inter != nullptr, ErrorMessagesInstructions::missingParameter("intersection"));
  REQUIRE(position >= 0 && position <= getLength(), ErrorMessagesInstructions::outOfBoundsRequire("position"));

  intersectionBindings.push_back({inter, position});

  ENSURE(intersectionBindings.back().intersection == inter, ErrorMessagesInstructions::notUpdatedSuccessfully("intersectionBindings"));
}

const std::vector<RoadIntersectionBinding> &Road::getIntersectionBindings() const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
  return intersectionBindings;
}