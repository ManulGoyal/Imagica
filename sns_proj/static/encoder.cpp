// usage:   ./output infile outfile.imgc Q
//          Q = 1 to 100

#include "utils.hpp"
#include "huffmanCoding.hpp"

void compressImg(const Mat& img, string filename, int Q) {
    FILE* fp = fopen("temp.txt", "w");
    fprintf(fp, "%d %d %d ", img.rows, img.cols, Q);
    createDctMatrix();
    Mat ycbcr = convertToYCbCr(img);
    
    Mat padded = addPadding(ycbcr);
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
                // cout<<quantized;
                int q_block[N][N], zz_arr[N*N];
                convertMatToArr(quantized, q_block);
                block_to_zigzag(q_block, zz_arr);
                run_length_encode(zz_arr, fp);
                
            }
        }
    }
    fclose(fp);
    huffmanEncode("temp.txt", filename);
    
    remove("temp.txt");
}


int main(int argc, char* argv[]) {
   
    // Mat image(8, 8, CV_8UC3);
    // int aaa[8][8] = {{154, 123, 123, 123, 123, 123, 123, 136},
    //                 {192, 180, 136, 154, 154, 154, 136, 110},
    //                 {254, 198, 154, 154, 180, 154, 123, 123},
    //                 {239, 180, 136, 180, 180, 166, 123, 123},
    //                 {180, 154, 136, 167, 166, 149, 136, 136},
    //                 {128, 136, 123, 136, 154, 180, 198, 154},
    //                 {123, 105, 110, 149, 136, 136, 180, 166},
    //                 {110, 136, 123, 123, 123, 136, 154, 136}};
    // for (int i = 0; i < 8; i++)
    // {
    //     ptr(p, i, image, uint8);
    //     for (int j = 0, j1 = 0; j < 24; j+=3, j1++)
    //     {
    //         p[j] = p[j+1] = p[j+2] = aaa[i][j1];
    //     }
  
    // }
    // for
    string fname(argv[2]);
    Mat image = imread(argv[1] , IMREAD_COLOR);
    compressImg(image, fname, atoi(argv[3]));
    return 0;
}
