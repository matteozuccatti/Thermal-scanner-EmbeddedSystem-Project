#include <iostream>
#include <stdint.h>
#include <iomanip>
#include <windows.h>
#include <string>
#include <vector>
#include "cam_lib.h"
#include <cmath>
#include <algorithm>

/*
----------- AUXILIARY FUNCTIONS -------------------
*/

int GetImageH(camera mode) {
	switch (mode) {
	case LeptonFlir_BW:		return 60;  // Lepton BW
	case LeptonFlir_C:		return 60;  // Lepton C
	case HM01B0_QQVGA_BW:	return 122;  // HM01B0 BW 
	case HM01B0_QQVGA_C:	return 122;  // HM01B0 C
	case HM01B0_QVGA_BW:	return 244;  // HM01B0 BW QVGA
	case TERMOSCAN:			return 122;  // TERMOSCAN
	case TERMOSCAN_HM01B0:	return 122;  // Termoscan 1 
	case TERMOSCAN_Lepton:	return 0;   // Termoscan 2 
	default: return 0;
	}
}
int GetImageW(camera mode) {
	switch (mode) {
	case LeptonFlir_BW:		return 80;  // Lepton BW
	case LeptonFlir_C:		return 80;  // Lepton C
	case HM01B0_QQVGA_BW:	return 164;  // HM01B0 BW 
	case HM01B0_QQVGA_C:	return 164;  // HM01B0 C
	case HM01B0_QVGA_BW:	return 326;  // HM01B0 BW QVGA
	case TERMOSCAN:			return 164;  // TERMOSCAN
	case TERMOSCAN_HM01B0:	return 164;  // Termoscan 1 
	case TERMOSCAN_Lepton:	return 0;   // Termoscan 2 
	default: return 0;
	}
}
int GetMatrixH(camera mode) {
	switch (mode) {
	case LeptonFlir_BW:		return 60;  // Lepton BW
	case LeptonFlir_C:		return 60;  // Lepton C
	case HM01B0_QQVGA_BW:	return 122;  // HM01B0 BW 
	case HM01B0_QQVGA_C:	return 122;  // HM01B0 C
	case HM01B0_QVGA_BW:	return 244;  // HM01B0 BW QVGA
	case TERMOSCAN:			return 122;  // TERMOSCAN
	case TERMOSCAN_HM01B0:	return 122;  // Termoscan 1 
	case TERMOSCAN_Lepton:	return 60;   // Termoscan 2 
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
	case TERMOSCAN:			return 164*3;	// TERMOSCAN
	case TERMOSCAN_HM01B0:	return 164;		// Termoscan 1 
	case TERMOSCAN_Lepton:	return 80*3;	// Termoscan 2 
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
	else if (mode == TERMOSCAN) {
		static uint8_t RB[164*3 +1 ] = { 0 };
		return RB;
	}
	else if (mode == TERMOSCAN_HM01B0) {
		static uint8_t RB[164 + 1] = { 0 };
		return RB;
	}
	else if (mode == TERMOSCAN_Lepton) {
		static uint8_t RB[80*3 + 1] = { 0 };
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
	BackgroundImg.resize(sizeMatrix);
	BackgroundValue.resize(sizeMatrix);
	BackgroundSTD.resize(sizeMatrix); 

	streamW = mult * imageW;
	streamH = mult * imageH;
	if(_cam!= 0 && (_cam!=TERMOSCAN_Lepton))numCam++;
	if (_cam != 0) frameNumber = 0; 

	readBuff = GetReadBuff(_cam); 
	if(_cam!=0)	PrintSpecs(); 

	
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
void StreamingMode::IncrementFrame(){ frameNumber++; }
int StreamingMode::GetFrameN() { return frameNumber; }
void StreamingMode::CreateBackground() { BackgroundImg = UartPixelMatrix; }
void StreamingMode::SetAvg(bool v) { avg = v; }
bool StreamingMode::GetAvg() { return avg; }
void StreamingMode::Mult(int n) { 
	mult = n;
	streamW = n * imageW; 
	streamH = n * imageH; 
}
void StreamingMode::UpdateBackground() {
	std::vector<uint8_t>::iterator itrP = UartPixelMatrix.begin();
	std::vector<uint8_t>::iterator itrB = BackgroundImg.begin();
	//std::vector<uint8_t>::iterator itrV = BackgroundValue.begin();
	//std::vector<uint8_t>::iterator itrS = BackgroundSTD.begin(); 
	bool there_is_black = false; 
	for (itrB; itrB < BackgroundImg.end(); itrP++, itrB++) {
		*itrB = (int)(alpha * (*itrP) + (double)(1 - alpha) * (*itrB)); 
		//B_t = alpha * I_t     + (1 - alpha) * B_(t-1); 
		if (*itrB == 0) there_is_black = true; 
	}
	//if (there_is_black) std::cout << "New background with some black\n";

}

void DisplayImage(StreamingMode str, std::vector<uint8_t>& UartPixelMatrix,  cimg_library::CImg <unsigned char>* bg) {

	// GRAYSCALE SINGLE CAMERA 
	if (str.GetCam() == LeptonFlir_BW || str.GetCam() == HM01B0_QQVGA_BW || str.GetCam() == HM01B0_QVGA_BW) {
		std::vector<uint8_t>::iterator itr;
		uint8_t r, g, b;
		int Index = 0;
		int MatrixWidth =str.MatrixW();
		for (itr = UartPixelMatrix.begin(); itr < UartPixelMatrix.end(); itr++, Index++) {
			r = *itr;
			g = *itr;
			b = *itr;
			const unsigned char colorPixel[] = { r, g, b };
			int row = (int)Index / MatrixWidth;
			int col = (int)Index % MatrixWidth;
			*bg->draw_rectangle(str.GetMult() * (col + 1), str.GetMult() * (row + 1), 
				                str.GetMult() * col,       str.GetMult() * row, 
				                colorPixel);
		}
	}

	// LEPTON FLIR COLOR 
	if (str.GetCam() == LeptonFlir_C) {
		std::vector<uint8_t>::iterator itr;
		uint8_t r, g, b;
		int Index = 0;
		int MatrixWidth = str.MatrixW();
		for (itr = UartPixelMatrix.begin(); itr < UartPixelMatrix.end(); itr += 3, Index += 3) {
			r = *itr;
			g = *(itr + 1);
			b = *(itr + 2);
			const unsigned char colorPixel[] = { r, g, b };
			int row = (int)Index / MatrixWidth;
			int col = (int)((int)Index % MatrixWidth) / 3;
			*bg->draw_rectangle(str.GetMult() * (col + 1), str.GetMult() * (row + 1),
				str.GetMult() * col, str.GetMult() * row,
				colorPixel);
		}
	}

	// TERMOSCAN 
	if (str.GetCam() == TERMOSCAN) {
		std::vector<uint8_t>::iterator itr;
		uint8_t r, g, b;
		int Index = 0;
		int MatrixWidth = str.MatrixW();
		for (itr = UartPixelMatrix.begin(); itr < UartPixelMatrix.end(); itr += 3, Index += 3) {
			int row = (int)Index / MatrixWidth;
			int col = (int)((int)Index % MatrixWidth) / 3;
			if (row > 31 && row < 91 && col>126 && col < 366) {
				// COLOR IMG 
				r = *itr;
				g = *(itr + 1);
				b = *(itr + 2);
			}
			else {
				// BW 
				r = *itr;
				g = *itr;
				b = *itr;
			}
			const unsigned char colorPixel[] = { r, g, b };
			
			*bg->draw_rectangle(str.GetMult() * (col + 1), str.GetMult() * (row + 1),
				str.GetMult() * col, str.GetMult() * row,
				colorPixel);
		}
	}
}

void DisplayImgTermoscanner(StreamingMode str1, std::vector<uint8_t>& UartPixelMatrix1, 
							StreamingMode str2, std::vector<uint8_t>& UartPixelMatrix2, 
							cimg_library::CImg <unsigned char>* bg) {
	std::vector<uint8_t>::iterator itr1,itr2;
	itr2 = UartPixelMatrix2.begin(); 
	uint8_t r, g, b; 
	uint8_t Rc, Rl, Rr, Ru, Rd;
	uint8_t Gc, Gl, Gr, Gu, Gd;
	uint8_t Bc, Bl, Br, Bu, Bd;
	uint8_t Rlu, Rru, Rld, Rrd;
	uint8_t Glu, Gru, Gld, Grd;
	uint8_t Blu, Bru, Bld, Brd;
	uint8_t r_temp, g_temp, b_temp;
	int Index = 0; 
	int MatrixWidth1 = str1.MatrixW();
	int MatrixWidth2 = str2.MatrixW(); 

	for (itr1 = UartPixelMatrix1.begin(); itr1 < UartPixelMatrix1.end(); itr1++, Index++) {
		int row = (int)Index / MatrixWidth1;  
		int col = (int)Index % MatrixWidth1;

		if (row >= 31 && row < 91 && col>=42 && col < 122) {
			
			if (row >= 32 && row < 90 && col >= 43 && col < 121) {
				
				r_temp = Average(itr2, MatrixWidth2, 0.6, 0.075, 0.025); 
				itr2++; 
				g_temp = Average(itr2, MatrixWidth2, 0.6, 0.075, 0.025);
				itr2++;
				g_temp = Average(itr2, MatrixWidth2, 0.6, 0.075, 0.025);
				itr2++;
			}
			else {

				r_temp = *(itr2++);
				g_temp = *(itr2++);
				b_temp = *(itr2++);
			}
			
			// FUSE IMG 
			r = 0.7 * (*itr1) + 0.3 * r_temp;
			g = 0.7 * (*itr1) + 0.3 * g_temp;
			b = 0.7 * (*itr1) + 0.3 * b_temp;

		}
		else {
			// HIMAX IMG 
			r = *itr1;
			g = *itr1;
			b = *itr1;
		}

		const unsigned char colorPixel[] = { r, g, b };
		
		*bg->draw_rectangle(str1.GetMult() * (col + 1), str1.GetMult() * (row + 1),
			str1.GetMult() * col, str1.GetMult() * row,
			colorPixel);
	}
}

bool DetectMotion(std::vector<uint8_t>& BackgroundImg, std::vector<uint8_t>& UartPixelMatrix) {
	std::vector<uint8_t>::iterator itrP; 
	std::vector<uint8_t>::iterator itrB = BackgroundImg.begin();
	for (itrP = UartPixelMatrix.begin(); itrP < UartPixelMatrix.end(); itrP++, itrB++) {
		if (std::abs(*itrP - *itrB) > 5) {
			return true;
		}
	}
	return false; 
}

uint8_t Average(std::vector<uint8_t>::iterator itr, int MatrixWidth, double c1, double c2) {
	if ((c1 + 4 * c2) != 1.00) std::cout << "Error in Average coefficients\n"; 
	uint8_t c, l, r, u, d, temp; 
	c = *(itr);
	l = *(itr - 3);
	r = *(itr + 3);
	u = *(itr - MatrixWidth);
	d = *(itr + MatrixWidth);
	temp = c1 * c + c2 * l + c2 * r + c2 * u + c2 * d;
	return temp; 
}
uint8_t Average(std::vector<uint8_t>::iterator itr, int MatrixWidth, double c1, double c2, double c3) {
	if ((c1 + 4*c2 + 4*c3) != 1.00) std::cout << "Error in Average coefficients\n";
	uint8_t c, l, r, u, d, temp;
	uint8_t lu, ru, ld, rd; 
	c = *(itr);
	l = *(itr - 3);
	r = *(itr + 3);
	u = *(itr - MatrixWidth);
	d = *(itr + MatrixWidth);
	lu = *(itr - 3 - MatrixWidth);
	ru = *(itr + 3 - MatrixWidth);
	ld = *(itr - 3 + MatrixWidth);
	rd = *(itr + 3 + MatrixWidth);
	temp = c1*c + c2*l + c2*r + c2*u + c2*d + c3*lu + c3*ru + c3*ld + c3*rd;
	return temp; 
}

uint8_t MedianFilter(std::vector<uint8_t>::iterator itr, int MatrixWidth) {
	std::vector<uint8_t> container; 
	container.push_back(*(itr - 3 - MatrixWidth));  // lu 
	container.push_back(*(itr     - MatrixWidth));  // u
	container.push_back(*(itr + 3 - MatrixWidth));  // ru
	container.push_back(*(itr - 3              ));  // l 
	container.push_back(*(itr                  ));  // c 
	container.push_back(*(itr + 3              ));  // r 
	container.push_back(*(itr - 3 + MatrixWidth));  // ld
	container.push_back(*(itr     + MatrixWidth));  // d
	container.push_back(*(itr + 3 + MatrixWidth)); // rd 

	std::sort(container.begin(), container.end()); 
	return container[5]; 
}




void StreamingMode::PrintSpecs() {
	std::cout << "\nCamera : ";
	switch (cam) {
	case LeptonFlir_BW:			std::cout << "LeptonFlir B&W (RAW14)";		break;
	case LeptonFlir_C:			std::cout << "LeptonFlir Colour (RGB 888)";	break;
	case HM01B0_QQVGA_BW:		std::cout << "HM01B0 B&W ";					break;
	case HM01B0_QQVGA_C:		std::cout << "HM01B0 Colour";				break;
	case HM01B0_QVGA_BW:		std::cout << "HM01B0 B&W - HIGH RES";		break;
	case TERMOSCAN:				std::cout << "Termoscanner";				break;
	case TERMOSCAN_HM01B0:		std::cout << "Termoscanner - Himax  img";	break;
	case TERMOSCAN_Lepton:		std::cout << "Termoscanner - Lepton img";	break;
	default:					std::cout << "No camera mode detected";		break;
	}
	std::cout << "\n\tImage Height  : " << imageH;
	std::cout << "\n\tImage Width   : " << imageW;
	std::cout << "\n\tMatrix Height : " << matrixH;
	std::cout << "\n\tMatrix Width  : " << matrixW;
	std::cout << "\n\tNumber of cam : " << numCam;
	std::cout << std::endl;
}

void PrintTitle() {
	std::cout << "+--------------------------------------------------+\n";
	std::cout << "|                                                  |\n";
	std::cout << "|            ~ STM32 EMBEDDED SYSTEM  ~            |\n";
	std::cout << "|                   stream video                   |\n";
	std::cout << "|                                                  |\n";
	std::cout << "+--------------------------------------------------+\n\n";
};



/*
--------------- UART ------------------
*/
HANDLE init_UART() {

	// OPEN THE PORT
	HANDLE hSerial;

	hSerial = CreateFileA("COM3",           //AVI COM 3, name of file I want to open, --> https://electronics.stackexchange.com/questions/58717/error-argument-of-type-cons-char-is-incompatible-with-parameters-of-type-l
		GENERIC_READ,// | GENERIC_WRITE,    // tells windows I want to read/write
		0,                                  // should be 0  
		0,                                  // should be 0 
		OPEN_EXISTING,                      // tells windows that I should open only an existing file
		FILE_ATTRIBUTE_NORMAL,              // tells qwindows I want standard case
		0);                                 // should be 0

	if (hSerial == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			// serial port doesn't exist
		}
		// some other errror
		std::cout << "Errore nel HANDLE" << std::endl;
	}



	// SET UP PARAMETERS FOR THE CONNECTION
	DCB dcbSerialParams = { 0 };

	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	if (!GetCommState(hSerial, &dcbSerialParams)) {
		// error getting state
		std::cout << "Errore nel GetCommState" << std::endl;
	}

	dcbSerialParams.BaudRate = 1764705;//CBR_256000 - 460800
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	if (!SetCommState(hSerial, &dcbSerialParams)) {     // actually set the parameters psecified above
		// error settign serial port state
		std::cout << "Errore nel SetCommState" << std::endl;

	}


	COMMTIMEOUTS timeouts = { 0 };

	timeouts.ReadIntervalTimeout = 50;          // how long to wait between receiving char before timing out
	timeouts.ReadTotalTimeoutConstant = 50;     // how long to wait before return
	timeouts.ReadTotalTimeoutMultiplier = 50;   // how much additional time to wait before returning to each byte requested in read
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 50;

	if (!GetCommTimeouts(hSerial, &timeouts)) { // apply the setting to the serial port
		// error occured
		std::cout << "Errore nel GetCommTimeouts" << std::endl;

	}

	return hSerial;
}
