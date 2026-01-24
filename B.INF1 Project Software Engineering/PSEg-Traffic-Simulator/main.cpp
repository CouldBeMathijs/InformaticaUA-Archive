#include "classes/Simulation.h"
#include "io/input/XMLInput.h"
#include "io/output/SimulatieExporter.h"
#include "io/output/simulation/Gui.h"
#include <iostream>

int main() {
  Simulation simulation = XMLInput::XMLInput("xml-files/main.xml", std::cerr);
  // === Start GUI
  Gui gui(&simulation);
  gui.run();

  simulation.runSec(10);

  SimulatieExporter::exportOn(std::cout, simulation);
  SimulatieExporter::doGraphicsImpression(std::cout, simulation);

  return 0;
}