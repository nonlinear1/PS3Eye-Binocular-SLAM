#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	using namespace ps3eye;
	//using namespace ofxCv;
	//using namespace cv;

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

	//cam1.setup(640, 480);

	initCams();

	// let image handlers manage getting images -- from cameras, from disk -- always two (right is null if need be)
	// but for now just load a file
	leftFrameHandler();
	
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
		firstCam = new scannerEye();
		firstCam->initCam(0, 640, 480, 60);
		if (numberOfCams > 1)
		{
			secondCam = new scannerEye();
			secondCam->initCam(1, 640, 480, 60);
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
	if (firstCam)
	{
		firstCam->deInitCam();
		delete firstCam;
	}
	if (secondCam)
	{
		secondCam->deInitCam();
		delete secondCam;
	}

}

void ofApp::leftFrameHandler()
{
	leftFrame.load("./images/quack.jpg");

	//leftFrame = new CVEye(0, ((PS3EyePlugin *) eyeDriver), true);
}

void ofApp::leftFrameDraw()
{
	leftFrame.draw(100, 100, cam_width, cam_height); // left frame at (0,0) right frame at (640,0)
	//leftDrawFrame.draw(0, 0, 640, 480); 
	leftDrawFrame.loadData(leftEyeFrame);
	leftDrawFrame.draw(0, 0, 640, 480);

	//convertColor(leftDrawFrame, gray, CV_RGB2GRAY);
	//Canny(gray, edge, mouseX, mouseY, 3);
	//Sobel(gray, sobel);
	//gray.update();
	//sobel.update();
	//edge.update();



	// make a shader? three layers -- 0) color image 1) opencv feature overlay
	// maybe shader for getting greyscale too? so 0) color 1) greyscale by averageing color 2) opencv 
}

//--------------------------------------------------------------
void ofApp::update(){
	//cam1.update();
	//unsigned char* leftPixels = NULL;
	//ps3eye_grab_frame(firstCam, leftPixels);
	ps3eye_grab_frame(firstCam->camInstance, leftPixels);
	//ps3eye_grab_frame(firstCam, leftEyeFrame.getData());

	leftEyeFrame.setFromPixels(leftPixels, 640, 480, 3);

	//firstCam->ps32eyeref->getLastFramePointer();
	// iterate over unsigned char* and add to ofPixels?
	/*for (int i = 0; i < (cam_width*cam_height * 3); i++)
	{
		leftEyeFrame[i] = leftPixels[i];
	}*/


}

//--------------------------------------------------------------
void ofApp::draw(){
	//cam1.draw(0, 0);
	
	leftFrameDraw();
	

	gui.begin();

	if (camStatus)
	{
		ps3EyeStatusDraw();
	}

	// main settings widget
	
	if (settingsWidget) // replace with checkbox bool for testing
	{
		drawSettingsWidget();
		// overall settings widget, includes sub-widgets
		// for each Eye: draw settings widget
	}
	gui.end();

	// let display handlers manage displaying images -- default to displaying both image sources, left is first if only one present --
	// but for now just display one image directly
	//prepareRenderField();
	
}

/*
void ofApp::prepareRenderField()
{

}
*/


void ofApp::drawSettingsWidget()
{
	{
	ImGui::Begin("Settings");
	ImGui::Text("Float value: %.2f", testFloat);
	ImGui::SliderFloat(" ", &testFloat, 0.0f, 100.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	}
}

void ofApp::ps3EyeStatusDraw()
{
	ImGui::Begin("PS3 Eye Status");
	ImGui::Text("Number of cameras attached: %i", numberOfCams);
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
