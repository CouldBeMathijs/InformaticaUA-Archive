#include "Intersection.h"
#include "../DesignByContract.h"
#include "../ErrorMessages.h"
#include <algorithm>
#include <array>
#include <ctime>
#include <iostream>
#include <random>
#include <stdexcept>

Intersection::Intersection() {
    _initCheck = this;
    ENSURE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationEnsure("Intersection"));
}

bool Intersection::properlyInitialized() const {
    return _initCheck;
}

void Intersection::addOutgoingRoad(Road *outRoad, double position) {
  REQUIRE(
      this->properlyInitialized(),
      ErrorMessagesInstructions::properInitializationRequire("Intersection"));
  REQUIRE(outRoad != nullptr,
          ErrorMessagesInstructions::missingParameter("road"));
  REQUIRE(outRoad->properlyInitialized(),
          ErrorMessagesInstructions::properInitializationRequire("Road"));
  REQUIRE(position == -1 || (position > 0 && position <= outRoad->getLength()),
          ErrorMessagesInstructions::outOfBoundsRequire("position"));
  if (position < 0)
    position = static_cast<double>(outRoad->getLength());
  outgoingDetailed.push_back({outRoad, position});
  ENSURE(outgoingDetailed.back().road == outRoad,
         ErrorMessagesInstructions::notUpdatedSuccessfully("outgoingDetailed"));
}

std::array<SmartTrafficLight *, 2> Intersection::getTrafficLights() {
    return {firstLight, firstLight->getLinkedLight()};
}

void Intersection::addIncomingRoad(Road* inRoad, double position) {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Intersection"));
    REQUIRE(inRoad != nullptr, ErrorMessagesInstructions::missingParameter("road"));
    REQUIRE(inRoad->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
    REQUIRE(position == -1 || (position > 0 && position <= inRoad->getLength()),
            ErrorMessagesInstructions::outOfBoundsRequire("position"));
    if (position < 0) position = static_cast<double>(inRoad->getLength());
    incomingDetailed.push_back({inRoad, position});
    ENSURE(incomingDetailed.back().road == inRoad, ErrorMessagesInstructions::notUpdatedSuccessfully("incomingDetailed"));

    SmartTrafficLight* smartLight = new SmartTrafficLight(inRoad, static_cast<unsigned int>(position - 0.00001), false, 0, 10);
    inRoad->addStop(smartLight);

    if (firstLight) {
        smartLight->setLinkedLight(firstLight);
        firstLight->setLinkedLight(smartLight);
    } else {
        firstLight = smartLight;
    }
}

std::vector<ConnectedRoad> Intersection::getIncomingRoadsDetailed() const {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Intersection"));
    return incomingDetailed;
}

std::vector<ConnectedRoad> Intersection::getOutgoingRoadsDetailed() const {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Intersection"));
    return outgoingDetailed;
}

const std::vector<Road*>& Intersection::getIncomingRoads() const {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Intersection"));
    static std::vector<Road*> result;
    result.clear();
    for (const auto& entry : incomingDetailed) result.push_back(entry.road);
    ENSURE(result.size() == incomingDetailed.size(), ErrorMessagesInstructions::mismatchedComputedValue("incoming roads", "incomingDetailed"));
    return result;
}

const std::vector<Road*>& Intersection::getOutgoingRoads() const {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Intersection"));
    static std::vector<Road*> result;
    result.clear();
    for (const auto& entry : outgoingDetailed) result.push_back(entry.road);
    ENSURE(result.size() == outgoingDetailed.size(), ErrorMessagesInstructions::mismatchedComputedValue("outgoing roads", "outgoingDetailed"));
    return result;
}

Road* Intersection::getRandomNextRoad() const {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Intersection"));
    std::vector<Road*> validNexts;

    for (const auto& entry : outgoingDetailed) {
        if (entry.position != entry.road->getLength()) {
            validNexts.push_back(entry.road);
        }
    }

    if (validNexts.empty()) return nullptr;

    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<std::size_t> dist(0, validNexts.size() - 1);
    Road* out = validNexts[dist(rng)];

    ENSURE(std::any_of(outgoingDetailed.begin(), outgoingDetailed.end(),
                       [out](const ConnectedRoad& cr) { return cr.road == out; }),
           ErrorMessagesInstructions::mismatchedComputedValue("returnvalue", "outgoingroads"));

    return out;
}
