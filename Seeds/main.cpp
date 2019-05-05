// Practice.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <experimental/filesystem>
#include "watershed.hpp"
#include <opencv2/core/core_c.h>

using namespace cv;
namespace fs = std::experimental::filesystem;

int main(int argc, char *argv[]) {

	std::string dirName = "C:\\Rice1";
	std::vector<std::string> files;

	for (const auto & entry : fs::directory_iterator(dirName))
	{
		string entry_str = entry.path().string();
		files.push_back(entry_str);
	}

	ofstream myfile;

	myfile.open("result.csv");

	for (int i = 0; i < files.size(); ++i) {

		vector<double> length;
		vector<double> width;
		vector<double> area;

		Mat image = imread(files.at(i));
		int count = mainAlg(image, length, width, area);
		myfile << i << ", " << count << "\n";
	}

	myfile.close();

	return 0;
}

