#include "Header.h"

Mat RGBtoXYZ(Mat img)
{
    // Heigt/Width IMage
    int him = img.rows;
    int wim = img.cols;

    // New IMage
	Mat nim(him, wim, CV_16SC3);

    double vR, vG, vB;
    double X, Y, Z;

    for (int i = 0; i < him; i++) {
        for (int j = 0; j < wim; j++) {
            Vec3s intest = img.at<Vec3b>(i, j);

            vR = intest[2] / 255.0;
            vG = intest[1] / 255.0;
            vB = intest[0] / 255.0;


            if (vR > 0.04045) {
                vR = pow((vR + 0.055) / 1.055, 2.4);
            }
            else {
                vR = vR / 12.92;
            }

            if (vG > 0.04045) {
                vG = pow((vG + 0.055) / 1.055, 2.4);
            }
            else {
                vG = vG / 12.92;
            }

            if (vB > 0.04045) {
                vB = pow((vB + 0.055) / 1.055, 2.4);
            }
            else {
                vB = vB / 12.92;
            }

            vR = vR * 100.0;
            vG = vG * 100.0;
            vB = vB * 100.0;

            X = vR * 0.4124 + vG * 0.3576 + vB * 0.1805;
            Y = vR * 0.2126 + vG * 0.7152 + vB * 0.0722;
            Z = vR * 0.0193 + vG * 0.1192 + vB * 0.9505;

            intest[2] = X;
            intest[1] = Y;
            intest[0] = Z;

            nim.at<Vec3s>(i, j) = intest;
        }
    }

    return nim;
}


Mat XYZtoLab(Mat img)
{
    // Зависит от источника света, для D65 :
    double refX = 95.047;
    double refY = 100.000;
    double refZ = 108.883;

    // Heigt/Width IMage
    int him = img.rows;
    int wim = img.cols;

    // New IMage
    Mat nim(him, wim, CV_16SC3);

    double vX, vY, vZ;
    double cieL, ciea, cieb;

    for (int i = 0; i < him; i++) {
        for (int j = 0; j < wim; j++) {
            Vec3s intest = img.at<Vec3s>(i, j);

            vX = intest[2] / refX;
            vY = intest[1] / refY;
            vZ = intest[0] / refZ;

            if (vX > 0.008856) {
                vX = pow(vX, (1 / 3.0));
            }
            else {
                vX = (7.787 * vX) + (16.0 / 116.0);
            }

            if (vY > 0.008856) {
                vY = pow(vY, (1 / 3.0));
            }
            else {
                vY = (7.787 * vY) + (16.0 / 116.0);
            }

            if (vZ > 0.008856) {
                vZ = pow(vZ, (1 / 3.0));
            }
            else {
                vZ = (7.787 * vZ) + (16.0 / 116.0);
            }

            cieL = (116 * vY) - 16;
            ciea = 500 * (vX - vY);
            cieb = 200 * (vY - vZ);

            intest[2] = cieL;
            intest[1] = ciea;
            intest[0] = cieb;

            nim.at<Vec3s>(i, j) = intest;
        }
    }

    return nim;
}