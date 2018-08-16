#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	gui.setup();

	ImGui::GetIO().MouseDrawCursor = false;
	//backgroundColor is stored as an ImVec4 type but can handle ofColor
	backgroundColor = ofColor(114, 144, 154);

	cam1.setup(640, 480);
}

//--------------------------------------------------------------
void ofApp::update(){
	cam1.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	cam1.draw(0, 0);
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
