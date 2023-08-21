#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <windows.h>
#include <chrono>
#include <stdio.h>

using namespace std;
using namespace cv;

void clearscreen() {
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

int main(int argc, char** argv) {
    string videoPath;
    int asciiWidth = 128;
    int asciiHeight = 64;

    if (argc <= 1) {
        cout << "Enter the path of the video: ";
        cin >> videoPath;
        cout << "Enter the width for ASCII art: ";
        cin >> asciiWidth;
        cout << "Enter the height for ASCII art: ";
        cin >> asciiHeight;
    } else {
        for (int i = 1; i < argc; ++i) {
            string arg = argv[i];

            if (arg == "-w" && i + 1 < argc) {
                asciiWidth = atoi(argv[i + 1]);
                ++i;
            } else if (arg == "-h" && i + 1 < argc) {
                asciiHeight = atoi(argv[i + 1]);
                ++i;
            } else {
                videoPath = arg;
            }
        }
    }

    if (videoPath.empty()) {
        cerr << "Error: Video path not provided." << endl;
        return -1;
    }

    VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        cerr << "Error: Could not open the video." << endl;
        return -1;
    }

    double frameRate = cap.get(CAP_PROP_FPS);
    double frameTime = 1000.0 / frameRate;
    std::chrono::milliseconds frameTimeMs(static_cast<int>(frameTime - 4.3));

    const string asciiChars = " .':-=+*#%@";

    Mat frame;

    while (cap.read(frame)) {
        auto start = std::chrono::high_resolution_clock::now();

        Mat resizedGrayFrame;
        cvtColor(frame, resizedGrayFrame, COLOR_BGR2GRAY);
        resize(resizedGrayFrame, resizedGrayFrame, Size(asciiWidth, asciiHeight));

        string asciiFrame;
        for (int i = 0; i < resizedGrayFrame.rows; i++) {
            for (int j = 0; j < resizedGrayFrame.cols; j++) {
                uchar pixelValue = resizedGrayFrame.at<uchar>(i, j);
                int index = static_cast<int>(pixelValue * (asciiChars.size() - 1) / 255.0);
                asciiFrame += asciiChars[index];
            }
            asciiFrame += '\n';
        }

        clearscreen();
        cout << asciiFrame << flush;

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::this_thread::sleep_for(frameTimeMs - (duration));
    }

    cap.release();
    return 0;
}
