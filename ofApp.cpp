#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(1.5);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}
//--------------------------------------------------------------
void ofApp::update() {

	this->face.clear();
	this->frame.clear();

	vector<glm::vec3> base_location = {

	glm::vec3(-300, -300, 0), glm::vec3(0, -300, 0), glm::vec3(300, -300, 0),
	glm::vec3(-300, 0, 0), glm::vec3(0, 0, 0), glm::vec3(300, 0, 0),
	glm::vec3(-300, 300, 0), glm::vec3(0, 300, 0), glm::vec3(300, 300, 0),
	};

	for (auto& location : base_location) {

		for (int radius = 50; radius <= 130; radius += 20) {

			this->setRingToMesh(this->face, this->frame, location, radius, 45);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofSetColor(39);
	this->face.draw();

	ofSetColor(239);
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float height) {

	int index = face_target.getNumVertices();

	for (int deg = 0; deg < 360; deg += 1) {

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * 0.5));

		for (auto& vertex : vertices) {

			auto noise_value = ofNoise(
				(location.x + vertex.x) * 0.00035,
				(location.y + vertex.y) * 0.00035 + ofGetFrameNum() * 0.008);

			auto rotation_x = glm::rotate(glm::mat4(), ofMap(noise_value, 0, 1, -PI * 3, PI * 3), glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), ofMap(noise_value, 0, 1, -PI * 3, PI * 3), glm::vec3(0, 1, 0));
			auto rotation_z = glm::rotate(glm::mat4(), ofMap(noise_value, 0, 1, -PI * 3, PI * 3), glm::vec3(0, 0, 1));

			vertex = glm::vec4(vertex, 0) * rotation_z * rotation_y * rotation_x + glm::vec4(location, 0);
		}

		auto face_index = face_target.getNumVertices();
		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		auto frame_index = frame_target.getNumVertices();
		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}