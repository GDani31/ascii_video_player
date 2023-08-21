#pragma ide diagnostic ignored "EndlessLoop"
#include <iostream>
#include <thread>
#include <vector>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <opencv2/core/utils/logger.hpp>

void clearscreen()
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

void clearBox()
{
    HANDLE hOut;
    COORD Position;
    DWORD Written;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 10;
    FillConsoleOutputCharacter(hOut,' ', 1000000, Position, &Written);

    SetConsoleCursorPosition(hOut, Position);
}


void processFrame(const cv::Mat& frame, int frameNumber, int totalFrames, std::vector<std::string>& asciiFrames) {

    cv::Mat resizedFrame;
    cv::resize(frame, resizedFrame, cv::Size(100, 75));

    std::string asciiFrame;
    for (int y = 0; y < resizedFrame.rows; ++y) {
        for (int x = 0; x < resizedFrame.cols; ++x) {
            const cv::Vec3b& pixel = resizedFrame.at<cv::Vec3b>(y, x);
            int intensity = (pixel[0] + pixel[1] + pixel[2]) / 3;
            char asciiChar = "@%#*+=-:. "[intensity / 25];
            asciiFrame += asciiChar;
        }
        asciiFrame += '\n';
    }

    asciiFrames[frameNumber] = asciiFrame;

    std::cout << "Frame: " << frameNumber + 1 << "/" << totalFrames << std::endl;
}

int main() {

    for( int n = 0; n < 50 ; n++ )
    {
        std::cout<<"_";
    }
    std::cout<<"\n";

    std::string videoPath;
    std::cout << "Enter the path of the video: ";
    std::cin >> videoPath;


    cv::VideoCapture capture(videoPath);
    if (!capture.isOpened()) {
        std::cerr << "Error opening the video file." << std::endl;
        return 1;
    }


    int totalFrames = capture.get(cv::CAP_PROP_FRAME_COUNT);
    std::cout << "Total frames in the video: " << totalFrames << std::endl;


    std::vector<std::string> asciiFrames(totalFrames);
    std::vector<std::thread> threads;

    for (int frameNumber = 0; frameNumber < totalFrames; ++frameNumber) {
        cv::Mat frame;
        capture >> frame;

        threads.emplace_back(processFrame, frame, frameNumber, totalFrames, std::ref(asciiFrames));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    clearBox();

    for (const std::string& asciiFrame : asciiFrames) {

        clearscreen();

        std::cout << asciiFrame;

        int frameRate = static_cast<int>(capture.get(cv::CAP_PROP_FPS));
        std::chrono::milliseconds sleepDuration(1000 / frameRate);
        std::this_thread::sleep_for(sleepDuration);

    }

    capture.release();
    return 0;
}









