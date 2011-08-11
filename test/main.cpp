#include "../include/gameaudio/gameaudio.h"
#include <iostream>
#ifndef _WIN32
	#include <unistd.h>
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
	SetCurrentDirectory((std::wstring(szDirectoryName) + TEXT("/../")).c_str());
#endif
	
	ISoundManager& sm = getSoundManager();

	ISound& sound = sm.createSound("test/sound.wav", false, true);
	sound.play();
	
	while (true) {
		cout << boost::lexical_cast<string>(sound.getPlayPositionBySecs()) << endl;
		usleep(100000);
	}
	
	return 0;
}