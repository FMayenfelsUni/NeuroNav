#include <cstdio>
#include <chrono>
#include <Processing.NDI.Lib.h>

#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "Processing.NDI.Lib.x64.lib")
#else // _WIN64
#pragma comment(lib, "Processing.NDI.Lib.x86.lib")
#endif // _WIN64
#endif // _WIN32

int main(int argc, char* argv[])
{
    // Not required, but "correct" (see the SDK documentation).
    if (!NDIlib_initialize())
        return 0;

    // We are going to create an NDI finder that locates sources on the network.
    NDIlib_find_instance_t pNDI_find = NDIlib_find_create_v2();
    if (!pNDI_find)
        return 0;

    const char* desiredIpAddress = "131.246.110.186"; // Modify this to your desired IP address
    const uint16_t desiredPort = 49265; // Modify this to your desired port

    // Run for one minute
    using namespace std::chrono;
    for (const auto start = high_resolution_clock::now(); high_resolution_clock::now() - start < minutes(1);) {
        // Wait up till 5 seconds to check for new sources to be added or removed
        if (!NDIlib_find_wait_for_sources(pNDI_find, 5000 /* milliseconds */)) {
            printf("No change to the sources found.\n");
            continue;
        }

        // Get the updated list of sources
        uint32_t no_sources = 0;
        const NDIlib_source_t* p_sources = NDIlib_find_get_current_sources(pNDI_find, &no_sources);

        // Search for the desired source
        bool foundSource = false;
        for (uint32_t i = 0; i < no_sources; i++) {
            if (strcmp(p_sources[i].connection_address, desiredIpAddress) == 0 &&
                p_sources[i].connection_port == desiredPort) {
                printf("Desired source found: %s\n", p_sources[i].p_ndi_name);
                foundSource = true;
                break;
            }
        }

        if (foundSource)
            break; // Exit the loop if the desired source is found
        else
            printf("Desired source not found.\n");
    }

    // Destroy the NDI finder
    NDIlib_find_destroy(pNDI_find);

    // Finished
    NDIlib_destroy();

    // Success. We are done
    return 0;
}
