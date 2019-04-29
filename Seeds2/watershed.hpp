#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/types_c.h>
#include <fstream>

using namespace std;
using namespace cv;

Mat imageRead(std::string path);
Mat laplacian(Mat &sharp);

Mat erode(Mat &subPlans, Mat &imgLaplacian);

Mat getMarkers(Mat subPlans, Mat &sureFigure, Mat &unknown);
vector<vector<Point> > drawContours(Mat markers, Mat sureFigure);

int mainAlg(Mat resultImage, std::vector<double> &length,
	std::vector<double> &width, std::vector<double> &area);


std::vector<std::string> getFileList(std::string path);

