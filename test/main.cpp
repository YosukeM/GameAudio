#include "../include/gameaudio/gameaudio.h"
#include <iostream>
#include <unistd.h>

using namespace gameaudio;
using namespace std;

int main(int argc, const char** argv) {
	chdir("../../");
	char c;	// wait
	
	ISoundManager& sm = getSoundManager();
	ISound& sound = sm.createSound("test/sound.wav", false, false);
	sound.play();
	
	cin >> c;

	ISound& sound2 = sm.createSound("test/sound2.ogg", false, false);
	sound2.play();
	
	cin >> c;
	
	return 0;
}