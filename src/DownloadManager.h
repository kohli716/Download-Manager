#ifndef DOWNLOAD_MANAGER_H
#define DOWNLOAD_MANAGER_H

#include <string>

class DownloadManager {
private:
    std::string url;         // URL to download
    std::string outputPath;  // Path to save the downloaded file
    int numThreads;          // Number of threads (segments)

public:
    DownloadManager(const std::string& url, const std::string& outputPath, int numThreads = 4);
    void startDownload();    // Starts the download process
};

#endif // DOWNLOAD_MANAGER_H
