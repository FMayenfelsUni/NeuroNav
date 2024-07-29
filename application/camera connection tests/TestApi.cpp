// main.cpp

#include <QCoreApplication>
#include <iostream>
#include "CombinedApi.h"

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    // Initialize the NDI API
    CombinedApi capi;

    // Connect to the device
    std::string hostname = "YOUR_DEVICE_HOSTNAME_OR_IP"; // Replace with your device's hostname or IP address
    int connectionResult = capi.connect(hostname);

    // Check if connection was successful
    if (connectionResult == 0) {
        std::cout << "Connected to device: " << hostname << std::endl;

        // Retrieve and print device information
        std::cout << "Firmware Version: " << capi.getUserParameter("Features.Firmware.Version") << std::endl;

        // Disconnect from the device
        capi.disconnect();
        std::cout << "Disconnected from device." << std::endl;
    }
    else {
        std::cerr << "Failed to connect to device." << std::endl;
    }

    return a.exec();
}
// copy CombinedApi.h into include/ directory + may require to change project file