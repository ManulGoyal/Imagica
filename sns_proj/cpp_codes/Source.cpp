#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
int reflect(int M, int x)
{
	if (x < 0)
	{
		return -x - 1;
	}
	if (x >= M)
	{
		return 2 * M - x - 1;
	}
	return x;
}
int circular(int M, int x)
{
	if (x < 0)
		return x + M;
	if (x >= M)
		return x - M;
	return x;
}
void noBorderProcessing(Mat src, Mat dst, float Kernel[][3])
{
	float sum;
	for (int y = 1; y < src.rows - 1; y++) {
		for (int x = 1; x < src.cols - 1; x++) {
			sum = 0.0;
			for (int k = -1; k <= 1; k++) {
				for (int j = -1; j <= 1; j++) {
					sum = sum + Kernel[j + 1][k + 1] * src.at<uchar>(y - j, x - k);
				}
			}
			dst.at<uchar>(y, x) = sum;
		}
	}
}
void refletedIndexing(Mat src, Mat dst, float Kernel[][3])
{
	float sum, x1, y1;
	for (int y = 0; y < src.rows; y++) {
		for (int x = 0; x < src.cols; x++) {
			sum = 0.0;
			for (int k = -1; k <= 1; k++) {
				for (int j = -1; j <= 1; j++) {
					x1 = reflect(src.cols, x - j);
					y1 = reflect(src.rows, y - k);
					sum = sum + Kernel[j + 1][k + 1] * src.at<uchar>(y1, x1);
				}
			}
			dst.at<uchar>(y, x) = sum;
		}
	}
}
void circularIndexing(Mat src, Mat dst, float Kernel[][3])
{
	float sum, x1, y1;
	for (int y = 0; y < src.rows; y++) {
		for (int x = 0; x < src.cols; x++) {
			sum = 0.0;
			for (int k = -1; k <= 1; k++) {
				for (int j = -1; j <= 1; j++) {
					x1 = circular(src.cols, x - j);
					y1 = circular(src.rows, y - k);
					sum = sum + Kernel[j + 1][k + 1] * src.at<uchar>(y1, x1);
				}
			}
			dst.at<uchar>(y, x) = sum;
		}
	}
}
int main()
{
	// Choice will be fed from user on the website....
	int choice;
	cin >> choice;
	Mat src, dst;
	// src = imread("lenna.jpg",ImreadModes::IMREAD_GRAYSCALE);
	src = imread("lenna.jpg",  IMREAD_COLOR);
	// imwrite("out.jpg", src);
	
	if (!src.data)
	{
		return -1;
	}
	dst = src.clone();
	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++)
		{
			dst.at<Vec3b>(y, x)[0] = 0.0;
			dst.at<Vec3b>(y, x)[1] = 0.0;
			dst.at<Vec3b>(y, x)[2] = 0.0;
		}
	if (choice == 1)
	{
		// Identical Image output...
		float Kernel[3][3] = {
						  {0, 0, 0},
						  {0, 1, 0},
						  {0, 0, 0}
		};
		circularIndexing(src, dst, Kernel);
	}
	else if (choice == 2)
	{
		// Edge detected Image...
		float Kernel[3][3] = {
						  {-1,-1,-1},
						  {-1, 8,-1},
						  {-1,-1,-1}
		};
		circularIndexing(src, dst, Kernel);
	}
	else if (choice == 3)
	{
		// Sharpened Image...
		float Kernel[3][3] = {
						  {0,-1,0},
						  {-1, 5,-1},
						  {0, -1,0}
		};
		circularIndexing(src, dst, Kernel);
	}
	else if (choice == 4)
	{
		// Embossing
		float Kernel[3][3] = {
						  {-2,-1,0},
						  {-1, 1,1},
						  {0, 1, 2}
		};
		circularIndexing(src, dst, Kernel);
	}
	else if (choice == 5)
	{
		// Box/Normal Blurring
		float Kernel[3][3] = {
							{1 / 9.0, 1 / 9.0, 1 / 9.0},
							{1 / 9.0, 1 / 9.0, 1 / 9.0},
							{1 / 9.0, 1 / 9.0, 1 / 9.0}
		};
		circularIndexing(src, dst, Kernel);
	}
	else if (choice == 6)
	{
		// User's Play time...
		float Kernel[3][3];
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				cin >> Kernel[i][j];
			}
		}
		circularIndexing(src, dst, Kernel);
	}
	imwrite("out.jpg", dst);
	// Boom!!!....Your wish has been granted...Image saved :)....
	return 0;
}