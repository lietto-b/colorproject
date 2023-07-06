#include "Header.h"
#include "HelperFunction.h"

int main()
{
	setlocale(LC_ALL, "Russian");

	Mat src = imread("DSC_0007.jpg"); // Оригинальное изображение
	Mat mask = getMask(src, 1); // Получение маски от оригинального изображения
	Mat srcMask = combineMask(mask, src);

	imwrite("Produce/0src.png", src);
	imwrite("Produce/1mask.png", mask);
	imwrite("Produce/2srcMask.png", srcMask);

	// Получение координат кирпичей
	vector<vector<Point>> BrickXY = Coordinate(mask, src);

	// Сохранение координат в файл
	ofstream f("Produce/kirp.txt");
	BrickXY = sortCoordinate(BrickXY);
	for (vector<Point> c : BrickXY) {
		f << c.at(0).x << " " << c.at(0).y << " "
			<< c.at(1).x << " " << c.at(1).y << " "
			<< c.at(2).x << " " << c.at(2).y << " "
			<< c.at(3).x << " " << c.at(3).y << endl;
	}
	f.close();

	// "Выделение" (нарезание) кирпичей 
	cuttingObject(srcMask, BrickXY);

	/*
		Для моделей всех кирпичей
	*/
	/*string nameIMGbirck;
	Mat imgFORb;
	vector<vector<specialPoint3>> vectorForFileAm;
	ofstream fFORb;
	for (int ib = 0; ib < 44; ib++) {
		nameIMGbirck = "Produce/CUT/";
		nameIMGbirck = nameIMGbirck + to_string(ib + 1) + "_cutDSC.png";
		cout << ":" << nameIMGbirck << endl;

		imgFORb = XYZtoLab(RGBtoXYZ(imread(nameIMGbirck)));

		nameIMGbirck = "Produce/CUTtxt/" + to_string(ib + 1) + "_brickLAB.txt";
		fFORb.open(nameIMGbirck);

		fFORb << "L\ta\tb" << endl;

		vectorForFileAm = arraySegmentation(imgFORb, 1000);

		for (int i = 0; i < vectorForFileAm.size(); i++) {
			cout << fixed << setprecision(3) <<
				"segment[" << i << "]: " 
				<< amountWeightSegment(vectorForFileAm.at(i))[2] << " "
				<< amountWeightSegment(vectorForFileAm.at(i))[1] << " "
				<< amountWeightSegment(vectorForFileAm.at(i))[0] << endl;

			fFORb << fixed << setprecision(3)
				<< amountWeightSegment(vectorForFileAm.at(i))[2] << "\t"
				<< amountWeightSegment(vectorForFileAm.at(i))[1] << "\t"
				<< amountWeightSegment(vectorForFileAm.at(i))[0] << endl;
		}

		for (int iib = 0; iib < imgFORb.rows; iib++) {
			for (int jjb = 0; jjb < imgFORb.cols; jjb++) {
				Vec3s intestb = imgFORb.at<Vec3s>(iib, jjb);

				if (intestb[2] != 0 && intestb[1] != 0 && intestb[0] != 0)
					fFORb << intestb[2] << "\t" << intestb[1] << "\t" << intestb[0] << endl;
			}
		}
		fFORb.close();
	}*/
	// для отдельного файла
	/*ofstream fFORbSOLO;
	nameIMGbirck = "Produce/CUTtxt/allLAB.txt";
	fFORbSOLO.open(nameIMGbirck);
	fFORbSOLO << "№\tL\ta\tb" << endl;
	for (int ib = 0; ib < 44; ib++) {
		nameIMGbirck = "Produce/CUT/";
		nameIMGbirck = nameIMGbirck + to_string(ib + 1) + "_cutDSC.png";
		cout << ":" << nameIMGbirck << endl;

		imgFORb = XYZtoLab(RGBtoXYZ(imread(nameIMGbirck)));



		vectorForFileAm = arraySegmentation(imgFORb, 1000);

		for (int i = 0; i < vectorForFileAm.size(); i++) {
			cout << fixed << setprecision(3) <<
				"segment[" << i << "]: "
				<< amountWeightSegment(vectorForFileAm.at(i))[2] << " "
				<< amountWeightSegment(vectorForFileAm.at(i))[1] << " "
				<< amountWeightSegment(vectorForFileAm.at(i))[0] << endl;

			fFORbSOLO << fixed << setprecision(3)
				<< to_string(ib + 1) << "\t"
				<< amountWeightSegment(vectorForFileAm.at(i))[2] << "\t"
				<< amountWeightSegment(vectorForFileAm.at(i))[1] << "\t"
				<< amountWeightSegment(vectorForFileAm.at(i))[0] << endl;
		}

	}
	fFORbSOLO.close();*/

	// Как вычислять значения для эталонного кирпича
	Mat refImg = XYZtoLab(RGBtoXYZ(imread("Produce/CUT/30_cutDSC.png")));
	double* WeightS = new double[3];
	WeightS[0] = 0; WeightS[1] = 0; WeightS[2] = 0;

	vector<vector<specialPoint3>> newRefSep;
	newRefSep = arraySegmentation(refImg, 60);
	for (int i = 0; i < newRefSep.size(); i++) {
		for (int j = 0; j < 3; j++)
			WeightS[j] = WeightS[j] + amountWeightSegment(newRefSep.at(i))[j];
	}
	WeightS[2] = WeightS[2] / newRefSep.size();
	WeightS[1] = WeightS[1] / newRefSep.size();
	WeightS[0] = WeightS[0] / newRefSep.size();

	cout << endl << "Average value:" << endl
		<< "L: " << WeightS[2] << endl
		<< "a: " << WeightS[1] << endl
		<< "b: " << WeightS[0] << endl << endl;


	string fileNameIMG = "Produce/CUT/2";
	string newNameFileIMG;
	Mat src2, XYZt, LABt;
	Mat badImg;
	double* amountWS = new double[3];
	double* badPercent = new double[3];
	int countBadPercentChip = 0;	// Скол
	int countBadPercentCrack = 0;	// Трещина
	int countBadPercentColor = 0;	// Цвет
	int segmentSizeProc = 10;
	for (int fic = 0; fic < 3; fic++) {
		newNameFileIMG = to_string(fic + 1) + "_cutDSC.png";
		src2 = imread(fileNameIMG + newNameFileIMG);
		XYZt = RGBtoXYZ(src2);
		LABt = XYZtoLab(XYZt);
		badImg = src2;

		/* Для примера, необходимо разделить на сегменты 20х20 и посчитать веса */
		vector<vector<specialPoint3>> newVectorSep;
		newVectorSep = arraySegmentation(LABt, segmentSizeProc);

		countBadPercentChip = 0;	// Скол
		countBadPercentCrack = 0;	// Трещина
		countBadPercentColor = 0;	// Цвет
		for (int i = 0; i < newVectorSep.size(); i++) {
			amountWS = amountWeightSegment(newVectorSep.at(i));
			for (int bi = 0; bi < 3; bi++) {
				badPercent[bi] = ((amountWS[bi] / WeightS[bi]) - 1) * 100;
			}
			if (badPercent[2] > 15.5) {
				badImg = drawBadSegment(src2, segmentSizeProc, i);
				countBadPercentColor++;
			}
			if (abs(badPercent[2]) > 11.5 && abs(badPercent[1]) > 6.5 && abs(badPercent[0]) > 6.5) {
				badImg = drawBadSegment(src2, segmentSizeProc, i);
				if (badPercent[2] < (-22.5)) countBadPercentCrack++;
				else countBadPercentChip++;
			}
		}

		imwrite("Produce/BAD/bad" + newNameFileIMG, badImg);

		src2.release();
		XYZt.release();
		LABt.release();
		badImg.release();
	}

	system("pause");
}
