#include "testApp.h"

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
    
    drawGestureDebug();
    
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

void testApp::drawGestureDebug() {
    ofDrawBitmapString("X Velocity " + ofToString(listener.xRunningVelocities.back()), 50, 50);
    ofDrawBitmapString("X Velocity " + ofToString(listener.xRunningDeltas.back()), 50, 150);
    
    
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2 - listener.xRunningDeltas.size(), ofGetHeight() / 2);
    ofSetColor(255, 255, 255);
    ofPath path;
    path.setFilled(false);
    int x = 0;
    for (list<float>::iterator i = listener.xRunningDeltas.begin();
         
         i != listener.xRunningDeltas.end();
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

void testApp::keyPressed(int key){

}
