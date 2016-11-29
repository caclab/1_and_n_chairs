#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//WEBSOCKETS CONFIG
	ofSetLogLevel(OF_LOG_VERBOSE);
	client.connect("127.0.0.1", 9091);
	ofSetLogLevel(OF_LOG_ERROR);
	client.addListener(this);

    lastReconnectTime = ofGetElapsedTimeMillis();

	drawImage = false;
	loadImage = false;

    aspectRatioW = ofGetHeight()/ofGetWidth();
}

//--------------------------------------------------------------
string ofApp::getImageURL(const char* cmd){
    //cout<< "Searching for image " + ofToString(cmd)<< endl;
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    // cout<< "RESULT: " + result<< endl;
	return result;
}

//--------------------------------------------------------------
void ofApp::transformImage(){
    if(image.getWidth()>image.getHeight()){
        cout<< "WIDTH is bigger"<< endl;
        //image.resize(ofGetHeight()/aspectRatioW, ofGetWidth());
        image.resize(ofGetHeight(), ofGetWidth());
    }else{
        cout<< "HEIGHT is bigger"<< endl;
        //image.resize(ofGetHeight(), ofGetWidth()/aspectRatioW);
        image.resize(ofGetHeight(), ofGetWidth());
    }
    image.rotate90(3);
}

//--------------------------------------------------------------
void ofApp::update(){
    if(client.isConnected()){
    	if(loadImage){
    		image.load("data/image.jpg");
            transformImage();
    		loadImage = false;
            client.send("$image;ready");
    	}
    }else{
        uint64_t now = ofGetElapsedTimeMillis();
        if (now - lastReconnectTime > 5000) {
            cout<< "Reconnecting..."<< endl;
            client.close();
            client.connect("127.0.0.1", 9091); 
            lastReconnectTime = ofGetElapsedTimeMillis();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
    if(client.isConnected()){
    	// ofSetColor(255);
    	if(drawImage){
            if(image.isAllocated()){
                image.draw(0,0);
            }
    	}
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"on open"<<endl;
    client.send("$image;");
}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
}

//--------------------------------------------------------------
void ofApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args ){
    //cout<<"got message "<<args.message<<endl;

	string allTogether = "";
	for(int i=0; i<(ofSplitString(args.message, " ")).size();i++){
		allTogether = allTogether + ofSplitString(args.message, " ")[i];
        if(i<(ofSplitString(args.message, " ")).size()-1){
            allTogether += "_";
        }
	}

	cout<< allTogether<< endl;

    if(args.message == "$reset;"){
    	drawImage = false;
    }else if(args.message == "$display;"){
        drawImage = true;
    }else{
    	//cout<< ("python data/get_image.py " + args.message).c_str()<< endl;
    	string url = getImageURL(("node data/app.js " + allTogether).c_str());
    	//cout<< "URL: " + url<< endl;
    	loadImage = true;
    }
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
