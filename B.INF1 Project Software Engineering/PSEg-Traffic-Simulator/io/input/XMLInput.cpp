#include "XMLInput.h"
#include "../../DesignByContract.h"
#include "../../ErrorMessages.h"
#include "../../classes/Simulation.h"
#include "../../classes/Stop.h"
#include "tinyxml.h"
#include <array>
#include <iostream>
#include <set>
#include <stdexcept>
#include <ostream>

void XMLInput::XMLInput(const char *filename, Simulation *simulation, std::ostream& errout) {
  // Validate the constructor parameters
  REQUIRE(filename != nullptr,
          ErrorMessagesInstructions::missingParameter("filename"));
  REQUIRE(simulation != nullptr,
          ErrorMessagesInstructions::missingParameter("simulation"));

  // Load the XML file
  TiXmlDocument doc;
  if (!doc.LoadFile(filename)) {
    errout << ErrorMessagesReadXml::invalidFile(filename) << std::endl;
    return;
  }

  // Get the root element
  TiXmlElement *root = doc.FirstChildElement();
  if (!root) {
    errout << ErrorMessagesReadXml::noValidParameter("root") << std::endl;
    return;
  }

  // Process all <BAAN> elements (Roads)
  for (TiXmlElement *elem = root->FirstChildElement("BAAN"); elem != nullptr;
       elem = elem->NextSiblingElement("BAAN")) {
    try {
      // <naam>
      TiXmlElement *nameElem = elem->FirstChildElement("naam");
      if (nameElem == nullptr) {
        throw std::invalid_argument(
            ErrorMessagesReadXml::missingParameter("name", "Road"));
      }

      // <lengte>
      TiXmlElement *lengthElem = elem->FirstChildElement("lengte");
      if (lengthElem == nullptr) {
        throw std::invalid_argument(
            ErrorMessagesReadXml::missingParameter("length", "Road"));
      }

      // Convert length to integer
      int length;
      try {
        length = std::stoi(lengthElem->GetText());
      } catch (const std::invalid_argument &) {
        throw std::invalid_argument(
            ErrorMessagesReadXml::invalidType("length", "integer"));
      }

      // Validate length
      if (length < 0) {
        throw std::invalid_argument(
            ErrorMessagesReadXml::mustBePositive("length"));
      }

      // Extract the road name
      std::string roadName = nameElem->GetText();
      if (roadName.empty()) {
        throw std::invalid_argument(ErrorMessagesReadXml::missingValue("name"));
      }

      // Ensure it's unique in the simulation
      if (simulation->findRoadWithName(roadName) != nullptr) {
        throw std::invalid_argument(
            ErrorMessagesReadXml::onlyOneAllowed("road", "Simulation"));
      }

      // Create and add the Road
      Road *road = new Road(length, roadName);
      simulation->addRoad(road);

    } catch (const std::exception &e) {
      errout << e.what() << std::endl;
    }
  }

  // Process all other elements: <VERKEERSLICHT>, <VOERTUIG>,
  // <VOERTUIGGENERATOR>, <BUSHALTE>, <KRUISPUNT>
  for (TiXmlElement *elem = root->FirstChildElement(); elem != nullptr;
       elem = elem->NextSiblingElement()) {
    // Skip if it's a <BAAN> element (already processed)
    if (strcmp(elem->Value(), "BAAN") == 0) {
      continue;
    }

    // TRAFFIC LIGHT: <VERKEERSLICHT>
    if (strcmp(elem->Value(), "VERKEERSLICHT") == 0) {
      try {
        // <baan>
        TiXmlElement *roadNameElem = elem->FirstChildElement("baan");
        if (roadNameElem == nullptr) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::missingParameter("name", "TrafficLight"));
        }

        // <positie>
        TiXmlElement *positionElem = elem->FirstChildElement("positie");
        if (positionElem == nullptr) {
          throw std::invalid_argument(ErrorMessagesReadXml::missingParameter(
              "position", "TrafficLight"));
        }

        // <cyclus>
        TiXmlElement *cycleElem = elem->FirstChildElement("cyclus");
        if (cycleElem == nullptr) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::missingParameter("cycle", "TrafficLight"));
        }

        // Convert position to int
        int position;
        try {
          position = std::stoi(positionElem->GetText());
        } catch (const std::invalid_argument &) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::invalidType("position", "integer"));
        }

        // Validate position
        if (position < 0) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::mustBePositive("position"));
        }

        // Convert cycle to int
        int cycle;
        try {
          cycle = std::stoi(cycleElem->GetText());
        } catch (const std::invalid_argument &) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::invalidType("cycle", "integer"));
        }

        // Validate cycle
        if (cycle < 0) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::mustBePositive("cycle"));
        }

        // Extract the road name
        std::string roadName = roadNameElem->GetText();
        if (roadName.empty()) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::missingValue("name"));
        }

        // Find the Road in the simulation
        Road *road = simulation->findRoadWithName(roadName);
        if (road == nullptr) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::missingValue("road"));
        }

        // Create the TrafficLight and add it
        TrafficLight *trafficLight = new TrafficLight(road, position, cycle);
        road->addStop(trafficLight);

      } catch (const std::exception &e) {
        errout << e.what() << std::endl;
      }

      // VEHICLE: <VOERTUIG>
    } else if (strcmp(elem->Value(), "VOERTUIG") == 0) {
      try {
        // <baan>
        TiXmlElement *roadNameElem = elem->FirstChildElement("baan");
        if (roadNameElem == nullptr) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::missingParameter("name", "Vehicle"));
        }

        // <positie>
        TiXmlElement *positionElem = elem->FirstChildElement("positie");
        if (positionElem == nullptr) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::missingParameter("position", "Vehicle"));
        }

        // Extract the road name
        std::string roadName = roadNameElem->GetText();
        if (roadName.empty()) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::missingValue("name"));
        }

        // Convert position to int
        int position;
        try {
          position = std::stoi(positionElem->GetText());
        } catch (const std::invalid_argument &) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::invalidType("position", "integer"));
        }

        // Validate position
        if (position < 0) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::mustBePositive("position"));
        }

        // Find the Road
        Road *road = simulation->findRoadWithName(roadName);
        if (road == nullptr) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::missingValue("road"));
        }

        // <type>
        Vehicle *vehicle;
        TiXmlElement *typeElem = elem->FirstChildElement("type");
        if (typeElem) {
          std::string type = typeElem->GetText();
          if (type == "auto") {
            vehicle = new Car(road, position);
          } else if (type == "bus") {
            vehicle = new Bus(road, position);
          } else if (type == "ziekenwagen") {
            vehicle = new Ambulance(road, position);
          } else if (type == "brandweerwagen") {
            vehicle = new FireTruck(road, position);
          } else if (type == "politiewagen") {
            vehicle = new Police(road, position);
          } else {
            throw std::invalid_argument(
                ErrorMessagesInstructions::invalidVehicleType(type));
          }
        } else {
          vehicle = new Car(road, position);
        }

        // Create the Vehicle and add it
        road->addVehicle(vehicle);

      } catch (const std::exception &e) {
        errout << e.what() << std::endl;
      }

      // VEHICLE GENERATOR: <VOERTUIGGENERATOR>
    } else if (strcmp(elem->Value(), "VOERTUIGGENERATOR") == 0) {
      try {
        // <baan>
        TiXmlElement *roadNameElem = elem->FirstChildElement("baan");
        if (roadNameElem == nullptr) {
          throw std::invalid_argument(ErrorMessagesReadXml::missingParameter(
              "name", "VehicleGenerator"));
        }

        // <frequentie>
        TiXmlElement *frequencyElem = elem->FirstChildElement("frequentie");
        if (frequencyElem == nullptr) {
          throw std::invalid_argument(ErrorMessagesReadXml::missingParameter(
              "frequency", "VehicleGenerator"));
        }

        // Extract the road name
        std::string roadName = roadNameElem->GetText();
        if (roadName.empty()) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::missingValue("name"));
        }

        // Convert frequency to int
        int frequency;
        try {
          frequency = std::stoi(frequencyElem->GetText());
        } catch (const std::invalid_argument &) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::invalidType("frequency", "integer"));
        }

        // Validate frequency
        if (frequency < 0) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::mustBePositive("frequency"));
        }

        // Find the Road
        Road *road = simulation->findRoadWithName(roadName);
        if (road == nullptr) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::missingValue("road"));
        }

        // Check if there's already an active VehicleGenerator
        if (road->getVehicleGenerator() != nullptr &&
            road->getVehicleGenerator()->getFrequency() != 0) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::onlyOneAllowed("vehicleGenerator", "Road"));
        }
        VehicleGenerator *vehicleGenerator = nullptr;

        // <type>
        TiXmlElement *typeElem = elem->FirstChildElement("type");
        if (typeElem) {
          vehicleGenerator =
              new VehicleGenerator(frequency, road, typeElem->GetText());
        } else {
          vehicleGenerator = new VehicleGenerator(frequency, road, "random");
        }

        // Create and set the VehicleGenerator
        road->setVehicleGenerator(vehicleGenerator);

      } catch (const std::exception &e) {
        errout << e.what() << std::endl;
      }

    } else if (strcmp(elem->Value(), "BUSHALTE") == 0) {
      try {
        // <baan>
        TiXmlElement *roadNameElem = elem->FirstChildElement("baan");
        if (roadNameElem == nullptr) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::missingParameter("name", "TrafficLight"));
        }

        // <positie>
        TiXmlElement *positionElem = elem->FirstChildElement("positie");
        if (positionElem == nullptr) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::missingParameter("position", "Vehicle"));
        }

        // <wachttijd>
        TiXmlElement *waitTimeElem = elem->FirstChildElement("wachttijd");
        if (waitTimeElem == nullptr) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::missingParameter("waittime", "Vehicle"));
        }

        // Extract the road name
        std::string roadName = roadNameElem->GetText();
        if (roadName.empty()) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::missingValue("name"));
        }

        // Convert position to int
        int position;
        try {
          position = std::stoi(positionElem->GetText());
        } catch (const std::invalid_argument &) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::invalidType("position", "integer"));
        }

        int waitTime;
        try {
          waitTime = std::stoi(waitTimeElem->GetText());
        } catch (const std::invalid_argument &) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::invalidType("waittime", "integer"));
        }

        // Validate position
        if (position < 0) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::mustBePositive("position"));
        }

        // Validate position
        if (waitTime < 0) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::mustBePositive("waittime"));
        }

        // Find the Road
        Road *road = simulation->findRoadWithName(roadName);
        if (road == nullptr) {
          throw std::invalid_argument(
              ErrorMessagesReadXml::missingValue("road"));
        }

        // Create the TrafficLight and add it
        BusStop *busStop = new BusStop(road, position, waitTime);
        road->addStop(busStop);

      } catch (const std::exception &e) {
        errout << e.what() << std::endl;
      }
    } else if (strcmp(elem->Value(), "KRUISPUNT") == 0) {
      Intersection *intersection = new Intersection();
      try {
        // Itereer over alle <baan> elementen
        for (TiXmlElement *roadElem = elem->FirstChildElement("baan");
             roadElem != nullptr;
             roadElem = roadElem->NextSiblingElement("baan")) {

          // <naam>
          TiXmlElement *nameElem = roadElem->FirstChildElement("naam");
          if (nameElem == nullptr || nameElem->GetText() == nullptr ||
              std::string(nameElem->GetText()).empty()) {
            throw std::invalid_argument(
                ErrorMessagesReadXml::missingParameter("name", "Intersection"));
          }
          std::string roadName = nameElem->GetText();

          // <positie>
          TiXmlElement *positionElem = roadElem->FirstChildElement("positie");
          if (positionElem == nullptr || positionElem->GetText() == nullptr) {
            throw std::invalid_argument(ErrorMessagesReadXml::missingParameter(
                "position", "Intersection"));
          }

          int position;
          try {
            position = std::stoi(positionElem->GetText());
          } catch (const std::invalid_argument &) {
            throw std::invalid_argument(
                ErrorMessagesReadXml::invalidType("position", "integer"));
          }

          if (position < 0) {
            throw std::invalid_argument(
                ErrorMessagesReadXml::mustBePositive("position"));
          }

          // Zoek de bijbehorende Road
          Road *road = simulation->findRoadWithName(roadName);
          if (road == nullptr) {
            throw std::invalid_argument(
                ErrorMessagesReadXml::missingValue("road: " + roadName));
          }

          // Voeg toe aan kruispunt
          intersection->addIncomingRoad(road, position);
          intersection->addOutgoingRoad(road, position);
          road->addIntersectionAt(intersection, position);
        }

        // Check voor <verkeerslicht>
        TiXmlElement *trafficLightElem =
            elem->FirstChildElement("verkeerslicht");
        if (trafficLightElem != nullptr) {
          double min;
          TiXmlElement *minElem = trafficLightElem->FirstChildElement("minimum");
          try {
            min = std::stod(minElem->GetText());
          } catch (const std::invalid_argument &) {
            throw std::invalid_argument(
                ErrorMessagesReadXml::invalidType("minimum", "double"));
          }

          double max;
          TiXmlElement *maxElem = trafficLightElem->FirstChildElement("maximum");
          try {
            max = std::stod(maxElem->GetText());
          } catch (const std::invalid_argument &) {
            throw std::invalid_argument(
                ErrorMessagesReadXml::invalidType("maximum", "double"));
          }
          if (min < 0) {
            throw std::invalid_argument(ErrorMessagesReadXml::mustBePositive("minimum"));
          }
          if (max < 0) {
            throw std::invalid_argument(ErrorMessagesReadXml::mustBePositive("maximum"));
          }
          for (SmartTrafficLight* light : intersection->getTrafficLights()) {
            light->setVisibility(true);
            light->setMinStaticTime(min);
            light->setMaxStaticTime(max);
          }
        }
        simulation->addIntersection(intersection);
      } catch (const std::exception &e) {
        delete intersection;
        errout << e.what() << std::endl;
      }
    }
  }
}

Simulation XMLInput::XMLInput(const char *input, std::ostream& errout) {
  Simulation sim;
  XMLInput(input, &sim, errout);
  return sim;
}
