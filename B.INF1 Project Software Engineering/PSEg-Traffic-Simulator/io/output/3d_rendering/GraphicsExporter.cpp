#include "GraphicsExporter.h"
#include "../../../DesignByContract.h"
#include "../../../ErrorMessages.h"
#include "../../../classes/Stop.h"
#include "../../../classes/Vehicle.h"
#include "../LayoutUtils.h"
#include "fstream"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#ifdef _WIN32
#define M_PI 3.14159265358979323846
#endif

void GraphicsExporter::exportToImage(Simulation *simulation, const std::string &filename, const std::string& type, const double& x_eye, const double& y_eye, const double& z_eye) {
    // REQUIRE preconditions
    REQUIRE(simulation != nullptr, ErrorMessagesInstructions::missingParameter("simulation"));
    REQUIRE(simulation->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Simulation"));
    REQUIRE(!filename.empty(), ErrorMessagesInstructions::missingParameter("filename"));
    REQUIRE(type == "Wireframe" || type == "ZBufferedWireframe", "Type must be 'Wireframe' or 'ZBufferedWireframe'");

    int nrFigures = 0;
    int size = 2500;
    std::string computedFilename;
    if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".ini") {
        computedFilename = filename;
    } else {
        computedFilename = filename + ".ini";
    }
    std::ofstream iniFile(computedFilename);


    static std::vector<std::vector<Road*>> allComponents = Utils::groupConnectedRoads(simulation->getRoads());

    std::unordered_map<Road*, sf::Vector2f> roadStartPositions;
    std::unordered_map<Road*, float> roadAngles;

    Utils::computeLayoutForChain(allComponents[0], roadStartPositions, roadAngles);

    std::vector<std::ostringstream> figures;
    int index = 0;
    constexpr float roadWidth = 2.0f;
    for (Road* r : allComponents[0]) {
      auto itPos = roadStartPositions.find(r);
        sf::Vector2f origin = itPos->second;

        auto itAngle = roadAngles.find(r);
        double angleDeg = itAngle->second;

        // Create the first edge of the road
        std::ostringstream figure1;
        figure1 << "[Figure" << index << "]" << std::endl;
        figure1 << "type = \"LineDrawing\"" << std::endl;
        figure1 << "rotateX = 0" << std::endl;
        figure1 << "rotateY = 0" << std::endl;
        figure1 << "rotateZ = " << angleDeg << std::endl;
        figure1 << "scale = 1.0" << std::endl;
        figure1 << "center = (" << origin.x << ", " << origin.y << ", 0)" << std::endl;
        figure1 << "color = (1.0, 1.0, 1.0)" << std::endl;
        figure1 << "nrPoints = 4" << std::endl;
        figure1 << "nrLines = 4" << std::endl;
        figure1 << "point0 = (" << 0 << ", " << 0 << ", 0)" << std::endl;
        figure1 << "point1 = (" << r->getLength() << ", " << 0 << ", 0)" << std::endl;
        figure1 << "point2 = (" << 0 << ", " << -roadWidth << ", 0)" << std::endl;
        figure1 << "point3 = (" << r->getLength() << ", " << -roadWidth << ", 0)" << std::endl;
        figure1 << "line0 = (0, 1)" << std::endl;
        figure1 << "line1 = (2, 3)" << std::endl;
        figure1 << "line2 = (0, 2)" << std::endl;
        figure1 << "line3 = (1, 3)" << std::endl;

        figures.push_back(std::move(figure1));
        index += 1;
        nrFigures += 1;

        for(auto c : r->getVehicles()){
            std::ostringstream figure2;
            figure2 << "[Figure" << index << "]" << std::endl;
            figure2 << "type = \"LineDrawing\"" << std::endl;
            figure2 << "rotateX = 0" << std::endl;
            figure2 << "rotateY = 0" << std::endl;
            figure2 << "rotateZ = " << angleDeg << std::endl;
            figure2 << "scale = 1.0" << std::endl;
            figure2 << "center = (" << origin.x + (c->getPosition()) * cos(angleDeg * M_PI/180) << ", " << origin.y  + (c->getPosition()) * sin(angleDeg * M_PI/180)<< ", 0)" << std::endl;
            if(dynamic_cast<Car*>(c)){
                figure2 << "color = (0.0, 1.0, 0.0)" << std::endl;
            } else if(dynamic_cast<Ambulance*>(c)){
                figure2 << "color = (1.0, 1.0, 1.0)" << std::endl;
            } else if(dynamic_cast<FireTruck*>(c)){
                figure2 << "color = (1.0, 0.0, 0.0)" << std::endl;
            } else if(dynamic_cast<Bus*>(c)){
                figure2 << "color = (1.0, 1.0, 0.0)" << std::endl;
            } else if(dynamic_cast<Police*>(c)){
                figure2 << "color = (0.0, 0.0, 1.0)" << std::endl;
            } else {
                figure2 << "color = (1.0, 1.0, 1.0)" << std::endl;
            }
            if(!dynamic_cast<Bus*>(c)){
                std::ifstream carfile("./io/output/3d_rendering/auto.ini"); // Open the file
                std::ostringstream car;
                if (carfile) {
                    car << carfile.rdbuf();
                }
                figure2 << car.str();
                figures.push_back(std::move(figure2));
            }else{
                std::ifstream carfile("./io/output/3d_rendering/bus.ini"); // Open the file
                std::ostringstream car;
                if (carfile) {
                    car << carfile.rdbuf();
                }
                figure2 << car.str();
                figures.push_back(std::move(figure2));
            }

            index += 1;
            nrFigures += 1;
        }

        for(auto s : r->getStops()){
            if(dynamic_cast<BusStop*>(s)){
                std::ostringstream figure4;
                figure4 << "[Figure" << index << "]" << std::endl;
                figure4 << "type = \"LineDrawing\"" << std::endl;
                figure4 << "rotateX = 90" << std::endl;
                figure4 << "rotateY = 0" << std::endl;
                figure4 << "rotateZ = " << angleDeg << std::endl;
                figure4 << "scale = 0.1" << std::endl;
                figure4 << "center = (" << origin.x + (s->getPosition() + 8) * cos(angleDeg * M_PI/180) << ", " << origin.y  + (s->getPosition()) * sin(angleDeg * M_PI/180)<< ", 0)" << std::endl;
                std::ifstream busfile("./io/output/3d_rendering/bushalte.ini"); // Open the file
                std::ostringstream bus;
                if (busfile) {
                    bus << busfile.rdbuf();
                }
                figure4 << bus.str();
                figures.push_back(std::move(figure4));
                index += 1;
                nrFigures += 1;

            }else if(auto lightOB = dynamic_cast<TrafficLight*>(s)){
                if(auto smartlight = dynamic_cast<SmartTrafficLight*>(lightOB)){
                    if(!smartlight->isVisible()){
                        continue;
                    }
                }
                std::ostringstream figure3;
                figure3 << "[Figure" << index << "]" << std::endl;
                figure3 << "type = \"LineDrawing\"" << std::endl;
                figure3 << "rotateX = 90" << std::endl;
                figure3 << "rotateY = 0" << std::endl;
                figure3 << "rotateZ = " << angleDeg << std::endl;
                figure3 << "scale = 0.1" << std::endl;
                figure3 << "center = (" << origin.x + (s->getPosition()) * cos(angleDeg * M_PI/180) << ", " << origin.y  + (s->getPosition()) * sin(angleDeg * M_PI/180)<< ", 0)" << std::endl;
                if(lightOB->getCurrentState() == LightState::RED){
                    figure3 << "color = (1.0, 0.0, 0.0)" << std::endl;
                }else if(lightOB->getCurrentState() == LightState::GREEN){
                    figure3 << "color = (0.0, 1.0, 0.0)" << std::endl;
                }else if(lightOB->getCurrentState() == LightState::ORANGE){
                    figure3 << "color = (1.0, 0.5, 0.0)" << std::endl;
                }

                std::ifstream lightfile("./io/output/3d_rendering/light.ini"); // Open the file
                std::ostringstream light;
                if (lightfile) {
                    light << lightfile.rdbuf();
                }
                figure3 << light.str();
                figures.push_back(std::move(figure3));
                index += 1;
                nrFigures += 1;
            }
        }
    }

    iniFile << "[General]" << std::endl;
    iniFile << "type = \"" << type << "\"" << std::endl;
    iniFile << "size = " << size << std::endl;
    iniFile << "backgroundcolor = (0, 0, 0)" << std::endl;
    iniFile << "nrFigures = " << nrFigures << std::endl;
    iniFile << "eye = (" << std::to_string(x_eye) << ", " << std::to_string(y_eye) << ", " << std::to_string(z_eye) << ")" << std::endl;
    iniFile << std::endl;

    for (const auto& figure : figures) {
        iniFile << figure.str() << std::endl;
    }

    iniFile.close();

    std::string command;
#ifdef _WIN32
    command = "io\\output\\3d_rendering\\graphics-engine\\engine.exe";
#else
    command = "./io/output/3d_rendering/graphics-engine/engine";
#endif
    command += " " + computedFilename; // Append the filename to the command

    // Run the command
    int returnCode = std::system(command.c_str());

    if (returnCode != 0) {
        std::cerr << "Error: Command failed with return code " << returnCode << std::endl;
    }

    // ENSURE postconditions
    std::ifstream checkFile(computedFilename);
    ENSURE(checkFile.good(), "Output ini file must exist after execution");
    checkFile.close();

    // Check if graphics engine produced output (typically creates image file with same base name)
    std::string imageFilename = computedFilename.substr(0, computedFilename.find_last_of('.')) + ".bmp";
    std::ifstream checkImageFile(imageFilename);
    ENSURE(returnCode == 0, "Graphics engine must execute successfully");
    checkImageFile.close();

    ENSURE(simulation->properlyInitialized(), "Simulation state must remain unchanged");
}