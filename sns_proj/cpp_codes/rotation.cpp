// usage:   ./output infile outfile degrees_to_rotate mode
//          mode = 0 bilinear and mode = 1 bicubic degrees are positive for anticlock

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

double linear_interpolate(double x1, double x2, double dx) {
    return (1-dx)*x1 + dx*x2;
}

double bicubic_interpolation (double A, double B, double C, double D, double t) {
    double a = (-A / 2.0f) + ((3.0f*B) / 2.0f) - ((3.0f*C) / 2.0f) + (D / 2.0f);
    double b = A - ((5.0f*B) / 2.0f) + (2.0f*C) - (D / 2.0f);
    double c = (-A / 2.0f) + (C / 2.0f);
    double d = B; 
    return a*t*t*t + b*t*t + c*t + d;
}

uint8 roundPixel255(double x) {
    int y = round(x);
    if(y > 255) return 255;
    if(y < 0) return 0;
    return (uint8)y;
}

Mat rotateImage(Mat& src, double phi, int mode) {         //phi in radian, positive for anticlockwise
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
            if(mode == 0 || outOfBoundsX(src, floorX-1) || outOfBoundsX(src, ceilX+1) || outOfBoundsY(src, floorY-1) || outOfBoundsY(src, ceilY+1)) {
                for (int l = 0; l < 3; l++)
                {
                    double tl = src.at<Vec3b>(floorY, floorX)[l];
                    double tr = src.at<Vec3b>(floorY, ceilX)[l];
                    double bl = src.at<Vec3b>(ceilY, floorX)[l];
                    double br = src.at<Vec3b>(ceilY, ceilX)[l];
                    double top = linear_interpolate(tl, tr, deltaX);
                    double bottom = linear_interpolate(bl, br, deltaX);
                    double val = linear_interpolate(top, bottom, deltaY);
                    dest.at<Vec3b>(i, j)[l] = roundPixel255(val);
                }
                // continue;
            } else {
                for (int l = 0; l < 3; l++)
                {
                    double p00 = src.at<Vec3b>(floorY-1, floorX-1)[l];
                    double p01 = src.at<Vec3b>(floorY-1, floorX)[l];
                    double p02 = src.at<Vec3b>(floorY-1, ceilX)[l];
                    double p03 = src.at<Vec3b>(floorY-1, ceilX+1)[l];

                    double p10 = src.at<Vec3b>(floorY, floorX-1)[l];
                    double p11 = src.at<Vec3b>(floorY, floorX)[l];
                    double p12 = src.at<Vec3b>(floorY, ceilX)[l];
                    double p13 = src.at<Vec3b>(floorY, ceilX+1)[l];

                    double p20 = src.at<Vec3b>(ceilY, floorX-1)[l];
                    double p21 = src.at<Vec3b>(ceilY, floorX)[l];
                    double p22 = src.at<Vec3b>(ceilY, ceilX)[l];
                    double p23 = src.at<Vec3b>(ceilY, ceilX+1)[l];
                    
                    double p30 = src.at<Vec3b>(ceilY+1, floorX-1)[l];
                    double p31 = src.at<Vec3b>(ceilY+1, floorX)[l];
                    double p32 = src.at<Vec3b>(ceilY+1, ceilX)[l];
                    double p33 = src.at<Vec3b>(ceilY+1, ceilX+1)[l];
                    
                    double y0 = bicubic_interpolation(p00, p01, p02, p03, deltaX);
                    double y1 = bicubic_interpolation(p10, p11, p12, p13, deltaX);
                    double y2 = bicubic_interpolation(p20, p21, p22, p23, deltaX);
                    double y3 = bicubic_interpolation(p30, p31, p32, p33, deltaX);

                    double val = bicubic_interpolation(y0, y1, y2, y3, deltaY);
                    dest.at<Vec3b>(i, j)[l] = roundPixel255(val);
                }
                
            }
            
        }
        
    }
    return dest;
    
}

int main(int argc, char* argv[]) {
    Mat img = imread(argv[1], IMREAD_COLOR);
    Mat rotated = rotateImage(img, atof(argv[3])*M_PI/180, atoi(argv[4]));
    imwrite(argv[2], rotated);
}
