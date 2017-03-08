# 1_&_n_chairs

1 & N chairs is an autonomous system which uses an on-line cognitive computation engine, custom software, a usb camera and 2 screens each connected to a miniature Linux PC. The process starts with the camera automatically tilting, panning and zooming the image into a unique part of the physical wooden chair (see figure 4). On each cycle the camera searches for a new unique point of view of the wooden object. Once it targets its new frame, it captures a picture, during this process the spectator is able to see, in real-time, what the camera sees and targets through the camera's display (see item 2 in figure 3). The captured image is then sent to the on-line cognitive computing system which after a few seconds calls-back with a caption phrasing what it sees; a human like description of the reality encountered in the photograph. At this point of the process the textual interpretation given by the cognitive machine is used in two different forms; it is rendered as text on the right screen of the installation (see item 7 in figure 3), while in parallel, the same text is used as the query for a web-based image search. The returned image is finally displayed on the left screen mounted on the wall (see item 6 in figure 3). The process loops, each cycle starting with a different and unique image of the original chair, hence rendering  each instance as a unique imagined reality based on the algorithms behind the cognitive computational model.

###Requirements

####Operating Systems

For all Raspberry Pis download and install Raspbian Jessie [DOWNLOAD](https://www.raspberrypi.org/downloads/raspbian/) and [SETUP GUIDE](https://www.raspberrypi.org/documentation/installation/installing-images/README.md)

####Rarspberry Pi CAMERA:

This part of the syste uses a raspberry pi 3 model B [LINK](https://www.raspberrypi.org/products/raspberry-pi-3-model-b/)

1. Download and install openframeworks for raspberry pi armv6 [DOWNLOAD](http://openframeworks.cc/download/) and [SETUP INSTRUCTIONS](http://openframeworks.cc/setup/raspberrypi/) 
2. Download ofxLibwebsockets addon [DOWNLOAD](https://github.com/robotconscience/ofxLibwebsockets)
3. Make sure you install the right version of requests for python, this project uses version 2.6.0 since it makes use of InsecureRequestWarning:
	sudo easy_install --upgrade pip
	sudo pip install requests==2.6.0

####Rarspberry Pi IMAGE:

This part of the syste uses a raspberry pi 1 model B [LINK](https://www.raspberrypi.org/products/model-b/)

1. Download and install openframeworks for raspberry pi armv6 [DOWNLOAD](http://openframeworks.cc/download/) and [SETUP INSTRUCTIONS](http://openframeworks.cc/setup/raspberrypi/) 
2. Download ofxLibwebsockets addon [DOWNLOAD](https://github.com/robotconscience/ofxLibwebsockets)

####Rarspberry Pi TEXT:

This part of the syste uses a raspberry pi 1 model B [LINK](https://www.raspberrypi.org/products/model-b/)

1. Download and install openframeworks for raspberry pi armv6 [DOWNLOAD](http://openframeworks.cc/download/) and [SETUP INSTRUCTIONS](http://openframeworks.cc/setup/raspberrypi/) 
2. Download ofxLibwebsockets addon [DOWNLOAD](https://github.com/robotconscience/ofxLibwebsockets)
3. Download ofxCenteredTrueTypeFont addon [DOWNLOAD](https://github.com/armadillu/ofxCenteredTrueTypeFont)

###Setup Static IPs for all Raspberry Pis
RbPi camera is 192.168.1.100
RbPi image is 192.168.1.110
RbPi text is 192.168.1.120

###Configuration

All 3 application contain a config.xml file located at /appName/bin/data

####Camera App

Located at: /1_n_chairs_camera/bind/data/config.xml
	
	<CONFIG>
	<panTopMax>0</panTopMax>
	<panBotMax>0</panBotMax>
	<panRightMax>0</panRightMax>
	<panLeftMax>0</panLeftMax>
	<zoomMax>10.0</zoomMax>
	<connectionTimeout>5000</connectionTimeout>
	</CONFIG>

- panLefMax: 			Maximum left margin to which camera can pan
- panRightMax:		Maximum right margin to which camera can pan
- panTopMax: 			Maximum top margin to which camera can pan
- panBotMax: 			Maximum bottom margin to which camera can pan
- zoomMax:			Maximum zoom allowed
- cycleTimeLimit: 	Time between cycles of the system
- port:				Port to open with the websocket server
- fontSize: 			Font size for display messages

###Protocol Design

- server: 1_&_n_chairs_camera	-> ID: $camera;
- client: 1_&_n_chairs_image	-> ID: $image;
- client: 1_&_n_chairs_text	-> ID: $text;

####STARTING

When the system is on, the server waits for a message from each client:
	
	$image;ok
	$text;ok

If one of the messages is not received, the system cannot will NOT start.

####SYNCHRONIZING

Every time the server broadcasts a message to the clients, whichever clients finishes processing first waits for the other to finish before displaying the result on the screen.

			SERVER
			|	 |
	 CLIENT-1	 CLIENT-2

The server is the bridge between the clients. Once a client is ready to display on its screen, it first sends a message to the server. The server waits for the other client to send the READY to display message. Once the server receives both READY signals, it broadcasts back a message for displaying.

	$image;ready
	$text;ready
	-----------
	$display;

####RESTARTING A CYLCE

The server sends a reset message and a new cycle begins:
	
	$reset;

####CLIENT LOST

If a client connection is lost, the server checks for its IP and stops the system. This is checked on the onClose() event of the ofxLibwebsocket library.

- $image; is 192.168.1.110
- $text; is 192.168.1.120

the system only runs if both state booleans are true:

	if(isImageConnected && isTextConnected){
		//RUN SYSTEM
	}else{
		//WAIT FOR BOTH CLIENTS TO CONNECT
	}

####Auto Start APP at boot

Run the following command on the terminal:

	crontab -u pi -e
	
Add the path to the executable file, example:

	@reboot /usr/bin/sudo -u pi -H /home/pi/REST-OF-PATH/APP-NAME
