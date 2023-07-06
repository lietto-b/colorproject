#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <typeinfo>
#include <vector>
#include <algorithm>

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include <opencv2/imgproc/imgproc_c.h>
#include "math.h"
#include <cmath>

#include <stdlib.h>
#include <stdio.h>

using namespace std;
using namespace cv;

struct specialPoint3
{
	int b = 0;
	int a = 0;
	int L = 0;
};
