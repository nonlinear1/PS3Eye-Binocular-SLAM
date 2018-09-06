#include "ofApp.h"

using namespace ps3eye;
using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){

	//try: loadSettings();
	
	//printf(swapCameras ? "True" : "False"); 
	gui.setup();
	backgroundColor = ofColor(118, 131, 142);
	ofSetBackgroundColor(backgroundColor);
	ofSetWindowTitle("prototype build--bmp18");
	ofSetDataPathRoot("../data/");
	loadSettings();

	stereo = cv::StereoSGBM::create(minDisp, numDisp, blockSize, P1, P2, disp12MaxDiff, preFilterCap, uniquenessRatio, speckleWindowSize, speckleRange, mode);

	//testFloat = 23.0f;

	// single canvas for whole screen, can choose -- left + right display, left only, or right only
	// floating gui bar for selecting options
		// can make static first if easier, but dno't hink it'll be an issue
	// load and play local video -- have a "setUpVideoFeed" function that grabs video for both left and right
		// choose from streaming sources, from USB, or from disk
		// video playback is frame-by-frame, uncompressed if possible
			// this is for ease of openCL and stuff
			// also for ease of prototyping before getting the camera drivers ready
	// options for various openCV functions to be run on video
	// options for ordering openCV functions? or just pre-baked "this is how i do the complex stuff" function?


	ImGui::GetIO().MouseDrawCursor = false;
	//backgroundColor is stored as an ImVec4 type but can handle ofColor
	//backgroundColor = ofColor(114, 144, 154); // <-- pretty sure this requires themed imgui stuff

	// setup GUI or imgui stuff if needed

	initCams();

	// let image handlers manage getting images -- from cameras, from disk -- always two (right is null if need be)
	// but for now just load a file
	//leftFrameHandler();
	
}

/**
 * Looks for one or two PS3 Eye Cams, asks them to initialize themselves through the scannerEye class
 **/
void ofApp::initCams()
{
	ps3eye_init();
	numberOfCams = ps3eye_count_connected();
	// for (int i = 0; i < numberOfCams; i++) {init Cam[i]} sort of thing
	// OR
	// if (numberOfCams > 0) {init first AND if (nubmerOfCams > 1) {init Second)
	// some way of choosing other cameras? probably don't need to prepare for that --
	// goal of this is to be standalone single function software/hardware. Can
	// let others modify as needed if they desire different functionality.
	//firstCam = ps3eye_open(0, cam_width, cam_height, cam_refresh, PS3EYE_FORMAT_RGB); // to change, close and reopen camera?
	if (numberOfCams > 0)
	{
		leftCam = new scannerEye();
		leftCam->initCam(0, 640, 480, 60);
		// allocate array here, delete in deinit
		leftPixels = new unsigned char[leftCam->cam_width * leftCam->cam_height * 3]; // raw image size is (width * height * color channels per pixel) bytes
		leftCameraDraw = true;
		leftEyeFrame_scaled.allocate(320, 240, 3);

		if (numberOfCams > 1)
		{
			rightCam = new scannerEye();
			rightCam->initCam(1, 640, 480, 60);
			rightPixels = new unsigned char[rightCam->cam_width * rightCam->cam_height * 3];
			rightCameraDraw = true;
			rightEyeFrame_scaled.allocate(320, 240, 3);

			if (swapCameras)
			{
				swap(leftCam, rightCam);
			}
		}
	}

}

/*void ofApp::updateCams()
{
	// if numberOfCams = 0; exit gracefully
	// if numberOfCams = 1; remove any extras gracefully
	// if numberOfCams = 2; ensure both cameras are initialized and rendering
}*/

void ofApp::deInitCams()
{
	//ps3eye_close(firstCam);
	//ps3eye_uninit();
	if (leftCam)
	{
		leftCam->deInitCam();
		delete leftPixels;
		delete leftCam;
	}
	if (rightCam)
	{
		rightCam->deInitCam();
		delete rightPixels;
		delete rightCam;
	}

}

void ofApp::leftFrameDraw()
{ 
	leftEyeFrame.resizeTo(leftEyeFrame_scaled);
	//leftEyeFrame.resize(320, 240, OF_INTERPOLATE_BILINEAR);
	// IF (notScaled): load leftDrawFrame from leftEyeFrame
	//leftDrawFrame.loadData(leftEyeFrame);
	//leftDrawFrame.draw(0, 0, leftCam->cam_width, leftCam->cam_height);
	// ELSE: load leftDrawFrame from leftEyeFrame_scaled
	leftDrawFrame.loadData(leftEyeFrame_scaled);
	leftDrawFrame.draw(320, 480, 320, 240);

	// make a shader? three layers -- 0) color image 1) opencv feature overlay
	// maybe shader for getting greyscale too? so 0) color 1) greyscale by averageing color 2) opencv 
}

void ofApp::rightFrameDraw()
{
	// IF windowWidth > leftCamWidth + rightCamWidth THEN draw each at size
	// ELSE: draw each at windowWidth/2, with the proper aspect ratio
	/*
	if (ofGetWindowWidth() >= (leftCam->cam_width + rightCam->cam_width))
	{
		if (ofGetWindowWidth() > (leftCam->cam_width * 2))
		{
			ofSetWindowShape(leftCam->cam_width * 2, ofGetWindowHeight());
		}
		rightDrawFrame.loadData(rightEyeFrame);
		rightDrawFrame.draw(leftCam->cam_width, 0, rightCam->cam_width, rightCam->cam_height);
	}
	else
	{
		int ratio = (ofGetWindowWidth() / 2) / leftCam->cam_width;
		rightDrawFrame.loadData(rightEyeFrame);
		rightDrawFrame.draw(leftCam->cam_width*ratio, 0, rightCam->cam_width*ratio, rightCam->cam_height*ratio);
	}
	*/
	//rightEyeFrame.resize(320, 240, OF_INTERPOLATE_BILINEAR);
	//rightDrawFrame.loadData(rightEyeFrame);
	rightEyeFrame.resizeTo(rightEyeFrame_scaled);
	rightDrawFrame.loadData(rightEyeFrame_scaled);
	rightDrawFrame.draw(640, 480, 320, 240);
}

void ofApp::cvFrameDraw()
{
	cvImage.draw(320, 0);
	//ofImage mask;
	//mask.load(edge); // use a shader for this
	//grey.draw(0, 480);
}

//--------------------------------------------------------------
void ofApp::update(){
	// check for additional cameras? check for removed camera?
	//unsigned char* leftPixels = NULL;
	//ps3eye_grab_frame(firstCam, leftPixels);
	//ps3eye_grab_frame(firstCam->camInstance, leftPixels);
	
	
	frameUpdater(); // updates left and right frames
}

void ofApp::frameUpdater()
{
	// if we're drawing the cameras directly:
	if (leftCam)
	{
		leftEyeFrame.setFromPixels(leftCam->grabRawFrame(), leftCam->cam_width, leftCam->cam_height, 3);
		leftMat = toCv(leftEyeFrame);
	}
	
	if (rightCam)
	{
		rightEyeFrame.setFromPixels(rightCam->grabRawFrame(), rightCam->cam_width, rightCam->cam_height, 3);
		rightMat = toCv(rightEyeFrame);
	}

	// if we're using openCV and stuff, render a composite first, then send to leftEyeFrame etc.
	// ...or render to a FBO and just do that? That would fit nicely in a composite-maker function
	
	//if (!callibrateMonoWidget && !callibrateStereoWidget) // facilitates drawing captured frames w/ overlay?
	if (doOpenCvStuff)
	{
		openCvStuff();
	}
}

// toMat() function

void ofApp::openCvStuff()
{
	
	convertColor(leftEyeFrame, grey, CV_RGB2GRAY);
	Canny(grey, cvImage, thresh1, thresh2, 3);
	//Sobel(grey, sobel);
	grey.update();
	//sobel.update();
	cvImage.update();
	// StereoBM vs StereoSGBM vs adapt one from the papers?
	
	//printf("'stereo' has min disparity of: %i\n", stereo->getMinDisparity());

	/*
	convertColor(leftEyeFrame, lGrey, CV_RGB2GRAY);
	convertColor(rightEyeFrame, rGrey, CV_RGB2GRAY);
	leftGrey = toCv(lGrey);
	rightGrey = toCv(rGrey);
	//stereo->compute(leftGrey, rightGrey, dispGrey);
	stereo->compute(leftGrey, rightGrey, dispGrey);
	ofPixels dispPix;
	toOf(dispGrey, dispPix);
	cvImage.setFromPixels(dispPix);
	cvImage.update();*/
}

/*
 * Button in Settings widget to calibrate individual cameras. For ease we'll use one set of 
 * calibration data for both cameras -- they should be nearly identical. We'll use the 
 * single-camera calibration data to help calibrate our stereo camera setup.
 */
void ofApp::callibrateMono()
{
	

	ImGui::Begin("Calibrate Individual PS3EYEs", &closeButtonOnWidgets, ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::Text("Threshold 1");

	// if haven't started capturing, be text input. once click "Begin Calibration", they draw
	// as ImGui::Text, only usable inputs are "Capture Frame" and Cancel Calibration buttons 

	if (inMiddleOfCallibrating) // this is where we come once we've begun callibrating -- we enter in "else" below
	{
		// only have "Capture Frame" and "Cancel Callibration" buttons in here, but also any
		// mid-callibration output
		ImGui::Text("Captured %i of %i callibration images", numFramesCaptured, numFramesNeeded);
		ImGui::Text("Board width: %i squares", boardWidth);
		if (numFramesCaptured >= numFramesNeeded)
		{
			inMiddleOfCallibrating = false;
			finishedCallibrating = true;
		}
		if (ImGui::Button("Capture Frame"))
		{
			 // capture frame, send to callibration function
			 // should we store a vector of frames for reference? output them along with the callibration data?
			printf("\n");
			printf(" _.---.__________|(0)|__\n");
			printf("|. . . . . . . . . . . .|\n");
			printf("|. . . . ._____. . . . .|\n");
			printf("|. . . . /     \\ . . . .|\n");
			printf("|. . . . | ( ) | . . . .|\n");
			printf("|. . . . \\_____/ . . . .|\n");
			printf("|. . . . . . . . . . . .|\n");
			printf("|_______________________|\n"); // just because it was fun :)

			// capture frame to variable
			// have option to retake frame?
			// write frame to file "/calibration-images/image01.bmp"
			// call function with frame as param

			numFramesCaptured +=1;
			
			toOf(leftMat, cvImage);
			cvImage.update();

			
		}
		if (ImGui::Button("Cancel Callibration"))
		{
			inMiddleOfCallibrating = false;
		}
	}
	else // this is where we come when callibrateMono() is first called, and when we're done callibrating
	{
		if (finishedCallibrating)
		{
			if (ImGui::Button("Save Callibration File"))
			{
				// save file here
				// when saving the callibration file, give warning about overwriting previous file. 
				// maybe even a modal dialog if loadedSingleCallibration is true
				finishedCallibrating = false;
				callibrateMonoWidget = false;
			}
			if (ImGui::Button("Cancel"))
			{
				finishedCallibrating = false;
				callibrateMonoWidget = false;
				numFramesCaptured = 0;
			}
		}
		else
		{
			// parameter input and "Begin Callibration" button
			// do some "ImGui::SameLine" stuff to make it look like it does mid-callibration
			ImGui::Text("Board width: ");
			ImGui::SameLine(100);
			ImGui::InputInt("##board width", &boardWidth);
			if (ImGui::Button("Begin Callibration"))
			{
				inMiddleOfCallibrating = true;
			}
			if (ImGui::Button("Cancel"))
			{
				callibrateMonoWidget = false;
				numFramesCaptured = 0; // reset that frame capture counter back to zero
			}
		}
	}

	
	// text input box for params (also declare them above here)
	// when parm updated, update derivative params as well (boardSize, boardArea, etc.)
	//      - will this happen automatically? input values rollover frame to frame, but
	//        would other declared variables? Test :D
	ImGui::End();

	// for loop -- for each in numImages, do: 
	//	- present user with button which captures frame
	//	- on the frame, do the calibration things
	//		- findChessboardCorners etc.
	// - at end of for loop, allow option to take extra frames, 
	// - or just export calibration data to file AND load it internally
	// - exported filename: ps3eye_individual_alibration_mm-dd-yyyy.xml
	


}

/*
 * Have button in Settings widget to calibrate, when pressed it launches calibrateStereo()
 * calibrateStereo() 
 */
void ofApp::callibrateStereo()
{

}

//--------------------------------------------------------------
void ofApp::draw(){
	//cam1.draw(0, 0);
	if (leftCameraDraw)
	{
		leftFrameDraw();
		// use toOf(mat, ofImage) and ofImage.update() to draw back to ofImage? 
	}
	if (rightCameraDraw)
	{
		rightFrameDraw();
	}

	if (cvDraw)
	{
		cvFrameDraw();
	}

	gui.begin();

	{
		ImGui::Begin("Edge Detection Settings", &closeButtonOnWidgets, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Checkbox("Engage OpenCV", &doOpenCvStuff);
		ImGui::Text("Threshold 1");
		ImGui::SliderFloat("##1", &thresh1, 0.0f, 250.0f); // the "##" appears allow a non-displayed widget label
		ImGui::Text("Threshold 2");
		ImGui::SliderFloat("##2", &thresh2, 0.0f, 500.0f);
		ImGui::End();
	}

	if (camStatusWidget)
	{
		drawCameraStatus();
	}

	// main settings widget
	
	if (cameraSettingsWidget) // replace with checkbox bool for testing
	{
		drawCameraSettingsWidget();
		// overall settings widget, includes sub-widgets
		// for each Eye: draw settings widget
	}

	if (callibrateMonoWidget)
	{
		callibrateMono();
	}

	
	if (callibrateStereoWidget)
	{
		callibrateStereo();
	}
	

	gui.end();

}

/*
 * For settings we want: 
 *	- Radio buttons for left, right, or both (cameras)
		- how handle if only one camera attached?
 *	- button to swap left/right cams
 *
 */
void ofApp::drawCameraSettingsWidget()
{
	{
	ImGui::Begin("Camera Settings", &closeButtonOnWidgets, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	//ImGui::Text("Float value: %.2f", testFloat);
	// disable right camera draw if only one camera attached? include selection and swap
	// button in IF (more than 1 camera):
	ImGui::Checkbox("Left Camera Draw", &leftCameraDraw);
	ImGui::Checkbox("Right Camera Draw", &rightCameraDraw);
	ImGui::Checkbox("Processd Image Draw", &cvDraw);
	

	if (ImGui::Button("Callibrate individual PS3 Eye"))
	{
		callibrateMonoWidget = true;
	}

	if (ImGui::Button("Callibrate Stereo PS3 Eye Setup"))
	{
		callibrateStereoWidget = true;
	}

	if (ImGui::Button("Swap Left and Right Cameras"))
	{
		if (numberOfCams > 1)
		{
			swap(leftCam, rightCam);
			swapCameras = !swapCameras;
			//printf("SWAPP'D");
		}
	}
	ImGui::Text("Application average %.3f \nms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	}
}

/*
 * For Camera Status we want:
 *	- number of cameras attached
 *	- button to de-initialize a camera (in order to remove it)?
 *	- stats of each camera; resolution, refresh rate
 */
void ofApp::drawCameraStatus()
{
	ImGui::Begin("PS3 Eye Camera Status", &closeButtonOnWidgets, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Number of cameras attached: %i", numberOfCams);
	// for (each camera):
	//     resolution, refresh rate, left/right
	if (leftCam)
	{
		ImGui::Text("\nLeft Camera: ");
		ImGui::Text("Resolution: %i by %i", leftCam->cam_width, leftCam->cam_height);
		ImGui::Text("Refresh rate: %iHz", leftCam->cam_refresh);
	}
	if (rightCam)
	{
		ImGui::Text("\nRight Camera: ");
		ImGui::Text("Resolution: %i by %i", rightCam->cam_width, rightCam->cam_height);
		ImGui::Text("Refresh rate: %iHz", rightCam->cam_refresh);
	}

	ImGui::End();
}

void ofApp::saveSettings()
{
	settings.setValue("settings:swapCameras", swapCameras);
	//printf(swapCameras ? "true" : "false");
	settings.saveFile("settings.xml");

		/*
		bool leftCameraDraw = false;
		bool rightCameraDraw = false;
		bool cvDraw = false;
		float thresh1 = 90;  
		float thresh2 = 70;
		bool swapCameras;
		*/
}

/*
 * 
 */
void ofApp::loadSettings()
{
	if (settings.loadFile("settings.xml"))
	{
		printf("Successfully loaded settings.xml.");
	}
	else
	{
		printf("Couldn't load settings.xml");
	}
		//ofEnableDataPath();
	swapCameras = settings.getValue("settings:swapCameras", false);
}

/*
 * Attempts to load "singleCameraCallibration.xml"
 */
void ofApp::loadSingleCallibration()
{
	loadedSingleCallibration = settings.loadFile("singleCameraCallibration.xml");
	if (loadedSingleCallibration)
	{
		// parse through all callibration values
		// if one not present or invalid, set loadedSingleCallibration back to false
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
	ofSetWindowShape(960, 720);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::exit()
{
	//delete stereo;
	saveSettings();
	deInitCams();
	//testFloat->store(); // test out the persistent storage of values/settings
}
