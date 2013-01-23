//
//  GestureDetector.cpp
//  Gesture Developer
//
//  Created by Michael Feldstein on 1/22/13.
//
//

#include "GestureDetector.h"

using namespace Leap;
using namespace std;

void GestureListener::setDelegate(GestureDelegate *d) {
    delegate = d;
}

void GestureListener::onInit(const Leap::Controller &controller) {
    printf("onInit");
}

void GestureListener::onFrame(const Leap::Controller& controller) {
    const Frame frame = controller.frame();
    const Frame lastFrame = controller.frame(1);
    const FingerList fingers = frame.fingers();
    if (!fingers.empty() && lastFrame.isValid()) {
        // Calculate the hand's average finger tip position
        Vector avgVel;
        float dt = frame.timestamp() - lastFrame.timestamp();
        dt *= 1.0e-6;
        for (int i = 0; i < fingers.count(); ++i) {
            avgVel += fingers[i].tipVelocity();
        }
        avgVel /= (float)fingers.count();
        xVelocityQueue.push_back(avgVel.x);
        xDeltas.push_back(avgVel.x * dt);
        if (xVelocityQueue.size() > 20) xVelocityQueue.pop_front();
        if (xDeltas.size() > 3) xDeltas.pop_front();
    } else if (xDeltas.size() > 0) {
        std::deque<float>().swap(xVelocityQueue);
        std::deque<float>().swap(xDeltas);
    }
    
    float avgVelX = 0;
    float totalXDelta = 0;
    for (deque<float>::iterator i = xVelocityQueue.begin();
         i != xVelocityQueue.end();
         ++i) {
        avgVelX += *i;
    }
    for (deque<float>::iterator i = xDeltas.begin();
         i != xDeltas.end();
         ++i) {
        totalXDelta += *i;
    }
    //if (xVelocityQueue.size() > 0) avgVelX /= xVelocityQueue.size();
    xRunningVelocities.push_back(avgVelX);
    xRunningDeltas.push_back(totalXDelta);
    
    if (totalXDelta > SWIPE_THRESHOLD && !swiping) {
        swiping = true;
        if (delegate != NULL) delegate->swipeRight();
    }
    
    if (totalXDelta < -SWIPE_THRESHOLD && !swiping) {
        swiping = true;
        if (delegate != NULL) delegate->swipeLeft();
    }
    
    if (abs(totalXDelta) < REST_THRESHOLD) {
        swiping = false;
    }
    
    if (fingers.count() > 1 && !fingersUp) {
        fingersUp = true;
        if (delegate != NULL) delegate->fingersUp();
    }
    
    if (fingers.count() <= 1 && fingersUp) {
        fingersUp = false;
        if (delegate != NULL) delegate->fingersDown();
    }
}
