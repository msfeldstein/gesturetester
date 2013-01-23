//
//  GestureDetector.h
//  Gesture Developer
//
//  Created by Michael Feldstein on 1/22/13.
//
//

#ifndef __Gesture_Developer__GestureDetector__
#define __Gesture_Developer__GestureDetector__

#include <iostream>
#include <stdlib.h>
#include <deque>
#include <list>
#include "Leap.h"



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
    void setDelegate(GestureDelegate* d);
    
    // Should be private, public for debugging in OF.
    const float SWIPE_THRESHOLD = 32;
    const float REST_THRESHOLD = 5;
    
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


#endif /* defined(__Gesture_Developer__GestureDetector__) */
