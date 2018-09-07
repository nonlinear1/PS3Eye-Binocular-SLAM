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
	loadSingleCalibration();

	//stereo = cv::StereoSGBM::create(minDisp, numDisp, blockSize, P1, P2, disp12MaxDiff, preFilterCap, uniquenessRatio, speckleWindowSize, speckleRange, mode);
	stereo2 = cv::StereoBM::create(numDisp, blockSize);

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
	
	//if (!calibrateMonoWidget && !calibrateStereoWidget) // facilitates drawing captured frames w/ overlay?
	if (doOpenCvStuff)
	{
		openCvStuff();
	}
}

// toMat() function

void ofApp::openCvStuff()
{
	/*
	convertColor(leftEyeFrame, grey, CV_RGB2GRAY);
	Canny(grey, cvImage, thresh1, thresh2, 3);
	//Sobel(grey, sobel);
	grey.update();
	//sobel.update();
	cvImage.update();
	// StereoBM vs StereoSGBM vs adapt one from the papers?
	*/
	
	Mat newMat;
	undistort(leftMat, newMat, K, D);
	toOf(newMat, cvImage);
	cvImage.update();
	

	//printf("'stereo' has min disparity of: %i\n", stereo->getMinDisparity());

	
	/*
	convertColor(leftEyeFrame, lGrey, CV_RGB2GRAY);
	convertColor(rightEyeFrame, rGrey, CV_RGB2GRAY);

	Mat lleftGrey = toCv(lGrey);
	Mat rrightGrey = toCv(rGrey);

	undistort(lleftGrey, leftGrey, K, D);
	undistort(rrightGrey, rightGrey, K, D);

	//stereo->compute(leftGrey, rightGrey, dispGrey);
	stereo2->compute(leftGrey, rightGrey, dispGrey);

	//drawMat(dispGrey, 320, 0);

	toOf(dispGrey, cvImage);
	cvImage.update();*/
}

/*
 * Button in Settings widget to calibrate individual cameras. For ease we'll use one set of 
 * calibration data for both cameras -- they should be nearly identical. We'll use the 
 * single-camera calibration data to help calibrate our stereo camera setup.
 */
void ofApp::calibrateMono()
{
	ImGui::Begin("Calibrate Individual PS3EYEs", &closeButtonOnWidgets, ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::Text("Threshold 1");

	// if haven't started capturing, be text input. once click "Begin Calibration", they draw
	// as ImGui::Text, only usable inputs are "Capture Frame" and Cancel Calibration buttons 

	if (inMiddleOfCalibrating) // this is where we come once we've begun calibrating -- we enter in "else" below
	{
		// only have "Capture Frame" and "Cancel Calibration" buttons in here, but also any
		// mid-calibration output
		ImGui::Text("Captured %i of %i calibration images", numFramesCaptured, numFramesNeeded);
		ImGui::Text("Board width: %i squares", boardWidth);
		ImGui::Text("Board height: %i squares", boardHeight);
		ImGui::Text("Square size: %fmm", squareSize);
		ImGui::Text("Board area: %i squares", boardArea);

		if (numFramesCaptured >= numFramesNeeded)
		{
			// now we're ready to call the final calibration number crunching function
			monoCalibrateFinal();

			inMiddleOfCalibrating = false;
			finishedCalibrating = true;
		}
		if (ImGui::Button("Capture Frame"))
		{
			 // capture frame, send to calibration function
			 // should we store a vector of frames for reference? output them along with the calibration data?
			{
				printf("\n");
				printf(" _.---.__________|(0)|__\n");
				printf("|. . . . . . . . . . . .|\n");
				printf("|. . . . ._____. . . . .|\n");
				printf("|. . . . /     \\ . . . .|\n");
				printf("|. . . . | ( ) | . . . .|\n");
				printf("|. . . . \\_____/ . . . .|\n");
				printf("|. . . . . . . . . . . .|\n");
				printf("|_______________________|\n"); // just because it was fun :)
			}
			// capture frame to variable
			// have option to retake frame?
			// write frame to file "/calibration-images/image01.png"?
			// call function with frame as param

			numFramesCaptured +=1;
			
			//toOf(leftMat, cvImage);
			//cvImage.update();

			//monoCalibrateOnFrame(leftMat);
			monoCalibrateOnFrame();
		}
		if (ImGui::Button("Cancel Calibration"))
		{
			inMiddleOfCalibrating = false;
		}
	}
	else // this is where we come when calibrateMono() is first called, and when we're done calibrating
	{
		if (finishedCalibrating)
		{
			ImGui::Text("Warning: If saved, this batch of calibration data \nwill overwrite any previous calibration files.");
			ImGui::Text("Please backup any calibration files you wish saved.");
			if (ImGui::Button("Save Calibration File"))
			{
				// save file here
				// when saving the calibration file, give warning about overwriting previous file. 
				// maybe even a modal dialog if loadedSingleCalibration is true
				/*
				FileStorage fs("calibration.txt", FileStorage::WRITE);
				fs << "K" << K;
				fs << "D" << D;
				fs << "boardWidth" << boardWidth + 1;
				fs << "boardHeigt" << boardHeight + 1;
				fs << "squareSize" << squareSize;
				*/
				FileStorage singleCalibration("../data/singleCameraCalibration.xml", FileStorage::WRITE);
				singleCalibration << "K" << K;
				singleCalibration << "D" << D;
				singleCalibration << "boardWidth" << boardWidth + 1;
				singleCalibration << "boardHeigt" << boardHeight + 1;
				singleCalibration << "squareSize" << squareSize;
				singleCalibration << "numFramesNeeded" << numFramesNeeded;

				finishedCalibrating = false;
				calibrateMonoWidget = false;
				numFramesCaptured = 0;
				loadedSingleCalibration = true;
			}
			if (ImGui::Button("Cancel"))
			{
				finishedCalibrating = false;
				calibrateMonoWidget = false;
				numFramesCaptured = 0;
			}
		}
		else
		{
			// parameter input and "Begin Calibration" button
			// do some "ImGui::SameLine" stuff to make it look like it does mid-calibration
			ImGui::Text("Board width in squares: ");
			//ImGui::SameLine(100);
			ImGui::InputInt("##board width", &boardWidth);

			ImGui::Text("Board height in squares: ");
			//ImGui::SameLine(100);
			ImGui::InputInt("##board height", &boardHeight);

			ImGui::Text("Square side length in mm:");
			//ImGui::Text("length (mm): ");
			//ImGui::SameLine(100);
			ImGui::InputFloat("##square width", &squareSize);

			ImGui::Text("Number of calibration images");
			//ImGui::Text("images: ");
			//ImGui::SameLine(100);
			ImGui::InputInt("##number of frames", &numFramesNeeded);

			if (ImGui::Button("Begin Calibration"))
			{
				// calculate derivative values here
				boardWidth -= 1;  // we subtract 1 to count interior corners;
				boardHeight -= 1; // only want points where 4 squares meet
				boardSize = Size(boardWidth, boardHeight); 
				boardArea = boardWidth * boardHeight;
				inMiddleOfCalibrating = true;
			}
			if (ImGui::Button("Cancel"))
			{
				calibrateMonoWidget = false;
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
 * Modified from https://sourishghosh.com/2016/camera-calibration-cpp-opencv/ 
 * and https://github.com/sourishg/stereo-calibration/blob/master/calib_intrinsic.cpp
 */
void ofApp::monoCalibrateOnFrame()//(Mat ourFrame)
{
	Mat grey;
	//cvtColor(ourFrame, grey, CV_RGB2GRAY);
	cvtColor(leftMat, grey, CV_RGB2GRAY);
	toOf(grey, cvImage);
	cvImage.update();

	bool found = false;
	found = findChessboardCorners(grey, boardSize, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
	
	if (found)
	{
		cornerSubPix(grey, corners, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
		drawChessboardCorners(grey, boardSize, corners, found);
		toOf(grey, cvImage);
		cvImage.update();
	}
	else
	{
		//printf("No chessboard corners found :/\n");
		numFramesCaptured -= 1; // if this frame didn't work, we'll have to capture another one.
	}

	vector<Point3f> objPts;

	for (int i = 0; i < boardHeight; i++)
	{
		for (int j = 0; j < boardWidth; j++)
		{
			objPts.push_back(Point3f((float)j * squareSize, (float)i * squareSize, 0));
		}
	}

	if (found)
	{
		imagePoints.push_back(corners);
		objectPoints.push_back(objPts);
	}

}

void ofApp::monoCalibrateFinal()
{
	int flag = 0;
	flag |= CV_CALIB_FIX_K4;
	flag |= CV_CALIB_FIX_K5;

	calibrateCamera(objectPoints, imagePoints, leftMat.size(), K, D, rotationVectors, translationVectors, flag);
}

/*
 * Have button in Settings widget to calibrate, when pressed it launches calibrateStereo()
 * calibrateStereo() 
 */
void ofApp::calibrateStereo()
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

	if (calibrateMonoWidget)
	{
		calibrateMono();
	}

	
	if (calibrateStereoWidget)
	{
		calibrateStereo();
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
	

	if (ImGui::Button("Calibrate individual PS3 Eye"))
	{
		calibrateMonoWidget = true;
	}

	if (ImGui::Button("Calibrate Stereo PS3 Eye Setup"))
	{
		calibrateStereoWidget = true;
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
	ImGui::Text("Individual camera calibration %s", loadedSingleCalibration ? "is loaded" : "is not loaded");
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
		printf("Successfully loaded settings.xml\n");
	}
	else
	{
		printf("Couldn't load settings.xml\n");
	}
		//ofEnableDataPath();
	swapCameras = settings.getValue("settings:swapCameras", false);
}

/*
 * Attempts to load "singleCameraCalibration.xml"
 */
void ofApp::loadSingleCalibration()
{
	//loadedSingleCalibration = settings.loadFile("singleCameraCalibration.xml");
	FileStorage singleCalibration;
	loadedSingleCalibration = singleCalibration.open("../data/singleCameraCalibration.xml", FileStorage::READ);
	if (loadedSingleCalibration)
	{
		// parse through all calibration values
		// if one not present or invalid, set loadedSingleCalibration back to false
		singleCalibration["K"] >> K;
		singleCalibration["D"] >> D;
		singleCalibration["boardWidth"] >> boardWidth;
		singleCalibration["boardHeight"] >> boardHeight;
		singleCalibration["squareSize"] >> squareSize;
		printf("Successfully loaded singleCameraCalibration.xml\n");
	}
	else
	{
		printf("Couldn't load singleCameraCalibration.xml\n");
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
	delete stereo2;
	saveSettings();
	deInitCams();
}
