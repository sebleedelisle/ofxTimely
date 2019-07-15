#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	soundCue.load("HOCTheme.mp3");
	soundCue.setName("Music");
	
	soundCue.setStart(0);
	soundCue.autoSetEnd();
	soundCue.setLocked(true);
	timeline.addCue(soundCue);
	
	CueBase& cue = *(new CueBase());
	cue.setStart(1);
	cue.setEnd(2);

	timeline.addCue(cue,1);
	
	timeline.loadSettings();
	//timeline.locked = true;
}

//--------------------------------------------------------------
void ofApp::update(){
	timeline.update(ofGetLastFrameTime());
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	float border = 5;
	timeline.draw(border,ofGetHeight()-border, ofGetWidth()-(border*2));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key==' ') timeline.togglePlayPause();
	if(key==OF_KEY_RIGHT) timeline.skipForward(ofGetKeyPressed(OF_KEY_SHIFT));
	if(key==OF_KEY_LEFT) timeline.skipBack(ofGetKeyPressed(OF_KEY_SHIFT));

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
