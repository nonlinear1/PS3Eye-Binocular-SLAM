#pragma once

#include "ofMain.h"
#include "ps3eye.h"
#include "ofxImGui.h"
#include "libusb.h"
#include "ps3eye_capi.h"
#include "scannerEye.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawCameraSettingsWidget();
		void leftFrameDraw();
		void rightFrameDraw();
		void frameUpdater();


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
		/*
		cv::Mat source, source_grey;
		cv::Mat dest, detected_edges;
		
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
		void drawCameraStatus();
		
		
		scannerEye* leftCam;
		scannerEye* rightCam;
		int numberOfCams; // number of PS3 Eye Cameras attached

		
		// back to our stuff
		
		ofxImGui::Gui gui;
		ImVec4 backgroundColor;
		
		
		bool leftCameraDraw = false;
		bool rightCameraDraw = false;
		
		// image + frame vars

		unsigned char* leftPixels; // stores raw image data, pixel by pixel
		unsigned char* rightPixels;

		ofPixels leftEyeFrame;
		ofTexture leftDrawFrame;

		ofPixels rightEyeFrame;
		ofTexture rightDrawFrame;

		cv::Mat leftMat;
		cv::Mat rightMat;

		

		// Widget variables
		bool cameraSettingsWidget = true;
		bool camStatusWidget = true;
};
