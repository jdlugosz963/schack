#include "mp3player.h"

MP3Player *mp3Player;

void setup() {
	Serial.begin(9600);
	SD.begin(SS);

	mp3Player = new MP3Player();
}


void loop() {
	mp3Player->loop();
}
