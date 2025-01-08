#include <iostream>
#include "DownloadManager.h"

int main() {
    std::string url = "http://techslides.com/demos/sample-videos/small.mp4"; // Replace with the desired test URL
    std::string outputPath = "small.mp4"; // Replace with the desired output file name
    int numThreads = 4;
    // int numThreads = 6;

    DownloadManager manager(url, outputPath, numThreads);
    manager.startDownload();

    return 0;
}
