#pragma once

#include "ofMain.h"
#include "ofxLibwebsockets.h"

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
		
		void setupCycle();
		string getCaption(const char* cmd);//Get caption from captionbot.ai
		void panZoomCamera();//camera movements
		bool startPanZoom, zooming, panX, panY;

		int isFullScreen;

		bool captureFrame;

		ofVideoGrabber video;
		int camWidth, camHeight;
		float zoom, maxZoom;
		float zMax;
		float posX, posY, maxPosX, maxPosY;
		int panRightMax, panLeftMax, panTopMax, panBotMax;
		float panSpeed, zoomSpeed;

		//WEBSOCKETS
        ofxLibwebsockets::Server server;
        int port;
        bool bSetup; //socket setup
        string toSend; //data to send to machine via websockets
        void onConnect( ofxLibwebsockets::Event& args );
        void onOpen( ofxLibwebsockets::Event& args );
        void onClose( ofxLibwebsockets::Event& args );
        void onIdle( ofxLibwebsockets::Event& args );
        void onMessage( ofxLibwebsockets::Event& args );
        void onBroadcast( ofxLibwebsockets::Event& args );

        //booleans for checking status of clients
        bool isImageConnected, isImageReady;
        bool isTextConnected, isTextReady;

        //fonts
        ofTrueTypeFont font;
        int fontSize;

        //Cycle timer
        uint64_t startTime;
        uint64_t timeLimit;
        bool isCycleCounting;

        ofXml XML; //config file

};
