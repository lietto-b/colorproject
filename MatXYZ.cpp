#include "Header.h"

vector<vector<Point>> sortCoordinate(vector<vector<Point>> approx_vector)
{
	/*
	на этапе сортировки
	- сперва сортируется каждая строка (каждый кирпич, его координаты в одном направлении)
	- затем определяются центры и по центрам сортируются кирпичи

		Требуемый порядок координат ОДНОГО кирпича:
		LeftDown -> LeftUp -> RightUp -> RightDown
	*/

	vector<vector<Point>> apr = approx_vector;
	int a[2], b[2], c[2], d[2],
		center[2],
		ld[2],/*->*/ lu[2],/*->*/ ru[2],/*->*/ rd[2];

	for (int u = 0; u < 44; u++) {
		a[0] = apr.at(u).at(0).x;	a[1] = apr.at(u).at(0).y;
		b[0] = apr.at(u).at(1).x;	b[1] = apr.at(u).at(1).y;
		c[0] = apr.at(u).at(2).x;	c[1] = apr.at(u).at(2).y;
		d[0] = apr.at(u).at(3).x;	d[1] = apr.at(u).at(3).y;

		for (int i = 0; i < 2; i++) {
			center[i] = (a[i] + b[i] + c[i] + d[i]) / 4;
		}
		/*cout << "Center Coordinate x = " << center[0] << ", y = " << center[1] << "." << endl;*/

		if ((a[0] < center[0]) && (a[1] > center[1])) {
			ld[0] = a[0];		ld[1] = a[1];
		}
		else if ((a[0] < center[0]) && (a[1] < center[1])) {
			lu[0] = a[0];		lu[1] = a[1];
		}
		else if ((a[0] > center[0]) && (a[1] < center[1])) {
			ru[0] = a[0];		ru[1] = a[1];
		}
		else if ((a[0] > center[0]) && (a[1] > center[1])) {
			rd[0] = a[0];		rd[1] = a[1];
		}

		if ((b[0] < center[0]) && (b[1] > center[1])) {
			ld[0] = b[0];		ld[1] = b[1];
		}
		else if ((b[0] < center[0]) && (b[1] < center[1])) {
			lu[0] = b[0];		lu[1] = b[1];
		}
		else if ((b[0] > center[0]) && (b[1] < center[1])) {
			ru[0] = b[0];		ru[1] = b[1];
		}
		else if ((b[0] > center[0]) && (b[1] > center[1])) {
			rd[0] = b[0];		rd[1] = b[1];
		}

		if ((c[0] < center[0]) && (c[1] > center[1])) {
			ld[0] = c[0];		ld[1] = c[1];
		}
		else if ((c[0] < center[0]) && (c[1] < center[1])) {
			lu[0] = c[0];		lu[1] = c[1];
		}
		else if ((c[0] > center[0]) && (c[1] < center[1])) {
			ru[0] = c[0];		ru[1] = c[1];
		}
		else if ((c[0] > center[0]) && (c[1] > center[1])) {
			rd[0] = c[0];		rd[1] = c[1];
		}

		if ((d[0] < center[0]) && (d[1] > center[1])) {
			ld[0] = d[0];		ld[1] = d[1];
		}
		else if ((d[0] < center[0]) && (d[1] < center[1])) {
			lu[0] = d[0];		lu[1] = d[1];
		}
		else if ((d[0] > center[0]) && (d[1] < center[1])) {
			ru[0] = d[0];		ru[1] = d[1];
		}
		else if ((d[0] > center[0]) && (d[1] > center[1])) {
			rd[0] = d[0];		rd[1] = d[1];
		}

		apr.at(u).at(0).x = ld[0];	apr.at(u).at(0).y = ld[1];
		apr.at(u).at(1).x = lu[0];	apr.at(u).at(1).y = lu[1];
		apr.at(u).at(2).x = ru[0];	apr.at(u).at(2).y = ru[1];
		apr.at(u).at(3).x = rd[0];	apr.at(u).at(3).y = rd[1];
	}

	int cent[44][2];
	for (int z = 0; z < 44; z++) {
		a[0] = apr.at(z).at(0).x;	a[1] = apr.at(z).at(0).y;
		b[0] = apr.at(z).at(1).x;	b[1] = apr.at(z).at(1).y;
		c[0] = apr.at(z).at(2).x;	c[1] = apr.at(z).at(2).y;
		d[0] = apr.at(z).at(3).x;	d[1] = apr.at(z).at(3).y;

		cent[z][0] = (a[0] + b[0] + c[0] + d[0]) / 4;
		cent[z][1] = (a[1] + b[1] + c[1] + d[1]) / 4;

		//cout << "Center Coordinate x = " << cent[z][0] << ", y = " << cent[z][1] << "." << endl;
	}

	int temp[2];
	for (int n = 0; n < 43; n++) {
		for (int l = 0; l < 43; l++) {
			if (cent[l][1] > cent[l + 1][1]) {
				temp[0] = cent[l][0]; temp[1] = cent[l][1];
				cent[l][0] = cent[l + 1][0]; cent[l][1] = cent[l + 1][1];
				cent[l + 1][0] = temp[0]; cent[l + 1][1] = temp[1];
			}
		}
	}

	int ttemp[2];
	int size = 44;
	int temp_m, temp_curt;
	for (int m = 0; m < 44; m = m + 4) {
		temp_m = m;
		temp_curt = temp_m + 3;
		for (int n = temp_m; n < temp_curt; n++) {
			for (int l = temp_m; l < temp_curt; l++) {
				if (cent[l][0] > cent[l + 1][0]) {
					ttemp[0] = cent[l][0]; ttemp[1] = cent[l][1];
					cent[l][0] = cent[l + 1][0]; cent[l][1] = cent[l + 1][1];
					cent[l + 1][0] = ttemp[0]; cent[l + 1][1] = ttemp[1];
				}
			}
		}
	}

	int cent2[44][2];
	int dotes[44][8];

	for (int z = 0; z < 44; z++) {
		dotes[z][0] = apr.at(z).at(0).x;	dotes[z][1] = apr.at(z).at(0).y;
		dotes[z][2] = apr.at(z).at(1).x;	dotes[z][3] = apr.at(z).at(1).y;
		dotes[z][4] = apr.at(z).at(2).x;	dotes[z][5] = apr.at(z).at(2).y;
		dotes[z][6] = apr.at(z).at(3).x;	dotes[z][7] = apr.at(z).at(3).y;

		cent2[z][0] = (dotes[z][0] + dotes[z][2] + dotes[z][4] + dotes[z][6]) / 4;
		cent2[z][1] = (dotes[z][1] + dotes[z][3] + dotes[z][5] + dotes[z][7]) / 4;
	}

	for (int g = 0; g < 44; g++) {
		for (int y = 0; y < 44; y++) {
			if ((cent[g][1] == cent2[y][1]) && (cent[g][0] == cent2[y][0])) {
				apr.at(g).at(0).x = dotes[y][0];	apr.at(g).at(0).y = dotes[y][1];
				apr.at(g).at(1).x = dotes[y][2];	apr.at(g).at(1).y = dotes[y][3];
				apr.at(g).at(2).x = dotes[y][4];	apr.at(g).at(2).y = dotes[y][5];
				apr.at(g).at(3).x = dotes[y][6];	apr.at(g).at(3).y = dotes[y][7];
				y = 44; //break;
			}
		}
	}

	return apr;
}

vector<vector<Point>> Coordinate(Mat mask, Mat src)
{
	Mat edged, kernel, closed, gray, bw, smoth;
	GaussianBlur(mask, smoth, Size(7, 7), 0);
	Canny(smoth, edged, 0, 255);

	kernel = getStructuringElement(MORPH_RECT, Point(3, 3));
	morphologyEx(edged, closed, MORPH_CLOSE, kernel);

	vector<vector<Point>> cnts;
	findContours(closed, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	double peri;
	vector<vector<Point>> approxs;
	vector<Point> approx;

	int total = 0;
	for (vector<Point>c : cnts) {
		peri = arcLength(c, true);
		approxPolyDP(c, approx, 0.02 * peri, true);

		if (approx.size() == 4 && peri > 900) {
			approxs.push_back(approx);
			total++;
		}
	}
	cout << "Count of kirp: " << total << endl;

	return approxs;
}