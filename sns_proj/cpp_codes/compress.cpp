// usage:   ./output infile outfile Q
//          Q = 1 to 100

#include<opencv2/opencv.hpp>
#include<iostream>
typedef unsigned char uint8;
#define cptr(p, i, img, type) const type* p = (img).ptr<type>(i)
#define ptr(p, i, img, type) type* p = (img).ptr<type>(i)
using namespace std;
using namespace cv;
#define N 8
#define LUM 0
#define CHROM 1

const double PI = M_PI;
Mat dctMatrix(N, N, CV_64FC1);
Mat dctMatrix_t(N, N, CV_64FC1);
uint8 baseQuantMatY[N][N] ={{16,    11,    10,    16,    24,    40,    51,    61},
                            {12,    12,    14,    19,    26,    58,    60,    55},
                            {14,    13,    16,    24,    40,    57,    69,    56},
                            {14,    17,    22,    29,    51,    87,    80,    62},
                            {18,    22,    37,    56,    68,   109,   103,    77},
                            {24,    35,    55,    64,    81,   104,   113,    92},
                            {49,    64,    78,    87,   103,   121,   120,   101},
                            {72,    92,    95,    98,   112,   100,   103,    99}};
uint8 baseQuantMatC[N][N] ={{17, 18, 24, 47, 99, 99, 99, 99},
                            {18, 21, 26, 66, 99, 99, 99, 99},
                            {24, 26, 56, 99, 99, 99, 99, 99},
                            {47, 66, 99, 99, 99, 99, 99, 99},
                            {99, 99, 99, 99, 99, 99, 99, 99},
                            {99, 99, 99, 99, 99, 99, 99, 99},
                            {99, 99, 99, 99, 99, 99, 99, 99},
                            {99, 99, 99, 99, 99, 99, 99, 99}};

Mat divideMat(const Mat& m1, uint8 m2[N][N]) {
    Mat m(N, N, CV_16SC1);
    for (int i = 0; i < N; i++)
    {
        cptr(p1, i, m1, double);
        // cptr(p2, i, m2, uint8);
        ptr(p, i, m, short);
        for (int j = 0; j < N; j++)
        {
            p[j] = round(p1[j]/m2[i][j]);
        }
    }
    return m;
}

Mat multiplyMat(const Mat& m1, uint8 m2[N][N]) {
    Mat m(N, N, CV_64FC1);
    for (int i = 0; i < N; i++)
    {
        cptr(p1, i, m1, short);
        // cptr(p2, i, m2, uint8);
        ptr(p, i, m, double);
        for (int j = 0; j < N; j++)
        {
            p[j] = p1[j]*m2[i][j];
        }
    }
    return m;
}

Mat splitMat(const Mat& m1, int offset) {
    Mat m2(m1.rows, m1.cols, CV_8UC1);
    for (int i = 0; i < m1.rows; i++)
    {
        cptr(p1, i, m1, uint8);
        ptr(p2, i, m2, uint8);
        for (int j = 0, k = 0; k < m1.cols; j+=3, k++)
        {
            p2[k] = p1[j+offset];
        }
        
    }
    return m2;
}

void printDoubleMat(const Mat& mat) {
    for(int i = 0; i< mat.rows; i++) {
        cptr(p, i, mat, double);
        for (int j = 0; j < mat.cols; j++)
        {
            printf("%.4f ", p[j]);
        }
        cout<<endl;
    }
}

uint8 roundPixel255(double x) {
    int y = round(x);
    if(y > 255) return 255;
    if(y < 0) return 0;
    return (uint8)y;
}

int roundPixel127(double x) {
    int y = round(x);
    if(y > 127) return 127;
    if(y < -128) return -128;
    return y;
}

Mat convertToYCbCr(const Mat& img) {
    Mat m(img.rows, img.cols, CV_8UC3);
    for(int i = 0;i < img.rows;i++){
        const uint8* img_p = img.ptr<uint8>(i);
        uint8* m_p = m.ptr<uint8>(i);
        for(int j = 0;j < 3*img.cols;j+=3){
            m_p[j] = roundPixel255(0.299*img_p[j+2]+0.587*img_p[j+1]+0.114*img_p[j]);
            m_p[j+1] = roundPixel255(128-0.168736*img_p[j+2]-0.331264*img_p[j+1]+0.5*img_p[j]);
            m_p[j+2] = roundPixel255(128+0.5*img_p[j+2]-0.418688*img_p[j+1]-0.081312*img_p[j]); 
        }
    } 
    return m; 
}

Mat convertToRGB(const Mat& img) {
    Mat m(img.rows, img.cols, CV_8UC3);
    for(int i = 0;i < img.rows;i++){
        const uint8* img_p = img.ptr<uint8>(i);
        uint8* m_p = m.ptr<uint8>(i);
        for(int j = 0;j < 3*img.cols;j+=3){
            m_p[j] = roundPixel255(img_p[j]+1.772*(img_p[j+1]-128));
            m_p[j+1] = roundPixel255(img_p[j]-0.344136*(img_p[j+1]-128)-0.714136*(img_p[j+2]-128));
            m_p[j+2] = roundPixel255(img_p[j]+1.402*(img_p[j+2]-128)); 
        }
    } 
    return m; 
}

void createDctMatrix() {           // N is the block size
    double inv_sqrt = 1/sqrt((double)N);
    for (int i = 0; i < N; i++)
    {
        double* p = (dctMatrix).ptr<double>(i);
        for (int j = 0; j < N; j++)
        {
            if(i == 0) p[j] = inv_sqrt;
            else p[j] = sqrt(2)*inv_sqrt*cos(((2*j+1)*i*PI)/(2*N));
        }
 
    }
    dctMatrix_t = dctMatrix.t();
}

void getQuantizationMatrix(uint8 qmaty[N][N], int Q, int type) {
    double fact;
    if(Q > 50) fact = (100-Q)/50.0;
    else fact = 50.0/Q;
    for (int i = 0; i < N; i++)
    {
        uint8* p;
        if(type == LUM) p = baseQuantMatY[i];
        else p = baseQuantMatC[i];
        for (int j = 0; j < N; j++)
        {
            int curr = round(p[j]*fact);
            if(curr < 1) qmaty[i][j] = 1;
            else if(curr > 255) qmaty[i][j] = 255;
            else qmaty[i][j] = curr;
        }
        
    }
    
}

Mat addPadding(const Mat& img) {
    int w = N*ceil(img.cols/(double)N), h = N*ceil(img.rows/(double)N);
    Mat mat(h, w, CV_8UC3, Scalar(0, 0, 0));
    
    for (int i = 0; i < img.rows; i++)
    {
        cptr(p1, i, img, uint8);
        ptr(p2, i, mat, uint8);
        for (int j = 0; j < 3*img.cols; j += 3)
        {
            p2[j] = p1[j]; p2[j+1] = p1[j+1]; p2[j+2] = p1[j+2];
        }
        Vec3b pixel = img.at<Vec3b>(i, img.cols-1);
        for(int j = img.cols; j < w; j++) {
            mat.at<Vec3b>(i, j) = pixel;
        }
        
    }
    for (int i = 0; i < w; i++)
    {
        Vec3b pixel = img.at<Vec3b>(img.rows-1, i);
        for (int j = img.rows; j < h; j++)
        {
            mat.at<Vec3b>(j, i) = pixel;
        }
        
    }
    
    return mat;
}

Mat getBlock(const Mat& img, int x, int y, int l) {
    Mat block(N, N, CV_64FC1);
    for (int i = 0; i < N; i++)
    {
        cptr(p1, x+i, img, uint8);
        ptr(p2, i, block, double);
        for (int j = 0, k = 0; j < N; j++, k += 3)
        { 
            // for (int k = 0; k < 3; k++) p2[j+k] = p1[y+j+k];
            p2[j] = p1[y+k+l];
        }
        
    }
    return block;
}

Mat getDct(const Mat& img) {
    return dctMatrix*img*dctMatrix_t;
}

Mat getIDct(const Mat& img) {
    return dctMatrix_t*img*dctMatrix;
}

Mat compressImg(const Mat& img, int Q=50) {
    createDctMatrix();
    Mat ycbcr = convertToYCbCr(img);
    Mat compressed(img.rows, img.cols, CV_8UC3);
    
    Mat padded = addPadding(ycbcr);
    Mat temp(padded.rows, padded.cols, CV_16SC3); 
    // cout << padded;
    uint8 qmaty[N][N];
    uint8 qmatc[N][N];
    getQuantizationMatrix(qmaty, Q, LUM);
    getQuantizationMatrix(qmatc, Q, CHROM);
    
    for (int i = 0; i < padded.rows; i+=N)
    {
        for (int j = 0, j2 = 0; j < padded.cols; j+=N, j2 += 3*N)
        {
            for (int l = 0; l < 3; l++)
            {
                Mat block = getBlock(padded, i, j2, l)-128;
                Mat fdct = getDct(block);
                Mat quantized = (l == 0) ? divideMat(fdct, qmaty) : divideMat(fdct, qmatc);
                Mat dequantized = multiplyMat(quantized, qmaty);
                Mat idct = getIDct(dequantized);

                for (int m = 0; m < N; m++)
                {
                    if(i+m >= img.rows) break;
                    cptr(ip, m, idct, double);
                    ptr(op, i+m, compressed, uint8);
                    for (int n1 = 0, n2 = 0; n1 < N; n1++, n2+=3)
                    {
                        if(j+n1 >= img.cols) break;
                        op[j2+n2+l] = roundPixel127(ip[n1]) + 128;
                    }
                    
                }
                
            }
        }
    }

    return convertToRGB(compressed);
}

double calculateMSE(const Mat& m1, const Mat& m2) {
    double mse = 0;
    for (int i = 0; i < m1.rows; i++)
    {
        cptr(p1, i, m1, uint8);
        cptr(p2, i, m2, uint8);
        for (int j = 0; j < 3*m1.cols; j+=3)
        {
            for (int l = 0; l < 3; l++)
            {
                mse += (p1[j+l]-p2[j+l])*(p1[j+l]-p2[j+l]);
            }
            
        }
        
    }
    mse = mse / (3*m1.rows*m1.cols);
    return mse;
}

int main(int argc, char* argv[]) {
   
    Mat image;
    image = imread(argv[1] , IMREAD_COLOR);
    Mat compressed = compressImg(image, atoi(argv[3]));

    std::vector<int> compression_params;
    compression_params.push_back(IMWRITE_JPEG_QUALITY);
    compression_params.push_back(100);
    imwrite(argv[2], compressed, compression_params);

    double mse = calculateMSE(image, compressed);
    double psnr = 20*log10(255) - 10*log10(mse);

    FILE* fp = fopen("psnr.txt", "w");
    fprintf(fp, "%.3lf %.3lf", mse, psnr);
    fclose(fp);

    return 0;
}
