#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(0);

	//WEBSOCKETS CONFIG
	ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port = 9091;
	options.bUseSSL = false; // you'll have to manually accept this self-signed cert if 'true'!
    server.setup( options );
    server.addListener(this);// this adds your app as a listener for the server

    //CAMERA and Window
	camWidth = 1280;
	camHeight = 720;

	ofSetWindowShape(camWidth, camHeight);

	//we can now get back a list of devices.
    vector<ofVideoDevice> devices = video.listDevices();

    for(int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

    video.setDeviceID(0);
    video.setDesiredFrameRate(60);
    video.initGrabber(camWidth, camHeight);

    zoom = 1.0;
    maxZoom = 1.0;
    startPanZoom = false;
    zooming = false;
    zoomSpeed = 0.01;
    panX = false;
    panY = false;
    panSpeed = 1;

    captureFrame = false;

    //Clients' status
    isImageConnected = false;
    isTextConnected = false;
    isImageReady = false;

    //font 
    ofTrueTypeFont::setGlobalDpi(72);
    font.load("verdana.ttf", 24, true, true);
}

//--------------------------------------------------------------
void ofApp::setupCycle(){
	startPanZoom = true;
	zooming = true;
	panX = true;
	panY = true;
	maxZoom = ofRandom(1.0, 10.0);
	maxPosX = ofRandom(-camWidth/2, camWidth/2);
	maxPosY = ofRandom(-camHeight/2, camHeight/2);
	server.send("$reset;");
}

//--------------------------------------------------------------
void ofApp::panZoomCamera(){
	if(zooming){
		if(zoom<=maxZoom){
			zoom+=zoomSpeed;
			if(zoom>maxZoom){
				zooming = false;
			}
		}else{
			zoom-=zoomSpeed;
			if(zoom<=maxZoom){
				zooming = false;
			}
		}
	}

	if(panX){
		if(posX<=maxPosX){
			posX+=panSpeed;
			if(posX>maxPosX){
				panX = false;
			}
		}else{
			posX-=panSpeed;
			if(posX<=maxPosX){
				panX = false;
			}
		}
	}

	if(panY){
		if(posY<=maxPosY){
			posY+=panSpeed;
			if(posY>maxPosY){
				panY = false;
			}
		}else{
			posY-=panSpeed;
			if(posY<=maxPosY){
				panY = false;
			}
		}
	}

	if(!zooming && !panX && !panY){
		startPanZoom = false;
		captureFrame = true;
	}

}

//--------------------------------------------------------------
string ofApp::getCaption(const char* cmd){
    cout<< "Requesting data from captionbot.ai"<< endl;
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
	return result;
}

//--------------------------------------------------------------
void ofApp::update(){

	if(isImageConnected && isTextConnected){
		//RUN SYSTEM
		video.update();

		if(startPanZoom){
			panZoomCamera();
		}

		if(captureFrame){	
			ofSaveScreen("frame.png");
			captureFrame = false;
			string result = getCaption("python data/captionbot.py data/frame.png");
			while (result == ""){
				cout<< "\nRetrying..."<< endl;
				result = getCaption("python data/captionbot.py data/frame.png");
			}
	        cout<< "RESULT: " + result<< endl;
	        server.send(result);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(255,255,255);

	if(isImageConnected && isTextConnected){
		//RUN SYSTEM
		ofPushMatrix();
		    //move "transformation point" from top left corner to centre
		    //by translating by half the frame size
		    ofTranslate(camWidth/2 + posX, camHeight/2 + posY);
		    //scale from centre
		    ofScale(zoom, zoom);
		    //move coordinate system back to top left, and draw the video
		    video.draw(-camWidth/2 - posX, -camHeight/2 - posY);
	  	ofPopMatrix(); 
	}else{
		string initMessage = "Waiting for Clients...";
		font.drawString(initMessage, 20, 40);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	setupCycle();
}

//--------------------------------------------------------------
void ofApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"new connection open"<<endl;
    cout<<args.conn.getClientIP()<< endl;
}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
    cout<< args.conn.getClientIP()<< endl;
    
    string ip = args.conn.getClientIP();
    
    if(ip == "192.168.1.110"){
    	isImageConnected = false;
    }

    if(ip == "192.168.1.120"){
    	isTextConnected = false;
    }
}

//--------------------------------------------------------------
void ofApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args ){
    cout<<"got message: "<<args.message<<endl;

    if(args.message == "$image;"){
    	isImageConnected = true;
    }

    if(args.message == "$text;"){
    	isTextConnected = true;
    }

    if(args.message == "$image;ready"){
    	isImageReady = true;
    }

    if(args.message == "$text;ready"){
    	
    }

    if(args.message == "$reset;"){
    	setupCycle();
    }
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;    
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
