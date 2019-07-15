//
//  CueLaserGraphic.hpp
//
//  Created by Seb Lee-Delisle on 29/07/2018.
//



#pragma once
#include "ofMain.h"
#include "CueTransformable.h"
#include "ofxLaserGraphic.h"
#include "ofxLaserManager.h"
#include "ofxSvg.h"


class CueLaserGraphic : public CueTransformable{
    
public:
    
    CueLaserGraphic() : laser(*ofxLaser::Manager::instance()) {};
    virtual void loadFromSVG(string filename, int targetZone = 0);
    virtual void update(float time, bool isPlaying);
    
    ofxLaser::Graphic graphic;
    ofxSVG svg;
	
	int targetZone = 0;
	
	string type = "CueLaserGraphic";
	
protected:
    
    ofxLaser::Manager& laser;
	
    
private:
    
};
