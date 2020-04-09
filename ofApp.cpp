#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();

	// Initialize ofxBulletWorld
	this->cam.setPosition(glm::vec3(0, 3.f, -5.f));
	this->cam.lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -1, 0));
	this->world.setup();
	this->world.enableGrabbing();
	this->world.setCamera(&this->cam);
	this->world.setGravity(ofVec3f());

	// Create Big Box
	ofVec3f start_location;
	ofPoint dimens;
	float bounds_width = 300;
	float h_width = bounds_width * 0.5;
	float depth = 10.;
	float h_depth = depth * 0.5;
	this->bounds_shape.reset(new ofxBulletCustomShape());
	this->bounds_shape->create(world.world, ofVec3f(0, 0, 0), 10.);
	for (int i = 0; i < 6; i++) {
		if (i == 0) { // ground //
			start_location.set(0., h_width + h_depth, 0.);
			dimens.set(bounds_width, depth, bounds_width);
		}
		else if (i == 1) { // back wall //
			start_location.set(0, 0, h_width + h_depth);
			dimens.set(bounds_width, bounds_width, depth);
		}
		else if (i == 2) { // right wall //
			start_location.set(h_width + h_depth, 0, 0.);
			dimens.set(depth, bounds_width, bounds_width);
		}
		else if (i == 3) { // left wall //
			start_location.set(-h_width - h_depth, 0, 0.);
			dimens.set(depth, bounds_width, bounds_width);
		}
		else if (i == 4) { // ceiling //
			start_location.set(0, -h_width - h_depth, 0.);
			dimens.set(bounds_width, depth, bounds_width);
		}
		else if (i == 5) { // front wall //
			start_location.set(0, 0, -h_width - h_depth);
			dimens.set(bounds_width, bounds_width, depth);
		}
		btBoxShape* boxShape = ofBtGetBoxCollisionShape(dimens.x, dimens.y, dimens.z);
		this->bounds_shape->addShape(boxShape, start_location);
	}
	this->bounds_shape->add();

	// Create Spheres
	for (int i = 0; i < 255; i++) {

		unique_ptr<ofxBulletSphere> sphere(new ofxBulletSphere);
		float size = 5;
		sphere->create(this->world.world, ofVec3f(ofRandom(-100, 100), ofRandom(-100, 100), ofRandom(-100, 100)), 1, size);
		sphere->setRestitution(1.0);
		sphere->add();
		this->sphere_list.push_back(move(sphere));

		vector<glm::vec3> log;
		this->log_list.push_back(log);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	for (int i = 0; i < this->sphere_list.size(); i++) {

		this->log_list[i].push_back(this->sphere_list[i]->getPosition());

		while (this->log_list[i].size() > 30) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}

	// Big Box Moving To Center 
	if (!ofGetMousePressed()) {

		ofVec3f diff = ofVec3f(0, 0, 0) - bounds_shape->getPosition();
		diff *= 500.f;
		bounds_shape->applyCentralForce(diff);
	}

	// Compute Spheres Relation
	for (int i = 0; i < this->sphere_list.size(); i++) {

		for (int j = i + 1; j < this->sphere_list.size(); j++) {

			float distance = this->sphere_list[i]->getPosition().distance(this->sphere_list[j]->getPosition());
			if (distance < 80) {

				ofVec3f diff = this->sphere_list[i]->getPosition() - this->sphere_list[j]->getPosition();
				ofMap(distance, 0, 80, 20, -2);
				this->sphere_list[i]->applyCentralForce(diff);
				this->sphere_list[j]->applyCentralForce(-diff);
			}
		}
	}

	this->world.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	// Draw Big Box
	ofNoFill();
	if (ofGetMousePressed()) {

		ofSetColor(39, 39, 239);
	}
	else {

		ofSetColor(39);
	}
	bounds_shape->transformGL();
	ofDrawBox(glm::vec3(0, 0, 0), 300);
	bounds_shape->restoreTransformGL();


	for (int i = 0; i < this->sphere_list.size(); i++) {

		glm::vec3 param;
		switch ((int)ofRandom(3)) {
		case 0:
			param = glm::vec3(5, 0, 0);
			break;
		case 1:
			param = glm::vec3(0, 5, 0);
			break;
		case 2:
			param = glm::vec3(0, 0, 5);
			break;
		}

		ofMesh face, frame;
		frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

		for (int k = 0; k < this->log_list[i].size(); k++) {

			auto& log = this->log_list[i][k];

			face.addVertex(log + param);
			face.addVertex(log - param);

			frame.addVertex(log + param);
			frame.addVertex(log - param);

			if (k > 0) {

				face.addIndex(k * 2); face.addIndex(k * 2 - 1); face.addIndex(k * 2 - 2);
				face.addIndex(k * 2); face.addIndex(k * 2 + 1); face.addIndex(k * 2 - 1);

				frame.addIndex(k * 2); frame.addIndex(k * 2 - 2);
				frame.addIndex(k * 2 + 1); frame.addIndex(k * 2 - 1);
			}
		}

		frame.addIndex(0); frame.addIndex(1);
		frame.addIndex(frame.getNumVertices() - 1); frame.addIndex(frame.getNumVertices() - 2);

		ofSetColor(39);
		face.drawFaces();

		ofSetColor(239);
		frame.drawWireframe();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}