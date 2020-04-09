﻿#pragma once
#include "ofMain.h"
#include "ofxBullet.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key) {};
	void keyReleased(int key) {};
	void mouseMoved(int x, int y) {};
	void mouseDragged(int x, int y, int button) {};
	void mousePressed(int x, int y, int button) {};
	void mouseReleased(int x, int y, int button) {};
	void windowResized(int w, int h) {};
	void dragEvent(ofDragInfo dragInfo) {};
	void gotMessage(ofMessage msg) {};

	ofCamera cam;
	ofxBulletWorldRigid world;

	unique_ptr<ofxBulletCustomShape> bounds_shape;
	vector<unique_ptr<ofxBulletSphere>> sphere_list;
	vector<vector<glm::vec3>> log_list;
};