// CVTutor.Video.Background01.cpp : Defines the entry point for the console application.
//

// Tutorial
// http://docs.opencv.org/3.1.0/d1/dc5/tutorial_background_subtraction.html


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
void processImages(const vector<string> filenames);
int processImage(const string filename, Ptr<BackgroundSubtractor> pMOG2);


// Usage example:
// .\CVTutor.Video.Background01.exe ..\..\sample-frames\birds\bird0.png  ..\..\sample-frames\birds\bird1.png  ..\..\sample - frames\birds\bird2.png  ..\..\sample - frames\birds\bird3.png
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
	processImages(filenames);

	return EXIT_SUCCESS;
}

void processImages(const vector<string> filenames)
{
	// create GUI windows
	namedWindow("Frame");
	namedWindow("FG Mask MOG 2");

	// create Background Subtractor objects
	Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2(); //MOG2 approach

	for (auto f : filenames) {
		if (processImage(f, pMOG2)) {
			cerr << "Unable to open image file: " << f << endl;
			exit(EXIT_FAILURE);
		}
		char keyboard = (char) 0;
		while (keyboard != 'q' && keyboard != 27) {
			keyboard = waitKey(10);
		}
		//if (keyboard == 'q' && keyboard == 27) {
		//	break;
		//}
	}
	cout << "Processed all image files." << endl;

	// destroy GUI windows
	destroyAllWindows();
}

// Non-zero return value means error occurred.
int processImage(const string filename, Ptr<BackgroundSubtractor> pMOG2) 
{
	static int counter = 0;

	//read the first file of the sequence
	Mat frame = imread(filename);
	if (frame.empty()) {
		//error in opening the image
		return -1;
	}

	Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
	pMOG2->apply(frame, fgMaskMOG2);
	rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
		cv::Scalar(255, 255, 255), -1);
	putText(frame, std::to_string(counter), cv::Point(15, 15),
		FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

	//show the current frame and the fg masks
	imshow("Frame", frame);
	imshow("FG Mask MOG 2", fgMaskMOG2);

	//int keyboard = 0;
	//while ((char)keyboard != 'q' && (char)keyboard != 27) {
	//	//get the input from the keyboard
	//	keyboard = waitKey(30);
	//}
	counter++;

	return 0;
}
