

/*Created by Robert Pavlovskis on 04/07/2016
This is example shows how you can apply a different types of Displacment Maps to a 360° video.
The sketch references one of Mick Greirsons AV Labs on deforming meshes with sinusoids.*/



#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	img.loadImage("forest_japan.BMP");



	//How much image is scaled down image for lower-res plane
	scale = 15;

	//Dimensions
	width = img.getWidth();
	height = img.getHeight();

	//Scaled Dimensions
	sWidth = (int)width / scale;
	sHeight = (int)height / scale;

	//Assigns length to array
	pixelin = new unsigned char[width* height * 3];
	pixelout = new unsigned char[sWidth* sHeight * 3];




	//Clipping distance for camera
	cam.setFarClip(4000000000000);

	//Field of Vision
	fov = 90;

	//Place camera inside sphere at 0,0,0
	cam.setAutoDistance(false);




	//Sphere
	sphereReference.set(400, 400);
	sphereReference.setResolution(35);



	//GUI
	gui.setup();
	//Magnitude of the displacments
	gui.add(extrude.setup("Extrusion", 0.025, 0, 0.5));
	//Displacment mapping with textures
	gui.add(texture.setup("Texture Displace", false));
	//Use sinusoids to displace your sphere
	gui.add(sinusoid.setup("Sin Displace", false));
	//Displace sphere in a pattern through even iterations
	gui.add(pattern.setup("Pattern Displace", false));
	gui.add(iterations.setup("Pattern Iterations", 1, 1, 100));
	gui.add(rotation.setup("Rotation", 0.02, 0, 0.5));
	gui.add(distance.setup("Distance", 0, -5000, 5000));
	gui.add(fov.setup("FOV", 90, 0, 360));
	gui.add(vertices.setup("Vertices", 35, 1, 100));
}

//--------------------------------------------------------------
void ofApp::update() {

	//This code makes a scaled down version of our texture
	//So we can displace our spheres surface with less performance loss
	//It is in Update because it would also work with video
	///vid.update();
	pixelin = img.getPixels();
	for (int i = 0; i < width; i += scale) {
		for (int j = 0; j < height; j += scale) {

			//Map 2D For loop to 1D pixel Array
			pointer = (j*width + i);
			//Map the same loop to a scaled down version of the Array and Image
			sPointer = ((j / scale)*sWidth + (i / scale));

			pixelout[sPointer * 3 + 0] = pixelin[pointer * 3 + 0];//red
			pixelout[sPointer * 3 + 1] = pixelin[pointer * 3 + 1];//green
			pixelout[sPointer * 3 + 2] = pixelin[pointer * 3 + 2];//blue

		}
	}
	//Writes pixels to scaled down Displacment Texture
	scaled.setFromPixels(pixelout, sWidth, sHeight, OF_IMAGE_COLOR);

	//Update camera position
	cam.setPosition(0, 0, distance);

	//Rotates Sphere
	sphereReference.rotate(rotation, 0, rotation, 0);

	//Set Field of Vision
	cam.setFov(fov);

	//Set Sphere Resolution
	sphereReference.setResolution(vertices);
}

//--------------------------------------------------------------
void ofApp::draw() {

	//We are going to start by copying the sphereReference mesh into the deformPlane vboMesh
	deformPlane = sphereReference.getMesh();

	//We want to enable it to show the mesh
	ofEnableDepthTest();

	//We're going to create a 3 dimensional float vector to store each vertex
	ofVec3f vertexValue;

	//Loop goes through all the vertecies we want to transform
	for (int i = 0; i < deformPlane.getNumIndices(); i++) { 

	    //Update gradually
		amp = 0.000001; 

		//Get the current vertex value and store it.
		int vertexIndex = deformPlane.getIndex(i);
		vertexValue = deformPlane.getVertex(vertexIndex); 

		//Enables Pattrent Displacment
		if (pattern) {
			//Displaces vertecies in an even pattern 
			//by only applying to an index divisible by a number you chose
			if ((i & iterations) == 0) { 
				for (int j = 0; j < 10; j++) {
					vertexValue.x *= (extrude + 1);
					vertexValue.y *= (extrude + 1);
					vertexValue.z *= (extrude + 1);
				}
			}
		}

		//Displaces sphere with sinewaves, credit to Mick Grierson and his AV Lab examples
		if (sinusoid) {
			//Determine magnitude and phase based on mouse movement and iteration
			float er = cos(sin(mouseX  * amp*i));
			float ery = cos(cos(mouseY  * amp*i));

			//Apply sine and cos displacment to vertecies
			vertexValue.x *= er; 
			vertexValue.y *= ery;
			//Both sine and cos for distance
			vertexValue.z *= er*ery;
		}

		//Displaces mesh based on its texture map
		if (texture) {	

		    //Maps number of Indicies to number of Pixels
			pointer = ofMap(i, 0, deformPlane.getNumIndices(), 0, scaled.getWidth()*scaled.getHeight());
		
			//Get colour value of pixel
			ofFloatColor sampleColor(scaled.getPixels()[pointer * 3],				// r
			                       	scaled.getPixels()[pointer * 3 + 1],			// g
				                    scaled.getPixels()[pointer * 3 + 2]);			// b

			vertexValue.x *= sampleColor.getBrightness() * extrude + 1;
			vertexValue.y *= sampleColor.getBrightness() * extrude + 1;
			vertexValue.z *= sampleColor.getBrightness() * extrude + 1;
		}

		//Set vertex position
		deformPlane.setVertex(vertexIndex, vertexValue); // write it all back, including the new z.
	}


	cam.begin();
	//Store current position, anything drawn after maintains these coordinates
	sphereReference.transformGL();

	//Map texture coordinates to sphere
	sphereReference.mapTexCoordsFromTexture(img.getTexture());

	//Now we draw and texture bind our newly deformed sphere mesh
	img.getTexture().bind();
	deformPlane.draw();
	img.getTexture().unbind();

	//Restore the coordinate system
	sphereReference.restoreTransformGL();
	cam.end();

	//Disable depth testing to draw image and video
	ofDisableDepthTest();

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo info) {

	//Load drag & dropped image
	img.loadImage(info.files[0]);
}
