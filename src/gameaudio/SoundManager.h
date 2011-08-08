#ifndef GAMEAUDIO_SOUNDMANAGER_H_
#define GAMEAUDIO_SOUNDMANAGER_H_

#include "../../include/gameaudio/ISoundManager.h"

#include <alc.h>
#include <vector>
#include <list>

namespace gameaudio {

	class ISound;
	class IListener;
	class Sound;
	class Listener;
	class FileFactory;

	class SoundManager : public ISoundManager {
		ALCdevice* _device;
		ALCcontext* _context;
		std::vector<Sound*> _sounds;
		std::list<IFileFactory*> _fileFactories;
		Listener *_listener;
		FileFactory *_defaultFileFactory;

	public:
		SoundManager();
		~SoundManager();

		virtual ISound& createSound(const char* filepath, bool loop, bool stream, unsigned looppos = 0, ISound::encoding_type encoding = ISound::ENCODING_AUTO);
		virtual ISound& createSound(const wchar_t* filepath, bool loop, bool stream, unsigned looppos = 0, ISound::encoding_type encoding = ISound::ENCODING_AUTO);
		
		virtual IListener& getListener() const;

		virtual void addFileFactory(IFileFactory*);
		virtual void removeFileFactory(IFileFactory*);
	};

};

#endif