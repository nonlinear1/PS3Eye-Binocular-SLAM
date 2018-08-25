#include "ofMain.h"
#include "ofApp.h"
//#include "ofGLProgrammableRenderer.h" // what's this do again?

//========================================================================
int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
	

	// PS3Eye-Binocular-SLAM
	/*
	 *	Step 1) Mount two PS3 Eye cameras on a rigid board. 
	 *	Step 2) Slowly wave board around room. 
	 *	Step 3) ??? 
	 *	Step 4) Look at shiny new photo-textured 3D model of room. 
	 *
	 *	This project is primarily concerned with Step 3, but will additionally include notes from steps 1, 2, and 4.
	 */
}
