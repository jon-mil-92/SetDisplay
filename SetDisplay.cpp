/**
 * Original Author: Jonathan Miller
 * Created: 03-31-2024
 * Version: 1.0.0
 * License: The MIT License - https://mit-license.org/
 * Copyright (c) 2024 Jonathan Miller
 **/

#include <iostream>
#include <Windows.h>
#include <vector>

using namespace std;

// Define a vector of allowed DPI scale percentage values.
static const vector<int> DPI_SCALE_PERCENTAGES = {100, 125, 150, 175, 200, 225, 250, 300, 350};

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
    ChangeDisplaySettings(&devMode, CDS_UPDATEREGISTRY);
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
    SystemParametersInfo(SPI_GETLOGICALDPIOVERRIDE, 0, (LPVOID)&defaultDpiScaleIndex, 0);

    // Return the absolute value of the default DPI scale percentage index since SystemParametersInfo returns the negative index value.
    return abs(defaultDpiScaleIndex);
}

/*
* This method sets the DPI scale percentage to the given value.
* 
* @param dpiScalePercentage - The DPI scale percentage to set.
*/
void setDpiScale(int dpiScalePercentage) {
    // Get the index of the default DPI scale percentage.
    int defaultDpiScaleIndex = getDefaultDpiScaleIndex();

    // Define an integer to hold the index of the DPI scale percentage to set.
    int dpiScaleIndex = 0;

    // For each possible defined DPI scale percentage...
    for (int i = 0; i < DPI_SCALE_PERCENTAGES.size(); i++) {
        // Store the index of the found DPI scale percentage.
        if (dpiScalePercentage == DPI_SCALE_PERCENTAGES.at(i)) {
            dpiScaleIndex = i;
        }
    }

    // Define an integer to hold the relative DPI scale percentage index for the given DPI scale percentage compared to the default index.
    int relativeIndex = 0;

    // Calculate the relative DPI scale percentage index.
    relativeIndex = dpiScaleIndex - defaultDpiScaleIndex;

    // Set the given DPI scale percentage value using the relative DPI scale percentage index.
    SystemParametersInfo(SPI_SETLOGICALDPIOVERRIDE, relativeIndex, (LPVOID)0, 1);
}

/*
* The main method calls functions to update the display mode and DPI scale percentage.
* CL arguments are as follows: resolution width, resolution height, bit depth, refresh rate, and DPI scale percentage.
*/
int main(int argc, char *argv[]) {
    // Set the display mode given from the command line arguments.
    setDisplayMode(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));

    // Set the DPI scale percentage given from the command line argument.
    setDpiScale(atoi(argv[5]));

    // End of program.
    return 0;
}