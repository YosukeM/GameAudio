#include "../include/gameaudio/gameaudio.h"
#include <iostream>
#ifndef _WIN32
	#include <unistd.h>
#else
	#include <Windows.h>
	#include <string>
#endif

using namespace gameaudio;
using namespace std;

int main(int argc, const char** argv) {
#ifndef _WIN32
	chdir("../../");
#else
	TCHAR szDirectoryName[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDirectoryName);
	SetCurrentDirectory((std::wstring(szDirectoryName) + TEXT("/../")).c_str());
#endif
	char c;	// wait
	
	ISoundManager& sm = getSoundManager();

	ISound& sound2 = sm.createSound("test/sound.wav", true, false, 44100);
	sound2.play();
	
	cin >> c;
	
	return 0;
}