#include "Header.h"

/*
	Полная маска изображения получается следующим образом:
	1. Изначально по заданным параметрам (которые были получены в ходе научного исследования) 
		определяется цвет кирпича и по ним строится первичная маска;
	2. Первичная маска "расширяется" на 1 пиксель для заполнения пустот, 
		которые могут возникать из-за динамичного параметра освещения и других;
	3. Из расширенной маски удаляются "шумы", "одиночные пиксели", в результате получается полная маска изображения.
*/

void voidRemoval(Mat &img)
{
	int black = 0;
	int count = 0;
	cout << "Cols: " << img.cols << endl << "Rows: " << img.rows << endl << "CxR: " << img.cols * img.rows << endl;

	for (int i = 1; i < img.cols - 1; i++)	{
		for (int j = 1; j < img.rows - 1; j++)		{
			Vec3b intest = img.at<Vec3b>(j, i);

			Vec3b coord[4];
			coord[0] = img.at<Vec3b>(j, i - 1);
			coord[1] = img.at<Vec3b>(j, i + 1);
			coord[2] = img.at<Vec3b>(j - 1, i);
			coord[3] = img.at<Vec3b>(j + 1, i);

			if (intest[0] == 0 && intest[1] == 0 && intest[2] == 0)	{
				black = 1;
			}
			else {
				black = 0;
			}
			if ((coord[0][0] == 55 && coord[0][1] == 135 && coord[0][2] == 200) && (black == 1)) {
				coord[0][0] = 160;
				coord[0][1] = 70;
				coord[0][2] = 160;
				//cout << "_UPUPUP_" << endl;

				img.at<Vec3b>(j, i - 1) = coord[0];
			}

			if ((coord[1][0] == 55 && coord[1][1] == 135 && coord[1][2] == 200) && (black == 1)) {
				coord[1][0] = 160;
				coord[1][1] = 70;
				coord[1][2] = 160;
				//cout << "_DOWNDOWNDOWN_" << endl;

				img.at<Vec3b>(j, i + 1) = coord[1];
			}

			if ((coord[2][0] == 55 && coord[2][1] == 135 && coord[2][2] == 200) && (black == 1)) {
				coord[2][0] = 160;
				coord[2][1] = 70;
				coord[2][2] = 160;
				//cout << "_LEFTLEFTLEFT_" << endl;

				img.at<Vec3b>(j - 1, i) = coord[2];
			}

			if ((coord[3][0] == 55 && coord[3][1] == 135 && coord[3][2] == 200) && (black == 1)) {
				coord[3][0] = 160;
				coord[3][1] = 70;
				coord[3][2] = 160;
				//cout << "_RIGHTRIGHTRIGHT_" << endl;

				img.at<Vec3b>(j + 1, i) = coord[3];
			}

			black = 0;
			count++;
		}
	}

	cout << count << endl;
}

void noiseRemoval(Mat &img)
{
	for (int i = 0; i < img.cols; i++)	{
		for (int j = 0; j < img.rows; j++) {
			Vec3b intest = img.at<Vec3b>(j, i);
			if (intest[0] == 160 && intest[1] == 70 && intest[2] == 160) {
				intest[0] = 0;
				intest[1] = 0;
				intest[2] = 0;

				img.at<Vec3b>(j, i) = intest;
			}
		}
	}
}

Mat getMask(Mat src, int fx)
{
	if (fx != 0 && fx != 1)
		fx = 1;

	Mat img;
	src.copyTo(img);
	double R = 0.0, G = 0.0, B = 0.0;
	double GR = 0.0, BR = 0.0;

	for (int i = 0; i < img.cols; i++) {
		for (int j = 0; j < img.rows; j++) {
			Vec3b intest = img.at<Vec3b>(j, i);
			R = intest[2];
			G = intest[1];
			B = intest[0];

			GR = G / R;
			BR = B / R;

			if ((GR > 0.5) && (GR < 0.7) && (BR > 0.3) && (BR < 0.5) && (R > 150)) {
				//кирпич
				intest[0] = 55;
				intest[1] = 135;
				intest[2] = 200;

				img.at<Vec3b>(j, i) = intest;
			}
			else if ((GR > 0.6) && (GR < 0.75) && (BR > 0.40) && (BR < 0.65) && (R > 150)) {
				//другой цвет (белый, выделяется красным, уже не нужно)
				if (fx == 0) {
					intest[0] = 20;
					intest[1] = 20;
					intest[2] = 250;

				}
				else if (fx == 1) {
					//если не нужно выделять другим цветом
					intest[0] = 55;
					intest[1] = 135;
					intest[2] = 200;
				}

				img.at<Vec3b>(j, i) = intest;
			}
			else {
				intest[0] = 0;
				intest[1] = 0;
				intest[2] = 0;

				img.at<Vec3b>(j, i) = intest;
			}
		}
	}

	voidRemoval(img);
	noiseRemoval(img);

	return img;
}

Mat combineMask(Mat mask, Mat src)
{
	Mat combine;
	src.copyTo(combine);

	for (int i = 0; i < mask.cols; i++) {
		for (int j = 0; j < mask.rows; j++) {
			Vec3b intest = mask.at<Vec3b>(j, i);
			Vec3b intestSRC = src.at<Vec3b>(j, i);	

			// не кирпич
			if ((intest[2] == 0) && (intest[1] == 0) && (intest[0] == 0)) {
				combine.at<Vec3b>(j, i) = intest;
			}
			else {
				combine.at<Vec3b>(j, i) = intestSRC;
			}
		}
	}

	return combine;
}