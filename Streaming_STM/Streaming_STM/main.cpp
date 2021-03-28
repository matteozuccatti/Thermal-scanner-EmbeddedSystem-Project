#include <stdlib.h>
#include <vector>
#include "cam_lib.h"
#include <iomanip>
#include <iostream>
#include <chrono> 

int StreamingMode::numCam = 0; 

int main() {

	PrintTitle(); 

	// HM01B0_QQVGA_BW    HM01B0_QVGA_BW   
	// LeptonFlir_BW      LeptonFlir_C  
	// TERMOSCAN_HM01B0   TERMOSCAN_Lepton  
	// NO_CAM

	StreamingMode stream(LeptonFlir_C);
	stream.Mult(9); 
	StreamingMode stream2(NO_CAM);
	stream2.Mult(3);

	bool motion_detection = false;

	//Initialize UART 
	HANDLE hSerial = init_UART();
	DWORD dwBytesRead = 0;

	std::cout << "\nWaiting for UART to send data ...\n\n";
	
	cimg_library::CImg <unsigned char> bg(stream.StreamW(), stream.StreamH(), 1, 3, 255);
	cimg_library::CImgDisplay dsp(stream.StreamW(), stream.StreamH(), "Stream Camera 1", 0);

	cimg_library::CImg <unsigned char> bg2(stream2.StreamW(), stream2.StreamH(), 1, 3, 255);
	cimg_library::CImgDisplay dsp2(stream2.StreamW(), stream2.StreamH(), "Stream Camera 2", 0);

	dsp.display(bg);
	if ((stream.NumCam() > 1) && (stream2.GetCam() != TERMOSCAN_Lepton)) dsp2.display(bg2);

	int frameNumber = 0;

	while (1) {
		
		
		if (stream.GetCam() != TERMOSCAN) {

			int i = 0;
			// LOADING DATA CAM 1 
			while (i < stream.MatrixH() && ReadFile(hSerial, stream.ReadBuff(), stream.Bytes2Read(), &dwBytesRead, NULL)) {
				for (int j = 0; j < stream.Bytes2Read(); j++) {
					stream.UartPixelMatrix[j + (i * stream.MatrixW())] = stream.readBuff[j];
				}
				i++;
			}
			// MOTION DETECTION CAM 1 
			if (motion_detection) {
				if (stream.GetFrameN() == 1) {
					stream.CreateBackground();
				}
				stream.UpdateBackground();

				if (stream.GetFrameN() > 2 && DetectMotion(stream.BackgroundImg, stream.UartPixelMatrix)) {
					std::cout << "Frame n. " << std::setw(2) << frameNumber++ << "  -  MOTION DETECTED" << "\r";
				}
				else {
					std::cout << "Frame n. " << std::setw(2) << frameNumber++ << "                    " << "\r";
				}
				stream.IncrementFrame();
			}
			std::cout << "Frame n. " << std::setw(2) << frameNumber++ << "                    " << "\r";


			// LOADING DATA CAM 2 
			if (stream.NumCam() > 1 || stream2.GetCam()==TERMOSCAN_Lepton) {
				i = 0;
				while (i < stream2.MatrixH() && ReadFile(hSerial, stream2.ReadBuff(), stream2.Bytes2Read(), &dwBytesRead, NULL)) {
					for (int j = 0; j < stream2.Bytes2Read(); j++) {
						stream2.UartPixelMatrix[j + (i * stream2.MatrixW())] = stream2.readBuff[j];
					}
					i++;
				}
			}
		}
		else {

			long int p = 0; 
			while (p < stream.SizeMatrix() && ReadFile(hSerial, stream.ReadBuff(), stream.Bytes2Read(), &dwBytesRead, NULL)) {
				int row = (int)p / stream.MatrixW(); 
				int col = (int)((int)p % stream.MatrixW()) / 3;

				if (row > 31 && row < 91 && col>126 && col < 366) {
					// COLOR IMG 
					stream.UartPixelMatrix[p] = stream.readBuff[0]; 
					p++; 
				}else {
					// BW IMG
					stream.UartPixelMatrix[p] = stream.readBuff[0];
					p += 3; 
				}
			}
			std::cout << "Frame n. " << std::setw(2) << frameNumber++ << "\r";
		}

		

		// DISPLAY IMAGE 
		if (stream.GetCam() != TERMOSCAN_HM01B0 && stream.GetCam() != TERMOSCAN_Lepton) {
			DisplayImage(stream, stream.UartPixelMatrix, &bg);
			dsp.display(bg);
			DisplayImage(stream2, stream2.UartPixelMatrix, &bg2);
			if (stream.NumCam() > 1) {
				dsp2.display(bg2);
			}
		}
		else {
			DisplayImgTermoscanner(stream, stream.UartPixelMatrix, stream2, stream2.UartPixelMatrix, &bg);
			dsp.display(bg); 
		}
		

	}
	

	return 0; 
}