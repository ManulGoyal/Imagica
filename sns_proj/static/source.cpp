#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#define cptr(p, i, img, type) const type* p = (img).ptr<type>(i)
#define ptr(p, i, img, type) type* p = (img).ptr<type>(i)
using namespace std;
using namespace cv;

int circular(int M, int x)
{
	if (x < 0)
		return x + M;
	if (x >= M)
		return x - M;
	return x;
}

uchar roundPixel255(float x) {
    int y = round(x);
    if(y > 255) return 255;
    if(y < 0) return 0;
    return (uchar)y;
}

void applyConvolution(Mat src, Mat& dst, float Kernel[][5], bool gray = false)
{
	float sum, x1, y1;
	for (int y = 0; y < src.rows; y++) {
        // cptr(p1, y, src, uchar);
        // ptr(p2, y, dst, uchar);
		for (int x = 0; x < src.cols; x++) {
            for(int l = 0; l < (gray ? 1 : 3); l++) {
                // if(l <= layer) {
				sum = 0.0;
				for (int k = -2; k <= 2; k++) {
					for (int j = -2; j <= 2; j++) {
						x1 = circular(src.cols, x - j);
						y1 = circular(src.rows, y - k);
						int val = gray ? src.at<uchar>(y1, x1) : src.at<Vec3b>(y1, x1)[l];
						sum = sum + Kernel[j + 2][k + 2] * val;
					}
				}
				if(gray) dst.at<uchar>(y, x) = roundPixel255(sum);
				else dst.at<Vec3b>(y, x)[l] = roundPixel255(sum);
                // } else {
                    // dst.at<Vec3b>(y, x)[l] = src.at<Vec3b>(y, x)[l];
                // }
            }
		}
	}
}

int main(int argc, char* argv[])
{
	// inpfile choice val[25]
	// if(argc != 4) return -1;
	// Choice will be fed from user on the website....
	int choice = atoi(argv[3]);
	Mat src;
	src = imread(argv[1],ImreadModes::IMREAD_COLOR);
	if (!src.data)
	{
		return -1;
	}
	Mat dst(src.rows, src.cols, CV_8UC3, Scalar(0, 0, 0));
	Mat dst_gray(src.rows, src.cols, CV_8UC1, Scalar(0, 0, 0));
	if (choice == 1)
	{
		// Box Blur...
		float Kernel[5][5] = {	{0, 0, 0, 0, 0},
						  		{0, 1/9.0, 1/9.0, 1/9.0, 0},
						  		{0, 1/9.0, 1/9.0, 1/9.0, 0},
						  		{0, 1/9.0, 1/9.0, 1/9.0, 0},
						  		{0, 0, 0, 0, 0}};
		applyConvolution(src, dst, Kernel);
	}
	else if (choice == 2)
	{
		Mat gray_img;
		cvtColor(src, gray_img, COLOR_BGR2GRAY);
		// Edge detected Image...
		float Kernel[5][5] = {	{0, 0, 0, 0, 0},
						  		{0, -1,-1,-1, 0},
						  		{0, -1, 8,-1, 0},
						  		{0, -1,-1,-1, 0},
								{0, 0, 0, 0, 0}};
		applyConvolution(gray_img, dst_gray, Kernel, true);
	}
	else if (choice == 3)
	{
		// Sharpened Image...
		float Kernel[5][5] = {	{0, 0, 0, 0, 0},
						  		{0, 0,-1, 0, 0},
						  		{0,-1, 5,-1, 0},
						  		{0, 0,-1, 0, 0},
								{0, 0, 0, 0, 0}};
		applyConvolution(src, dst, Kernel);
	}
	else if (choice == 4)
	{
		// Embossing
		float Kernel[5][5] = {	{0, 0, 0, 0, 0},
						  		{0,-2,-1, 0, 0},
						  		{0,-1, 1, 1, 0},
						  		{0, 0, 1, 2, 0},
								{0, 0, 0, 0, 0}};
		applyConvolution(src, dst, Kernel);
	}
	else if (choice == 5)
	{
        float line[5] = {1, 4, 6, 4, 1};
		// Gaussian Blurring...
		float Kernel[5][5];
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                Kernel[i][j] = line[i]*line[j]/256.0;
            }
            
        }
        
		applyConvolution(src, dst, Kernel);
	}
	else if (choice == 6)
	{
		// User's Play time...
		float Kernel[5][5];
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				Kernel[i][j] = atof(argv[5*i+j+3]);
			}
		}
		applyConvolution(src, dst, Kernel);
	} 
	if(choice == 2) imwrite(argv[2], dst_gray);
	else imwrite(argv[2], dst);
	// Boom!!!....Your wish has been granted...Image saved :)....
	return 0;
}
