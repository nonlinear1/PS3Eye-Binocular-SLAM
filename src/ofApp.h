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
		void loadSingleCalibration();
		void calibrateMono();
		void calibrateStereo();

		void monoCalibrateOnFrame();// (cv::Mat frame);
		void monoCalibrateFinal();


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

		// calibrate single camera variables
		bool loadedSingleCalibration = false; // if a calibration file is successfully loaded, set to tru
		bool inMiddleOfCalibrating = false; // set to true whenever we're calibrating -- useful for calibration GUI logic
		bool finishedCalibrating = false;
		int boardWidth = 7; // width in squares
		int boardHeight = 10; // height in squares
		float squareSize = 22.5; // sidelength of squarse in mm
		int numFramesNeeded = 30; // how many frames to capture for calibration
		int numFramesCaptured = 0; // counter for keeping track of how many calibratin frames we've captured
		
		cv::Size boardSize; // derived from above values
		int boardArea;  // derived from above values

		vector<vector<cv::Point3f>> objectPoints;
		vector<vector<cv::Point2f>> imagePoints;
		vector<cv::Point2f> corners;

		cv::Mat K; // camera matrix
		cv::Mat D; // distortion coefficients
		vector<cv::Mat> rotationVectors;
		vector<cv::Mat> translationVectors;

		// opencv stuff

		cv::Ptr<cv::StereoSGBM> stereo;
		ofImage lGrey;
		ofImage rGrey;
		cv::Mat leftGrey;
		cv::Mat rightGrey;
		cv::Mat dispGrey;
		int numDisp = 144;//48;//16;//144; // 16?
		int preFilterCap = 63;
		int minDisp = -39;
		int uniquenessRatio = 10;
		int speckleWindowSize = 10;//100;
		int speckleRange = 32;
		int disp12MaxDiff = 1;
		bool fullDP = false;
		int P1 = 480;
		int P2 = 640;
		int blockSize = 13;
		int mode = 0;

		//****

		ofImage grey, sobel;
		ofImage cvImage;
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
		
		// image + frame vars -- these are the folks that update every frame

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
		bool doOpenCvStuff = false;
		bool cameraSettingsWidget = true;
		bool camStatusWidget = true;
		bool calibrateMonoWidget = false;
		bool calibrateStereoWidget = false;
		
		
};
