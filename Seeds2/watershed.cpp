#include "stdafx.h"
#include "watershed.hpp"
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

const double PIXEL_SIZE = 0.102;

Mat imageRead(std::string path) {
	Mat img = imread(path.c_str());
	if (!img.data) {
		throw std::invalid_argument("This file doesn't exist");
	}
	return imread(path.c_str());
}

Mat laplacian(Mat &sharp) {
	Mat kernel = (Mat_<float>(3, 3) << 1, 1, 1, 1, -8, 1, 1, 1, 1);
	Mat imgLaplacian;
	filter2D(sharp, imgLaplacian, CV_32F, kernel);
	return imgLaplacian;
}


Mat erode(Mat &subPlans, Mat &imgLaplacian) {
	subPlans.convertTo(subPlans, CV_8UC3);
	imgLaplacian.convertTo(imgLaplacian, CV_8UC3);


	blur(subPlans, subPlans, Size(3, 3));


	int erodeSz = 4;
	Mat structuringElement = cv::getStructuringElement(MORPH_ELLIPSE,
		Size(2 * erodeSz + 1,
			2 * erodeSz + 1),
		Point(erodeSz, erodeSz));

	erode(subPlans, subPlans, structuringElement);
	dilate(subPlans, subPlans, structuringElement);
	bitwise_not(subPlans, subPlans);


	threshold(subPlans, subPlans, 30, 255,
		CV_THRESH_BINARY | CV_THRESH_OTSU);

	return subPlans;
}

Mat getMarkers(Mat subPlans, Mat &sureFigure, Mat &unknown) {
	Mat opening;
	Mat kernel_1(3, 3, CV_8U, Scalar(1));
	morphologyEx(subPlans, opening, MORPH_OPEN, kernel_1,
		Point(-1, -1), 1);

	Mat sure_bg;
	dilate(opening, sure_bg, kernel_1, Point(-1, -1), 3);

	Mat distTransform;
	distanceTransform(opening, distTransform, CV_DIST_L2, 5);

	double minVal, maxVal;
	Point minLoc, maxLoc;
	minMaxLoc(distTransform, &minVal, &maxVal, &minLoc, &maxLoc);

	threshold(distTransform, sureFigure, 0, 255, 0);

	Mat sureFigure1;
	sureFigure.convertTo(sureFigure1, CV_8UC1);
	subtract(sure_bg, sureFigure1, unknown);

	sureFigure.convertTo(sureFigure, CV_32SC1, 1.0);

	return Mat::zeros(sureFigure.rows, sureFigure.cols, CV_32SC1);
}


vector<vector<Point> > drawContours(Mat markers, Mat sureFigure) {
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(sureFigure, contours, hierarchy, RETR_CCOMP,
		CHAIN_APPROX_SIMPLE);

	int compCount = 0;

	for (int index = 0; index >= 0; index = hierarchy[index][0], compCount++)
		drawContours(markers, contours, index, Scalar::all(compCount + 1),
			-1, 8, hierarchy, INT_MAX);
	return contours;
}


int mainAlg(Mat resultImage, std::vector<double> &length,
	std::vector<double> &width, std::vector<double> &area) {

	Mat hsv;
	cvtColor(resultImage, hsv, CV_BGR2HSV);

	vector<Mat> channels;
	split(hsv, channels);


	Mat sharp = channels[1];
	  imwrite("result1.png", sharp);
	Mat imgLaplacian = laplacian(sharp);
	imgLaplacian = laplacian(imgLaplacian);
	  imwrite("result2.png", imgLaplacian);


	channels[1].convertTo(sharp, CV_32F);

	Mat subPlans = sharp - imgLaplacian;
	subPlans = erode(subPlans, imgLaplacian);


	Mat sureFigure, contour;

	Mat markers = getMarkers(subPlans, sureFigure, contour);

	vector<vector<Point> > contours = drawContours(markers, sureFigure);

	markers = markers + 1;
	for (int i = 0; i < markers.rows; i++) {
		for (int j = 0; j < markers.cols; j++) {
			unsigned char &v = contour.at<unsigned char>(i, j);
			if (v == 255) {
				markers.at<int>(i, j) = 0;

			}
		}
	}

	watershed(resultImage, markers);

	for (int i = 0; i < markers.rows; i++) {
		for (int j = 0; j < markers.cols; j++) {
			int index = markers.at<int>(i, j);
			if (index == -1)
				resultImage.at<Vec3b>(i, j) = Vec3b(0, 255, 0);

		}
	}

	int count = 0;
	for (unsigned int i = 0; i < contours.size(); i = i + 2) {
		RotatedRect rect = minAreaRect(contours[i]);
		putText(resultImage, to_string(count + 1), rect.center,
			FONT_ITALIC, 2, Scalar(0, 0, 255), 1);
		length.push_back(rect.size.height * PIXEL_SIZE);
		width.push_back(rect.size.width * PIXEL_SIZE);
		area.push_back(contourArea(contours[i]) * PIXEL_SIZE * PIXEL_SIZE);
		count++;
	}

	  imwrite("result.png", resultImage);

	return count;
}

std::vector<std::string> getFileList(std::string path) {

	int idx = 0;
	
	std::vector<std::string> files;
	/*if ((dir = opendir(path.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_name != std::string("..") and ent->d_name != std::string(".")) {
				files.push_back(ent->d_name);
			}
		}
		closedir(dir);
	}*/

	//ofstream fout("amount.txt");

	

	return files;
}


