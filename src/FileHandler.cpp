#include "FileHandler.h"
#include <fstream>
#include <iostream>
#include <filesystem>

void FileHandler::combineSegments(const std::string& outputPath, int totalSegments) {
    std::ofstream outputFile(outputPath, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not create output file: " << outputPath << std::endl;
        return;
    }

    for (int i = 0; i < totalSegments; ++i) {
        std::string partFile = outputPath + ".part" + std::to_string(i);
        std::ifstream inputFile(partFile, std::ios::binary);

        if (!inputFile.is_open()) {
            std::cerr << "Error: Could not open segment file: " << partFile << std::endl;
            continue;
        }

        outputFile << inputFile.rdbuf();
        inputFile.close();
    }

    outputFile.close();
    std::cout << "Segments combined into: " << outputPath << std::endl;
}

void FileHandler::cleanupSegments(const std::string& outputPath, int totalSegments) {
    for (int i = 0; i < totalSegments; ++i) {
        std::string partFile = outputPath + ".part" + std::to_string(i);
        if (std::filesystem::remove(partFile)) {
            std::cout << "Deleted temporary file: " << partFile << std::endl;
        } else {
            std::cerr << "Error: Could not delete file: " << partFile << std::endl;
        }
    }
}
