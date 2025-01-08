#include "DownloadManager.h"
#include "SegmentDownloader.h"
#include "FileHandler.h"
#include <iostream>
#include <vector>
#include <thread>
#include <curl/curl.h>

DownloadManager::DownloadManager(const std::string& url, const std::string& outputPath, int numThreads)
    : url(url), outputPath(outputPath), numThreads(numThreads) {}

void DownloadManager::startDownload() {
    std::cout << "Starting download: " << url << std::endl;

    // Step 1: Get the file size using a HEAD request
    CURL* curl = curl_easy_init();
    std::cout<<curl;
    long fileSize = 0;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);  // Perform a HEAD request
        curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
        CURLcode res = curl_easy_perform(curl);
        
        if (res == CURLE_OK) {
            // Get the content length (file size)
            curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &fileSize);
            std::cout << "File size: " << fileSize << " bytes" << std::endl;
        } else {
            std::cerr << "Error fetching file size: " << curl_easy_strerror(res) << std::endl;
            return;
        }

        curl_easy_cleanup(curl);
    }

    // Step 2: Start downloading segments in parallel
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([this, i, fileSize]() {
            SegmentDownloader segment(this->url, this->outputPath, i, this->numThreads);
            segment.downloadSegment(fileSize);  // Pass the file size to each segment
        });
    }

    // Step 3: Join threads (wait for all segments to download)
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "All segments downloaded successfully." << std::endl;

    // Step 4: Combine the segments into the final file
    FileHandler::combineSegments(outputPath, numThreads);

    // Step 5: Clean up temporary segment files
    FileHandler::cleanupSegments(outputPath, numThreads);

    std::cout << "Download completed: " << outputPath << std::endl;
}
