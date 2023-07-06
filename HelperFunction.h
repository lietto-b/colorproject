#include "Header.h"

#ifndef Header_h
#define Header_h 

// функции

// Функции получения маски изображения
void voidRemoval(Mat &img);
void noiseRemoval(Mat &img);
Mat getMask(Mat src, int fx); // Построение маски
Mat combineMask(Mat mask, Mat src); // Выделение объекта

vector<vector<Point>> sortCoordinate(vector<vector<Point>> approx_vector); // Сортировка вектора координат
vector<vector<Point>> Coordinate(Mat mask, Mat src); // Построение контура кирпичей на изображении, получение вектора координат

// Вырезание каждого кирпича отдельно
void cuttingObject(Mat& img, vector<vector<Point>>& approx_vector);

// Свои функции конвертации цветовых пространств
Mat RGBtoXYZ(Mat img);
Mat XYZtoLab(Mat img);

template<class T>
void displaySpecialVector(vector<vector<T>> const& mat);
// Сегментирование изображения, формирование вектора сегментов
vector<vector<specialPoint3>> arraySegmentation(Mat img, int sizeSegmentation);

// Вычисление веса каждого сегмента
void removeSimWeightSegment(vector<int>& weightVector);
double* amountWeightSegment(vector<specialPoint3> weightVector);

Mat drawBadSegment(Mat src, int sizeSegmentation, int step);

#endif //Header