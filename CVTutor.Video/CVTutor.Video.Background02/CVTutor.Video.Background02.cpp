// CVTutor.Video.Background02.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/video.hpp>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;


// Forward decl.
int processImages(const vector<string> filenames);
int processImage(const string filename, const Mat bgFrame);


// Usage example:
// .\CVTutor.Video.Background02.exe ..\..\sample-frames\birds\bird0.png  ..\..\sample-frames\birds\bird1.png  ..\..\sample - frames\birds\bird2.png  ..\..\sample - frames\birds\bird3.png
// The first image is the background image.
int main(int argc, char* argv[])
{
	//check for the input parameter correctness
	if (argc < 3) {
		cerr << "Incorrect input list" << endl;
		cerr << "exiting..." << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	auto filenames = vector<string>();
	for (auto i = 1; i < argc; i++) {
		filenames.push_back(argv[i]);
	}
	if (processImages(filenames)) {
		cerr << "Unable to open background image file: " << filenames[0] << endl;
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

int processImages(const vector<string> filenames)
{
	// create GUI windows
	namedWindow("Frame");
	namedWindow("Foreground segmented");

	// Background image:
	Mat bgFrame = imread(filenames[0]);
	if (bgFrame.empty()) {
		//error in opening the image
		return -1;
	}


	for (auto i=1; i < filenames.size();i++) {   // Note the starting index.
		auto f = filenames[i];
		if (processImage(f, bgFrame)) {
			cerr << "Unable to open image file: " << f << endl;
			exit(EXIT_FAILURE);
		}
		char keyboard = (char)0;
		while (keyboard != 'q' && keyboard != 27) {
			keyboard = waitKey(10);
		}
	}
	cout << "Processed all image files." << endl;

	// destroy GUI windows
	destroyAllWindows();

	return 0;
}

// Non-zero return value means error occurred.
int processImage(const string filename, Mat bgFrame)
{
	static int counter = 0;

	// Read the foreground file.
	Mat fgFrame = imread(filename);
	if (fgFrame.empty()) {
		return -1;
	}

	// Just use diff...
	Mat bgSubtracted;
	absdiff(fgFrame, bgFrame, bgSubtracted);

	rectangle(fgFrame, cv::Point(10, 2), cv::Point(100, 20),
		cv::Scalar(255, 255, 255), -1);
	putText(fgFrame, std::to_string(counter), cv::Point(15, 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

	//show the current frame and the fg masks
	imshow("Frame", fgFrame);
	imshow("Foreground segmented", bgSubtracted);

	//int keyboard = 0;
	//while ((char)keyboard != 'q' && (char)keyboard != 27) {
	//	//get the input from the keyboard
	//	keyboard = waitKey(30);
	//}
	counter++;

	return 0;
}
