//
//  CueTransformable.h
//  BecHillLasers
//
//  Created by Seb Lee-Delisle on 30/07/2018.
//
//

#pragma once
#include "ofMain.h"
#include "CueBase.h"

class CueTransformable : public CueBase {
	
	public:
	
	void setPosition(glm::vec3 pos) {
		setPosition(pos.x, pos.y, pos.z);
	}
	
	void setPosition(float x, float y, float z) {
		position.x = x;
		position.y = y;
		position.z = z;
	}
	
	glm::vec3 getPosition() {
		return position;
	}
	
	float getScale() {
		return scale;
	}
	void setScale(float s) {
		scale = s;
	}
		

	protected:

	glm::vec3 position = glm::vec3();
	float scale = 0;
	float rotation = 0;
	
	private:

};
