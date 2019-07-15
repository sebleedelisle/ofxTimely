#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
	
	loadDirectory("svgs/Texts/", 1);
	loadDirectory("svgs/Graphics/", 2);
	
    // set up the laser manager
    
	laserWidth = 800;
	laserHeight = 600;
	laser.setup(laserWidth, laserHeight);
	
    // load the IP address of the Etherdream
    ofBuffer buffer = ofBufferFromFile("dacIP1.txt");
    string dacIp1 = buffer.getText();
	
	ofLog(OF_LOG_NOTICE, "Dac1 IP : " + dacIp1); 
	// if there's no file, then use the default IP address :
    if(dacIp1=="") dacIp1 ="10.0.1.49";
    dac1.setup(dacIp1);

	laser.addProjector(dac1);
	
	ofxGuiSetFont("Verdana.ttf", 9);
	
    laser.initGui();
    
    ofParameterGroup params;
	
    laser.gui.add(params);

	
	
	musicCue.load("EdithPiaf.mp3");
	musicCue.setName("Music");
	
	musicCue.setStart(0);
	musicCue.autoSetEnd();
	musicCue.setLocked(true);
	timeline.addCue(musicCue);
	
	timeline.loadSettings();
	
	laser.gui.add(timeline.locked);

	 
}

//--------------------------------------------------------------
void ofApp::update(){
    laser.update();
	timeline.update(ofGetLastFrameTime());
}


void ofApp::draw() {
	

	ofBackground(0);
    
	//ofxLaser::Graphic& laserGraphic = laserGraphics[currentSVG];
	
	ofNoFill();
	ofSetLineWidth(1);
	//ofDrawRectangle(0,0,laserWidth, laserHeight);
    
	int ypos = laserHeight+10;
    int xpos = 360;
	//ofDrawBitmapString("Current SVG : "+ofToString(currentSVG) + " "+fileNames[currentSVG], xpos, ypos+=30);
//
//	ofDrawBitmapString("Left and Right Arrows to change current SVG", xpos, ypos+=30);
//    ofDrawBitmapString("'F' to toggle fullscreen", xpos, ypos+=20);
//    ofDrawBitmapString("'TAB' to toggle laser preview mode", xpos, ypos+=20);
//    ofDrawBitmapString("Adjust Render Profiles -> Default to affect laser speed", xpos, ypos+=20);
//
	
    int laserframerate = laser.getProjectorFrameRate(0); 
    ofDrawBitmapString("Laser framerate  : " + ofToString(laser.getProjectorFrameRate(0))+ " " + ofToString(laser.getProjectorFrameRate(1)), 20, 20);
	ofDrawBitmapString("Render framerate : " + ofToString(ofGetFrameRate()), 20, 35);
	
	
	
    laser.send();
    laser.drawUI();
	
	float border = 5;
	timeline.draw(border,ofGetHeight()-border, ofGetWidth()-(border*2));
	ofFill(); 
	
//	ofPushMatrix();
//	ofTranslate(100,100);
//	test1->svg.draw();
//	ofTranslate(0,200);
//	test2->svg.draw();
//	ofPopMatrix();
//	//needABeer.
	

}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
   
//   if(key =='f') {
//        ofToggleFullscreen();
//    } else if (key == OF_KEY_LEFT) {
//        currentSVG--;
//        if(currentSVG<0) currentSVG = laserGraphics.size()-1;
//    } else if (key == OF_KEY_RIGHT) {
//        currentSVG++;
//        if(currentSVG>=laserGraphics.size()) currentSVG = 0;
//    }
//
    
    if(key==' ') timeline.togglePlayPause();
	if(key==OF_KEY_RIGHT) timeline.skipForward(ofGetKeyPressed(OF_KEY_SHIFT));
	if(key==OF_KEY_LEFT) timeline.skipBack(ofGetKeyPressed(OF_KEY_SHIFT));
	
    if(key=='f') ofToggleFullscreen();
    if(key==OF_KEY_TAB) laser.nextProjector();

}


void ofApp :: loadDirectory(string path, int tracknum) {
	
	// iterates through the contents of a folder. If it finds an SVG it loads it as a CueLaserGraphic,
	// if it finds a directory, it loads it as a CueLaserAnimation.
	// get the filenames of all the svgs in the data/svgs folder
	//string path = "svgs/Texts/";
	ofDirectory dir(path);
	dir.allowExt("svg");
	dir.listDir();
	dir.sort();
	
	// and load them all
	vector<ofFile> files = dir.getFiles();
	ofSort(files, SVGLoader::sortalgo);
	for(int i = 0; i<files.size();i++) {
		const ofFile & file = files.at(i);
		
		//ofxSVG svg;
		//svg.load(file.getAbsolutePath());
		ofLog(OF_LOG_NOTICE,file.getAbsolutePath());
		CueLaserGraphicTransition* laserGraphicCue = new CueLaserGraphicTransition();
		
		//laserGraphicCues.push_back(laserGraphicCue);
		
		laserGraphicCue->loadFromSVG(file.getAbsolutePath(), tracknum-1);
		laserGraphicCue->setName(file.getBaseName());
		
		// TODO : find next empty position on timeline?
		
		laserGraphicCue->setStart((i)*4+12);
		laserGraphicCue->setDuration(3);
		
//		laserGraphicCue->targetZone =
		
		timeline.addCue(*laserGraphicCue,tracknum);
		
		fileNames.push_back(file.getFileName());
		
		ofLog(OF_LOG_NOTICE, laserGraphicCue->type);
		
	}
	
	dir = ofDirectory(path);
	dir.listDir();
	const vector<ofFile>& allFiles = dir.getFiles();
	
	vector<ofFile> directories;
	for(int i = 0; i<allFiles.size(); i++) {
		if(allFiles[i].isDirectory()) {
			directories.push_back(allFiles[i]);
		}
		
	}
	ofSort(directories, SVGLoader::sortalgo);
	float cuetime = 0;
	for(int i = 0; i<directories.size(); i++) {
		
		//ofDirectory subdir = ofDirectory(directories[i].getAbsolutePath());
		
		CueLaserAnimation &cue = *(new CueLaserAnimation());
		
		cue.loadSVGSequence(directories[i].getAbsolutePath(), tracknum-1);
		cue.setStart(cuetime);
		cuetime+=cue.getDuration();
		
		cue.setName(directories[i].getBaseName());
		timeline.addCue(cue, tracknum);
		
		ofLog(OF_LOG_NOTICE, cue.type); 
		
	}
	
}


//--------------------------------------------------------------
void ofApp::exit(){
    laser.saveSettings();
    dac1.close();

}


void ofApp::mouseDragged( ofMouseEventArgs & mouse ){
	
}
void ofApp::mousePressed( ofMouseEventArgs & mouse ){
	
	
}
void ofApp::mouseReleased(ofMouseEventArgs & mouse ){
	
}
