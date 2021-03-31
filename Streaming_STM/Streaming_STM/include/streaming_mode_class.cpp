#include <iostream>
#include <stdint.h>
#include <iomanip>
#include <windows.h>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#include "streaming_mode_class.h"
#include "core_func.h"



int GetImageH(camera mode) {
	switch (mode) {
	case LeptonFlir_BW:		return 60;  // Lepton BW
	case LeptonFlir_C:		return 60;  // Lepton C
	case HM01B0_QQVGA_BW:	return 122; // HM01B0 BW 
	case HM01B0_QQVGA_C:	return 122; // HM01B0 C
	case HM01B0_QVGA_BW:	return 244; // HM01B0 BW QVGA
	case TERMOSCAN_HM01B0:	return 122; // Termoscan 1 
	case TERMOSCAN_Lepton:	return 0;   // Termoscan 2 
	default: return 0;
	}
}
int GetImageW(camera mode) {
	switch (mode) {
	case LeptonFlir_BW:		return 80;  // Lepton BW
	case LeptonFlir_C:		return 80;  // Lepton C
	case HM01B0_QQVGA_BW:	return 164; // HM01B0 BW 
	case HM01B0_QQVGA_C:	return 164; // HM01B0 C
	case HM01B0_QVGA_BW:	return 326; // HM01B0 BW QVGA
	case TERMOSCAN_HM01B0:	return 164; // Termoscan 1 
	case TERMOSCAN_Lepton:	return 0;   // Termoscan 2 
	default: return 0;
	}
}
int GetMatrixH(camera mode) {
	switch (mode) {
	case LeptonFlir_BW:		return 60;  // Lepton BW
	case LeptonFlir_C:		return 60;  // Lepton C
	case HM01B0_QQVGA_BW:	return 122; // HM01B0 BW 
	case HM01B0_QQVGA_C:	return 122; // HM01B0 C
	case HM01B0_QVGA_BW:	return 244; // HM01B0 BW QVGA
	case TERMOSCAN_HM01B0:	return 122; // Termoscan 1 
	case TERMOSCAN_Lepton:	return 60;  // Termoscan 2 
	default: return 0;
	}
}
int GetMatrixW(camera mode) {
	switch (mode) {
	case LeptonFlir_BW:		return 80;		// Lepton BW
	case LeptonFlir_C:		return 80 * 3;  // Lepton C
	case HM01B0_QQVGA_BW:	return 164;		// HM01B0 BW 
	case HM01B0_QQVGA_C:	return 164;		// HM01B0 C
	case HM01B0_QVGA_BW:	return 326;		// HM01B0 BW QVGA
	case TERMOSCAN_HM01B0:	return 164;		// Termoscan 1 
	case TERMOSCAN_Lepton:	return 80 * 3;	// Termoscan 2 
	default: return 0;
	}
}
uint8_t* GetReadBuff(camera mode) {
	if (mode == LeptonFlir_BW) {
		static uint8_t RB[80 + 1] = { 0 };
		return RB;
	}
	else if (mode == LeptonFlir_C) {
		static uint8_t RB[80 * 30 + 1] = { 0 };
		return RB;
	}
	else if (mode == HM01B0_QQVGA_BW) {
		static uint8_t RB[164 + 1] = { 0 };
		return RB;
	}
	else if (mode == HM01B0_QQVGA_C) {
		static uint8_t RB[164 + 1] = { 0 };
		return RB;
	}
	else if (mode == HM01B0_QVGA_BW) {
		static uint8_t RB[326 + 1] = { 0 };
		return RB;
	}
	else if (mode == TERMOSCAN_HM01B0) {
		static uint8_t RB[164 + 1] = { 0 };
		return RB;
	}
	else if (mode == TERMOSCAN_Lepton) {
		static uint8_t RB[80 * 3 + 1] = { 0 };
		return RB;
	}
	else {

	}
}

/*
----------- STREAMING CLASS FUNCTIONS  -------------------
*/

StreamingMode::StreamingMode(camera _cam)
	:numBystes2Read((const int)GetMatrixW(_cam))
{
	cam = _cam;
	imageH = GetImageH(_cam);
	imageW = GetImageW(_cam);
	matrixH = GetMatrixH(_cam);
	matrixW = GetMatrixW(_cam);

	sizeMatrix = matrixH * matrixW;
	UartPixelMatrix.resize(sizeMatrix);


	streamW = mult * imageW;
	streamH = mult * imageH;
	if (_cam != 0 && (_cam != TERMOSCAN_Lepton))numCam++;

	readBuff = GetReadBuff(_cam);
	if (_cam != 0)	PrintSpecs();

}
int StreamingMode::ImageH(void) { return imageH; }
int StreamingMode::ImageW(void) { return imageW; }
int StreamingMode::MatrixH(void) { return matrixH; }
int StreamingMode::MatrixW(void) { return matrixW; }
int StreamingMode::SizeMatrix(void) { return sizeMatrix; }
uint8_t* StreamingMode::ReadBuff(void) { return readBuff; }
unsigned int StreamingMode::StreamW() { return streamW; }
unsigned int StreamingMode::StreamH() { return streamH; }
const int StreamingMode::Bytes2Read() { return numBystes2Read; }
int StreamingMode::NumCam() { return numCam; }
int StreamingMode::GetMult() { return mult; }
camera StreamingMode::GetCam() { return cam; }
void StreamingMode::SetAvg(bool v) { avg = v; }
bool StreamingMode::GetAvg() { return avg; }
void StreamingMode::Mult(int n) {
	mult = n;
	streamW = n * imageW;
	streamH = n * imageH;
}
void StreamingMode::SetSaveFrame(bool a) { saveFrame = a; }
bool StreamingMode::GetSaveFrame() { return saveFrame; }