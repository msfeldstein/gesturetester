#pragma once

#include "ofMain.h"
#include "Leap.h"
#include <stdlib.h>

class GestureDelegate {
public:
    virtual ~GestureDelegate(){};

    virtual void swipeRight() {};
    virtual void swipeLeft() {};
    virtual void fingersUp() {};
    virtual void fingersDown() {};
    virtual void fingersSpread() {};
    virtual void fingersUnspread() {};
};

class GestureListener : public Leap::Listener {
    
    void onInit(const Leap::Controller& controller);
    void onFrame(const Leap::Controller& controller);
public:
    void draw();
    void setDelegate(GestureDelegate* d);
private:
    GestureDelegate* delegate;
    // Rolling history of values, limited to last N frames.
    std::deque<float> xVelocityQueue;
    std::deque<float> xDeltas;
    
    // Historical values computed from the previous deques.
    std::list<float> xRunningVelocities;
    std::list<float> xRunningDeltas;
    
    // Debouncing for gestures
    bool swiping = false;
    bool fingersUp = false;
};


class testApp : public ofBaseApp, public GestureDelegate {
	public:
		void setup();
		void update();
		void draw();
    
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

