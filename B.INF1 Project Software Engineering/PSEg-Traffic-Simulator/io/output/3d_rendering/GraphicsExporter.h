//============================================================================
// Name        : GraphicsExporter.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : Everything to do with graphics engine integrations
//============================================================================

#ifndef PSEG_TRAFFIC_ENGINE_GRAPHICSEXPORTER_H
#define PSEG_TRAFFIC_ENGINE_GRAPHICSEXPORTER_H

#include "../../../classes/Road.h"
#include "../../../classes/Simulation.h"

/**
 * @brief Class for everything to do with graphics engine integrations
 */
class GraphicsExporter {
public:
  /**
   * Uses the graphics engine to visualize the current state of the simulation
   * \n REQUIRE(simulation->properlyInitialized(), This function is called as an
   * error when the constructor of the class has not successfully been
   * initialized.)
   * \n REQUIRE(type == "ZBufferedWireframe" || type == "Wireframe", "Error:
   type <type> is not supported")
   * ENSURE(simulation->properlyInitialized(), "Simulation state must remain
   unchanged");
   *

   * @param simulation The simulation to visualize
   * @param filename The filename to visualize too
   * @param type The type of image to be generated (currently only supporting
   Wireframe
   * @param x_eye The x eye coordinate
   * @param y_eye The y eye coordinate
   * @param z_eye The z eye coordinate
   */
  static void exportToImage(Simulation *simulation, const std::string &filename,
                            const std::string &type = "ZBufferedWireframe",
                            const double &x_eye = 100,
                            const double &y_eye = 100,
                            const double &z_eye = 1000);
};

#endif // PSEG_TRAFFIC_ENGINE_GRAPHICSEXPORTER_H
