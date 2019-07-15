//
//  Draggable.cpp
//
//  Created by Seb Lee-Delisle on 31/07/2018.
//
//

#include "Draggable.h"

bool Draggable::hitTest(float x, float y){
	return rect.inside(x, y);
}
bool Draggable::hitTest(glm::vec3 hitPos){
	return hitTest(hitPos.x, hitPos.y);
}

void Draggable::startDrag(glm::vec3 clickPos, bool dragXAxis, bool dragYAxis){
	
	clickOffset = clickPos - rect.getTopLeft();
	isDragging = true;
	xAxis = dragXAxis;
	yAxis = dragYAxis;
	startPos = rect.getTopLeft();
	dragStartTime = ofGetElapsedTimef();
	
	
}
bool Draggable::updateDrag(glm::vec3 pos){
	if(isDragging) {
		if(ofGetElapsedTimef()-dragStartTime>dragDelay) {
			if(xAxis) {
				rect.setX(startPos.x + (((pos.x - clickOffset.x) - startPos.x) * (ofGetKeyPressed(OF_KEY_SHIFT) ? 0.2:1)));
			}
			if(yAxis) {
				rect.setY(startPos.y + (((pos.y - clickOffset.y) - startPos.y) * (ofGetKeyPressed(OF_KEY_SHIFT) ? 0.2:1)));
			}
		}
		return true;
	} else {
		return false;
	}
	
}
bool Draggable::stopDrag(){
	if(isDragging) {
		isDragging = false;
		return true;
	} else {
		return false;
	}
}
