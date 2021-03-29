# Thermal-scanner-EmbeddedSystem-Project
This repository contains the code developed for the project course Embedded Systems - University of Trento

# Project's goal 
Our project's goal was to develop and build a thermal scanner prototype based on an STM32 microcontroller. The developed code allows the user to fuse the images coming from two different cameras (visual and thermal), as well as visualizing the end result thanks to the open source library CImg.  

# Hardware 
The hardware components that have been used for this project are : 
* STM32 F401RE 
* Himax HM01B0 visual camera 
* Lepton Flir v2.0 infra-red camera 

# Code 
The code is mainly splitted into 2 separate libraries
* Streaming_STM  
   - C++ application for reading the data transmitted from the STM and displaying the image on screen
* Camera_STM 
   - STM code developed in the CubeIDE environment for reading the data captured by the two cameras. 
   - Further distinction is made for code which is generic-purpose and code which is specific for a single camera  

# Results 
Video footage of the working prototype can be found at the following link : 
[Experimental video footage](https://www.youtube.com/watch?v=Bs92HlgQElU)

Mockup still images can be found in the folder `./images/` of this repository. 

# Mentions 
The code for the Lepton library has been highly inspired by the work of NachtRaveVL (https://github.com/NachtRaveVL/Lepton-FLiR-Arduino). 
