#include <stdlib.h>
#include <vector>
#include <iomanip>
#include <iostream>
#include <chrono> 
#include <string>
#include "include/streaming_mode_class.h"
#include "include/core_func.h"


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
	stream.Mult(1);	 // change scaling of the displayed image 
	StreamingMode stream2(NO_CAM);
	stream2.Mult(3);

	// Stored images 
	ResetNumberOfStoredFrames();
	int nStoredImgs = ReadNumberOfStoredFrames();
	std::string name_str;
	//Initialize UART 
	HANDLE hSerial = init_UART();
	DWORD dwBytesRead = 0;

	std::cout << "\nWaiting for UART to send data ...\n\n";
	// Cam 1
	cimg_library::CImg <unsigned char> bg(stream.StreamW(), stream.StreamH(), 1, 3, 255);
	cimg_library::CImgDisplay dsp(stream.StreamW(), stream.StreamH(), "Stream Camera 1", 0);
	// Cam 2 
	cimg_library::CImg <unsigned char> bg2(stream2.StreamW(), stream2.StreamH(), 1, 3, 255);
	cimg_library::CImgDisplay dsp2(stream2.StreamW(), stream2.StreamH(), "Stream Camera 2", 0);
	//Fused (iff termoscan) -> size of the himax 
	cimg_library::CImg <unsigned char> bg3(stream.StreamW(), stream.StreamH(), 1, 3, 255);
	cimg_library::CImgDisplay dsp3(stream.StreamW(), stream.StreamH(), "Stream Camera 3", 0);

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
		DisplayImage2(stream,  stream.UartPixelMatrix,  &bg,
					  stream2, stream2.UartPixelMatrix, &bg2, &bg3); 
		if (stream.GetCam() == TERMOSCAN_HM01B0 && stream2.GetCam() == TERMOSCAN_Lepton) {
			dsp3.display(bg3);
		}
		else {
			dsp.display(bg);
		}
		
		if (stream.NumCam() > 1) dsp2.display(bg2); 

		// --- SAVE DATA 
		if (GetKeyState(VK_SPACE) & 0x8000) {
			std::cout << "SPACE BAR PRESSED ! \n";
			UpdateNumberOfStoredFrames(&nStoredImgs);
			// Save cam 1  
			name_str = ".\\storage\\" + std::to_string(nStoredImgs) + "_BW.bmp";
			//const char * name_char = name_str.c_str(); 
			bg.save_bmp(name_str.c_str());
			// Save cam 2 
			name_str = ".\\storage\\" + std::to_string(nStoredImgs) + "_C.bmp";
			//name_char = name_str.c_str();
			if (stream.NumCam() > 1 || stream2.GetCam() == TERMOSCAN_Lepton) bg2.save_bmp(name_str.c_str());
			// Save cam 3 -> fused 
			name_str = ".\\storage\\" + std::to_string(nStoredImgs) + "_F.bmp";
			//name_char = name_str.c_str();
			if (stream.GetCam() == TERMOSCAN_HM01B0 && stream2.GetCam() == TERMOSCAN_Lepton) bg3.save_bmp(name_str.c_str());
		}
		

	}
	
	return 0; 
}