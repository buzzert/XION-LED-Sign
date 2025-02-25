#include <string>

class PanelConfiguration
{
public:
    PanelConfiguration(int rows = 16, int chainedDisplays = 1, int parallelDisplays = 1);

    void readConfigurationFile(const std::string &configurationFilename);

    int rows;
    int chainedDisplays;
    int parallelDisplays;
};
