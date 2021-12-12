#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
//#ifdef TARGET_OPENGLES
 ofDisableArbTex(); 
// ofShaderSettings settings;
// settings.bindDefaults = false;
// settings.shaderFiles.insert(1,shadersES2/shader);
//shader.setup(settings);
	shader.load("shadersES2/shader");
//#else
//	if(ofIsGLProgrammableRenderer()){
		//shader.load("shadersGL3/shader");
//	}else{
		//shader.load("shadersGL2/shader");
	//}
//#endif

    img.load("img.jpg");

    plane.set(800, 600, 10, 10);
    plane.mapTexCoords(0, 0, img.getWidth(), img.getHeight());
}


//--------------------------------------------------------------
void ofApp::update() {
 //   shader.load("shadersES2/shader");
}

//--------------------------------------------------------------
void ofApp::draw() {
    	

    // bind our texture. in our shader this will now be tex0 by default
    // so we can just go ahead and access it there.
   img.getTexture().bind();
   	  
    // start our shader, in our OpenGL3 shader this will automagically set
    // up a lot of matrices that we want for figuring out the texture matrix
    // and the modelView matrix
//	 shader.setUniformTexture("src_tex_unit4", img.getTexture(), 4); 

   shader.begin();
// auto prshader = ofGLProgrammableRenderer()->getCurrentShader();
// shader.setUniformTexture("src_tex_unit0", img.getTexture(), 0); 
 shader.setUniformMatrix4f("modelViewProjectionMatrix", ofGetCurrentRenderer()->getCurrentMatrix(OF_MATRIX_PROJECTION) *ofGetCurrentRenderer()->getCurrentMatrix(OF_MATRIX_MODELVIEW));
 //shader.setUniformMatrix4f(modelViewProjectionMatrix, ofGetCurrentRenderer()->getCurrentMatrix(OF_MATRIX_MODELVIEW);

    // get mouse position relative to center of screen

    float xPosition = ofGetFrameNum() % ofGetWidth();
    
    float mousePosition = ofMap(xPosition, 0, ofGetWidth(), 1.0, -1.0, true);
    mousePosition *= plane.getWidth();
    shader.setUniform1f("mouseX", mousePosition);
    shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

    plane.draw();

    ofPopMatrix();
    
   shader.end();
    	//img.update();

    img.getTexture().unbind();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
