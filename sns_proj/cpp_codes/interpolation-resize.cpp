//Image Resizing using Bicubic Interpolation
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
using namespace cv;
using namespace std;
#define roundPixel(v, min, max) if (v < min) { v = min; } else if (v > max) { v = max; }
Mat addpadding(const Mat m1, int border) {
  Mat m(m1.rows + 2*border, m1.cols + 2*border, CV_8UC3, Scalar(0,0,0));
  copyMakeBorder(m1, m, border, border,border, border, BORDER_REPLICATE);
  return m;
}
double Bicubic (double A, double B, double C, double D, double t) {
    double a = (-A / 2.0f) + ((3.0f*B) / 2.0f) - ((3.0f*C) / 2.0f) + (D / 2.0f);
    double b = A - ((5.0f*B) / 2.0f) + (2.0f*C) - (D / 2.0f);
    double c = (-A / 2.0f) + (C / 2.0f);
    double d = B; 
    return a*t*t*t + b*t*t + c*t + d;
}
Mat scale(const Mat M, double ratio, int r1, int c1, int padding) {
    int row = long(round(double(r1)*ratio));
    int col = long(round(double(c1)*ratio));
    Mat m(row,col,CV_8UC3,Scalar(0,0,0));
    for(int c = 0; c < 3; c++) {
        for(int i = 0; i < row; i++) {
            double v = double(i)/double(ratio);
            for(int j = 0; j < col; j++) {
                double u = double(j)/double(ratio);
                int yint = round(u-0.5) + padding-1;
                double yfract = u - floor(u);
                int xint = round(v-0.5) + padding-1;
                double xfract = v - floor(v);
                auto p00 = (double)M.at<Vec3b>(xint-1,yint-1)[c];
                auto p10 = (double)M.at<Vec3b>(xint+0,yint-1)[c];
                auto p20 = (double)M.at<Vec3b>(xint+1,yint-1)[c];
                auto p30 = (double)M.at<Vec3b>(xint+2,yint-1)[c];

                auto p01 = (double)M.at<Vec3b>(xint-1,yint+0)[c];
                auto p11 = (double)M.at<Vec3b>(xint+0,yint+0)[c];
                auto p21 = (double)M.at<Vec3b>(xint+1,yint+0)[c];
                auto p31 = (double)M.at<Vec3b>(xint+2,yint+0)[c];

                auto p02 = (double)M.at<Vec3b>(xint-1,yint+1)[c];
                auto p12 = (double)M.at<Vec3b>(xint+0,yint+1)[c];
                auto p22 = (double)M.at<Vec3b>(xint+1,yint+1)[c];
                auto p32 = (double)M.at<Vec3b>(xint+2,yint+1)[c];

                auto p03 = (double)M.at<Vec3b>(xint-1,yint+2)[c];
                auto p13 = (double)M.at<Vec3b>(xint+0,yint+2)[c];
                auto p23 = (double)M.at<Vec3b>(xint+1,yint+2)[c];
                auto p33 = (double)M.at<Vec3b>(xint+2,yint+2)[c];

                double col0 = Bicubic(p00, p10, p20, p30, xfract);
                double col1 = Bicubic(p01, p11, p21, p31, xfract);
                double col2 = Bicubic(p02, p12, p22, p32, xfract);
                double col3 = Bicubic(p03, p13, p23, p33, xfract);
                double value = Bicubic(col0, col1, col2, col3, yfract);
                roundPixel(value,0.0f,255.0f);
                if(abs(value-0.0) < 0.001 || abs(value-255.0) < 0.001)
                    value = M.at<Vec3b>(xint,yint)[c];
                m.at<Vec3b>(i,j)[c] = round(value);
            }
        }
    }
  return m;
}
int main( int argc, char** argv ){
    String imageName( argv[1] ); 
    if( argc > 1){
        imageName = argv[1];
    }
    double ratio = (argv[2][0] - '0') + (argv[2][1] - '0')/10.0;
    Mat image;
    image = imread( imageName, IMREAD_COLOR ); 
    if( image.empty() ) {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    Mat M = image;
    int size = M.rows;
    int c = 3, N = 3;
    int padding = 2;
    Mat M1 = addpadding(M, padding = max(2.0,abs(M.rows*(ratio-1))));
    Mat M2 = scale(M1,ratio,M.rows,M.cols,padding);
    imwrite("output.jpg",M2);
    waitKey(0);
    return 0;
}
