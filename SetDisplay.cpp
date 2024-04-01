/*
 * Original Author: Jonathan Miller
 * Created: 04-01-2024
 * Version: 1.1.0
 * License: The MIT License - https://mit-license.org/
 * Copyright (c) 2024 Jonathan Miller
 */
                                                                                                    
#include <iostream>
#include <Windows.h>
#include <vector>

using namespace std;

// Define a vector of allowed DPI scale percentage values.
static const vector<int> DPI_SCALE_PERCENTAGES = {100, 125, 150, 175, 200, 225, 250, 300, 350};

// Define an integer for the number of DPI scale percentage values in Windows 10 and Windows 11.
static const int NUM_OF_DPI_SCALE_PERCENTAGES = 9;

/*
* This method sets a scaling mode for the active display configuration.
* 
* @param scalingMode - The new scaling mode to apply for the active displays.
*/
void setDisplayScalingMode(int scalingMode) {
    // Initialize variables to hold the path array and mode info array buffer sizes.
    UINT32 numPathArrayElements = 0;
    UINT32 numModeInfoArrayElements = 0;

    // Variable to hold the new display scaling mode.
    DISPLAYCONFIG_SCALING displayConfigScalingMode;

    // Update the display scaling mode corresponding to the given scalingMode value.
    switch (scalingMode) {
    case 0:
        displayConfigScalingMode = DISPLAYCONFIG_SCALING_ASPECTRATIOCENTEREDMAX; // Preserve aspect ratio.
        break;
    case 1:
        displayConfigScalingMode = DISPLAYCONFIG_SCALING_STRETCHED; // Stretch to fill panel.
        break;
    case 2:
        displayConfigScalingMode = DISPLAYCONFIG_SCALING_CENTERED; // Centered in panel.
        break;
    default:
        displayConfigScalingMode = DISPLAYCONFIG_SCALING_ASPECTRATIOCENTEREDMAX; // Preserve aspect ratio by default.
        break;
    }

    // Variable to hold the current display topology after querying the display configuration.
    DISPLAYCONFIG_TOPOLOGY_ID* currentTopology = new DISPLAYCONFIG_TOPOLOGY_ID;

    // Retrieve the size of the buffers that are required to call the QueryDisplayConfig function.
    GetDisplayConfigBufferSizes(QDC_DATABASE_CURRENT, &numPathArrayElements, &numModeInfoArrayElements);

    // Initialize the path array and mode info array with the correct buffers to hold the active paths as defined in the
    // persistence database for the currently connected monitors.
    DISPLAYCONFIG_PATH_INFO* pathArray = new DISPLAYCONFIG_PATH_INFO[numPathArrayElements];
    DISPLAYCONFIG_MODE_INFO* modeInfoArray = new DISPLAYCONFIG_MODE_INFO[numModeInfoArrayElements];

    // Query the active display configuration.
    LONG queryDisplayConfigResult = QueryDisplayConfig(QDC_DATABASE_CURRENT, &numPathArrayElements, pathArray, 
            &numModeInfoArrayElements, modeInfoArray, currentTopology);

    // Check if the active display configuration was successfully queried, and output an error message if it failed.
    if (queryDisplayConfigResult != ERROR_SUCCESS) {
        std::cerr << "Failed to query the display configuration! Error Code: " << queryDisplayConfigResult << std::endl;
    }

    // Set the new display scaling mode for all paths.
    for (UINT32 i = 0; i < numPathArrayElements; ++i) {
        pathArray->targetInfo.scaling = displayConfigScalingMode;
    }

    // Apply the new display scaling mode.
    LONG setDisplayConfigResult = SetDisplayConfig(numPathArrayElements, pathArray, numModeInfoArrayElements, 
            modeInfoArray, SDC_APPLY | SDC_USE_SUPPLIED_DISPLAY_CONFIG | SDC_SAVE_TO_DATABASE);

    // Check if the display configuration could be set, and output an error message if it failed.
    if (setDisplayConfigResult != ERROR_SUCCESS) {
        std::cerr << "Failed to set the display configuration! Error Code: " << setDisplayConfigResult << std::endl;
    }

    // Clean up allocated memory.
    delete[] pathArray;
    delete[] modeInfoArray;
}

/*
* This method sets the current desktop display mode from the given parameters.
*
* @param width - The new horizontal resolution of the primary display.
* @param height - The new vertical resolution of the primary display.
* @param bitDepth - The new bit depth of the primary display.
* @param refreshRate - The new refresh rate of the primary display.
*/
void setDisplayMode(int width, int height, int bitDepth, int refreshRate) {
    // Create a devmode struct.
    DEVMODE devMode;

    // Initialize the memory for the devmode struct.
    memset(&devMode, 0, sizeof(devMode));
    devMode.dmSize = sizeof(devMode);

    // Update the devmode members with the given values.
    devMode.dmPelsWidth = width;
    devMode.dmPelsHeight = height;
    devMode.dmBitsPerPel = bitDepth;
    devMode.dmDisplayFrequency = refreshRate;
    devMode.dmDriverExtra = 0;
    devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

    // Update the current display mode.
    LONG changeDisplaySettingsResult = ChangeDisplaySettings(&devMode, CDS_UPDATEREGISTRY);

    // Check if the display mode could be set, and output an error message if it failed.
    if (changeDisplaySettingsResult != DISP_CHANGE_SUCCESSFUL) {
        std::cerr << "Failed to set the display mode! Error Code: " << changeDisplaySettingsResult << std::endl;
    }
}

/*
* This method gets the default DPI scale percentage index.
*
* @return The default DPI scale percentage index.
*/
int getDefaultDpiScaleIndex() {
    // Define an integer to hold the default DPI scale percentage index.
    int defaultDpiScaleIndex = 0;

    // Get the default DPI scale percentage index.
    bool getDefaultDpiScaleIndexResult = SystemParametersInfo(SPI_GETLOGICALDPIOVERRIDE, 0, 
            (LPVOID)&defaultDpiScaleIndex, 0);

    // Check if the default DPI scale percentage index could be retrieved, and output an error message if it failed.
    if (getDefaultDpiScaleIndexResult == 0) {
        std::cerr << "Failed to get the default DPI scale percentage index!" << std::endl;
    }

    // Return the absolute value of the default DPI scale percentage index since SystemParametersInfo returns the 
    // negative index value.
    return abs(defaultDpiScaleIndex);
}

/*
* This method sets the DPI scale percentage to the given value.
*
* @param dpiScalePercentage - The DPI scale percentage to set.
*/
void setDpiScalePercentage(int dpiScalePercentage) {
    // Get the index of the default DPI scale percentage.
    int defaultDpiScaleIndex = getDefaultDpiScaleIndex();

    // Define an integer to hold the index of the DPI scale percentage to set.
    int dpiScaleIndex = 0;

    // For each possible defined DPI scale percentage...
    for (int i = 0; i < NUM_OF_DPI_SCALE_PERCENTAGES; i++) {
        // Store the index of the found DPI scale percentage.
        if (dpiScalePercentage == DPI_SCALE_PERCENTAGES.at(i)) {
            dpiScaleIndex = i;
        }
    }

    // Define an integer to hold the relative DPI scale percentage index for the given DPI scale percentage compared to 
    // the default index.
    int relativeIndex = 0;

    // Calculate the relative DPI scale percentage index.
    relativeIndex = dpiScaleIndex - defaultDpiScaleIndex;

    // Set the given DPI scale percentage value using the relative DPI scale percentage index.
    bool setDpiScaleResult = SystemParametersInfo(SPI_SETLOGICALDPIOVERRIDE, relativeIndex, (LPVOID)0, 1);

    // Check if the DPI scale percentage could be set, and output an error message if it failed.
    if (setDpiScaleResult == 0) {
        std::cerr << "Failed to set the DPI scale percentage!" << std::endl;
    }
}

/*
* The main method calls functions to update the display mode, DPI scale percentage, and display scaling mode.
* CL arguments are as follows: resolution width, resolution height, bit depth, refresh rate, DPI scale percentage, and 
* display scaling mode.
*/
int main(int argc, char* argv[]) {
    // Set the display mode given from the command line arguments.
    setDisplayMode(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));

    // Set the DPI scale percentage given from the command line argument.
    setDpiScalePercentage(atoi(argv[5]));

    // Set the display scaling mode given from the command line argument.
    setDisplayScalingMode(atoi(argv[6]));

    // End of program.
    return 0;
}