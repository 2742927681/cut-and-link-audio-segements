#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>

struct TimeStamp {
    std::string start;
    std::string end;
};

std::vector<TimeStamp> readTimeStamps(const std::string& filename) {
    std::vector<TimeStamp> timestamps;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string start, end;
        if (std::getline(iss, start, '-') && std::getline(iss, end)) {
            end = end.substr(2); // Remove the "->" part
            timestamps.push_back({ start, end });
        }
    }
    return timestamps;
}

void extractAndMergeAudio(const std::string& inputAudio, const std::vector<TimeStamp>& timestamps, const std::string& outputAudio) {
    std::string tempFileList = "filelist.txt";
    std::ofstream fileList(tempFileList);
    for (size_t i = 0; i < timestamps.size(); ++i) {
        std::string outputSegment = "segment" + std::to_string(i) + ".mp3";
        std::string command = "ffmpeg -i " + inputAudio + " -ss " + timestamps[i].start + " -to " + timestamps[i].end + " -c copy " + outputSegment;
        std::system(command.c_str());
        fileList << "file '" << outputSegment << "'\n";
    }
    fileList.close();
    std::string mergeCommand = "ffmpeg -f concat -safe 0 -i " + tempFileList + " -c copy " + outputAudio;
    std::system(mergeCommand.c_str());
}

int main() {
    std::string inputAudio = "input.mp3";
    std::string timestampFile = "timestamps.txt";
    std::string outputAudio = "output.mp3";

    std::vector<TimeStamp> timestamps = readTimeStamps(timestampFile);
    extractAndMergeAudio(inputAudio, timestamps, outputAudio);

    std::cout << "Audio segments extracted and merged successfully!" << std::endl;
    return 0;
}

