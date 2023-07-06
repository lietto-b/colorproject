#include "Header.h"

#ifndef Header_h
#define Header_h 

// �������

// ������� ��������� ����� �����������
void voidRemoval(Mat &img);
void noiseRemoval(Mat &img);
Mat getMask(Mat src, int fx); // ���������� �����
Mat combineMask(Mat mask, Mat src); // ��������� �������

vector<vector<Point>> sortCoordinate(vector<vector<Point>> approx_vector); // ���������� ������� ���������
vector<vector<Point>> Coordinate(Mat mask, Mat src); // ���������� ������� �������� �� �����������, ��������� ������� ���������

// ��������� ������� ������� ��������
void cuttingObject(Mat& img, vector<vector<Point>>& approx_vector);

// ���� ������� ����������� �������� �����������
Mat RGBtoXYZ(Mat img);
Mat XYZtoLab(Mat img);

template<class T>
void displaySpecialVector(vector<vector<T>> const& mat);
// ��������������� �����������, ������������ ������� ���������
vector<vector<specialPoint3>> arraySegmentation(Mat img, int sizeSegmentation);

// ���������� ���� ������� ��������
void removeSimWeightSegment(vector<int>& weightVector);
double* amountWeightSegment(vector<specialPoint3> weightVector);

Mat drawBadSegment(Mat src, int sizeSegmentation, int step);

#endif //Header