#pragma once

#include <vector>
#include <stdint.h>
#include "CImg.h"
//#include "core_func.h"
// --- CAMERA MODES 
enum camera {
    NO_CAM = 0,         // no secondary camera
    LeptonFlir_BW,      // Lepton Flir black and white 
    LeptonFlir_C,       // Lepton Flir colored 
    HM01B0_QQVGA_BW,    // Himax QQVGA black and white
    HM01B0_QQVGA_C,     // Himax QQVGA colore (not implemented yet) 
    HM01B0_QVGA_BW,     // Himax QVGA black and white 
    TERMOSCAN_HM01B0,   // Thermal scanner -> single Himax image 
    TERMOSCAN_Lepton    // Thermal scanner -> single Lepton image
};
int GetImageH(camera cam);
int GetImageW(camera cam);
int GetMatrixH(camera cam);
int GetMatrixW(camera cam);
uint8_t* GetReadBuff(camera mode);

// --- STREAMINGMODE CLASS 
class StreamingMode {
public:
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
    void PrintSpecs();
    camera GetCam();
    void SetAvg(bool v);
    bool GetAvg();
    bool GetSaveFrame();
    void SetSaveFrame(bool a);

    std::vector<uint8_t> UartPixelMatrix;
    uint8_t* readBuff;

private:
    int imageH, imageW, matrixH, matrixW, sizeMatrix;
    static int numCam;
    camera cam;
    int mult = 1;
    const int numBystes2Read;
    unsigned int streamW, streamH;
    double alpha = 0.5;
    bool avg = false;
    bool saveFrame = false;
};

