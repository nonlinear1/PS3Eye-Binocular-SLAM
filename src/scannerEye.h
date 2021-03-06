#pragma once

#include "ps3eye.h"
#include "ps3eye_capi.h"
#include "ofMain.h"

class scannerEye //: public ps3eye::PS3EYECam 
{
	public:
		scannerEye();

		void setup();

		int camID; // ID for initializing camnera
		int cam_width;// = 640;
		int cam_height;// = 480;
		int cam_refresh;// = 60;
		bool camStatus = true;

		ps3eye_t* camInstance;

		ofPixels camOfPixels;
		ofTexture camOfTexture;
		//unsigned char camRawPixels[921600]; // create size based on widht/height. create in init function
		unsigned char* camRawPixels;

		void initCam(int = 0, int = 640, int = 480, int = 60);
		void deInitCam();
		unsigned char* grabRawFrame();

		// cv::Mat camMat; // output directly to Mat using a Mat constructor?

};