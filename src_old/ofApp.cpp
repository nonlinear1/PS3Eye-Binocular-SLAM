#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	gui.setup();

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
	backgroundColor = ofColor(114, 144, 154);

	// setup GUI

	//cam1.setup(640, 480);
	//x = 12.0f;
}

//--------------------------------------------------------------
void ofApp::update(){
	//cam1.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	//cam1.draw(0, 0);

	//gui.begin();

	// main settings widget
	//if (settingsWidget) // replace with checkbox bool for testing
	//{
		///*
		//ImGui::Begin("Settings");
		//ImGui::Text("Float value: %.2f", testFloat);
		//ImGui::Text("X = %.2f", x);
		//ImGui::SliderFloat("", &testFloat, 0.0f, 100.0f);
		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); 
		//*/
		//drawSettingsWidget();
	//}
	//gui.end();
}

/*void drawSettingsWidget()
{
	ImGui::Begin("Settings");
	ImGui::Text("Float value: %.2f", testFloat);
	ImGui::SliderFloat("", &testFloat, 0.0f, 100.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}*/

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

/*void ofApp::exit()
{
	//testFloat->store(); // test out the persistent storage of values/settings
}*/
