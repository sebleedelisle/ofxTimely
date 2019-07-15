//
//  Timeline.h
//  BecHillLasers
//
//  Created by Seb Lee-Delisle on 28/07/2018.
//
//

#pragma once
#include "ofMain.h"
#include "CueBase.h"
#include "ofxGui.h"
#include "ofxImGui.h"

class Timeline {
	
	public:
	
	Timeline();
	~Timeline();
	
	bool play();
	bool pause();
	bool togglePlayPause();
	bool getIsPlaying();
	
	float getPlayheadTime();
	void setPlayheadTime(float pos);
	
	float getDuration() {
		return duration;
	}
	
	void update(float deltaTime);
	
	// draw the timeline - x and y are the bottom left
	void draw(float x, float y, float w);

	void addCue(CueBase& cue, int track = 0);
	void setNumTracks(int num);
	
	void skipForward(bool fast = false);
	void skipBack(bool fast = false);
	
	void renderCue(CueBase & cue);
	
	void initListeners();
	void removeListeners();
	
	bool mousePressed(ofMouseEventArgs &e);
	bool mouseDragged(ofMouseEventArgs &e);
	bool mouseReleased(ofMouseEventArgs &e);
	bool mouseScrolled(ofMouseEventArgs &e);
	void startDragging(int cueIndex, glm::vec3 clickPos);
	void deselectCues();
    void sortCues();
	void checkBounds();
	void updateTrackRects();
	
	float timeToPosition(float time);
	float positionToTime(float position);
	
	void updateCueTimeForPosition(CueBase& cue);
	
    static bool sortAlgorithm(CueBase* a, CueBase* b) {
		if(a->isSelected()) return false;
		else if(b->isSelected()) return true;
        else return a->getStart()<b->getStart();
    }
	
	ofxPanel guiSettings; 
	
	bool saveSettings();
	bool loadSettings();
	
	ofColor trackColour;
	float border = 3;
	float trackHeight = 20;
	
	float scrollX = 0, scrollY = 0;
	float lastScrollXTime = 0, lastScrollYTime = 0;
	
	ofxImGui::Gui gui;
	
	bool drawCueGui(CueBase& cue);

	// for zooming in
	glm::vec3 scale, offset,
				clickPosition,
				startOffset;
	bool isDragging = false,
		isMouseDown = false; 
	ofParameter<bool> locked;
	
	
	protected:

	
	bool isPlaying;
	float playheadTime;
	Draggable playheadDraggable; 
	float duration = 10; // default length 10 seconds
	vector<CueBase*> cues;
	int numTracks;
	// keeps track of the last time the timeline was moved or
	// zoomed so we know when to fade out the scroll bar
	// TODO - this functionality should probably be moved somewhere huh?
	float lastTimelineMove = 0;
	
	float fps;
	bool snapToFrame;
	ofRectangle cueGuiRect;
	bool mouseIsOverGui;
	
	vector<ofRectangle> trackRects;
	ofRectangle timelineRect;
	
	
	
	private:

};
