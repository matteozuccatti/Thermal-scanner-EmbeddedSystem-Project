#include <stdlib.h>
#include <vector>
#include "cam_lib.h"
#include <iomanip>
#include <iostream>
#include <chrono> 

int StreamingMode::numCam = 0; 

int main() {

	PrintTitle(); 
/*
 Define the type of camera you are using 

 1 - LeptonFlir_BW		Lepton Flir black and white 
 2 - LptonFlir_C		Lepton Flir colored 
 3 - HM01B0_QQVGA_BW	Himax QQVGA black and white
 4 - HM01B0_QQVGA_C		Himax QQVGA colored (not implemented yet)
 5 - HM01B0_QVGA_BW		Himax QVGA black and white 
 6 - TERMOSCAN_HM01B0	Thermal scanner -> single Himax image (always 1st)
 7 - TERMOSCAN_Lepton	Thermal scanner -> single Lepton image (always 2nd)
     
	 NO_CAM				no secondary camera

  */
	StreamingMode stream(LeptonFlir_BW);
	stream.Mult(9);	 // change scaling of the displayed image 
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
		// --- READING UART DATA  
		// Cam 1 
		ReadCamData(stream, hSerial, &frameNumber, dwBytesRead); 
		// Cam 2 
		if (stream.NumCam() > 1 || stream2.GetCam()==TERMOSCAN_Lepton) {
			ReadCamData(stream2, hSerial, &frameNumber, dwBytesRead);
		}


		// --- DISPLAY IMAGE 
		if (stream.GetCam() == TERMOSCAN_HM01B0 && stream2.GetCam() == TERMOSCAN_Lepton) {
			// Thermoscanner -> 2 separate cameras are fused into a single image 
			DisplayImgTermoscanner(stream, stream.UartPixelMatrix, stream2, stream2.UartPixelMatrix, &bg);
			dsp.display(bg);
		}
		else {
			// Single cameras 
			// Cam 1 
			DisplayImage(stream, stream.UartPixelMatrix, &bg);
			dsp.display(bg);
			if (stream.NumCam() > 1) {
				DisplayImage(stream2, stream2.UartPixelMatrix, &bg2);
				dsp2.display(bg2);
			}
		}

		if (GetKeyState(VK_SPACE) & 0x8000) {
			std::cout << "SPACE BAR PRESSED ! \n";
		}

	}
	
	return 0; 
}