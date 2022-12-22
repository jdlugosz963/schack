#include "controller.h"
#include "mp3player.h"

MP3Player *mp3Player;
Controller *controller;

void setup() {
	Serial.begin(9600);
	SD.begin(SS);

	mp3Player = new MP3Player();
	controller = new Controller(mp3Player);
}


void loop() {
	mp3Player->loop();
}
