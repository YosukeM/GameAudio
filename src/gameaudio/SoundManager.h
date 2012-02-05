#ifndef GAMEAUDIO_SOUNDMANAGER_H_
#define GAMEAUDIO_SOUNDMANAGER_H_

#include "../../include/gameaudio/ISoundManager.h"

#include <alc.h>
#include <set>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>

namespace gameaudio {

	class ISound;
	class IListener;
	class SoundAbstract;
	class Listener;
	class FileFactory;
	class IFileReader;

	class SoundManager : public ISoundManager {
		ALCdevice* _device;
		ALCcontext* _context;
		std::set<SoundAbstract*> _sounds;
		std::list<IFileFactory*> _fileFactories;
		Listener *_listener;
		FileFactory *_defaultFileFactory;
		boost::mutex *_mutex;
		boost::thread *_thread;
		volatile bool _threadEnd;
		volatile bool _modified;
		SoundAbstract* _lastModified;

		SoundAbstract* _createSound(IFileReader*, ISound::encoding_type, bool stream, bool loop, unsigned looppos);
		void _update();

	public:
		SoundManager();
		~SoundManager();

		virtual ISound* createSound(const char* filepath, bool loop, bool stream, unsigned looppos = 0, ISound::encoding_type encoding = ISound::ENCODING_AUTO);
		virtual ISound* createSound(const wchar_t* filepath, bool loop, bool stream, unsigned looppos = 0, ISound::encoding_type encoding = ISound::ENCODING_AUTO);
		virtual void removeSound(ISound*);

		virtual IListener* getListener() const;

		virtual void addFileFactory(IFileFactory*);
		virtual void removeFileFactory(IFileFactory*);

		virtual void setDopplerFactor(float);
		virtual float getDopplerFactor();

		virtual void setSpeedOfSound(float);
		virtual float getSpeedOfSound();
	};

};

#endif