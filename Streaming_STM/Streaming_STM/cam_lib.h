#pragma once
#include "CImg.h"
#include <vector>
#include <stdint.h>

// CAMERA MODES 
enum camera {
    NO_CAM = 0,         // no secondary camera
    LeptonFlir_BW,      // Lepton Flir black and white 
    LeptonFlir_C,       // Lepton Flir colored 
    HM01B0_QQVGA_BW,    // Himax QQVGA black and white
    HM01B0_QQVGA_C,     // Himax QQVGA colore (not implemented yet) 
    HM01B0_QVGA_BW,     // Himax QVGA black and white 
    TERMOSCAN,          // Thermal scanner -> fused image from STM 
    TERMOSCAN_HM01B0,   // Thermal scanner -> single Himax image 
    TERMOSCAN_Lepton    // Thermal scanner -> single Lepton image
};
int GetImageH(camera cam);
int GetImageW(camera cam);
int GetMatrixH(camera cam);
int GetMatrixW(camera cam);

class StreamingMode { 
public : 
    StreamingMode(camera cam);
    int ImageH();
    int ImageW(); 
    int MatrixH(); 
    int MatrixW(); 
    int SizeMatrix();
    uint8_t* ReadBuff(); 
    void Mult(int n); 
    int GetMult();
    unsigned int StreamW();
    unsigned int StreamH(); 
    int NumCam();
    const int Bytes2Read();
    const int Bytes2Read2(); 
    void PrintSpecs();
    camera GetCam(); 
    void IncrementFrame(); 
    int GetFrameN();
    void CreateBackground(); 
    void UpdateBackground(); 
    void SetAvg(bool v); 
    bool GetAvg(); 

    std::vector<uint8_t> UartPixelMatrix;
    std::vector<uint8_t> BackgroundImg;
    std::vector<uint8_t> BackgroundValue;
    std::vector<uint8_t> BackgroundSTD; 
    uint8_t* readBuff;

private:
    int imageH, imageW, matrixH, matrixW, sizeMatrix; 
    static int numCam;
    camera cam; 
    int mult = 1;
    const int numBystes2Read;
    int frameNumber; 
    unsigned int streamW, streamH;
    double alpha = 0.5; 
    bool avg = false; 
};

void PrintTitle();
void DisplayImage(StreamingMode str, std::vector<uint8_t>& UartPixelMatrix, cimg_library::CImg <unsigned char>* bg);
bool DetectMotion(std::vector<uint8_t>& BackgroundImg, std::vector<uint8_t>& UartPixelMatrix);
void DisplayImgTermoscanner(StreamingMode str1, std::vector<uint8_t>& UartPixelMatrix1,
                            StreamingMode str2, std::vector<uint8_t>& UartPixelMatrix2,
                            cimg_library::CImg <unsigned char>* bg);
uint8_t Average(std::vector<uint8_t>::iterator itr, int MatrixWidth, double c1, double c2);
uint8_t Average(std::vector<uint8_t>::iterator itr, int MatrixWidth, double c1, double c2, double c3);
uint8_t MedianFilter(std::vector<uint8_t>::iterator itr, int MatrixWidth);
/*
-------------UART--------------
*/
HANDLE init_UART();


