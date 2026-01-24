#include "SimulatieExporter.h"
#include "../../DesignByContract.h"
#include "../../ErrorMessages.h"
#include <cmath>
std::string removeTrailingZeroes(std::string str) {
  std::string out = std::move(str);
  for (int i = static_cast<int>(out.size()) - 1; i >= 0; --i) {
    if (out[i] != '0') {
      if (out[i] == '.') {
        out.erase(i);
      }
      break;
    }
    out.erase(i);
  }
  return out;
}

void SimulatieExporter::exportOn(std::ostream &onStream,
                                 const Simulation &simulation) {
  REQUIRE(simulation.properlyInitialized(),
          ErrorMessagesInstructions::properInitializationEnsure("Simulation"));
  onStream << "Tijd: " << removeTrailingZeroes(std::to_string(simulation.getTime())) << "s" << "\n";
  int autoIndex = 1;
  for (auto *baan : simulation.getRoads()) {
    for (auto *vehicle : baan->getVehicles()) {
      onStream << "\nVehicle " << std::to_string(autoIndex) << "\n"
               << "-> baan: " + vehicle->getRoad()->getName() << "\n"
               << "-> positie: " + removeTrailingZeroes(
                                       std::to_string(vehicle->getPosition()))
               << "\n"
               << "-> snelheid: " +
                      removeTrailingZeroes(std::to_string(vehicle->getSpeed()))
               << std::endl;

      autoIndex++;
    }
  }
}

void SimulatieExporter::doGraphicsImpression(std::ostream &out,
                                             const Road *road,
                                             unsigned int indenting = 20) {
  const std::string &name = road->getName();
  const auto &roadLength = road->getLength();
  REQUIRE(indenting - name.size() > 0, ErrorMessagesInstructions::NeverReached);
  REQUIRE(road->properlyInitialized(),
        ErrorMessagesInstructions::properInitializationEnsure("Road"));

  // Line 1
  out << road->getName();
  out << std::string(indenting - name.size(), ' ');
  out << '|';
  std::string roadVisualization(roadLength, '=');
  for (auto vehicle : road->getVehicles()) {
    unsigned int pos = std::lround(vehicle->getPosition());
    unsigned int length = std::lround(vehicle->getLength());

    unsigned int startPos =
        std::max(static_cast<unsigned>(0), pos - length + 1);
    unsigned int endPos = std::min(roadLength - 1, pos);
    char ch = '?';
    if (dynamic_cast<Car *>(vehicle)) {
      ch = 'A';
    } else if (dynamic_cast<Bus *>(vehicle)) {
      ch = 'B';
    } else if (dynamic_cast<Ambulance *>(vehicle)) {
      ch = 'Z';
    } else if (dynamic_cast<Police *>(vehicle)) {
      ch = 'P';
    } else if (dynamic_cast<FireTruck *>(vehicle)) {
      ch = 'F';
    } else {
      REQUIRE(false, ErrorMessagesInstructions::NeverReached);
    }
    for (unsigned int i = startPos; i <= endPos; i++) {
      roadVisualization[i] = ch;
    }
  }
  for (auto ib : road->getIntersectionBindings()) {
    unsigned int pos = std::lround(ib.position);
    roadVisualization[pos] = 'x';
  }
  out << roadVisualization << std::endl;

  // Line 2
  std::string vv = " > verkeerslichten";
  out << vv;
  out << std::string(indenting - vv.size(), ' ');
  out << '|';
  roadVisualization.assign(roadLength, ' ');
  for (auto stop : road->getStops()) {
    unsigned int pos = std::lround(stop->getPosition());
    if (auto trafficLight = dynamic_cast<TrafficLight *>(stop)) {
      // Some traffic lights mustn't be shown
      if (auto smarttrafficlight =
              dynamic_cast<SmartTrafficLight *>(trafficLight)) {
        if (!smarttrafficlight->isVisible()) {
          continue;
        }
      }
      const auto &state = trafficLight->getCurrentState();
      if (state == LightState::GREEN) {
        roadVisualization[pos] = 'G';
      } else if (state == LightState::RED) {
        roadVisualization[pos] = 'R';
      } else if (state == LightState::ORANGE) {
        roadVisualization[pos] = 'O';
      }
    } else {
      roadVisualization[pos] = '|';
    }
  }
  out << roadVisualization << std::endl;

  // Line 3
  vv = " > bushaltes";
  out << vv;
  out << std::string(indenting - vv.size(), ' ');
  out << '|';
  roadVisualization.assign(roadLength, ' ');
  for (auto stop : road->getStops()) {
    unsigned int pos = std::lround(stop->getPosition());
    if (dynamic_cast<BusStop *>(stop)) {
      roadVisualization[pos] = 'B';
    }
  }
  out << roadVisualization << std::endl;
}

void SimulatieExporter::doGraphicsImpression(std::ostream &out,
                                             const Simulation &sim) {
  size_t indenting = 20;
  for (auto road : sim.getRoads()) {
    indenting = std::max(road->getName().size(), indenting);
  }
  for (auto road : sim.getRoads()) {
    doGraphicsImpression(out, road, indenting);
  }
}

void SimulatieExporter::visualizeSimulationState(std::ostream &onStream,
                                                 const Simulation &sim) {
  onStream << "This visualization mechanism is deprecated, please use the GUI "
              "or doGraphicsImpression() in stead...\n\n\n"
           << std::endl;
  REQUIRE(sim.properlyInitialized(),
        ErrorMessagesInstructions::properInitializationEnsure("Simulation"));
  for (auto *road : sim.getRoads()) {
    int roadLength = static_cast<int>(road->getLength());
    if (roadLength <= 0) {
      onStream << "Road \"" << road->getName() << "\" has invalid length.\n\n";
      continue;
    }

    std::vector<std::string> roadVisualization(roadLength, "═");

    for (auto *stop : road->getStops()) {
      int pos = static_cast<int>(stop->getPosition());
      if (dynamic_cast<TrafficLight *>(stop)) {
        auto traffic_light = dynamic_cast<TrafficLight *>(stop);
        if (pos >= 0 && pos < roadLength) {
          if (traffic_light->getCurrentState() == LightState::GREEN) {
            roadVisualization[pos] = "💚";
          } else if (traffic_light->getCurrentState() == LightState::RED) {
            roadVisualization[pos] = "🔴";
          } else if (traffic_light->getCurrentState() == LightState::ORANGE) {
            roadVisualization[pos] = "🔶";
          } else {
            ENSURE(false, ErrorMessagesInstructions::NeverReached);
          }
        }
        continue;
      }
      if (dynamic_cast<TrafficLight *>(stop)) {
        roadVisualization[pos] = "🚏";
        continue;
      }
      ENSURE(false, ErrorMessagesInstructions::NeverReached);
    }

    for (auto *vehicle : road->getVehicles()) {
      int pos = static_cast<int>(vehicle->getPosition());
      int length = static_cast<int>(vehicle->getLength());

      int startPos = std::max(0, pos - length + 1);
      int endPos = std::min(roadLength - 1, pos);

      // Achterkant
      if (startPos >= 0 && startPos < roadLength) {
        roadVisualization[startPos] = "🛞";
      }

      // Middenstuk (indien langer dan 2)
      for (int i = startPos + 1; i < endPos; i++) {
        if (i >= 0 && i < roadLength) {
          roadVisualization[i] = "🛞";
        }
      }

      // Begin van het voertuig
      if (endPos >= 0 && endPos < roadLength && length > 1) {
        roadVisualization[endPos] = "🚘";
      }
    }

    onStream << "Road \"" << road->getName() << "\" (length " << roadLength
             << "):\n";

    for (int i = 0; i < roadLength; ++i) {
      onStream << roadVisualization[i];
    }
    onStream << "\n\n";
  }
}