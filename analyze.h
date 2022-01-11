#pragma once
	// Jacob Melone
#include <iostream>
#include <iomanip>
#include <string.h>
#include "opencv2/opencv.hpp"
#include <immintrin.h>
#include <random>
#include <time.h>
#include <vector>
#include <string>
#include <fstream>
#include <omp.h>
	// Standard
using namespace std;
	// OpenCV
using namespace cv;

/*typedef struct node {
	int x, y, z;
	uchar r, g, b;
	string type;
	vector<string> files;
	void debug_print();
	void read_from_file(string );
	void save_to_file(string);
};*/

void init();

vector<vector<bool>> contrast(Mat);

void pixel_count(Mat, int&, int&);