//============================================================================
// Name        : Road.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : Class that represents a road, where vehicles can move,
//               and where traffic lights and a vehicle generator can be added.
//============================================================================

#ifndef ROAD_H
#define ROAD_H

#include "Intersection.h"
#include "VehicleGenerator.h"
#include <string>
#include <vector>

class Intersection;
class VehicleGenerator;
class TrafficLight;
class Vehicle;
class Stop;

/**
 * @brief A struct to keep intersections and the position of the intersection
 * together
 */
struct RoadIntersectionBinding {
  Intersection *intersection;
  double position;
};

/**
 * @brief A road on which vehicles can travel
 */
class Road {
private:
  Road *_initCheck;
  const std::string name;
  const unsigned int length;

  std::vector<Vehicle *> vehicles;
  std::vector<Stop *> stops;
  VehicleGenerator *vehicleGenerator;

  std::vector<RoadIntersectionBinding> intersectionBindings;

public:
  /**
   * \n REQUIRE(!name.empty(), "Error: 'name' cannot be NULL or empty.");
   * \n ENSURE(this->properlyInitialized(), "Error: Constructor of 'Road' must end in a correct initialized state.");
   * \n ENSURE(getLength() == length, "Error: Calculated value of 'length' does not match said value in getLength()");
   * \n ENSURE(getName() == name, "Error: Calculated value of 'name' does not match said value in getName()");
   * \n ENSURE(getVehicleGenerator() == nullptr, "Error: Calculated value of 'vehicleGenerator' does not match said value in getVehicleGenerator()");
   */
  Road(unsigned int length, const std::string &name);

  /**
   * @return true if the object is properly initialized, false otherwise.
   */
  [[nodiscard]] bool properlyInitialized() const;

  ~Road();

  /**
   * \n REQUIRE(this->properlyInitialized(), "Error: Road's constructor did not initialize properly");
   * \n REQUIRE(time >= 0, "Error: time cannot be negative.");
   * \n REQUIRE(timePerTick >= 0, "Error: timeEachTick cannot be negative.");
   */
  void tick(double time, double timePerTick);

  /**
   * \n REQUIRE(this->properlyInitialized(), "Error: Road's constructor did not initialize properly");
   * \n REQUIRE(vehicle != nullptr, "Error: 'vehicle' cannot be NULL or empty.");
   * \n ENSURE(vehicle == nullptr || vehicle->getRoad() != this, "Error: vehicle was not transferred correctly");
   */
  bool tryTransferVehicle(Vehicle *vehicle, size_t index);

  /**
   * \n REQUIRE(this->properlyInitialized(), "Error: Road's constructor did not initialize properly");
   * \n ENSURE(result == nullptr || result->position >= position, "Error: intersection result was not the closest valid one");
   */
  const RoadIntersectionBinding *findClosestIntersection(double position) const;

  /**
   * \n REQUIRE(this->properlyInitialized(), "Error: Road's constructor did not initialize properly");
   * \n ENSURE(!result.empty(), "Error: name was not updated correctly");
   */
  [[nodiscard]] const std::string &getName() const;

  /**
   * \n REQUIRE(this->properlyInitialized(), "Error: Road's constructor did not initialize properly");
   * \n ENSURE(result >= 0, "Error: length cannot be negative.");
   */
  [[nodiscard]] unsigned int getLength() const;

  /**
   * \n REQUIRE(this->properlyInitialized(), "Error: Road's constructor did not initialize properly");
   */
  [[nodiscard]] const std::vector<Vehicle *> &getVehicles() const;

  /**
   * \n REQUIRE(this->properlyInitialized(), "Error: Road's constructor did not initialize properly");
   */
  [[nodiscard]] const std::vector<Stop *> &getStops() const;

  /**
   * \n REQUIRE(this->properlyInitialized(), "Error: Road's constructor did not initialize properly");
   */
  [[nodiscard]] const std::vector<RoadIntersectionBinding> &getIntersectionBindings() const;

  /**
   * \n REQUIRE(this->properlyInitialized(), "Error: Road's constructor did not initialize properly");
   */
  [[nodiscard]] VehicleGenerator *getVehicleGenerator() const;

  /**
   * \n REQUIRE(this->properlyInitialized(), "Error: Road's constructor did not initialize properly");
   * \n REQUIRE(stop != nullptr, "Error: 'stop' cannot be NULL or empty.");
   * \n ENSURE(std::find(getStops().begin(), getStops().end(), stop) != getStops().end(), "Error: stop was not updated correctly");
   */
  void addStop(Stop *stop);

  /**
   * \n REQUIRE(this->properlyInitialized(), "Error: Road's constructor did not initialize properly");
   * \n REQUIRE(inter != nullptr, "Error: 'intersection' cannot be NULL or empty.");
   * \n REQUIRE(position >= 0 && position <= getLength(), "Error: 'position' cannot be out of bounds");
   * \n ENSURE(intersectionBindings.back().intersection == inter, "Error: intersectionBindings was not updated correctly");
   */
  void addIntersectionAt(Intersection *inter, double position);

  /**
   * \n REQUIRE(this->properlyInitialized(), "Error: Road's constructor did not initialize properly");
   * \n REQUIRE(vehicle != nullptr, "Error: 'vehicle' cannot be NULL or empty.");
   * \n ENSURE(std::find(getVehicles().begin(), getVehicles().end(), vehicle) != getVehicles().end(), "Error: vehicle was not updated correctly");
   */
  void addVehicle(Vehicle *vehicle);

  /**
   * \n REQUIRE(this->properlyInitialized(), "Error: Road's constructor did not initialize properly");
   * \n ENSURE(getVehicleGenerator() == generator, "Error: vehicleGenerator was not updated correctly");
   */
  void setVehicleGenerator(VehicleGenerator *generator);

  /**
   * \n REQUIRE(this->properlyInitialized(), "Error: Road's constructor did not initialize properly");
   * \n REQUIRE(vehicle != nullptr, "Error: 'vehicle' cannot be NULL or empty.");
   * \n ENSURE(std::find(getVehicles().begin(), getVehicles().end(), vehicle) == getVehicles().end(), "Error: vehicle was not deleted successfully");
   */
  void removeVehicle(const Vehicle *vehicle);

  /**
   * \n REQUIRE(this->properlyInitialized(), "Error: Road's constructor did not initialize properly");
   */
  [[nodiscard]] bool hasVehicleGenerator() const;
};

#endif // ROAD_H