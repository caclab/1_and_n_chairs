# 1_-_n_chairs

###Rarspberry Pi CAMERA:

1. Install openframeworks
2. Download ofxLibwebsockets
3. Make sure you install the right version of requests for python, this project uses version 2.6.0 since it makes use of InsecureRequestWarning:
	sudo easy_install --upgrade pip
	sudo pip install requests==2.6.0

###Setup Static IPs for all Raspberry Pis
RbPi camera is 192.168.1.100
RbPi image is 192.168.1.110
RbPi text is 192.168.1.120

###Protocol Design

server: 1_&_n_chairs_camera	-> ID: $camera;
client: 1_&_n_chairs_image	-> ID: $image;
client: 1_&_n_chairs_text	-> ID: $text;

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

$image; is 192.168.1.110
$text; is 192.168.1.120

the system only runs if both state booleans are true:

	if(isImageConnected && isTextConnected){
		//RUN SYSTEM
	}else{
		//WAIT FOR BOTH CLIENTS TO CONNECT
	}