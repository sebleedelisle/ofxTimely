//
//  CueBase.h
//  BecHillLasers
//
//  Created by Seb Lee-Delisle on 28/07/2018.
//
//

#pragma once
#include "ofMain.h"
#include "Draggable.h"

class CueBase : public Draggable{
	
	public:
	CueBase();
	
    virtual void start(float time) {};
    virtual void stop(float time) {};
	
	virtual void startDrag(glm::vec3 clickPos, bool dragXAxis = true, bool dragYAxis = true);
	virtual bool updateDrag(glm::vec3 pos);
	virtual bool stopDrag();

	
	virtual float getStart();
	virtual float getEnd();
	virtual float getDuration();
	
	virtual void setStart(float start);
	virtual void setDuration(float d);
	virtual void setEnd(float end);
	
	virtual void update(float time, bool isPlaying) {};

	// add type specific rendering here :
	virtual void drawCueBlock();

	
	virtual bool isActive(float time);
	string getName() ;
	void setName(string newname);
	
	bool overlapsCue(CueBase& cue);

	void setSelected (bool state);
	bool isSelected();
	
	void setLocked (bool state);
	bool isLocked();
	
	bool isEndDraggable = true;
	bool isStartDraggable = true;
	bool startDragging = false;
	bool endDragging = false; 
	
	int sideDragWidth = 4;
    
   
	
	//ofRectangle rect;
	ofColor fillColour = ofColor(70,70,90);
	ofColor strokeColour = ofColor(180,180,200);
	
	ofParameterGroup params;
	
	
	ofParameter<float> startTime, duration;
    ofParameter<string> name;
	ofParameter<bool> locked = false;
	ofParameter<int> trackNum = 0;
	
	bool fixedLength = false; 
	
	const string type = "CueBase";
	
	
	protected:
	
	bool selected = false;
	
	

	private:

};
