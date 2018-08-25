#pragma once



class scannerEye : public PS3EYECam
{
	public: 
		void setup(int = 0, int = 640, int 480, int 60, ps3eye_format = PS3EYE_FORMAT_RGB);

		int cam_width = 640;
		int cam_height = 480;
		int cam_refresh = 60;
		void initCam();
		void deInitCam();
		ps3eye_t* camRef;
		unsigned char leftPixels[921600]; // replace witha parameterized version --
										  // use those camera vars!!
		// check out other camera vars in ps3eye.h -- wrap what's needed, ignore the rest

};