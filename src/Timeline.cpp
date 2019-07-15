//
//  Timeline.cpp
//  BecHillLasers
//
//  Created by Seb Lee-Delisle on 28/07/2018.
//
//

#include "Timeline.h"

Timeline::Timeline() {
	initListeners();
	offset.x = 0;
	offset.y = 0;
	
	scale.x = 1;
	scale.y = 1;
	
	fps = 30;
	snapToFrame = true;
	locked.set("Lock timeline", false);
}

Timeline:: ~Timeline() {
	removeListeners();
}

bool Timeline::play(){
	isPlaying = true;
	
	// start cues?
    for(int i = 0; i<cues.size(); i++) {
        cues[i]->start(playheadTime);
    }
	return isPlaying;
}

bool Timeline::pause(){
	isPlaying = false;
	// stop cues?
    for(int i = 0; i<cues.size(); i++) {
        cues[i]->stop(playheadTime);
    }
	return isPlaying;

}
bool Timeline::togglePlayPause(){
	if(isPlaying) return pause();
	else return play();
}
bool Timeline::getIsPlaying(){
	return isPlaying;
}

void Timeline::skipForward(bool fast){
	float skipTime = 1.0f/30.0f;
	if(fast) skipTime = 1;
	playheadTime+=skipTime;
	if(playheadTime>duration) {
		playheadTime = duration;
		pause();
	}
}
void Timeline::skipBack(bool fast){
	float skipTime = 1.0f/30.0f;
	if(fast) skipTime = 1;
	playheadTime-=skipTime;
	if(playheadTime<0){
		playheadTime = 0;
	}
}

void Timeline::addCue(CueBase& cue, int track){
	cues.push_back(&cue);
	cue.trackNum = track;
	if(cue.getEnd()>duration) duration = cue.getEnd();
	
	if(track+1>numTracks) numTracks = track+1;
	cue.params.setName(cue.getName());
	guiSettings.add(cue.params);
}

void Timeline::setNumTracks(int num){
	numTracks = num; 
}

void Timeline::update(float deltaTime){
	if(getIsPlaying()) {
		playheadTime+=deltaTime;
		if(playheadTime>=duration){
			pause();
			playheadTime = duration;
		}
	}

	for(int i = 0; i<cues.size(); i++) {
		if(cues[i]->trackNum>=numTracks) numTracks=cues[i]->trackNum+1;
		cues[i]->update(playheadTime, getIsPlaying());
	}

	
}


void Timeline::draw(float x, float y, float w){

	// x / y is the bottom left. The height is determined by the track height.
	
	ofPushStyle();
	ofFill();
	
	// to do - that magic 4 is to allow for the playhead arrow
	timelineRect.set(x,y,w,-(trackHeight+border)*(numTracks)-4);
	timelineRect.standardize();
	
	if(locked) {
		
		ofRectangle bgrect(timelineRect);
		//bgrect.height+=3;
		ofPushStyle();
		ofSetColor(100,0,0);
		ofNoFill();
		ofSetLineWidth(2);
		ofDrawRectangle(bgrect);
		ofFill();
		
		ofDrawRectangle(bgrect);
		ofPopStyle();
	}
	
	
	
	updateTrackRects();
	trackColour = ofColor(20,30,60); // locked?  : ofColor(255,0,0);
	ofSetColor(trackColour);
	for(int i = 0; i<numTracks; i++) {
		ofDrawRectangle(x, y - ((i+1)*(trackHeight+border)), w, trackHeight);
		
	}

	
	
	for(int i = 0; i<cues.size(); i++) {
		CueBase & cue = *cues[i];
		
		ofRectangle& trackrect = trackRects[cue.trackNum];
		
		
		float left = timeToPosition(cue.getStart());// , 0, getDuration(), trackrect.getLeft(), trackrect.getRight());
		float width = timeToPosition(cue.getEnd()) - left;//, 0, getDuration(), 0,trackrect.getWidth());
		
		//cue.drawUI(x, , w, h, getDuration());

		if(!cue.isDragging) {
			
			if(width<5) width = 5;
			cue.rect.set(left, y - ((cue.trackNum+1)*(trackHeight+border)), width, trackHeight);
		}
		
//		// update the parameter limits... do we need this?
//		cue.startTime.setMin(0);
//		cue.startTime.setMax(getDuration());
//		cue.duration.setMin(0);
//		cue.duration.setMax(15);
//
		
		renderCue(cue);
		if(cue.isSelected()) {
			mouseIsOverGui = drawCueGui(cue);
		} else mouseIsOverGui = false;
		if(mouseIsOverGui) {
			//[[NSCursor pointingHandCursor] set];
		} else {
			//[[NSCursor arrowCursor] set];
		}
	}
	
	// update playhead
	
	ofRectangle& trackRect = trackRects[0];
	ofSetColor(ofColor::white);

	
	float playheadX = timeToPosition(playheadTime); // ofMap(playheadTime, 0, getDuration(), trackRect.getLeft(),trackRect.getRight()-2);
	
	float playheadHeight = ((numTracks+1)*(trackHeight+border)) - border;
	float playheadY = y+trackHeight - playheadHeight;
	ofDrawRectangle(playheadX, playheadY, 2, playheadHeight);
	float hsize = 6;
	float vsize = 4;
	ofDrawTriangle(playheadX+1, playheadY+1, playheadX+1-hsize, playheadY-vsize, playheadX+1+hsize, playheadY-vsize);
	playheadDraggable.rect.set(playheadX+1-hsize,playheadY-vsize, hsize*2, vsize*2);
	
	ofNoFill();
	ofSetColor(255,0,0);
	ofDrawRectangle(playheadDraggable.rect);
//
	
	
	checkBounds();
	
	if(ofGetElapsedTimef() - lastTimelineMove<2.0f) {
		ofFill();
		ofRectangle visiblerect = timelineRect;
		visiblerect.scale(scale);
		visiblerect.translate(offset);
		float scrollindicatorwidth = (timelineRect.getWidth()/visiblerect.getWidth())*timelineRect.getWidth();
		float scrollindicatorx = ofMap(timelineRect.getLeft(), visiblerect.getLeft(), visiblerect.getRight(), timelineRect.getLeft(), timelineRect.getRight());
		if(scrollindicatorwidth<timelineRect.getWidth()) {
			ofSetColor(255,ofMap(ofGetElapsedTimef() - lastTimelineMove, 1,1.2,100,0,true));
			ofDrawRectRounded(scrollindicatorx,y-2,scrollindicatorwidth, 4,2);
		}
	}
	ofPopStyle();
	

	
}

void Timeline:: updateTrackRects() {
	if(trackRects.size()!=numTracks) trackRects.resize(numTracks);
	
	//timelineRect.set(x,y-4,w,-(trackHeight+border)*(numTracks));
	float x = timelineRect.getLeft();
	float y = timelineRect.getBottom();
	float w = timelineRect.getWidth();
	for(int i = 0; i<numTracks; i++) {
		trackRects[i].set(x, y - ((i+1)*(trackHeight+border)), w, trackHeight);
		
		trackRects[i].scale(scale);
		trackRects[i].translate(offset);
		
		
	}
}


float Timeline::timeToPosition(float time){
	ofRectangle & trackrect = trackRects[0];
	return ofMap(time, 0, getDuration(), trackrect.getLeft(), trackrect.getRight());
}
float Timeline::positionToTime(float position){
	ofRectangle & trackrect = trackRects[0];
	return ofMap(position, trackrect.getLeft(), trackrect.getRight(), 0, duration);
}

void Timeline::renderCue(CueBase& cue) {
	
	cue.drawCueBlock(); // add any cue specific details here
	
}

float Timeline::getPlayheadTime(){
	return playheadTime;
}

void Timeline::setPlayheadTime(float pos){
	playheadTime = pos;
}

bool Timeline :: mousePressed(ofMouseEventArgs &e){
	glm::vec3 mousePoint(e.x, e.y, 0);

	isMouseDown = true;

	if(!mouseIsOverGui) {
		deselectCues();
	}
	
	bool cueHit = false;
	
	if(timelineRect.inside(mousePoint)) {

		
		if(!locked) {
			for(int i = cues.size()-1; i>=0; i--) {
				CueBase& cue = *cues[i];
				
				if(cue.hitTest(mousePoint)) {
					
					cue.setSelected(true);
					
					if(!cue.isLocked()) startDragging(i, mousePoint);
					cueHit = true;
					break;
				}
			}
			
			sortCues();
		}
		
		if(cueHit) {
			
		} else if(playheadDraggable.hitTest(mousePoint)) {
			playheadDraggable.startDrag(mousePoint);
		
		} else {
			// drag the timeline
			clickPosition = mousePoint;
			startOffset = offset;
			isDragging = true;
		}
	}
	
	return cueHit;
}


bool Timeline :: mouseDragged(ofMouseEventArgs &e){
	glm::vec3 mousePoint(e.x, e.y, 0);
	//mousePoint/=scale;
	//mousePoint-=offset;

	bool dragging = false;
	for(int i = 0; i<cues.size(); i++) {
		if(cues[i]->updateDrag(mousePoint)) {
			CueBase& cue = *cues[i];
			dragging = true;
			ofRectangle& trackrect = trackRects[cue.trackNum];
			
			updateCueTimeForPosition(cue);
			
			
			//			cout << cue.rect.getLeft() << " " << trackrect.getLeft() << " " << trackrect.getRight() << " " << getDuration() << " " << cue.getStart() << endl;
			
		}
		//bounds.growToInclude(handles[i]);
	}
	if(dragging) {
		sortCues();
	} else if(playheadDraggable.updateDrag(mousePoint)) {
		// if we're dragging the playhead
		ofRectangle& trackRect = trackRects[0];
		playheadTime = positionToTime(playheadDraggable.rect.getCenter().x);// ofMap(playheadDraggable.rect.getCenter().x, trackRect.getLeft(),trackRect.getRight()-2, 0, getDuration());
		
	} else if(isDragging) {
		// if we're dragging the timeline...
		mousePoint = glm::vec3(e.x, e.y, 0);
		offset.x = startOffset.x + (mousePoint.x - clickPosition.x);
		checkBounds();
		lastTimelineMove = ofGetElapsedTimef();
	}
	return dragging;
	
}

void Timeline::updateCueTimeForPosition(CueBase& cue) {
	
	
	float startTime =positionToTime(cue.rect.getLeft());
	if(snapToFrame) {
		float framenum = round(startTime*fps);
		startTime = framenum/fps;
		cue.rect.setX(timeToPosition(startTime)); 
	}
	
	cue.setStart(startTime);
}
	
	
bool Timeline :: mouseReleased(ofMouseEventArgs &e){
	
	isMouseDown = false;
	
	glm::vec3 mousePoint(e.x, e.y, 0);
	//mousePoint/=scale;
	//mousePoint-=offset;

	float time = ofGetElapsedTimef();
	bool wasDragging = false;
	
	for(int i = 0; i<cues.size(); i++) {
		CueBase& cue = *cues[i];
		if(cue.stopDrag()) {
			wasDragging = true;
			ofRectangle& trackrect = trackRects[cue.trackNum];
			//cue.setStart(ofMap(cue.rect.getLeft(), trackrect.getLeft(), trackrect.getRight(), 0,getDuration()));
			
			//cue.rect.set(left, y - (cue.tracknum*(h+border)), width, h);
		}
	}
//	ofLog(OF_LOG_NOTICE, "Cue update took : " + ofToString(ofGetElapsedTimef()-time));
	
	
    if(wasDragging) {
		//time = ofGetElapsedTimef();
		saveSettings();
		//ofLog(OF_LOG_NOTICE, "saveSettings took : " + ofToString(ofGetElapsedTimef()-time));
		//time = ofGetElapsedTimef();
		
        sortCues();
		//ofLog(OF_LOG_NOTICE, "cue sort took : " + ofToString(ofGetElapsedTimef()-time));
		//time = ofGetElapsedTimef();
		
    }
	if(isDragging) {
		isDragging = false;
	}
	playheadDraggable.stopDrag();
	return wasDragging;
	
}

bool Timeline :: mouseScrolled(ofMouseEventArgs &e) {
	//ofLog(OF_LOG_NOTICE, ofToString(e.scrollY));
	
	glm::vec3 mousePoint(e.x, e.y, 0);
	
//	mousePoint/=scale;
//	mousePoint-=offset;
	
	if(!isMouseDown && timelineRect.inside(mousePoint)) {
	
		
		if(e.scrollY!=0) {
//			if(ofElapsedTimef()-lastScrollYTime>0.2) {
//				scrollY = 0;
//			} else {
			
				float mouseTime = positionToTime(mousePoint.x);
				scale.x+=(e.scrollY*0.1);
				updateTrackRects();
				float newMouseTime = positionToTime(mousePoint.x);
				float timediff = newMouseTime - mouseTime;
				float posdiff = timeToPosition(timediff)-trackRects[0].getLeft();
				offset.x+=posdiff;
				updateTrackRects();
//			}
		}
		
		offset.x+=(e.scrollX * scale.x);
		checkBounds();
		lastTimelineMove = ofGetElapsedTimef();

		
	}

	
	return true;
	
}

void Timeline::checkBounds() {
	
	if(offset.x>0) offset.x = 0;
	
	float minscale = 1;//timelineRect.getWidth()/  getDuration();
	if(scale.x<minscale) scale.x = minscale;
	ofRectangle visiblerect = timelineRect;
	visiblerect.scale(scale);
	visiblerect.translate(offset);
	if(visiblerect.getRight()<timelineRect.getRight()) {
		offset.x += timelineRect.getRight()-visiblerect.getRight();
		
	}
	
//	ofLog(OF_LOG_NOTICE, "minscale : " + ofToString(minscale));
//	ofPushStyle();
//	ofNoFill();
////	ofSetColor(0,255,0);
////	ofDrawRectangle(visiblerect);
//	ofPopStyle();
	
	
}
//
//void  Timeline ::updateTimelineRect(){
//
//
//}


void  Timeline ::startDragging(int cueIndex, glm::vec3 clickPos) {
	
	cues[cueIndex]->startDrag(clickPos, true, false);
	
	
}

void Timeline::deselectCues() {
	for(int i = 0; i<cues.size(); i++) {
		cues[i]->setSelected(false);
	}
	//sortCues();
}


void Timeline::sortCues() {
	std::sort(cues.begin(), cues.end(), Timeline::sortAlgorithm );
}
void Timeline :: initListeners() {
	
	ofAddListener(ofEvents().mousePressed, this, &Timeline::mousePressed, OF_EVENT_ORDER_BEFORE_APP);
	ofAddListener(ofEvents().mouseReleased, this, &Timeline::mouseReleased, OF_EVENT_ORDER_BEFORE_APP);
	ofAddListener(ofEvents().mouseDragged, this, &Timeline::mouseDragged, OF_EVENT_ORDER_BEFORE_APP);
	ofAddListener(ofEvents().mouseScrolled, this, &Timeline::mouseScrolled, OF_EVENT_ORDER_BEFORE_APP);

	
}

void Timeline :: removeListeners() {
	
	ofRemoveListener(ofEvents().mousePressed, this, &Timeline::mousePressed, OF_EVENT_ORDER_BEFORE_APP);
	ofRemoveListener(ofEvents().mouseReleased, this, &Timeline::mouseReleased, OF_EVENT_ORDER_BEFORE_APP);
	ofRemoveListener(ofEvents().mouseDragged, this, &Timeline::mouseDragged, OF_EVENT_ORDER_BEFORE_APP);
	ofRemoveListener(ofEvents().mouseScrolled, this, &Timeline::mouseScrolled, OF_EVENT_ORDER_BEFORE_APP);

}


bool Timeline :: saveSettings(){
//	ofxPanel gui;
//
//	ofParameterGroup params;
//
//	for(int i = 0; i<cues.size(); i++) {
//		cues[i]->params.setName(cues[i]->getName());
//		params.add(cues[i]->params);
//
//	}
//
//	gui.add(params);
	guiSettings.saveToFile("timeline.json");
//	gui.clear();
	
	
}
bool Timeline :: loadSettings(){
//
//	ofxPanel gui;
//
//	ofParameterGroup params;
//
//
//	for(int i = 0; i<cues.size(); i++) {
//		cues[i]->params.setName(cues[i]->getName());
//		params.add(cues[i]->params);
//
//	}
//
//	gui.add(params);
//	gui.loadFromFile("timeline.json");
//
//
	
	guiSettings.loadFromFile("timeline.json");

	sortCues();
	
	
}


//--------------------------------------------------------------
bool Timeline::drawCueGui(CueBase& cue) {
    ofxImGui::Settings guiSettings = ofxImGui::Settings();
	
	cueGuiRect.set(cue.rect.getCenter().x, cue.rect.getTop() - 200,800,300);
	if(cueGuiRect.getRight()>ofGetWidth()) cueGuiRect.x = ofGetWidth()-cueGuiRect.getWidth();
	guiSettings.windowSize.set(cueGuiRect.getWidth(), cueGuiRect.getHeight());
	guiSettings.windowPos = cueGuiRect.getTopLeft();
	guiSettings.lockPosition = true;
	
	gui.begin();
	
	
	if (ofxImGui::BeginWindow(cue.getName(), guiSettings, false))
	{

//		if (ofxImGui::BeginTree("Settings", mainSettings))
//		{
	
			for(int i = 0; i<cue.params.size(); i++) {
				
				string paramtype = cue.params.getType(i);
				if(paramtype == typeid(ofParameter<float>).name()	) {
					ofxImGui::AddParameter(cue.params.getFloat(i));
				} else if(paramtype == typeid(ofParameter<int>).name()	) {
					ofxImGui::AddParameter(cue.params.getInt(i));
				} else if(paramtype == typeid(ofParameter<bool>).name()	) {
					ofxImGui::AddParameter(cue.params.getBool(i));
				}else if(paramtype == typeid(ofParameter<string>).name()	) {
					ofxImGui::AddParameter(cue.params.getString(i));
				}

			}

			
//			ofxImGui::EndTree(mainSettings);
//		}
//

		ofxImGui::EndWindow(guiSettings);
	}
	
	gui.end();
	
	return guiSettings.mouseOverGui;
}
