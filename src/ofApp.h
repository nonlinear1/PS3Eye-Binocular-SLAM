#pragma once

#include "ofMain.h"
#include "ps3eye.h"
#include "ofxImGui.h"
#include "libusb.h"
#include "ps3eye_capi.h"
#include "scannerEye.h"
//#include "ofxCv.h"
//#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawSettingsWidget();
		void leftFrameHandler();
		void leftFrameDraw();
		//void prepareRenderField();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void exit();

		// opencv stuff
		//ofPixels gray, edge, sobel;
		/*
		Mat source, source_grey;
		Mat dest, detected_edges;
		
		int edgeThresh = 1;
		int lowThreshold;
		int const max_lowThreshold = 100;
		int ratio = 3;
		int kernel_size = 3;
		*/
		//end opencv

		// ps3eye stuff
		int cam_width = 640;
		int cam_height = 480;
		int cam_refresh = 60;
		void initCams();
		void deInitCams();
		void ps3EyeStatusDraw();
		bool camStatus = true;

		//ps3eye_t* firstCam;
		scannerEye* firstCam;
		scannerEye* secondCam;
		int numberOfCams; // number of PS3 Eye Cameras attached

		ofPixels leftEyeFrame;
		ofTexture leftDrawFrame;
		unsigned char leftPixels[921600]; //= NULL;
		// back to our stuff
		
		ofxImGui::Gui gui;
		ImVec4 backgroundColor;
		
		//ofVideoGrabber cam1;

		// display formats: solo camera | double camera
		// solo camera: function to switch between them
		// double camera: function to swap left/right

		ofImage leftFrame;
		ofImage rightFrame;

		float testFloat = 23.0f;
		bool settingsWidget = true;
};
