#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <vector>

class FileHandler {
public:
    // Combines partial files into the final output file
    static void combineSegments(const std::string& outputPath, int totalSegments);
    // Cleans up temporary `.part` files
    static void cleanupSegments(const std::string& outputPath, int totalSegments);
};

#endif // FILE_HANDLER_H
