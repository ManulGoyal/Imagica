//usage: ./a.out img1 img2 output_file

#include "utils.hpp"

int main(int argc, char* argv[]) {
    Mat img1 = imread(argv[1], IMREAD_COLOR);
    Mat img2 = imread(argv[2], IMREAD_COLOR);

    double mse = calculateMSE(img1, img2);
    double psnr = 20*log10(255) - 10*log10(mse);

    FILE* fp = fopen(argv[3], "w");
    fprintf(fp, "%.3lf %.3lf", mse, psnr);
    fclose(fp);
}