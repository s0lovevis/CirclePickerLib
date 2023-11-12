#include <iostream>
#include <circle_picker.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 5) {
        std::cout << "Usage: main.exe -I [path to input image] -O [path to directory to put results]" << std::endl;
        return 1;
    }

    std::string inputPath, outputPath;
    for (int i = 1; i < argc; i += 2) {
        std::string arg = argv[i];
        if (arg == "-I") {
            inputPath = argv[i + 1];
        }
        else if (arg == "-O") {
            outputPath = argv[i + 1];
        }
    }

    cv::Mat image = cv::imread(inputPath);
    if (image.empty()) {
        std::cout << "Failed loading picture" << std::endl;
        return 1;
    }
    circlePicker picker(image);
    picker.pickMode();

    std::string outputFilePath = outputPath + "\\result.jpg";
    bool success = cv::imwrite(outputFilePath, picker.getCanvas());
    if (!success) {
        std::cout << "Failed to save result" << std::endl;
        return 1;
    }

    std::cout << "Program finished with success. Result have been saved to " << outputFilePath << std::endl;

    return 0;
}