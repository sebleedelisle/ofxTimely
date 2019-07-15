//
//  CueSound.h
//  BecHillLasers
//
//  Created by Seb Lee-Delisle on 28/07/2018.
//
//

#pragma once
#include "ofMain.h"
#include "CueBase.h"

class CueSound : public CueBase{
	
	public:
	CueSound(); 
    void load(string filename);
    virtual void start(float time);
    virtual void stop(float time);
    virtual void update(float time, bool isPlaying);
	
	
    void autoSetEnd();
	
	ofSoundPlayer sound;
	ofParameter<float> volume; 
	
	protected:
	float soundDuration = 0;
	
	private:

};
