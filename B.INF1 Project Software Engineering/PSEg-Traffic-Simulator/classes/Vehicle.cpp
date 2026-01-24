#include "Vehicle.h"

#include <algorithm>

#include "../DesignByContract.h"
#include "../ErrorMessages.h"
#include "Road.h"
#include "Stop.h"
#include "iostream"
#include <cmath>
#include <functional>
#include <random>
#include <sstream>
#include <stdexcept>

Vehicle::Vehicle(Road* road, unsigned int position, double speed, unsigned int length, double maxSpeed, double maxAcceleration, double maxBreakFactor, double minFollowDistance)
        : road(road),
          l(length),                // Length of vehicle
          x(position),              // Position
          v(speed),                 // Speed
          vmax(maxSpeed),           // Max Speed
          Vmax(16.6),               // Abs Max Speed
          a(0),                     // Acceleration
          amax(maxAcceleration),    // Max Acceleration
          bmax(maxBreakFactor),     // Max BreakFactor
          minFollowingDistance(minFollowDistance),
          decelerationDistance(50),
          stoppingDistance(15),
          decelerationFactor(0.4)
{
    _initCheck = this;
    REQUIRE(road != nullptr, ErrorMessagesInstructions::missingParameter("road"));
    REQUIRE(position <= road->getLength(), ErrorMessagesInstructions::outOfBoundsRequire("position"));
    ENSURE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationEnsure("Vehicle"));

}

bool Vehicle::properlyInitialized() const {
    return _initCheck == this;
}

void Vehicle::updateSpeedPosition(double dt) {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Vehicle"));
    REQUIRE(dt > 0, ErrorMessagesInstructions::mustBePositive("dt"));
    double x_old = x;
    double v_old = v;

    if (v + a * dt < 0) {
        x = x - std::pow(v, 2) / 2 * a;
        v = 0;
    } else {
        v = v + a * dt;
        x = x + v * dt + (a * std::pow(dt, 2)) / 2;
    }

    if (std::abs(getPosition() - x_old) > 5) {
        x = x_old;
        v = v_old;
        a = 0;
    }
    if (x_old > getPosition()) {
        setPosition(x_old);
    }
    ENSURE(x_old - 0.001 <= getPosition(), ErrorMessagesInstructions::valueNotIncreased("position"))
}

std::pair<const Vehicle*, double> Vehicle::findFrontVehicle() const {
    Vehicle* vehicleInFrontOfMe = nullptr;

    double closestPosC = std::numeric_limits<double>::max();

    for (Vehicle* vehicle : road->getVehicles()) {
        if (vehicle->getPosition() > getPosition() && vehicle->getPosition() < closestPosC && vehicle != this) {

            closestPosC = vehicle->getPosition();
            vehicleInFrontOfMe = vehicle;
        }
    }

    const Vehicle *frontVehicle = nullptr;
    double frontVehiclePos = 0.0;

    if (vehicleInFrontOfMe != nullptr) {
        if (closestIntersectionBinding && plannedNextRoad && vehicleInFrontOfMe->getPosition() > closestIntersectionBinding->position && plannedNextRoad != road) {
            vehicleInFrontOfMe = nullptr;
        } else {
            frontVehicle = vehicleInFrontOfMe;
            frontVehiclePos = vehicleInFrontOfMe->getPosition();
        }
    }

    if (vehicleInFrontOfMe == nullptr) {
        if (closestIntersectionBinding && plannedNextRoad && plannedNextRoad != road) {
            if (!plannedNextRoad->getVehicles().empty()) {
                RoadIntersectionBinding intersectionBindingNextRoad = RoadIntersectionBinding();

                for (const RoadIntersectionBinding& nexRoadIntersectionBinding: plannedNextRoad->getIntersectionBindings()) {
                    if (nexRoadIntersectionBinding.intersection == closestIntersectionBinding->intersection) {
                        intersectionBindingNextRoad = nexRoadIntersectionBinding;
                    }
                }

                double closestPosO = std::numeric_limits<double>::max();

                for (Vehicle* vehicle : plannedNextRoad->getVehicles()) {
                    if (vehicle->getPosition() >= intersectionBindingNextRoad.position && vehicle->getPosition() < closestPosO) {
                        closestPosO = vehicle->getPosition();
                        frontVehicle = vehicle;

                        frontVehiclePos = closestIntersectionBinding->position + (vehicle->getPosition() - intersectionBindingNextRoad.position);
                    }
                }
            }
        }
    }

    return { frontVehicle, frontVehiclePos };
}

void Vehicle::calculateAcceleration() {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Vehicle"));

    std::pair<const Vehicle*, double> result = findFrontVehicle();
    const Vehicle *frontVehicle = result.first;
    double frontVehiclePos = result.second;

    double delta = 0.0;
    if (frontVehicle) {
        double dx = frontVehiclePos - frontVehicle->getLength() - x;
        if (dx < 0.0) dx += frontVehicle->getLength();
        double dv = v - frontVehicle->v;

        if (dx > 0.0) {
            delta = (minFollowingDistance + std::max(0.0, v + (v * dv) / (2.0 * std::sqrt(amax * bmax)))) / dx;
        }

        if (delta < 0.01 && delta > 0) {
            //std::cout << "frontvehiclePos: " << frontVehiclePos << std::endl;
        }
    }

    a = amax * (1 - std::pow((v / vmax), 4) - std::pow(delta, 2));
}

std::pair<const TrafficLight*, double> Vehicle::findRelevantTrafficLight() const {
    TrafficLight* incomingLight = nullptr;

    const TrafficLight *activeLight = nullptr;
    double activeLightPos = -1;

    double minDist = std::numeric_limits<double>::max();
    for (Stop* stop : road->getStops()) {
        if (auto* light = dynamic_cast<TrafficLight*>(stop)) {
            double dist = light->getPosition() - x;
            if (dist >= 0 && dist < minDist && light->getCurrentState() != LightState::GREEN) {
                minDist = dist;
                incomingLight = light;
            }
        }
    }

    if (incomingLight != nullptr) {
        if (closestIntersectionBinding && plannedNextRoad && incomingLight->getPosition() > closestIntersectionBinding->position && plannedNextRoad != road) {
            activeLight = nullptr;
        } else {
            activeLight = incomingLight;
            activeLightPos = incomingLight->getPosition();
        }
    }

    if (activeLight == nullptr) {
        if (closestIntersectionBinding && plannedNextRoad && plannedNextRoad != road) {
            if (!plannedNextRoad->getStops().empty()) {
                RoadIntersectionBinding intersectionBindingNextRoad = RoadIntersectionBinding();

                for (RoadIntersectionBinding nextRoadIntersectionBinding: plannedNextRoad->getIntersectionBindings()) {
                    if (nextRoadIntersectionBinding.intersection == closestIntersectionBinding->intersection) {
                        intersectionBindingNextRoad = nextRoadIntersectionBinding;
                    }
                }

                double closestPosO = std::numeric_limits<double>::max();

                for (Stop *stop: plannedNextRoad->getStops()) {
                    if (TrafficLight* candidateLight = dynamic_cast<TrafficLight*>(stop)) {
                        if (stop->getPosition() >= intersectionBindingNextRoad.position && stop->getPosition() < closestPosO && candidateLight->getCurrentState() != LightState::GREEN) {
                            activeLight = candidateLight;
                            closestPosO = candidateLight->getPosition();
                            activeLightPos = closestIntersectionBinding->position + (candidateLight->getPosition() - intersectionBindingNextRoad.position);
                        }
                    }
                }
            }
        }
    }

    return { activeLight, activeLightPos };
}

void Vehicle::resetVmax() {
    if (!dynamic_cast<Bus*>(this)) {
        vmax = Vmax;
    } else {
        std::pair<const TrafficLight*, double> resultLight = findRelevantTrafficLight();
        std::pair<BusStop*, double> resultBusStop = findRelevantBusStop();
        if (resultLight.second > decelerationDistance && resultBusStop.second > decelerationDistance) {
            vmax = Vmax;
        }
    }
}

void Vehicle::brakeBeforeLight() {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Vehicle"));
    double x_old = getPosition();


    std::pair<const TrafficLight*, double> result = findRelevantTrafficLight();
    const TrafficLight *activeLight = result.first;
    double activeLightPos = result.second;

    if (activeLightPos < 0 || !activeLight) {
        resetVmax();
        return;
    }

    double distance = activeLightPos - x;
    LightState state = activeLight->getCurrentState();
    double requiredBrakingDist = (v * v) / (2 * bmax);

    switch (state) {
        case LightState::GREEN: {
            std::pair<const Vehicle*, double> resultLight = findFrontVehicle();
            const Vehicle *frontVehicle = resultLight.first;
            double frontVehiclePos = resultLight.second;

            if(frontVehicle && frontVehiclePos){
                if(frontVehiclePos - frontVehicle->getLength() - x + getLength() + 2 < stoppingDistance ){
                    if (distance > 0 && distance < stoppingDistance) {
                        a = -(bmax * v) / vmax;
                    } else if (distance > 0 && distance < decelerationDistance) {
                        vmax = decelerationFactor * Vmax;
                    } else {
                        resetVmax();
                    }

                    if (distance < 1) {
                        a *= 5;
                    }
                    break;
                }
            }

            decidedToStopForLight = false;
            break;
        }

        case LightState::ORANGE: {
            if (distance >= requiredBrakingDist) {
                decidedToStopForLight = true;
            }

            if (decidedToStopForLight) {
                if (distance > 0 && distance < stoppingDistance) {
                    a = -(bmax * v) / vmax;
                    if (v <= 0.05) v = 0;
                } else {
                    vmax = decelerationFactor * Vmax;
                }
            } else {
                resetVmax();
            }
            break;
        }

        case LightState::RED: {
            if (distance > 0 && distance < stoppingDistance) {
                a = -(bmax * v) / vmax;
            } else if (distance > 0 && distance < decelerationDistance) {
                vmax = decelerationFactor * Vmax;
            } else {
                resetVmax();
            }

            if (distance < 1) {
                a *= 5;
            }
            break;
        }
    }


    ENSURE(x_old <= getPosition(), ErrorMessagesInstructions::valueNotIncreased("position"))
}

std::pair<BusStop*, double> Vehicle::findRelevantBusStop() const {
    BusStop* inputBusStop = nullptr;
    double inputStopPos = -1;

    BusStop* activeBusStop = nullptr;
    double activeStopPos = -1;

    for (Stop* stop : road->getStops()) {
        if (auto* candidate = dynamic_cast<BusStop*>(stop)) {
            double dist = candidate->getPosition() - x;
            if (dist >= -10 && inputStopPos == -1) {
                inputBusStop = candidate;
                inputStopPos = candidate->getPosition();
            }
        }
    }

    if (inputBusStop != nullptr) {
        if (closestIntersectionBinding && plannedNextRoad && inputStopPos > closestIntersectionBinding->position && plannedNextRoad != road) {
            activeBusStop = nullptr;
            activeStopPos = -1;
        } else {
            activeBusStop = inputBusStop;
            activeStopPos = inputStopPos;
        }
    }

    if (closestIntersectionBinding && closestIntersectionBinding->position - x < decelerationDistance) {
        if (activeBusStop == nullptr && plannedNextRoad && plannedNextRoad != road) {
            RoadIntersectionBinding nextRoadBinding = RoadIntersectionBinding();
            for (const RoadIntersectionBinding& binding : plannedNextRoad->getIntersectionBindings()) {
                if (binding.intersection == closestIntersectionBinding->intersection) {
                    nextRoadBinding = binding;
                    break;
                }
            }

            for (Stop* stop : plannedNextRoad->getStops()) {
                if (auto* candidateStop = dynamic_cast<BusStop*>(stop)) {
                    if (candidateStop->getPosition() >= nextRoadBinding.position) {
                        activeBusStop = candidateStop;
                        activeStopPos = closestIntersectionBinding->position + (candidateStop->getPosition() - nextRoadBinding.position);
                        break;
                    }
                }
            }
        }
    }

    return { activeBusStop, activeStopPos };
}

void Bus::brakeBeforeBusStop(double currentTime) {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Bus"));
    REQUIRE(currentTime >= 0, ErrorMessagesInstructions::mustBePositive("currentTime"));
    double previousPosition = x;

    std::pair<BusStop*, double> resultBusStop = findRelevantBusStop();
    BusStop* activeBusStop = resultBusStop.first;
    double activeStopPos = resultBusStop.second;

    if (activeStopPos < 0 || !activeBusStop) {
        resetVmax();
        return;
    }

    double distance = activeStopPos - x;

    double lastArrival = activeBusStop->getLastChange(this);
    double timeSinceArrival = currentTime - lastArrival;

    bool shouldStop = (timeSinceArrival <= activeBusStop->getWaitTime()) && (getLastStop() != activeBusStop);

    if (shouldStop) {
        if (distance >= -10 && distance < stoppingDistance-10) {
            a = -(bmax * v) / vmax;
            if (v < 0.01) {
                v = 0;
                activeBusStop->setOccupied(this);
            }
            if (distance < -2) {
                a *= 5;
            }
        } else if (distance > 0 && distance < decelerationDistance) {
            vmax = decelerationFactor * Vmax;
        } else {
            resetVmax();
        }
    } else {
        resetVmax();

        if (lastArrival > 0 && activeBusStop->getOccupied() == this) {
            setLastStop(activeBusStop);
            activeBusStop->setOccupied(nullptr);
        }
    }

    ENSURE(previousPosition <= getPosition(), ErrorMessagesInstructions::valueNotIncreased("position"));
}

void Bus::setLastStop(BusStop *busstop) {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Bus"));
    REQUIRE(busstop == nullptr || busstop->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("BusStop"));
    lastStop = busstop;
    ENSURE(getLastStop() == busstop, ErrorMessagesInstructions::notUpdatedSuccessfully("busstop"))
}

BusStop *Bus::getLastStop() const {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Bus"));
    return lastStop;
}

void Vehicle::stopIfNeeded() {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Vehicle"));
    if (vmax == 0) {
        a = (vmax > 0) ? -bmax * (v / vmax) : - bmax;
    }
}

Road *Vehicle::getRoad() const {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Vehicle"));
    ENSURE(road != nullptr, ErrorMessagesInstructions::notUpdatedSuccessfully("road"));
    return road;
}

double Vehicle::getPosition() const {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Vehicle"));
    ENSURE(x >= 0, ErrorMessagesInstructions::notUpdatedSuccessfully("position"));
    return x;
}

double Vehicle::getSpeed() const {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Vehicle"));
    return v;
}

double Vehicle::getLength() const {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Vehicle"));
    ENSURE(l > 0, ErrorMessagesInstructions::notUpdatedSuccessfully("position"));

    return l;
}

Road *Vehicle::getPlannedNextRoad() const {
  REQUIRE(this->properlyInitialized(),
          ErrorMessagesInstructions::properInitializationRequire("Vehicle"));
  return plannedNextRoad;
}

const RoadIntersectionBinding *Vehicle::getClosestIntersectionBinding() const {
  REQUIRE(this->properlyInitialized(),
          ErrorMessagesInstructions::properInitializationRequire("Vehicle"));
  return closestIntersectionBinding;
}

void Vehicle::setClosestIntersectionBinding(const RoadIntersectionBinding *intersectionBinding) {
    REQUIRE(this->properlyInitialized(),
        ErrorMessagesInstructions::properInitializationRequire("Vehicle"));
    closestIntersectionBinding = intersectionBinding;
    ENSURE(this->getClosestIntersectionBinding() == intersectionBinding,
        ErrorMessagesInstructions::notUpdatedSuccessfully("intersectionBinding"));
}

void Vehicle::setPosition(double position) {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Vehicle"));
    x = position;
    ENSURE(this->getPosition() == position, ErrorMessagesInstructions::notUpdatedSuccessfully("position"));
}

void Vehicle::setSpeed(double speed) {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Vehicle"));
    v = speed;
    ENSURE(this->getSpeed() == speed, ErrorMessagesInstructions::notUpdatedSuccessfully("speed"));
}

void Vehicle::setRoad(Road* r) {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Vehicle"));
    this->road = r;
    ENSURE(this->getRoad() == road, ErrorMessagesInstructions::notUpdatedSuccessfully("road"));
}

void Vehicle::setPlannedNextRoad(Road *plannedRoad) {
    REQUIRE(this->properlyInitialized(),
        ErrorMessagesInstructions::properInitializationRequire("Vehicle"));
    plannedNextRoad = plannedRoad;
    ENSURE(this->getPlannedNextRoad() == plannedRoad, ErrorMessagesInstructions::notUpdatedSuccessfully("plannedRoad"));
}

std::string Vehicle::getTexture() {
  REQUIRE(this->properlyInitialized(),
          ErrorMessagesInstructions::properInitializationRequire("Vehicle"));
  ENSURE(!texture.empty(),
         ErrorMessagesInstructions::notUpdatedSuccessfully("texture"));
  return texture;
}

double Vehicle::getDecelerationDistance() const {
    REQUIRE(this->properlyInitialized(),
        ErrorMessagesInstructions::properInitializationRequire("Vehicle"));
    return decelerationDistance;
}

Car::Car(Road *road, unsigned int position) : Vehicle(road, position, 16.6, 4, 16.6, 1.44, 4.61, 4) {
    REQUIRE(road != nullptr, ErrorMessagesInstructions::missingParameter("road"));
    static std::mt19937 gen(std::random_device{}());

    // Define the weighted distribution of vehicle types
    std::discrete_distribution<int> typeDist({50, 20, 10, 5, 15}); // {Car, SUV, SportCar, PickupTruck, Taxi}

    int vehicleType = typeDist(gen);
    int randomNumber = 1;

    switch (vehicleType) {
    case 0: // Regular Car (1-12)
        randomNumber = std::uniform_int_distribution<>(1, 12)(gen);
        texture = "resources/sprites/vehicles/cars/car" + std::to_string(randomNumber) + ".png";
        break;
    case 1: // SUV (1-12)
        randomNumber = std::uniform_int_distribution<>(1, 12)(gen);
        texture = "resources/sprites/vehicles/suvs/suv" + std::to_string(randomNumber) + ".png";
        break;
    case 2: // Sport Car (1-26)
        randomNumber = std::uniform_int_distribution<>(1, 26)(gen);
        texture = "resources/sprites/vehicles/sportCars/sportCar" + std::to_string(randomNumber) + ".png";
        break;
    case 3: // Pickup Truck (1-6)
        randomNumber = std::uniform_int_distribution<>(1, 6)(gen);
        texture = "resources/sprites/vehicles/pickupTrucks/pickupTruck" + std::to_string(randomNumber) + ".png";
        break;
    case 4: // Taxi (1-2)
        randomNumber = std::uniform_int_distribution<>(1, 2)(gen);
        texture = "resources/sprites/vehicles/taxis/taxi" + std::to_string(randomNumber) + ".png";
        break;
    }

    ENSURE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationEnsure("Vehicle"));
    ENSURE(!texture.empty(), ErrorMessagesInstructions::notUpdatedSuccessfully("texture"));
}

Bus::Bus(Road *road, unsigned int position): Vehicle(road, position, 11.4, 12, 11.4, 1.22, 4.29, 12), lastStop(nullptr) {
    REQUIRE(road != nullptr, ErrorMessagesInstructions::missingParameter("road"));
    texture = "resources/sprites/vehicles/busses/bus.png";
    ENSURE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationEnsure("Vehicle"));
    ENSURE(!texture.empty(), ErrorMessagesInstructions::notUpdatedSuccessfully("texture"));
}

FireTruck::FireTruck(Road *road, unsigned int position): Vehicle(road, position, 14.6, 10, 14.6, 1.33, 4.56, 10) {
    REQUIRE(road != nullptr, ErrorMessagesInstructions::missingParameter("road"));
    texture = "resources/sprites/vehicles/fireTrucks/fireTruck1.png";
    ENSURE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationEnsure("Vehicle"));
    ENSURE(!texture.empty(), ErrorMessagesInstructions::notUpdatedSuccessfully("texture"));
}

Ambulance::Ambulance(Road *road, unsigned int position): Vehicle(road, position, 15.5, 8, 15.5, 1.44, 4.47, 8) {
    REQUIRE(road != nullptr, ErrorMessagesInstructions::missingParameter("road"));
    texture = "resources/sprites/vehicles/ambulances/ambulance1.png";
    ENSURE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationEnsure("Vehicle"));
    ENSURE(!texture.empty(), ErrorMessagesInstructions::notUpdatedSuccessfully("texture"));
}

Police::Police(Road *road, unsigned int position): Vehicle(road, position, 17.2, 6, 17.2, 1.55, 4.92, 6) {
    REQUIRE(road != nullptr, ErrorMessagesInstructions::missingParameter("road"));
    texture = "resources/sprites/vehicles/polices/police1.png";
    ENSURE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationEnsure("Vehicle"));
    ENSURE(!texture.empty(), ErrorMessagesInstructions::notUpdatedSuccessfully("texture"));
}