#include <iostream>
#include <stdint.h>
#include <iomanip>
#include <windows.h>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>

#include "streaming_mode_class.h"
#include "core_func.h"


void StreamingMode::PrintSpecs() {
	std::cout << "\nCamera : ";
	switch (cam) {
	case LeptonFlir_BW:			std::cout << "LeptonFlir B&W (RAW14)";		break;
	case LeptonFlir_C:			std::cout << "LeptonFlir Colour (RGB 888)";	break;
	case HM01B0_QQVGA_BW:		std::cout << "HM01B0 B&W ";					break;
	case HM01B0_QQVGA_C:		std::cout << "HM01B0 Colour";				break;
	case HM01B0_QVGA_BW:		std::cout << "HM01B0 B&W - HIGH RES";		break;
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



void DisplayImage(StreamingMode str, std::vector<uint8_t>& UartPixelMatrix, cimg_library::CImg <unsigned char>* bg) {
	if (str.GetCam() == LeptonFlir_BW || str.GetCam() == HM01B0_QQVGA_BW || str.GetCam() == HM01B0_QVGA_BW) {
		DisplayImageGrayScale(str, UartPixelMatrix, bg);
	}

	if (str.GetCam() == LeptonFlir_C) {
		DisplayImageRGB(str, UartPixelMatrix, bg);
	}
}

void DisplayImage2(StreamingMode str, std::vector<uint8_t>& UartPixelMatrix, cimg_library::CImg <unsigned char>* bg,
	StreamingMode str2, std::vector<uint8_t>& UartPixelMatrix2, cimg_library::CImg <unsigned char>* bg2,
	cimg_library::CImg <unsigned char>* bg3) {
	// CAM 1 
	if (str.GetCam() == LeptonFlir_BW || str.GetCam() == HM01B0_QQVGA_BW || str.GetCam() == HM01B0_QVGA_BW) {
		DisplayImageGrayScale(str, UartPixelMatrix, bg);
	}
	if (str.GetCam() == LeptonFlir_C) {
		DisplayImageRGB(str, UartPixelMatrix, bg);
	}

	// CAM 2 
	if (str2.GetCam() == LeptonFlir_BW || str2.GetCam() == HM01B0_QQVGA_BW || str2.GetCam() == HM01B0_QVGA_BW) {
		DisplayImageGrayScale(str2, UartPixelMatrix2, bg2);
	}
	if (str2.GetCam() == LeptonFlir_C) {
		DisplayImageRGB(str2, UartPixelMatrix2, bg2);
	}

	// TERMOSCAN 
	if (str.GetCam() == TERMOSCAN_HM01B0 && str2.GetCam() == TERMOSCAN_Lepton) {
		DisplayImageTermoscanner(str, UartPixelMatrix, str2, UartPixelMatrix2, bg3);
	}

}

void DisplayImageGrayScale(StreamingMode str, std::vector<uint8_t>& UartPixelMatrix, cimg_library::CImg <unsigned char>* bg) {
	// GRAYSCALE SINGLE CAMERAs 
	std::vector<uint8_t>::iterator itr;
	uint8_t r, g, b;
	int Index = 0;
	int MatrixWidth = str.MatrixW();
	for (itr = UartPixelMatrix.begin(); itr < UartPixelMatrix.end(); itr++, Index++) {
		r = *itr;
		g = *itr;
		b = *itr;
		const unsigned char colorPixel[] = { r, g, b };
		int row = (int)Index / MatrixWidth;
		int col = (int)Index % MatrixWidth;
		*bg->draw_rectangle(str.GetMult() * (col + 1), str.GetMult() * (row + 1),
			str.GetMult() * col, str.GetMult() * row,
			colorPixel);
	}
}

void DisplayImageRGB(StreamingMode str, std::vector<uint8_t>& UartPixelMatrix, cimg_library::CImg <unsigned char>* bg) {
	// LEPTON FLIR COLOR 
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

void DisplayImageTermoscanner(StreamingMode str1, std::vector<uint8_t>& UartPixelMatrix1,
	StreamingMode str2, std::vector<uint8_t>& UartPixelMatrix2,
	cimg_library::CImg <unsigned char>* bg) {
	std::vector<uint8_t>::iterator itr1, itr2;
	itr2 = UartPixelMatrix2.begin();
	uint8_t r, g, b;
	uint8_t r_temp, g_temp, b_temp;
	int Index = 0;
	int MatrixWidth1 = str1.MatrixW();
	int MatrixWidth2 = str2.MatrixW();

	for (itr1 = UartPixelMatrix1.begin(); itr1 < UartPixelMatrix1.end(); itr1++, Index++) {
		int row = (int)Index / MatrixWidth1;
		int col = (int)Index % MatrixWidth1;

		if (row >= 31 && row < 91 && col >= 42 && col < 122) {

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



uint8_t Average(std::vector<uint8_t>::iterator itr, int MatrixWidth, double c1, double c2) {
	// CROSS 
	if ((c1 + 4 * c2) != 1.00) std::cout << "Err    or in Average coefficients\n";
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
	// ROUND 
	if ((c1 + 4 * c2 + 4 * c3) != 1.00) std::cout << "Error in Average coefficients\n";
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
	temp = c1 * c + c2 * l + c2 * r + c2 * u + c2 * d + c3 * lu + c3 * ru + c3 * ld + c3 * rd;
	return temp;
}

uint8_t MedianFilter(std::vector<uint8_t>::iterator itr, int MatrixWidth) {
	std::vector<uint8_t> container;
	container.push_back(*(itr - 3 - MatrixWidth));  // lu 
	container.push_back(*(itr - MatrixWidth));  // u
	container.push_back(*(itr + 3 - MatrixWidth));  // ru
	container.push_back(*(itr - 3));  // l 
	container.push_back(*(itr));  // c 
	container.push_back(*(itr + 3));  // r 
	container.push_back(*(itr - 3 + MatrixWidth));  // ld
	container.push_back(*(itr + MatrixWidth));  // d
	container.push_back(*(itr + 3 + MatrixWidth)); // rd 

	std::sort(container.begin(), container.end());
	return container[5];
}



void ReadCamData(StreamingMode& stream, HANDLE hSerial, int* frameNumber, DWORD dwBytesRead) {
	int i = 0;
	while (i < stream.MatrixH() && ReadFile(hSerial, stream.ReadBuff(), stream.Bytes2Read(), &dwBytesRead, NULL)) {
		for (int j = 0; j < stream.Bytes2Read(); j++) {
			stream.UartPixelMatrix[j + (i * stream.MatrixW())] = stream.readBuff[j];
		}
		i++;
	}
	*frameNumber += 1;
	std::cout << "Frame n.  " << std::setw(2) << *frameNumber << "\r";
}

void StoreFrame(StreamingMode str, std::vector<uint8_t>& UartPixelMatrix) {

}

int ReadNumberOfStoredFrames() {
	std::fstream file;  
	std::string n_str; 
	file.open("n_tot_img.txt", std::ios::in); 
	if (!file) std::cout << "Error in opening the 'n_tot_img.txt' file \n"; 

	if (file.is_open()) {
		std::getline(file, n_str);
	}
	file.close(); 
	return std::stoi(n_str);
}

void UpdateNumberOfStoredFrames(int *n) {
	// update the current number of images 
	*n += 1; 

	// update the n. of imgs in the txt file 
	std::ofstream file;
	int new_n = *n;
	file.open("n_tot_img.txt"); 
	file << new_n; 
	file.close(); 
}
void ResetNumberOfStoredFrames() {
	// reset the n. of imgs in the txt file 
	std::ofstream file;
	file.open("n_tot_img.txt");
	int n = 0; 
	file << n;
	file.close();
}





























/*
------------------- UART ----------------------
*/
HANDLE init_UART() {

	// OPEN THE PORT
	HANDLE hSerial;

	hSerial = CreateFileA("COM3",           // name of file I want to open, --> https://electronics.stackexchange.com/questions/58717/error-argument-of-type-cons-char-is-incompatible-with-parameters-of-type-l
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