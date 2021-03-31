#pragma once

#include <vector>
#include <stdint.h>
#include <windows.h>
#include "CImg.h"
#include "streaming_mode_class.h"




// --- CORE FUNCTIONS 

void PrintTitle();
void DisplayImage(StreamingMode str, std::vector<uint8_t>& UartPixelMatrix, cimg_library::CImg <unsigned char>* bg);
void DisplayImageTermoscanner(StreamingMode str1, std::vector<uint8_t>& UartPixelMatrix1,
    StreamingMode str2, std::vector<uint8_t>& UartPixelMatrix2,
    cimg_library::CImg <unsigned char>* bg);
void DisplayImage2(StreamingMode str, std::vector<uint8_t>& UartPixelMatrix, cimg_library::CImg <unsigned char>* bg,
    StreamingMode str2, std::vector<uint8_t>& UartPixelMatrix2, cimg_library::CImg <unsigned char>* bg2,
    cimg_library::CImg <unsigned char>* bg3);
uint8_t Average(std::vector<uint8_t>::iterator itr, int MatrixWidth, double c1, double c2);
uint8_t Average(std::vector<uint8_t>::iterator itr, int MatrixWidth, double c1, double c2, double c3);
uint8_t MedianFilter(std::vector<uint8_t>::iterator itr, int MatrixWidth);
void ReadCamData(StreamingMode& stream, HANDLE hSerial, int* frameNumber, DWORD dwBytesRead);
void DisplayImageGrayScale(StreamingMode str, std::vector<uint8_t>& UartPixelMatrix, cimg_library::CImg <unsigned char>* bg);
void DisplayImageRGB(StreamingMode str, std::vector<uint8_t>& UartPixelMatrix, cimg_library::CImg <unsigned char>* bg);
void StoreFrame(StreamingMode str, std::vector<uint8_t>& UartPixelMatrix );
int ReadNumberOfStoredFrames(); 
void UpdateNumberOfStoredFrames(int* n);
void ResetNumberOfStoredFrames();

HANDLE init_UART();
