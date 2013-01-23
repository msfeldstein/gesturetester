#pragma once

#include "ofMain.h"
#include "Leap.h"
#include <stdlib.h>
#include "GestureDetector.h"

class testApp : public ofBaseApp, public GestureDelegate {
	public:
		void setup();
		void update();
		void draw();
    
    void drawGestureDebug();
    
    void swipeRight();
    void swipeLeft();
    void fingersUp();
    void fingersDown();

		
		void keyPressed(int key);
    
        Leap::Controller leap;
        GestureListener listener;
    bool fingersAreUp;
    float swipeLeftAlpha;
    float swipeRightAlpha;
};

