#pragma once

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int ConvertImage2ColorNames(vector<int>& colorNames, InputArray src, OutputArray dst);
int ConvertImage2Color(vector<int>& colorNames, InputArray src, OutputArray dst);


int ReadConfigs(string filePath, vector<int>& colorNames);
int ReadConfigs(string filePath, vector<vector<double>>& colorIndex);