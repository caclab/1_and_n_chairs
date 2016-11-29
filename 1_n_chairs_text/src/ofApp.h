#pragma once

#include "ofMain.h"
#include "ofxLibwebsockets.h"
#include "ofxCenteredTrueTypeFont.h"

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

		string makeTextBox(string s);
		ofxCenteredTrueTypeFont font;
		int fontSize;
		bool drawText;

		int marginX;

		string inputMessage;
		bool messageArrived;

		//WEBSOCKETS
		string address;
		int port;
		uint64_t lastReconnectTime;
        ofxLibwebsockets::Client client;
        void onConnect( ofxLibwebsockets::Event& args );
        void onOpen( ofxLibwebsockets::Event& args );
        void onClose( ofxLibwebsockets::Event& args );
        void onIdle( ofxLibwebsockets::Event& args );
        void onMessage( ofxLibwebsockets::Event& args );
        void onBroadcast( ofxLibwebsockets::Event& args );

        ofXml XML; //config file

        bool isFullScreen;
		
};
