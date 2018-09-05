#pragma once

#include "ofMain.h"
#include "ps3eye.h"
#include "ofxImGui.h"
#include "libusb.h"
#include "ps3eye_capi.h"
#include "scannerEye.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawCameraSettingsWidget();
		void leftFrameDraw();
		void rightFrameDraw();
		void cvFrameDraw();
		void frameUpdater();
		void openCvStuff();
		void saveSettings();
		void loadSettings();


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

		ofImage grey, edge, sobel;
		float thresh1 = 90;  
		float thresh2 = 70;
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
		bool swapCameras; // if true on exit, will tell app to swap cameras automatically on next launch

		// back to our stuff
		
		ofxImGui::Gui gui;
		ofxXmlSettings settings;
		ImVec4 backgroundColor;
		bool closeButtonOnWidgets = false;
		
		
		bool leftCameraDraw = false;
		bool rightCameraDraw = false;
		bool cvDraw = false;
		
		// image + frame vars

		unsigned char* leftPixels; // stores raw image data, pixel by pixel
		unsigned char* rightPixels;

		ofPixels leftEyeFrame;
		ofTexture leftDrawFrame;
		ofPixels leftEyeFrame_scaled;

		ofPixels rightEyeFrame;
		ofTexture rightDrawFrame;
		ofPixels rightEyeFrame_scaled;

		cv::Mat leftMat;
		cv::Mat rightMat;

		

		// Widget variables
		bool cameraSettingsWidget = true;
		bool camStatusWidget = true;
};
