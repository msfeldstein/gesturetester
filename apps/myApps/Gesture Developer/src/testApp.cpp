#include "testApp.h"

using namespace Leap;

const float SWIPE_THRESHOLD = 35;
const float REST_THRESHOLD = 5;

void testApp::setup(){
    leap.addListener(listener);
    listener.setDelegate(this);
    fingersAreUp = false;
}

void testApp::update(){

}

void testApp::swipeRight() {
    swipeRightAlpha = 1;
}

void testApp::swipeLeft() {
    swipeLeftAlpha = 1;
}

void testApp::fingersUp() {
    fingersAreUp = true;
}

void testApp::fingersDown() {
    fingersAreUp = false;
}

void testApp::draw(){
    ofSetColor(255, 255, 255, 255);
    ofBackground(0, 0, 0);
    listener.draw();
    
    ofEnableAlphaBlending();
    ofPushMatrix();
    ofSetColor(255, 255,255, 255.0f * swipeRightAlpha);
    ofDrawBitmapString("Swipe Right", 50, ofGetHeight() - 50);
    
    ofSetColor(255, 255,255, 255.0f * swipeLeftAlpha);
    ofDrawBitmapString("Swipe Left", 150, ofGetHeight() - 50);
    
    swipeRightAlpha -= 0.005f;
    swipeLeftAlpha -= 0.005f;
    if (fingersAreUp) {
        ofSetColor(255,255,255,255);
        ofDrawBitmapString("Fingers Up", 250, ofGetHeight() - 50);
    }
    ofPopMatrix();
    ofDisableAlphaBlending();
    
}

void testApp::keyPressed(int key){

}

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
        Vector avgPos;
        Vector avgVel;
        float dt = frame.timestamp() - lastFrame.timestamp();
        dt *= 1.0e-6;
        for (int i = 0; i < fingers.count(); ++i) {
            avgPos += fingers[i].tipPosition();
            avgVel += fingers[i].tipVelocity();
        }
        avgPos /= (float)fingers.count();
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
        printf("Swipe Right\n");
        swiping = true;
        if (delegate != NULL) delegate->swipeRight();
    }
    
    if (totalXDelta < -SWIPE_THRESHOLD && !swiping) {
        printf("Swipe Left\n");
        swiping = true;
        if (delegate != NULL) delegate->swipeLeft();
    }
    
    if (abs(totalXDelta) < REST_THRESHOLD) {
        swiping = false;
    }
    
    if (fingers.count() > 1 && !fingersUp) {
        printf("Fingers Up\n");
        fingersUp = true;
        if (delegate != NULL) delegate->fingersUp();
    }
    
    if (fingers.count() <= 1 && fingersUp) {
        printf("Fingers Down\n");
        fingersUp = false;
        if (delegate != NULL) delegate->fingersDown();
    }
}

void GestureListener::draw() {
    ofDrawBitmapString("X Velocity " + ofToString(xRunningVelocities.back()), 50, 50);
    ofDrawBitmapString("X Velocity " + ofToString(xRunningDeltas.back()), 50, 150);
    
    
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2 - xRunningDeltas.size(), ofGetHeight() / 2);
    ofSetColor(255, 255, 255);
    ofPath path;
    path.setFilled(false);
    int x = 0;
    for (list<float>::iterator i = xRunningDeltas.begin();

         i != xRunningDeltas.end();
         ++i) {
        path.lineTo(x, *i);
        x++;
    }
    path.draw();
    ofPopMatrix();
    

    for (int i = 0; i < 200; i += 50) {
        ofSetColor(255, 255, 255);
        ofDrawBitmapString(ofToString(i), 0, ofGetHeight() / 2 - i);
        ofDrawBitmapString(ofToString(i), 0, ofGetHeight() / 2 + i);
        ofSetColor(0, 0, 255);
        ofLine(0, ofGetHeight() / 2 - i, ofGetWidth(), ofGetHeight() / 2 - i);
        ofLine(0, ofGetHeight() / 2 + i, ofGetWidth(), ofGetHeight() / 2 + i);
    }
}