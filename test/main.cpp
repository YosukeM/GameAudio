#include "../include/gameaudio/gameaudio.h"
#include <iostream>
#ifndef _WIN32
	#include <unistd.h>
#define Sleep(x) usleep((x) * 1000)
#else
	#include <Windows.h>
	#include <string>

#endif

#include <boost/lexical_cast.hpp>

using namespace gameaudio;
using namespace std;

int main(int argc, const char** argv) {
#ifndef _WIN32
	chdir("../../");
#else
	TCHAR szDirectoryName[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDirectoryName);
	SetCurrentDirectory((std::wstring(szDirectoryName) + L"/../").c_str());
#endif
	
	ISoundManager& sm = getSoundManager();

	ISound& sound = sm.createSound("/Users/yosuke/Projects/bussen/bussen/blind-evil.ogg", false, true);
	sound.play();
	
	while (true) {
		cout << boost::lexical_cast<string>(sound.getPlayPositionBySecs()) << endl;
		Sleep(100);
	}

	sm.removeSound(sound);
	
	return 0;
}