#include "Header.h"

void cuttingObject(Mat &img, vector<vector<Point>> &apr)
{
	int road[4][2], //строка координат
		dote[4];

	int h, //hight
		w; //width

	int sl, //samaya left
		su, //samaya up
		sr, //samaya right
		sd; //samaya down

	int sX, //start X
		sY; //start Y

	string name_f;
	string num0 = "Produce/CUT/";
	string num1 = "_cutDSC.png";

	int id_brick = 0;
	while (id_brick < 44) {
		for (int i = 0; i < 4; i++)	{
			road[i][0] = apr.at(id_brick).at(i).x;
			road[i][1] = apr.at(id_brick).at(i).y;
		}

		sX = road[0][0];
		sr = 0;
		for (int i = 1; i < 4; i++)
			if (road[i][0] < sX) {
				sX = road[i][0];
				if (road[i - 1][0] > sr)
					sr = road[i - 1][0];
			}
			else {
				if (road[i][0] > sr)
					sr = road[i][0];
			}


		sY = road[0][1];
		sd = 0;
		for (int i = 1; i < 4; i++)
			if (road[i][1] < sY) {
				sY = road[i][1];
				if (road[i - 1][1] > sd)
					sd = road[i - 1][1];
			}
			else {
				if (road[i][1] > sd)
					sd = road[i][1];
			}


		h = abs(sY - sd); //высота изображения, разница самой верхней и самой нижней точки
		//тоже самое для ширины
		w = abs(sr - sX);

		//cout << "[#: " << id_brick << ", h: " << h << ", w: " << w << "]" << endl;

		Mat ROI(img, Rect(sX, sY, w, h));
		Mat croppedImmage;
		ROI.copyTo(croppedImmage);

		name_f = num0 + to_string(id_brick + 1) + num1;
		imwrite(name_f, croppedImmage);

		id_brick++;
	}
}

template<class T>
void displaySpecialVector(vector<vector<T>> const& mat) {
	int check = 0;
	int check2 = 0;
	for (vector<T> row : mat) {
		for (T val : row) {
			cout << "[" << val.b << " " << val.a << " " << val.L << "] ";
			check2++;
		}
		//cout << check << " " << row.size() << " ";
		check++; check2 = 0;
		cout << endl << endl << endl;
	}
	//cout << endl << endl;
	//cout << mat.size() << endl;
}

vector<vector<specialPoint3>> arraySegmentation(Mat img, int sizeSegmentation)
{
	string name_f;
	string num0 = "Produce/CUT/";
	string num1 = "_cutDSC.png";

	double him = img.rows;
	double wim = img.cols;

	int himSegmentCount, himSegmentLast,
		wimSegmentCount, wimSegmentLast;

	// количество сегментов по высоте
	himSegmentCount = ceil(him / sizeSegmentation);
	// высота последнего сегмента
	himSegmentLast = him - ((himSegmentCount - 1) * sizeSegmentation);

	// количество сегментов по ширине
	wimSegmentCount = ceil(wim / sizeSegmentation);
	// ширина последнего сегмента
	wimSegmentLast = wim - ((wimSegmentCount - 1) * sizeSegmentation);

	/*cout << "Size " << him << " x " << wim << endl
		<< "Count segment height = " << himSegmentCount << endl
		<< "Count segment width  = " << wimSegmentCount << endl
		<< "Size last segment on h = " << himSegmentLast << ", on w = " << wimSegmentLast << endl;*/

	//arraySegment.resize(him, vector<specialPoint3>(wim));

	int	iStep = 0,
		jStep = 0;
	vector<vector<specialPoint3>> arraySegment;
	vector<specialPoint3> weightList;
	specialPoint3 weightElem;

	for (int ih = 0; ih < himSegmentCount; ih++) {
		for (int jh = 0; jh < wimSegmentCount; jh++) {
			if (ih < (himSegmentCount - 1) && jh < (wimSegmentCount - 1)) {
				// центральная симметричная часть
				iStep = sizeSegmentation;
				jStep = sizeSegmentation;
			}
			else if (ih == (himSegmentCount - 1) && jh < (wimSegmentCount - 1)) {
				// нижняя часть
				iStep = himSegmentLast;
				jStep = sizeSegmentation;
			}
			else if (ih < (himSegmentCount - 1) && jh == (wimSegmentCount - 1)) {
				// правая часть
				iStep = sizeSegmentation;
				jStep = wimSegmentLast;
			}
			else if (ih == (himSegmentCount - 1) && jh == (wimSegmentCount - 1)) {
				// нижняя правая часть
				iStep = himSegmentLast;
				jStep = wimSegmentLast;
			}

			for (int i = 0; i < iStep; i++) {
				for (int j = 0; j < jStep; j++) {
					Vec3s intest = img.at<Vec3s>((ih * sizeSegmentation) + i, (jh * sizeSegmentation) + j);
					weightElem.L = intest[2];
					weightElem.a = intest[1];
					weightElem.b = intest[0];

					weightList.push_back(weightElem);
				}
			}
			arraySegment.push_back(weightList);
			weightList.clear();
		}
	}

	//displaySpecialVector(arraySegment);
	return arraySegment;
}

void removeSimWeightSegment(vector<double>& weightVector)
{
	// Быстрое удаление дублей из вектора
	auto end = weightVector.end();
	for (auto it = weightVector.begin(); it != end; ++it) {
		end = remove(it + 1, end, *it);
	}

	weightVector.erase(end, weightVector.end());
}

double* amountWeightSegment(vector<specialPoint3> weightVector)
{
	/*
		Функция разбора ОДНОГО сегмента
	*/

	int maxList = weightVector.size();
	double* amountWeight = new double[3];

	// 1. Выделить 3 вектора из основного
	vector<double> L_list;
	vector<vector<double>> ab_list;
	vector<double> ab_point;
	for (int i = 0; i < maxList; i++) {
		ab_point.push_back(weightVector.at(i).b);
		ab_point.push_back(weightVector.at(i).a);

		// 1.1 Удалить дубли из основных ЗАВИСИМЫХ векторов
		if (count(ab_list.begin(), ab_list.end(), ab_point) or (ab_point.at(0) == 0 && ab_point.at(1) == 0)) {
		}
		else {
			//cout << ab_point.at(0) << ", " << ab_point.at(1) << endl;
			ab_list.push_back(ab_point);
		}
		ab_point.clear();

		// 1.2 Удалить дубли из вектора L
		if (count(L_list.begin(), L_list.end(), weightVector.at(i).L) or (weightVector.at(i).L == 0)) {
		}
		else {
			L_list.push_back(weightVector.at(i).L);
		}
		ab_point.clear();
	}

	// 2. Посчитать среднее значение во всех векторах
	double sumVector = 0.0;

	if (ab_list.size() == 0) {
		amountWeight[0] = 0;
		amountWeight[1] = 0;
	}
	else {
		for (int i = 0; i < ab_list.size(); i++)
			sumVector = sumVector + ab_list.at(i).at(0);

		amountWeight[0] = sumVector / ab_list.size();
		sumVector = 0.0;

		for (int i = 0; i < ab_list.size(); i++)
			sumVector = sumVector + ab_list.at(i).at(1);

		amountWeight[1] = sumVector / ab_list.size();
		sumVector = 0.0;
	}

	if (ab_list.size() == 0) {
		amountWeight[2] = 0;
	}
	else {
		for (int i = 0; i < L_list.size(); i++)
			sumVector = sumVector + L_list.at(i);

		amountWeight[2] = sumVector / L_list.size();
		sumVector = 0.0;
	}
	
	return amountWeight;
}

Mat drawBadSegment(Mat src, int sizeSegmentation, int step)
{
	Mat img = src;

	double him = img.rows;
	double wim = img.cols;

	int himSegmentCount, himSegmentLast,
		wimSegmentCount, wimSegmentLast;

	// количество сегментов по высоте
	himSegmentCount = ceil(him / sizeSegmentation);
	// высота последнего сегмента
	himSegmentLast = him - ((himSegmentCount - 1) * sizeSegmentation);

	// количество сегментов по ширине
	wimSegmentCount = ceil(wim / sizeSegmentation);
	// ширина последнего сегмента
	wimSegmentLast = wim - ((wimSegmentCount - 1) * sizeSegmentation);

	/*cout << "Size " << him << " x " << wim << endl
		<< "Count segment height = " << himSegmentCount << endl
		<< "Count segment width  = " << wimSegmentCount << endl
		<< "Size last segment on h = " << himSegmentLast << ", on w = " << wimSegmentLast << endl;*/

		//arraySegment.resize(him, vector<specialPoint3>(wim));

	int	iStep = 0,
		jStep = 0;
	Point up, down;
	vector<vector<specialPoint3>> arraySegment;
	vector<specialPoint3> weightList;
	specialPoint3 weightElem;

	int stepX = 0, stepY = 0;
	for (int ih = 0; ih < himSegmentCount; ih++) {
		for (int jh = 0; jh < wimSegmentCount; jh++) {
			if (ih < (himSegmentCount - 1) && jh < (wimSegmentCount - 1)) {
				// центральная симметричная часть
				iStep = sizeSegmentation;
				jStep = sizeSegmentation;
			}
			else if (ih == (himSegmentCount - 1) && jh < (wimSegmentCount - 1)) {
				// нижняя часть
				iStep = himSegmentLast;
				jStep = sizeSegmentation;
			}
			else if (ih < (himSegmentCount - 1) && jh == (wimSegmentCount - 1)) {
				// правая часть
				iStep = sizeSegmentation;
				jStep = wimSegmentLast;
			}
			else if (ih == (himSegmentCount - 1) && jh == (wimSegmentCount - 1)) {
				// нижняя правая часть
				iStep = himSegmentLast;
				jStep = wimSegmentLast;
			}
		}
	}

	stepY = step;
	while (stepY > wimSegmentCount) {
		stepY = stepY - wimSegmentCount;
		stepX++;
	}
	cout << endl << stepX << ", " << stepY << endl;

	rectangle(img,
		cv::Point(stepY * sizeSegmentation, stepX * sizeSegmentation),
		cv::Point((stepY * sizeSegmentation) + sizeSegmentation, (stepX * sizeSegmentation) + sizeSegmentation),
		Scalar(0, 0, 255), 1);

	stepY = 0; stepX = 0;

	return img;
}