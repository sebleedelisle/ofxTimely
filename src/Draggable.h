//
//  Draggable.h
//  BecHillLasers
//
//  Created by Seb Lee-Delisle on 31/07/2018.
//
//

#pragma once
#include "ofMain.h"

class Draggable {
	
	public:
	
	bool hitTest(float x, float y);
	bool hitTest(glm::vec3 hitPos);
	virtual void startDrag(glm::vec3 clickPos, bool dragXAxis = true, bool dragYAxis = true);
	virtual bool updateDrag(glm::vec3 pos);
	virtual bool stopDrag();
	
	ofRectangle rect;
	bool isDragging = false, xAxis, yAxis;
	glm::vec3 clickOffset, startPos;
	float dragDelay = 0.2;
	float dragStartTime = 0;
	protected:
	

	private:

};
