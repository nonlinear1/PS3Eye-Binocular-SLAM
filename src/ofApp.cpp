#include "ofApp.h"

using namespace ps3eye;
using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){

	gui.setup();
	ofSetWindowTitle("prototype build--bmp18");
	ofSetDataPathRoot("../data/");
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
 * For now this directly initializes one camera. Soon: listen for all ps3eye cams, and tell theem to init themselves
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

		if (numberOfCams > 1)
		{
			rightCam = new scannerEye();
			rightCam->initCam(1, 640, 480, 60);
			rightPixels = new unsigned char[rightCam->cam_width * rightCam->cam_height * 3];
			rightCameraDraw = true;
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
	leftDrawFrame.loadData(leftEyeFrame);
	leftDrawFrame.draw(0, 0, leftCam->cam_width, leftCam->cam_height);

	//convertColor(leftDrawFrame, gray, CV_RGB2GRAY);
	//Canny(gray, edge, mouseX, mouseY, 3);
	//Sobel(gray, sobel);
	//gray.update();
	//sobel.update();
	//edge.update();



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

	rightDrawFrame.loadData(rightEyeFrame);
	rightDrawFrame.draw(leftCam->cam_width, 0, rightCam->cam_width, rightCam->cam_height);
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
	openCvStuff();
}

// toMat() function

void ofApp::openCvStuff()
{
	convertColor(leftEyeFrame, grey, CV_RGB2GRAY);
	Canny(grey, edge, thresh1, thresh2, 3);
	//Sobel(grey, sobel);
	grey.update();
	//sobel.update();
	edge.update();
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
	grey.draw(0, 480);
	edge.draw(640, 480);

	gui.begin();

	{
		ImGui::Begin("Edge Detection Settings");
		ImGui::SliderFloat("Threshold 1", &thresh1, 0.0f, 250.0f);
		ImGui::SliderFloat("Threshold 2", &thresh2, 0.0f, 500.0f);
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
	gui.end();

	// let display handlers manage displaying images -- default to displaying both image sources, left is first if only one present --
	// but for now just display one image directly
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
	ImGui::Begin("Camera Settings", false, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	//ImGui::Text("Float value: %.2f", testFloat);
	// disable right camera draw if only one camera attached? include selection and swap
	// button in IF (more than 1 camera):
	ImGui::Checkbox("Left Camera Draw", &leftCameraDraw);
	ImGui::Checkbox("Right Camera Draw", &rightCameraDraw);
	// if more than one camera: 

	if (ImGui::Button("Swap Left and Right Cameras"))
	{
		swap(leftCam, rightCam);
		//printf("SWAPP'D");
	}
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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
	ImGui::Begin("PS3 Eye Camera Status", false, ImGuiWindowFlags_AlwaysAutoResize);
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::exit()
{
	deInitCams();
	//testFloat->store(); // test out the persistent storage of values/settings
}
