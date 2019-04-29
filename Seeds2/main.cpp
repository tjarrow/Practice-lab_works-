// Practice.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <experimental/filesystem>
#include "watershed.hpp"
#include <opencv2/core/core_c.h>

using namespace std;
using namespace cv;
namespace fs = std::experimental::filesystem;

int main(int argc, char *argv[]) {
	
	//std::string dirName = "H:\\Grains_photo\\gradient1.pgm";
	std::vector<std::string> files;// = getFileList(dirName);

	/*for (const auto & entry : fs::directory_iterator(dirName))
	{
		string entry_str = entry.path().string();
		cout << entry.path().string() << endl;
		//files.push_back(entry_str);
	}*/
		//std::cout << entry.path() << std::endl;
	
	ofstream myfile;
	myfile.open("res1.csv");

	vector<double> length;
	vector<double> width;
	vector<double> area;
	Mat image = imread("11.jpg");

	int count = mainAlg(image, length, width, area);
	myfile << count << "\n";

	/*for (int i = 0; i < files.size(); ++i) {
		vector<double> length;
		vector<double> width;
		vector<double> area;
		Mat image = imread(files.at(i));
		 
		int count = mainAlg(image, length, width, area);
		myfile << i-2 <<","<< count << "\n";
	}*/

	myfile.close();

	return 0;
}

