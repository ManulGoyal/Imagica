#include<opencv2/opencv.hpp>
#include<iostream>
typedef unsigned char uint8;
#define cptr(p, i, img, type) const type* p = (img).ptr<type>(i)
#define ptr(p, i, img, type) type* p = (img).ptr<type>(i)
using namespace std;
using namespace cv;

bool outOfBoundsX(Mat& img, int x) {
    return (x < 0 || x >= img.cols);
}

bool outOfBoundsY(Mat& img, int y) {
    return (y < 0 || y >= img.rows);
}

double interpolate(double x1, double x2, double dx) {
    return (1-dx)*x1 + dx*x2;
}

uint8 roundPixel255(double x) {
    int y = round(x);
    if(y > 255) return 255;
    if(y < 0) return 0;
    return (uint8)y;
}

Mat rotateImage(Mat& src, double phi) {         //phi in radian, positive for anticlockwise
    Mat dest(src.rows, src.cols, CV_8UC3, Scalar(0, 0, 0));
    int iCentreX = src.cols/2, iCentreY = src.rows/2;
    for (int i = 0; i < src.rows; i++)
    {
        // cptr(ps, i, src, uint8);
        // ptr(pd, i, dest, uint8);
        for (int j = 0; j < src.cols; j++)
        {
            int y = iCentreY - i;
            int x = j - iCentreX;
            double r = sqrt(x*x+y*y);
            double theta = 0.0;
            if(x == 0) {
                if(y == 0) {
                    dest.at<Vec3b>(i, j) = src.at<Vec3b>(i, j);
                    continue;
                } else if (y < 0) {
                    theta = -M_PI/2;
                } else {
                    theta = M_PI/2;
                }
            } else {
                theta = atan2(y, x);
            }
            theta -= phi;
            double trueX = r*cos(theta) + iCentreX;
            double trueY = iCentreY - r*sin(theta);
            int floorX = floor(trueX);
            int floorY = floor(trueY);
            int ceilX = ceil(trueX);
            int ceilY = ceil(trueY);
            if(outOfBoundsX(src, floorX) || outOfBoundsX(src, ceilX) || outOfBoundsY(src, floorY) || outOfBoundsY(src, ceilY)) continue;
            double deltaX = trueX - floorX;
            double deltaY = trueY - floorY;
            for (int l = 0; l < 3; l++)
            {
                double tl = src.at<Vec3b>(floorY, floorX)[l];
                double tr = src.at<Vec3b>(floorY, ceilX)[l];
                double bl = src.at<Vec3b>(ceilY, floorX)[l];
                double br = src.at<Vec3b>(ceilY, ceilX)[l];
                double top = interpolate(tl, tr, deltaX);
                double bottom = interpolate(bl, br, deltaX);
                double val = interpolate(top, bottom, deltaY);
                dest.at<Vec3b>(i, j)[l] = roundPixel255(val);
            }
            
        }
        
    }
    return dest;
    
}

int main(int argc, char* argv[]) {
    Mat img = imread(argv[1], IMREAD_COLOR);
    Mat rotated = rotateImage(img, atof(argv[3])*M_PI/180);
    imwrite(argv[2], rotated);
}
