#include "controller.h"
#include "mp3player.h"
#include "webserver.h"

WebServer *webserver;
MP3Player *mp3Player;
Controller *controller;

void setup() {
	Serial.begin(9600);
	SD.begin(SS);

	mp3Player = new MP3Player();
	controller = new Controller(mp3Player);
	webserver = new WebServer(controller);
}


void loop() {
	mp3Player->loop();
	webserver->loop();
}
