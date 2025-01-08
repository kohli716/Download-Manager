#ifndef SEGMENT_DOWNLOADER_H
#define SEGMENT_DOWNLOADER_H

#include <string>

class SegmentDownloader {
private:
    std::string url;
    std::string outputPath;
    int segmentIndex;
    int totalSegments;

public:
    SegmentDownloader(const std::string& url, const std::string& outputPath, int segmentIndex, int totalSegments);
    void downloadSegment(long fileSize); // Downloads a specific segment of the file
};

#endif // SEGMENT_DOWNLOADER_H
