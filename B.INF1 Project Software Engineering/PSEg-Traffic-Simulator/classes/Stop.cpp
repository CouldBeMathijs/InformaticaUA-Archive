#include "Stop.h"
#include "../DesignByContract.h"
#include "../ErrorMessages.h"
#include "Road.h"
#include <iostream>

Stop::Stop(Road *road, unsigned int position) : position(position), road(road) {
  REQUIRE(road != nullptr, ErrorMessagesInstructions::missingParameter("road"));
  REQUIRE(position <= road->getLength(), ErrorMessagesInstructions::outOfBoundsRequire("position"));
  _initCheck = this;
  lastChange = 0;
  ENSURE(this->Stop::properlyInitialized(), ErrorMessagesInstructions::properInitializationEnsure("Stop"));
  REQUIRE(position >= 0, ErrorMessagesInstructions::mustBePositive("position"));
}

bool Stop::properlyInitialized() const {
  return _initCheck == this;
}

double Stop::getCurrentTime() const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Stop"));
  return currentTime;
}


Road *Stop::getRoad() const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Stop"));
  ENSURE(road != nullptr, ErrorMessagesInstructions::notUpdatedSuccessfully("road"));
  return road;
}

double Stop::getPosition() const {
  REQUIRE(properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Stop"));
  ENSURE(position <= getRoad()->getLength(), ErrorMessagesInstructions::outOfBoundsEnsure("position"))
  return position;
}

double Stop::getLastChange() const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Stop"));
  return lastChange;
}

void Stop::update(double) {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Stop"));
  ENSURE(false, ErrorMessagesInstructions::NeverReached);
}


void TrafficLight::update(double currentTime) {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("TrafficLight"));
  REQUIRE(currentTime >= 0, ErrorMessagesInstructions::mustBePositive("time"));

  this->currentTime = currentTime;

  double timeElapsed = currentTime - lastChange;

  unsigned int greenDuration = cycle;  // 100% of base cycle
  double orangeDuration = 0.1 * cycle; // 10% of green duration
  unsigned int redDuration = cycle;    // 100% of base cycle
  // double totalCycleTime = greenDuration + orangeDuration + redDuration;

  if (currentState == LightState::GREEN && timeElapsed >= greenDuration) {
      setCurrentState(LightState::ORANGE);
  } else if (currentState == LightState::ORANGE && timeElapsed >= orangeDuration) {
      setCurrentState(LightState::RED);
  } else if (currentState == LightState::RED && timeElapsed >= redDuration) {
      setCurrentState(LightState::GREEN);
  }

  REQUIRE(currentTime >= 0, ErrorMessagesInstructions::mustBePositive("time"));
}

TrafficLight::TrafficLight(Road *road, unsigned int position, unsigned int cycle)
    : Stop(road, position), cycle(cycle), currentState(LightState::GREEN) {
  REQUIRE(cycle >= 0, ErrorMessagesInstructions::mustBePositive("cycle"));
}

double TrafficLight::getCycle() const {
  REQUIRE(properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("TrafficLight"));
  ENSURE(cycle >= 0, ErrorMessagesInstructions::mustBePositive("cycle"));
  return cycle;
}

LightState TrafficLight::getCurrentState() const {
  REQUIRE(properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("TrafficLight"));
  return currentState;
}

void TrafficLight::setCurrentState(LightState lightstate) {
  REQUIRE(properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("TrafficLight"));
  currentState = lightstate;
  lastChange = currentTime;
  ENSURE(lastChange == currentTime, ErrorMessagesInstructions::notUpdatedSuccessfully("lastChange"));
}

bool TrafficLight::allowedToDrive() const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("TrafficLight"));
  return currentState == LightState::GREEN;
}

void TrafficLight::toggle() {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("TrafficLight"));

  switch (currentState) {
    case LightState::GREEN:
      setCurrentState(LightState::ORANGE);
      if (SmartTrafficLight* smartLight = dynamic_cast<SmartTrafficLight*>(this)) {
        SmartTrafficLight* linkedLight = smartLight->getLinkedLight();
        if (linkedLight) {
          linkedLight->setCurrentState(LightState::RED);
        }
      }
      break;
    case LightState::ORANGE:
      setCurrentState(LightState::RED);
      if (SmartTrafficLight* smartLight = dynamic_cast<SmartTrafficLight*>(this)) {
        SmartTrafficLight* linkedLight = smartLight->getLinkedLight();
        if (linkedLight) {
          linkedLight->setCurrentState(LightState::GREEN);
        }
      }
      break;
    case LightState::RED:
      setCurrentState(LightState::GREEN);
      if (SmartTrafficLight* smartLight = dynamic_cast<SmartTrafficLight*>(this)) {
        SmartTrafficLight* linkedLight = smartLight->getLinkedLight();
        if (linkedLight) {
          linkedLight->setCurrentState(LightState::RED);
        }
      }
      break;
  }
}

SmartTrafficLight::SmartTrafficLight(Road *road, unsigned int position, bool isVisible, double minStaticTime, double maxStaticTime) : TrafficLight(road, position, 0), minStaticTime(minStaticTime), maxStaticTime(maxStaticTime), visible(isVisible) {
  orangeDuration = isVisible ? std::min(0.1 * maxStaticTime, 5.0) : 0.0;
  ENSURE(this->Stop::properlyInitialized(), ErrorMessagesInstructions::properInitializationEnsure("SmartTrafficLight"));
  //A few REQUIRE's are called by the traffic light constructor this REQUIRES, so they are automatically resolved to true
  REQUIRE(minStaticTime < maxStaticTime, ErrorMessagesInstructions::invalidComparisonAsmallerthanB("maxStaticTime", "minStaticTime"));
}

void SmartTrafficLight::setLinkedLight(SmartTrafficLight* light) {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("SmartTrafficLight"));
  REQUIRE(light != nullptr, ErrorMessagesInstructions::missingParameter("light"));
  REQUIRE(light->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("SmartTrafficLight"));
  linkedLight = light;
  linkedLight->setCurrentState(LightState::RED);
  setCurrentState(LightState::GREEN);
  ENSURE(this->linkedLight == light, ErrorMessagesInstructions::notUpdatedSuccessfully("linkedLight"));
  ENSURE(this->getCurrentState() == LightState::GREEN, ErrorMessagesInstructions::notUpdatedSuccessfully("this lightstate"));
  ENSURE(light->getCurrentState() == LightState::RED, ErrorMessagesInstructions::notUpdatedSuccessfully("linkedLight's lightstate"));
}

void SmartTrafficLight::intersectionLightsSetStateToGreen() {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("SmartTrafficLight"));
  if (linkedLight->getCurrentState() == LightState::GREEN) {
    linkedLight->setCurrentState(LightState::ORANGE);
  } else if (linkedLight->getCurrentState() == LightState::ORANGE && linkedLight->getCurrentTime() - linkedLight->getLastChange() >= orangeDuration) {
    setCurrentState(LightState::GREEN);
    linkedLight->setCurrentState(LightState::RED);
  }
}

void SmartTrafficLight::update(double currentTime) {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("SmartTrafficLight"));
  this->currentTime = currentTime;

  if(checkAndSwitch()) return;

  if (currentState == LightState::RED) {
    if (wantsToBeGreen()) {
      if (linkedLight && !linkedLight->wantsToBeGreen()) {
          intersectionLightsSetStateToGreen();
      }
    }
  }
}

bool SmartTrafficLight::checkAndSwitch() {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("SmartTrafficLight"));
  if (currentTime - lastChange < minStaticTime) {
    return true;
  }
  if (currentTime - lastChange > maxStaticTime) {
    if (currentState == LightState::RED) {
      intersectionLightsSetStateToGreen();
    } else {
      linkedLight->intersectionLightsSetStateToGreen();
    }
    return true;
  }
  return false;
}

bool SmartTrafficLight::isVisible() const {
  REQUIRE(this->properlyInitialized(),
          ErrorMessagesInstructions::properInitializationRequire(
              "SmartTrafficLight"));
  return visible;
}

void SmartTrafficLight::setVisibility(const bool v) {
  REQUIRE(this->properlyInitialized(),
          ErrorMessagesInstructions::properInitializationRequire(
              "SmartTrafficLight"));
  visible = v;
  ENSURE(v == isVisible(),
         ErrorMessagesInstructions::notUpdatedSuccessfully("visibility"));
}
void SmartTrafficLight::setMinStaticTime(double m) {
  REQUIRE(this->properlyInitialized(),
          ErrorMessagesInstructions::properInitializationRequire(
              "SmartTrafficLight"));
  minStaticTime = m;
}
void SmartTrafficLight::setMaxStaticTime(double m) {
  REQUIRE(this->properlyInitialized(),
        ErrorMessagesInstructions::properInitializationRequire(
            "SmartTrafficLight"));
  maxStaticTime = m;
}

SmartTrafficLight *SmartTrafficLight::getLinkedLight() const {
  REQUIRE(this->properlyInitialized(),
        ErrorMessagesInstructions::properInitializationRequire(
            "SmartTrafficLight"));
  return linkedLight;
}

bool SmartTrafficLight::wantsToBeGreen() const {
  for (Vehicle* vehicle : road->getVehicles()) {
    double distance = getPosition() - vehicle->getPosition();
    if (distance >= 0 && distance < vehicle->getDecelerationDistance()) {
      return true;
    }
  }
  return false;
}

BusStop::BusStop(Road *road, unsigned int position, double waitTime): Stop(road, position), occupied(nullptr), waitTime(waitTime) {
  REQUIRE(waitTime > 0, ErrorMessagesInstructions::mustBePositive("waittime"));
}

BusStop::BusStop(Road *road, const unsigned int position) : Stop(road, position), occupied(nullptr), waitTime(20) {}

Bus *BusStop::getOccupied() const {
  REQUIRE(this->properlyInitialized(),
          ErrorMessagesInstructions::properInitializationEnsure("BusStop"));
  ENSURE(occupied == nullptr || occupied->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Bus"))
  return occupied;
}

void BusStop::update(double currentTime) {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("BusStop"));
  REQUIRE(currentTime >= 0, ErrorMessagesInstructions::mustBePositive("time"));
  if (!occupied) {
    lastChange = currentTime;
  }
}

bool BusStop::isAvailable(double currentTime) const {
  return (currentTime - lastOccupiedTime >= cooldownPeriod);
}

void BusStop::setOccupied(Bus *bus) {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("BusStop"));
  REQUIRE(bus == nullptr || bus->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Bus"));
  occupied = bus;
}

double BusStop::getLastChange(const Bus *busToCheck) const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("BusStop"));
  REQUIRE(busToCheck->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("busToCheck"));
  return busToCheck == occupied || occupied == nullptr ? lastChange : -std::numeric_limits<double>::infinity();
}
double BusStop::getWaitTime() const {
  REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("BusStop"));
  return waitTime;
}
