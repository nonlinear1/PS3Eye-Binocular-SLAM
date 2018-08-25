#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		//void drawSettingsWidget();

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
		//void exit();

		ofxImGui::Gui gui;
		ImVec4 backgroundColor;
		
		//ofVideoGrabber cam1;

		//ofImage leftFrame;
		//ofImage rightFrame;

		//float testFloat = 23.0f;
		//float x;
		//bool settingsWidget = true;
};
