#pragma once

#include "ofMain.h"
#include "ofxLaserManager.h"
#include "ofxLaserDacEtherdream.h"
#include "ofxGui.h"
#include "ofxSvg.h"
#include "ofxLaserGraphic.h"
#include "Timeline.h"
#include "CueSound.h"
#include "CueLaserGraphicTransition.h"
#include "CueLaserAnimation.h"



class ofApp : public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void mouseDragged( ofMouseEventArgs & mouse );
	void mousePressed( ofMouseEventArgs & mouse );
	void mouseReleased(ofMouseEventArgs & mouse );
	//void touchDown(ofTouchEventArgs &touch);
	
	void loadDirectory(string dirname, int tracknum);
	
	void keyPressed  (int key);
	
	
    vector<string> fileNames;
	
	ofxLaser::Manager laser;
	ofxLaser::DacEtherdream dac1;
	
	int laserWidth;
	int laserHeight;
	
	Timeline timeline;
	CueSound musicCue;
	
    vector<CueLaserGraphic*> laserGraphicCues;
	
};

