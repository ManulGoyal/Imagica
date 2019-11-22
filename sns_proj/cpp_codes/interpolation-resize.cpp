// Image Resizing using Bicubic Interpolation
// usage:   ./output inpfile outfile x-scale-factor y-scale-factor mode
//          mode = 0 for bilinear and 1 for bicubic

// Reference: https://blog.demofox.org/2015/08/15/resizing-images-with-bicubic-interpolation/

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
double linear_interpolate(double x1, double x2, double dx) {
    return (1-dx)*x1 + dx*x2;
}
double Bicubic (double A, double B, double C, double D, double t) {
    double a = (-A / 2.0f) + ((3.0f*B) / 2.0f) - ((3.0f*C) / 2.0f) + (D / 2.0f);
    double b = A - ((5.0f*B) / 2.0f) + (2.0f*C) - (D / 2.0f);
    double c = (-A / 2.0f) + (C / 2.0f);
    double d = B; 
    return a*t*t*t + b*t*t + c*t + d;
}
uchar roundPixel255(double x) {
    int y = round(x);
    if(y > 255) return 255;
    if(y < 0) return 0;
    return (uchar)y;
}
Mat scale(const Mat M, double ratio1, double ratio2, int r1, int c1, int padding, int mode) {
    int row = long(round(double(r1)*ratio1));
    int col = long(round(double(c1)*ratio2));
    Mat m(row,col,CV_8UC3,Scalar(0,0,0));
    for(int c = 0; c < 3; c++) {
        for(int i = 0; i < row; i++) {
            for(int j = 0; j < col; j++) {
                double v = double(i)/double(ratio1);
                double u = double(j)/double(ratio2);
                double yfract = u - floor(u);
                double xfract = v - floor(v);
                if(mode == 1) {
                    int yint = round(u-0.5) + padding-1;
                    int xint = round(v-0.5) + padding-1;
                    
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
                } else {
                    int y1 = floor(u)+padding;
                    int y2 = ceil(u)+padding;
                    int x1 = floor(v)+padding;
                    int x2 = ceil(v)+padding;
                    
                    double p00 = M.at<Vec3b>(x1,y1)[c];
                    double p01 = M.at<Vec3b>(x1,y2)[c];
                    double p10 = M.at<Vec3b>(x2,y1)[c];
                    double p11 = M.at<Vec3b>(x2,y2)[c];

                    double col0 = linear_interpolate(p00, p01, xfract);
                    double col1 = linear_interpolate(p10, p11, xfract);
                    double val = linear_interpolate(col0, col1, yfract);

                    m.at<Vec3b>(i,j)[c] = roundPixel255(val);
                }
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
    double ratio1 = atof(argv[3]);
    double ratio2 = atof(argv[4]);
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
    Mat M1 = addpadding(M, padding = max(2.0,max(abs(M.rows*(ratio1-1)), abs(M.cols*(ratio2-1)))));
    Mat M2 = scale(M1,ratio1, ratio2,M.rows,M.cols,padding, atoi(argv[5]));
    imwrite(argv[2],M2);
    waitKey(0);
    return 0;
}
