//============================================================================
// Name        : MainTests.cpp
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : The Main Test file which stores all the Google Tests
//============================================================================
#include "DesignByContract.h"
#include "ErrorMessages.h"
#include "classes/Simulation.h"
#include "classes/Stop.h"
#include "classes/Vehicle.h"
#include "io/input/XMLInput.h"
#include "io/output/3d_rendering/GraphicsExporter.h"
#include "io/output/SimulatieExporter.h"
#include <algorithm>
#include <filesystem>
#include <functional>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include "io/output/simulation/Button.h"
#include "io/output/simulation/ButtonManager.h"
#include "io/output/simulation/Camera.h"
#include "io/output/simulation/ResourceManager.h"
#include "io/output/simulation/RoadInfoPanel.h"
#include "io/output/simulation/SimulationRenderer.h"
#include "io/output/simulation/Gui.h"

#include "io/output/LayoutUtils.h"

#include <SFML/Graphics.hpp>

class TrafficSimulatorInputTests: public ::testing::Test {
protected:
    // You should make the members protected so that they can be
    // accessed from subclasses.

    // virtual void SetUp() will be called before each test is run.
    // You should define it if you need to initialize variables.
    // Otherwise, this can be skipped.
    virtual void SetUp() {
    }

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.
    // Otherwise, you do not have to provide it.
    virtual void TearDown() {
    }
};

class TrafficSimulatorTests: public ::testing::Test {
protected:
    virtual void SetUp() {
    }
    virtual void TearDown() {
    }
};

/**
 * @brief Helper function that checks whether an exception is thrown
 *        and if its message contains all of the expected error strings.
 */
void expectExceptionWithMessage(const std::vector<std::string>& errorMessages, const std::vector<std::function<void()>>& functions) {
    try {
        for (const auto& function : functions) {
            function();
        }
        std::cerr << "Expected exception not thrown" << std::endl;
        FAIL();
    } catch (std::exception &e) {
        std::string error = e.what();
        for (const auto& expected : errorMessages) {
            std::size_t pos = error.find(expected);
            EXPECT_NE(pos, std::string::npos) << "Expected error message not found, got: " << error;

            if (pos != std::string::npos) {
                // Remove the found part so it won't match again
                error.erase(pos, expected.length());
            }
        }
    }
}

/**
 * @brief Helper function that checks whether stderr contains all the
 *        expected strings after running the provided functions.
 * @param errorMessages A vector with errorMessages to expect
 * @param functions A vector with functions to run and which will be caught and
 * checked against the errorMessages
 */
void expectStderrWithMessage(const std::vector<std::string>& errorMessages, const std::vector<std::function<void()>>& functions) {
    testing::internal::CaptureStderr();


    for (const auto& function : functions) {
        function();
    }

    std::string consoleOutput = testing::internal::GetCapturedStderr();

    std::istringstream stream(consoleOutput);
    std::string line;
    std::vector<std::string> remainingLines;

    while (std::getline(stream, line)) {
        bool matched = false;
        for (const auto& expected : errorMessages) {
            if (line.find(expected) != std::string::npos) {
                matched = true;
                break;  // Match found, skip this line
            }
        }
        if (!matched) {
            remainingLines.push_back(line);
        }
    }

    if (!remainingLines.empty()) {
        std::ostringstream remainingOutput;
        for (const auto& l : remainingLines) {
            remainingOutput << l << "\n";
        }
        ADD_FAILURE() << "Unexpected extra error message(s) found:\n" << remainingOutput.str();
    }
}



// ============================================================
// =                   START OF TESTS                         =
// ============================================================

// ============================================================
// =                  INSTRUCTION TESTS                       =
// ============================================================

// ============================================================
// =   MISSING-PARAMETER TESTS – OPGEDEELD PER KLASSE         =
// ============================================================

// ---------- Simulation --------------------------------------
TEST_F(TrafficSimulatorInputTests, MissingParameterSimulation) {
    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("filename")},
    { []() { Simulation sim = XMLInput::XMLInput(nullptr); } });

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("road")},
    { []() {
        Simulation sim = XMLInput::XMLInput("xml-files/basic.xml");
        sim.addRoad(nullptr);
    } });

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("road")},
    { []() {
        Simulation sim;
        sim.addIntersection(nullptr);
    } });
}

// ---------- XMLInput ----------------------------------------
TEST_F(TrafficSimulatorInputTests, MissingParameterXMLInput) {
    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("filename")},
    { []() { XMLInput::XMLInput(nullptr, nullptr, std::cerr); } });

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("simulation")},
    { []() { XMLInput::XMLInput("notnullptr", nullptr, std::cerr); } });
}

// ---------- Road --------------------------------------------
TEST_F(TrafficSimulatorInputTests, MissingParameterRoad) {
    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("name")},
    { []() { Road r(0, ""); } });

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("stop")},
    { []() { Road r(0, "x"); r.addStop(nullptr); } });

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("vehicle")},
    { []() { Road r(0, "x"); r.addVehicle(nullptr); } });

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("vehicle")},
    { []() { Road r(0, "x"); r.removeVehicle(nullptr); } });

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("intersection")},
    { []() { Road r(100, "x"); r.addIntersectionAt(nullptr, 50); } });
}

// ---------- Vehicle -------------------------------
TEST_F(TrafficSimulatorInputTests, MissingParameterVehicleDerived) {
    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("road")},
    { []() { Car c(nullptr, 0); } });

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("road")},
    { []() { Bus b(nullptr, 0); } });

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("road")},
    { []() { FireTruck ft(nullptr, 0); } });

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("road")},
    { []() { Ambulance a(nullptr, 0); } });

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("road")},
    { []() { Police p(nullptr, 0); } });
}

// ---------- Stops ----------------
TEST_F(TrafficSimulatorInputTests, MissingParameterStops) {
    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("road")},
    { []() { TrafficLight tl(nullptr, 0, 0); } });

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("light")},
    { []() {
        Road r(100, "R1");
        SmartTrafficLight stl(&r, 20, true);
        stl.setLinkedLight(nullptr);
    } });

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("road")},
    { []() { BusStop halte(nullptr, 10); } });
}

// ---------- VehicleGenerator --------------------------------
TEST_F(TrafficSimulatorInputTests, MissingParameterVehicleGenerator) {
    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("road")},
    { []() { VehicleGenerator vg(0, nullptr, "auto"); } });
}

// ---------- Intersection -----------------------------
TEST_F(TrafficSimulatorInputTests, MissingParameterIntersection) {
    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("road")},
    { []() {
        Intersection inter;
        inter.addOutgoingRoad(nullptr, 10);
    } });

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("road")},
    { []() {
        Intersection inter;
        inter.addIncomingRoad(nullptr, 10);
    } });
}

// ============================================================
// =   MUST-BE-POSITIVE TESTS – OPGEDEELD PER KLASSE          =
// ============================================================

// ---------- Simulation --------------------------------------
TEST_F(TrafficSimulatorInputTests, MustBePositiveSimulationRunSec) {
    expectExceptionWithMessage({ErrorMessagesInstructions::mustBePositive("seconds")},
    { []() {
        Simulation sim;
        sim.runSec(-1);
    }});
}

// ---------- Road --------------------------------------------
TEST_F(TrafficSimulatorInputTests, MustBePositiveRoadTick) {
    expectExceptionWithMessage({ErrorMessagesInstructions::mustBePositive("time")},
    { []() {
        Road r(500, "PosTest");
        r.tick(-1, 10);
    }});

    expectExceptionWithMessage({ErrorMessagesInstructions::mustBePositive("timeEachTick")},
    { []() {
        Road r(500, "PosTest");
        r.tick(5, -1.0);
    }});
}

// ---------- Stops ------------------------------------
TEST_F(TrafficSimulatorInputTests, MustBePositiveStops) {
    expectExceptionWithMessage({ErrorMessagesInstructions::mustBePositive("time")},
    { []() {
        Road r(500, "PosTest");
        TrafficLight tl(&r, 100, 5);
        tl.update(-1);
    }});

    expectExceptionWithMessage({ErrorMessagesInstructions::mustBePositive("waittime")},
    { []() {
        Road r(100, "R1");
        BusStop halte(&r, 30, 0);
    }});

    expectExceptionWithMessage({ErrorMessagesInstructions::mustBePositive("time")},
    { []() {
        Road r(100, "R1");
        BusStop halte(&r, 30, 10);
        halte.update(-5.0);
    }});
}

// ---------- Vehicle ----------------------------
TEST_F(TrafficSimulatorInputTests, MustBePositiveVehicleUpdateDt) {
    expectExceptionWithMessage({ErrorMessagesInstructions::mustBePositive("dt")},
    { []() {
        Road r(200, "PosTest");
        Car car(&r, 0);
        car.updateSpeedPosition(-1.0);
    }});

    expectExceptionWithMessage({ErrorMessagesInstructions::mustBePositive("dt")},
    { []() {
        Road r(200, "PosTest");
        Car car(&r, 0);
        car.updateSpeedPosition(0.0);
    }});

    expectExceptionWithMessage({ErrorMessagesInstructions::mustBePositive("currentTime")},
    { []() {
        Road r(200, "BusTime");
        Bus bus(&r, 0);
        bus.brakeBeforeBusStop(-2.0);
    }});
}

// ---------- VehicleGenerator --------------------------------
TEST_F(TrafficSimulatorInputTests, MustBePositiveVehicleGeneratorTime) {
    expectExceptionWithMessage({ErrorMessagesInstructions::mustBePositive("time")},
    { []() {
        Road r(500, "PosTest");
        VehicleGenerator vg(10, &r, "auto");
        vg.tick(-1.0);
    }});

    expectExceptionWithMessage({ErrorMessagesInstructions::mustBePositive("time")},
    { []() {
        Road r(500, "PosTest");
        VehicleGenerator vg(10, &r, "auto");
        (void)vg.canGenerateVehicle(-1.0);
    }});
}

// ============================================================
// =   OUT-OF-BOUNDS-POSITION TESTS – PER KLASSE              =
// ============================================================

// ---------- Vehicle -----------------------------------------
TEST_F(TrafficSimulatorInputTests, OutOfBoundsVehiclePosition) {
    expectExceptionWithMessage({ErrorMessagesInstructions::outOfBoundsRequire("position")},
    { []() {
        Road road(500, "Too short");
        Car car(&road, 600);
        road.addVehicle(&car);
    }});
}

// ---------- Stops ------------------------------------
TEST_F(TrafficSimulatorInputTests, OutOfBoundsStopPosition) {
    expectExceptionWithMessage({ErrorMessagesInstructions::outOfBoundsRequire("position")},
    { []() {
        Road road(500, "Too short");
        TrafficLight tl(&road, 600, 5);
        road.addStop(&tl);
    }});

    expectExceptionWithMessage({ErrorMessagesInstructions::outOfBoundsRequire("position")},
    { []() {
        Road r(50, "Korte weg");
        BusStop halte(&r, 100);
    }});
}

// ---------- Road -------------------------
TEST_F(TrafficSimulatorInputTests, OutOfBoundsRoadAddIntersectionLow) {
    expectExceptionWithMessage({ErrorMessagesInstructions::outOfBoundsRequire("position")},
    { []() {
        Road road(100, "Testweg");
        Intersection inter;
        road.addIntersectionAt(&inter, -1);
    }});
}

TEST_F(TrafficSimulatorInputTests, OutOfBoundsRoadAddIntersectionHigh) {
    expectExceptionWithMessage({ErrorMessagesInstructions::outOfBoundsRequire("position")},
    { []() {
        Road road(100, "Testweg");
        Intersection inter;
        road.addIntersectionAt(&inter, 200);
    }});
}

// ---------- Intersection -------------------
TEST_F(TrafficSimulatorInputTests, OutOfBoundsIntersection) {
    expectExceptionWithMessage({ErrorMessagesInstructions::outOfBoundsRequire("position")},
    { []() {
        Road r(100, "R1");
        Intersection inter;
        inter.addOutgoingRoad(&r, -2);
    }});

    expectExceptionWithMessage({ErrorMessagesInstructions::outOfBoundsRequire("position")},
    { []() {
        Road r(100, "R1");
        Intersection inter;
        inter.addOutgoingRoad(&r, 150);
    }});

    expectExceptionWithMessage({ErrorMessagesInstructions::outOfBoundsRequire("position")},
    { []() {
        Road r(100, "R1");
        Intersection inter;
        inter.addIncomingRoad(&r, -5);
    }});

    expectExceptionWithMessage({ErrorMessagesInstructions::outOfBoundsRequire("position")},
    { []() {
        Road r(100, "R1");
        Intersection inter;
        inter.addIncomingRoad(&r, 250);
    }});
}

TEST_F(TrafficSimulatorInputTests, invalidComparisonInstructions) {
    expectExceptionWithMessage({ErrorMessagesInstructions::invalidComparisonAsmallerthanB("maxStaticTime", "minStaticTime")},
    {
        []() {
            Road r(100, "R1");
            SmartTrafficLight tl(&r, 10, true, 20, 15);
        }
    });
}

TEST_F(TrafficSimulatorInputTests, checkBusStop) {
    Road newRoad(200, "The road");
    auto busstop = new BusStop(&newRoad, 150);
    newRoad.addStop(busstop);
    auto bus = new Bus(&newRoad, 0);
    newRoad.addVehicle(bus);
    double time = 0;
    bool busStopped = false;
    bool busStartedAgain = false;

    while (bus->getPosition() < 180) {
        newRoad.tick(time, 0.1);
        time += 0.1;

        if (!busStopped && bus->getSpeed() < 1) {
            busStopped = true;
        }

        if (busStopped && bus->getSpeed() >= 1) {
            busStartedAgain = true;
            break;
        }
    }

    if (!busStopped) {
        FAIL() << "The bus never stopped at the bus stop as expected.";
    } else if (!busStartedAgain) {
        FAIL() << "The bus stopped but never started again.";
    }
    // Otherwise, test passes
}

TEST_F(TrafficSimulatorInputTests, checkTrafficLightVehicleShouldStop) {
    Road newRoad(200, "The road");
    auto trafficlight = new TrafficLight(&newRoad, 150, 9999);
    trafficlight->setCurrentState(LightState::RED);
    newRoad.addStop(trafficlight);
    auto bus = new Bus(&newRoad, 0);
    newRoad.addVehicle(bus);
    double time = 0;
    bool busStopped = false;
    bool busStartedAgain = false;

    while (bus->getPosition() < 180) {
        newRoad.tick(time, 0.1);
        time += 0.1;

        if (!busStopped && bus->getSpeed() < 1) {
            busStopped = true;
            trafficlight->setCurrentState(LightState::GREEN);
        }

        if (busStopped && bus->getSpeed() >= 1) {
            busStartedAgain = true;
            break;
        }
    }

    if (!busStopped) {
        FAIL() << "The bus never stopped at the traffic light as expected.";
    } else if (!busStartedAgain) {
        FAIL() << "The bus stopped but never started again.";
    }
    // Otherwise, test passes
}

TEST_F(TrafficSimulatorInputTests, checkTrafficLightVehicleShouldntStop) {
    Road newRoad(200, "The road");
    auto trafficlight = new TrafficLight(&newRoad, 150, 9999);
    trafficlight->setCurrentState(LightState::RED);
    newRoad.addStop(trafficlight);
    auto ambulance = new Ambulance(&newRoad, 0);
    newRoad.addVehicle(ambulance);
    double time = 0;
    while (ambulance->getPosition() < 180) {
        newRoad.tick(time, 0.1);
        ASSERT_EQ(trafficlight->getCurrentState(), LightState::RED);
        time += 0.1;
        if (ambulance->getSpeed() < 15) {
            FAIL() << "The ambulance slowed down at the red traffic light";
        }
    }
    // Otherwise, test passes
}


// ============================================================
// =                      XML TESTS                           =
// ============================================================

TEST_F(TrafficSimulatorInputTests, invalidFileXML) {
    expectStderrWithMessage({ErrorMessagesReadXml::invalidFile("xml-files/invalidFile.xml")},
    {
        []() { Simulation simulation = XMLInput::XMLInput("xml-files/invalidFile.xml"); }
    });
}

TEST_F(TrafficSimulatorInputTests, missingParametersXML) {
    expectStderrWithMessage(
        {
            ErrorMessagesReadXml::missingParameter("name", "Road"),
            ErrorMessagesReadXml::missingParameter("length", "Road"),

            ErrorMessagesReadXml::missingParameter("name", "TrafficLight"),
            ErrorMessagesReadXml::missingParameter("position", "TrafficLight"),
            ErrorMessagesReadXml::missingParameter("cycle", "TrafficLight"),

            ErrorMessagesReadXml::missingParameter("name", "Vehicle"),
            ErrorMessagesReadXml::missingParameter("position", "Vehicle"),

            ErrorMessagesReadXml::missingParameter("name", "VehicleGenerator"),
            ErrorMessagesReadXml::missingParameter("frequency", "VehicleGenerator")
        },
        {
            []() { Simulation simulation = XMLInput::XMLInput("xml-files/missingParameters.xml"); }
        }
    );
}

TEST_F(TrafficSimulatorInputTests, missingValueXML) {
    expectStderrWithMessage({
        ErrorMessagesReadXml::missingValue("road"),
        ErrorMessagesReadXml::missingValue("road"),
        ErrorMessagesReadXml::missingValue("road"),
        ErrorMessagesReadXml::invalidArgumentForContext("BUS"),
        ErrorMessagesReadXml::invalidArgumentForContext("BUS")
    },
    {
        []() { Simulation simulation = XMLInput::XMLInput("xml-files/missingValues.xml"); }
    });
}

TEST_F(TrafficSimulatorInputTests, mustBePositiveXML) {
    expectStderrWithMessage({
        ErrorMessagesReadXml::mustBePositive("position"),
        ErrorMessagesReadXml::mustBePositive("cycle"),
        ErrorMessagesReadXml::mustBePositive("length")},
    {
            []() { Simulation simulation = XMLInput::XMLInput(
                                "xml-files/mustBePositive.xml"); }
    });
}

TEST_F(TrafficSimulatorInputTests, onlyOneAllowedXML) {
    expectStderrWithMessage({
        ErrorMessagesReadXml::onlyOneAllowed("road", "Simulation"),
        ErrorMessagesReadXml::onlyOneAllowed("vehicleGenerator", "Road")
    },
    {
        []() { Simulation simulation = XMLInput::XMLInput("xml-files/onlyOneAllowed.xml"); }
    });
}

TEST_F(TrafficSimulatorInputTests, invalidArgumentForContextXML) {
    expectStderrWithMessage({ErrorMessagesReadXml::invalidArgumentForContext("INVALID")},
    {
        []() { Simulation simulation = XMLInput::XMLInput(
            "xml-files/invalidArgumentForContext.xml"); }
    });
}

TEST_F(TrafficSimulatorInputTests, invalidType) {
    expectStderrWithMessage({
        ErrorMessagesReadXml::invalidType("length", "integer"),
        ErrorMessagesReadXml::invalidType("position", "integer"),
        ErrorMessagesReadXml::invalidType("frequency", "integer"),
        ErrorMessagesReadXml::invalidType("cycle", "integer"),
        ErrorMessagesReadXml::invalidType("position", "integer"),
    },
    {
        []() { Simulation simulation = XMLInput::XMLInput("xml-files/invalidType.xml"); }
    });
}

TEST_F(TrafficSimulatorInputTests, edgeCasesXML) {
    expectStderrWithMessage({}, {
        []() { Simulation simulation = XMLInput::XMLInput(
                                "xml-files/testEdgeCases.xml"); }
    });
}

TEST_F(TrafficSimulatorInputTests, invalidVehiclesXmlXML) {
    expectStderrWithMessage({
        ErrorMessagesInstructions::outOfBoundsRequire("position"),
        ErrorMessagesInstructions::outOfBoundsRequire("position"),
       ErrorMessagesInstructions::invalidVehicleType("fiets")
    }, {
        []() { Simulation simulation = XMLInput::XMLInput("xml-files/testInvalidVehicles.xml"); }
    });
}

TEST_F(TrafficSimulatorInputTests, faultySmartTrafficLight) {
    expectStderrWithMessage({ErrorMessagesReadXml::invalidType("maximum", "double"), ErrorMessagesReadXml::mustBePositive("minimum")}, {[] {
        Simulation simulation = XMLInput::XMLInput("xml-files/faultySmartTrafficLight.xml");
    }});
}

// ============================================================
// =                    OUTPUT TESTS                          =
// ============================================================

TEST(SimulatieExporterTest, GraphicsImpressionOutputMatchesExpected) {
    Simulation simulation = XMLInput::XMLInput("xml-files/happyday1road.xml");
    std::ostringstream outputStream;
    SimulatieExporter::doGraphicsImpression(outputStream, simulation);
    std::string expectedOutput =
        "Middelheimlaan      |=================AAAA===============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================\n"
        " > verkeerslichten  |                                                                                                                                                                                                        G                                                                                                   |                                                                                                                                                                                   G                   \n"
        " > bushaltes        |                                                                                                                                                                                                                                                                                                            B                                                                                                                                                                                                       \n";
    EXPECT_EQ(outputStream.str(), expectedOutput);
}

TEST(SimulatieExporterTest, HappyDayNoErrors) {
    std::streambuf* originalCerr = std::cerr.rdbuf();
    std::stringstream capturedCerr;
    std::cerr.rdbuf(capturedCerr.rdbuf());

    Simulation simulation = XMLInput::XMLInput("xml-files/main.xml");
    simulation.runSec(10000);

    std::cerr.rdbuf(originalCerr);

    std::string errOutput = capturedCerr.str();
    EXPECT_TRUE(errOutput.empty()) << "Expected no output on std::cerr, but got:\n" << errOutput;
}


TEST(SimulatieExporterTest, ExportOnOutputMatchesExpected) {
    Simulation simulation = XMLInput::XMLInput("xml-files/happyday1road.xml");
    std::ostringstream outputStream;
    SimulatieExporter::exportOn(outputStream, simulation);
    std::string expectedOutput =    "Tijd: 0s\n\n"
                                    "Vehicle 1\n"
                                    "-> baan: Middelheimlaan\n"
                                    "-> positie: 20\n"
                                    "-> snelheid: 16.6\n\n"

                                    "Vehicle 2\n"
                                    "-> baan: Middelheimlaan\n"
                                    "-> positie: 0\n"
                                    "-> snelheid: 16.6\n";
    EXPECT_EQ(outputStream.str(), expectedOutput);
}

TEST(SimulatieExporterTest, emptyImageNameXML) {
    expectExceptionWithMessage({
            ErrorMessagesInstructions::missingParameter("filename")
    },{
        []() {
            Simulation simulation = XMLInput::XMLInput("xml-files/main.xml");

            simulation.runSec(10);
            GraphicsExporter::exportToImage(&simulation, "");
        }
    });
}

TEST(SimulatieExporterTest, MissingSimulationParameter) {
    expectExceptionWithMessage(
        {ErrorMessagesInstructions::missingParameter("simulation")},
        {
            []() {
                Simulation simulation = XMLInput::XMLInput("xml-files/main.xml");
                simulation.runSec(10);
                GraphicsExporter::exportToImage(nullptr, "hallo");
            }
        }
    );
}

TEST(SimulatieExporterTest, GraphicsExporterInvalidType) {
    expectExceptionWithMessage(
        {"Type must be 'Wireframe' or 'ZBufferedWireframe'"},
        { []() {
            Simulation sim = XMLInput::XMLInput("xml-files/main.xml");
            GraphicsExporter::exportToImage(&sim,
                                            "out_file",
                                            "WrongType",
                                            0, 0, 0);
        }}
    );
}

// ============================================================
// =          GUI / BUTTON  – CONTRACT- & FUNCTIE-TESTS       =
// ============================================================

TEST_F(TrafficSimulatorInputTests, GuiButtonTests) {
    expectExceptionWithMessage({ErrorMessagesInstructions::mustBePositive("Button size")},
    { []() {
        sf::Font font;
        Button btn(sf::Vector2f(-10.f, 20.f),
                   sf::Vector2f(0.f, 0.f),
                   "OK",
                   font);
    }});

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("label")},
    { []() {
        sf::Font font;
        Button btn(sf::Vector2f(100.f, 30.f),
                   sf::Vector2f(0.f, 0.f),
                   "",
                   font);
    }});

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("font")},
    { []() {
        sf::Font font;
        Button btn(sf::Vector2f(100.f, 30.f),
                   sf::Vector2f(0.f, 0.f),
                   "OK",
                   font);
    }});
}

// ============================================================
// =        GUI / BUTTONMANAGER – CONTRACT-TESTS              =
// ============================================================

TEST_F(TrafficSimulatorInputTests, ButtonManagerTests) {
    expectExceptionWithMessage({ErrorMessagesInstructions::mustBePositive("Button size")},
    { []() {
        sf::Font font;
        font.loadFromFile("resources/fonts/Anton-Regular.ttf");
        ButtonManager bm;
        bm.addButton(sf::Vector2f(-10.f, 20.f),
                     sf::Vector2f(0.f, 0.f),
                     "OK",
                     font,
                     [](){});
    }});

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("label")},
    { []() {
        sf::Font font;
        font.loadFromFile("resources/fonts/Anton-Regular.ttf");
        ButtonManager bm;
        bm.addButton(sf::Vector2f(100.f, 30.f),
                     sf::Vector2f(0.f, 0.f),
                     "",
                     font,
                     [](){});
    }});

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("font")},
    { []() {
        sf::Font font;
        ButtonManager bm;
        bm.addButton(sf::Vector2f(100.f, 30.f),
                     sf::Vector2f(0.f, 0.f),
                     "OK",
                     font,
                     [](){});
    }});
}

// ============================================================
// =                 GUI / CAMERA – FUNCTIE-TEST             =
// ============================================================

TEST_F(TrafficSimulatorInputTests, CameraTests) {
    Camera cam;
    EXPECT_EQ(cam.getOffset(), sf::Vector2f(0.f, 0.f));
}

// ============================================================
// =      RESOURCE-MANAGER – MISSING-PARAMETER TESTS          =
// ============================================================

TEST_F(TrafficSimulatorInputTests, ResourceManagerTests) {
    ResourceManager rm;

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("id")},
    { [&](){
        rm.loadTexture("", "resources/sprites/roads/road.png");
    }});

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("path")},
    { [&](){
        rm.loadTexture("road", "");
    }});

    ResourceManager rm2;

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("id")},
    { [&](){
        (void)rm2.getTexture("");
    }});

    ResourceManager rm3;

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("id")},
    { [&](){
        rm3.loadFont("", "resources/fonts/arial.ttf");
    }});

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("path")},
    { [&](){
        rm3.loadFont("arial", "");
    }});

    ResourceManager rm4;

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("id")},
    { [&](){
        (void)rm4.getFont("");
    }});
}

// ============================================================
// =          GUI / ROADINFOPANEL – CONTRACT-TESTS            =
// ============================================================

TEST_F(TrafficSimulatorInputTests, RoadInfoPanelTests) {
    RoadInfoPanel panel;
    sf::Font emptyFont;
    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("font")},
    { [&](){
        panel.setFont(emptyFont);
    }});

    RoadInfoPanel panel2;
    sf::RenderWindow dummy(sf::VideoMode(100,100), "dummy", sf::Style::None);
    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("font")},
    { [&](){
        panel2.draw(dummy, dummy.getSize());
    }});

    RoadInfoPanel panel3;
    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("road")},
    { [&](){
        (void)panel3.roadStartsAtIntersection(nullptr);
    }});
}

// ============================================================
// =        SIMULATIONRENDERER – MISSING-PARAMETER TESTS       =
// ============================================================

TEST_F(TrafficSimulatorInputTests, SimulationTests) {
    ResourceManager tm;
    SimulationRenderer sr(tm);

    sf::RenderWindow dummy(sf::VideoMode(50,50), "dummy", sf::Style::None);

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("simulation")},
    { [&](){
        sr.render(dummy, nullptr, sf::Vector2f(0.f,0.f), 10.f);
    }});

    ResourceManager tm2;
    RoadRenderer rr(tm2);

    sf::RenderWindow dummy2(sf::VideoMode(50,50), "dummy", sf::Style::None);

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("road")},
    { [&](){
        rr.renderRoad(dummy2,
                      nullptr,
                      sf::Vector2f(0.f,0.f),
                      0.f,
                      sf::Vector2f(0.f,0.f),
                      10.f);
    }});

    ResourceManager tm3;
    VehicleRenderer vr(tm3);

    sf::RenderWindow dummy3(sf::VideoMode(50,50), "dummy", sf::Style::None);

    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("vehicle")},
    { [&](){
        vr.renderVehicle(dummy3,
                         nullptr,
                         sf::Vector2f(0.f,0.f),
                         sf::Vector2f(1.f,0.f),
                         sf::Vector2f(0.f,0.f),
                         10.f);
    }});
}

TEST_F(TrafficSimulatorInputTests, MissingParameterGuiSimulation) {
    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("simulation")},
    { []() {
        Gui gui(nullptr);
    }});
}

// ============================================================
// =         LAYOUTUTILS – CONTRACT- & EDGE-CASE TESTS        =
// ============================================================

TEST_F(TrafficSimulatorInputTests, LayoutUtilsTests) {
    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("road")},
    { []() {
        std::vector<Road*> roads = {nullptr};
        (void)Utils::groupConnectedRoads(roads);
    }});
}

TEST_F(TrafficSimulatorInputTests, LayoutUtilsEmptyVector) {
    expectExceptionWithMessage({ErrorMessagesInstructions::notUpdatedSuccessfully("components")},
    { []() {
        std::vector<Road*> roads;
        (void)Utils::groupConnectedRoads(roads);
    }});
}

TEST_F(TrafficSimulatorInputTests, LayoutUtilsMissingRoadInChain) {
    expectExceptionWithMessage({ErrorMessagesInstructions::missingParameter("road")},
    { []() {
        std::vector<Road*> chain = {nullptr};
        std::unordered_map<Road*, sf::Vector2f> pos;
        std::unordered_map<Road*, float>        ang;
        Utils::computeLayoutForChain(chain, pos, ang);
    }});
}

TEST_F(TrafficSimulatorInputTests, LayoutUtilsUnconnectedChain) {
    expectExceptionWithMessage({ErrorMessagesInstructions::notUpdatedSuccessfully("layout")},
    { []() {
        Road r1(100, "A");
        Road r2(100, "B");
        std::vector<Road*> chain = {&r1, &r2};

        std::unordered_map<Road*, sf::Vector2f> pos;
        std::unordered_map<Road*, float>        ang;

        Utils::computeLayoutForChain(chain, pos, ang);
    }});
}

TEST(SimulatieExporterTest, engineFileInPlace) {
    namespace fs = std::filesystem;

#ifdef _WIN32
    const fs::path enginePath = "io/output/3d_rendering/graphics-engine/engine.exe";
#else
    const fs::path enginePath = "io/output/3d_rendering/graphics-engine/engine";
#endif

    EXPECT_TRUE(fs::exists(enginePath)) << "Expected engine binary not found at: " << enginePath;
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
