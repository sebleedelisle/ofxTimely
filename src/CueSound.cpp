//
//  CueSound.cpp
//  BecHillLasers
//
//  Created by Seb Lee-Delisle on 28/07/2018.
//
//

#include "CueSound.h"


CueSound::CueSound() {
	volume.set("Volume", 1,0,1);
	
}
void CueSound::load(string filename){
    
    sound.load(filename);
    sound.play();
    sound.setPosition(0.5f);
    soundDuration = (float)(2 * sound.getPositionMS())/1000.f;
    sound.stop();

	sound.setVolume(volume);
	
	//setName(filename); 
    
}

void CueSound::start(float time) {
    float soundPositionMS = (time-startTime)*1000;
    sound.play();
    sound.setPositionMS(soundPositionMS);
    
}
void CueSound::stop(float time) {
    sound.stop();
}
void CueSound::update(float time, bool isPlaying){
    // check to see if we should be playing
	sound.setVolume(volume);
    if(isActive(time) && isPlaying) {
        float soundPositionMS = (time-startTime)*1000;
        
        // if we aren't playing, then play!
        if(!sound.isPlaying()) {
            sound.play();
            sound.setPositionMS(soundPositionMS);
        } else {
            // check to see if we're roughly in the right place
            
            if(abs(sound.getPositionMS() - soundPositionMS)>50) {
                ofLog(OF_LOG_NOTICE, "sound cue retimed! " + ofToString((sound.getPositionMS() - soundPositionMS)));
                sound.setPositionMS(soundPositionMS);
            }
        }
    } else {
        sound.stop();
    }
    
}

void CueSound::autoSetEnd() {
    setEnd(getStart()+soundDuration);
}
