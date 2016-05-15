

/*Created by Robert Pavlovskis on 04/07/2016
This is example shows how you can apply a different types of Displacment Maps to a 360° video.
The sketch references one of Mick Greirsons AV Labs on deforming meshes with sinusoids.*/


#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//Pixel Arrays
		unsigned char * pixelout;
		unsigned char * pixelin;
		unsigned char * pixels;

		//Iterators
		int i, j, u, pointer, sPointer;

		//Dimensions
		int width, height, sWidth, sHeight, scale;

		//Image, orginal and scaled-down for a lower-res mesh
		ofImage img, scaled;

		//Adds camera and controls
		ofEasyCam cam;

		//Sphere we're going to base our mesh on
		ofSpherePrimitive sphereReference;
		//Mesh for deforming
		ofVboMesh deformPlane;

		//Amplitude determines how fast the sinusoidal deform moves along the phase
		float amp = 0;

		//oxGUI
		ofxFloatSlider rotation, extrude;
		ofxIntSlider distance, fov, vertices, iterations;
		ofxToggle texture, sinusoid, pattern;
		ofxPanel gui;
};
