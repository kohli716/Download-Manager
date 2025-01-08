#include "SegmentDownloader.h"
#include <curl/curl.h> // libcurl for HTTP requests
#include <fstream>
#include <iostream>
#include <sstream>

SegmentDownloader::SegmentDownloader(const std::string& url, const std::string& outputPath, int segmentIndex, int totalSegments)
    : url(url), outputPath(outputPath), segmentIndex(segmentIndex), totalSegments(totalSegments) {}

size_t writeCallback(void* buffer, size_t size, size_t nmemb, void* userdata) {
    std::ofstream* outFile = static_cast<std::ofstream*>(userdata);
    outFile->write(static_cast<char*>(buffer), size * nmemb);
    return size * nmemb;
}

void SegmentDownloader::downloadSegment(long fileSize) {
    // Calculate the range for this segment
    long segmentSize = fileSize / totalSegments;
    long start = segmentIndex * segmentSize;
    long end = (segmentIndex == totalSegments - 1) ? fileSize - 1 : start + segmentSize - 1;

    std::cout << "Downloading segment " << segmentIndex + 1 << "/" << totalSegments
              << " [" << start << "-" << end << "]" << std::endl;

    // Initialize CURL
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL for segment " << segmentIndex + 1 << std::endl;
        return;
    }

    // Set URL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Set the range for this segment
    std::ostringstream range;
    range << start << "-" << end;
    curl_easy_setopt(curl, CURLOPT_RANGE, range.str().c_str());

    // Set the write callback
    std::string partFilePath = outputPath + ".part" + std::to_string(segmentIndex);
    std::ofstream partFile(partFilePath, std::ios::binary);
    if (!partFile.is_open()) {
        std::cerr << "Error: Could not create file for segment " << segmentIndex + 1 << std::endl;
        curl_easy_cleanup(curl);
        return;
    }
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &partFile);

    // Perform the download
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Error in segment " << segmentIndex + 1 << ": " << curl_easy_strerror(res) << std::endl;
    } else {
        std::cout << "Segment " << segmentIndex + 1 << " downloaded successfully." << std::endl;
    }

    // Cleanup
    partFile.close();
    curl_easy_cleanup(curl);
}
