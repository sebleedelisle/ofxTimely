//
//  CueBase.cpp
//
//  Created by Seb Lee-Delisle on 28/07/2018.
//
//
#include "CueBase.h"

CueBase :: CueBase() {

//	startTime.setName("startTime");
//	duration.setName("duration");
	name.setName("Name");
	locked.setName("Lock");
//	trackNum.setName("track");
	params.add(name);
	params.add(startTime.set("Start", 0,0,100));
	params.add(duration.set("Duration", 2,0,5));
	params.add(locked);
	params.add(trackNum.set("Track", 0, 0, 5));
	
}

void CueBase :: setStart(float start){
	startTime = start;
}

void CueBase ::setDuration(float d) {
	duration = d;
}

void CueBase ::setEnd(float end){
	duration = end - startTime;
}
float CueBase ::getStart(){
	return startTime;
};
float CueBase ::getEnd(){
	return startTime+duration;
};
float CueBase ::getDuration() {
	return duration;
}

// add type specific rendering here :
void CueBase :: drawCueBlock() {
	
	ofRectangle drawRect = rect;
	drawRect.x = round(drawRect.x);
	drawRect.y = round(drawRect.y);
	drawRect.setWidth(round(drawRect.getWidth()));
	drawRect.setHeight(round(drawRect.getHeight()));
	
	// draw the background colour of the rectangle
	ofPushStyle();
	ofFill();
	ofSetColor(fillColour);
	ofDrawRectRounded(drawRect,3);
	
	float linewidth =2;
	ofSetLineWidth(linewidth);
	ofRectangle fillRect(drawRect);
	fillRect.x+=linewidth/2;
	fillRect.y+=linewidth/2;
	fillRect.width-=linewidth;
	fillRect.height-=linewidth;
	
	if(isSelected()) {
		ofSetColor(strokeColour);
		ofNoFill();
		
		ofEnableSmoothing();
		ofDrawRectRounded(fillRect,3);
	} else {
//		ofSetColor(20,30,60);// TODO - pass in track colour
	}
	
	
	ofFill(); 
	if(isEndDraggable) {
		ofRectangle endDragRect(drawRect.getRight(), drawRect.getTop(), -5, drawRect.getHeight());
		endDragRect.standardize();
		if(endDragRect.inside(ofGetMouseX(), ofGetMouseY())){
			ofSetColor(strokeColour);
			ofSetLineWidth(sideDragWidth);
			ofDrawRectangle(endDragRect);
		}
	}
	if(isStartDraggable) {
		ofRectangle startDragRect(drawRect.getLeft(), drawRect.getTop(), 5, drawRect.getHeight());
		
		if(startDragRect.inside(ofGetMouseX(), ofGetMouseY())){
			ofSetColor(strokeColour);
			ofSetLineWidth(sideDragWidth);
			ofDrawRectangle(startDragRect);
		}
	}
	
	
	
	ofSetColor(strokeColour);
	string label = ofToUpper(getName());
	if(label.size()*8 > fillRect.width-10) {
		int numchars = ((fillRect.width-10)/8)-1;
		if(numchars<0) numchars = 0;
		label.resize(numchars);
		label = label+"…";
		
	}
	ofDrawBitmapString(label, fillRect.getLeft()+4, fillRect.getCenter().y+5);
	ofPopStyle();
}


void CueBase::startDrag(glm::vec3 clickPos, bool dragXAxis, bool dragYAxis){
	Draggable::startDrag(clickPos, dragXAxis, dragYAxis);
	
	
}
bool CueBase::updateDrag(glm::vec3 pos){
	return Draggable::updateDrag(pos);
	
}
bool CueBase::stopDrag(){
	return Draggable::stopDrag();
	
}



bool CueBase :: isActive(float time) {
	return ((time>startTime) && (time<getEnd()));
}
string CueBase :: getName() {
	return name;
}
void CueBase :: setName(string newname) {
	name= newname;
}

bool CueBase :: overlapsCue(CueBase& cue) {
	
	if((getStart()<cue.getEnd()) && (getEnd()>cue.getStart())) return true;
	else return false;
	
}

void CueBase :: setSelected (bool state) {
	selected = state;
}
bool CueBase :: isSelected() {
	return selected;
}

void CueBase :: setLocked (bool state) {
	locked = state;
}
bool CueBase :: isLocked() {
	return locked;
}
