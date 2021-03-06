#ifndef GAMEAUDIO_ISOUNDMANAGER_H_
#define GAMEAUDIO_ISOUNDMANAGER_H_

#include "ISound.h"

namespace gameaudio {
	class ISound;
	class IListener;
	class ISoundManager;
	class IFileFactory;

	ISoundManager& getSoundManager();
	void releaseSoundManager();

	class ISoundManager {
	protected:
		ISoundManager() {};
		ISoundManager(const ISoundManager&);
		virtual ~ISoundManager() {};
	public:
		virtual ISound* createSound(const char* filepath, bool loop, bool stream, unsigned looppos = 0, ISound::encoding_type encoding = ISound::ENCODING_AUTO) = 0;
		virtual ISound* createSound(const wchar_t* filepath, bool loop, bool stream, unsigned looppos = 0, ISound::encoding_type encoding = ISound::ENCODING_AUTO) = 0;
		virtual void removeSound(ISound*) = 0;

		virtual IListener* getListener() const = 0;

		virtual void addFileFactory(IFileFactory*) = 0;
		virtual void removeFileFactory(IFileFactory*) = 0;

		virtual void setDopplerFactor(float) = 0;
		virtual float getDopplerFactor() = 0;

		virtual void setSpeedOfSound(float) = 0;
		virtual float getSpeedOfSound() = 0;
	};
};

#endif