#include "panel-configuration.h"

#include <iostream>

PanelConfiguration::PanelConfiguration(int rows, int chainedDisplays, int parallelDisplays)
    : rows(rows), chainedDisplays(chainedDisplays), parallelDisplays(parallelDisplays)
{

}

void PanelConfiguration::readConfigurationFile(const std::string &configurationFilename)
{
    // TODO
    std::cout << "Read configuration file unimplemented" << std::endl;
}
