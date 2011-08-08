#include "SoundManager.h"

#include <boost/scoped_ptr.hpp>
#include "../../include/gameaudio/Error.h"
#include "Sound.h"
#include "Listener.h"
#include "../../include/gameaudio/IFileFactory.h"
#include <algorithm>
#include "FileFactory.h"

using namespace gameaudio;

namespace {
	boost::scoped_ptr<SoundManager> gSoundManager;
}

ISoundManager& gameaudio::getSoundManager() {
	if (!gSoundManager) {
		gSoundManager.reset(new SoundManager());
	}
	return *(gSoundManager.get());
}

SoundManager::SoundManager()
	: _device(NULL), _context(NULL), _listener(NULL), _defaultFileFactory(NULL)
{
	_device = alcOpenDevice(NULL);
	if (!_device) throw Error("Cannot open OpenAL device");
    _context = alcCreateContext(_device, NULL);
	if (!_device) throw Error("Cannot create OpenAL context");
    alcMakeContextCurrent(_context);

	_defaultFileFactory = new FileFactory();
	addFileFactory(_defaultFileFactory);
	_listener = new Listener();
}

SoundManager::~SoundManager() {
	_fileFactories.clear();
	delete _defaultFileFactory;

	alcMakeContextCurrent(NULL);
	alcDestroyContext(_context);
	alcCloseDevice(_device);
}

template<typename T> ISound::encoding_type detectEncodingType(T path) {
	ISound::encoding_type encoding;
	if (path.rfind(".ogg") == path.size() - 4) {
		encoding = ISound::ENCODING_OGG_VORBIS;
	} else {
		encoding = ISound::ENCODING_WAV;
	}
	return encoding;
}
template<> ISound::encoding_type detectEncodingType(std::wstring path) {
	ISound::encoding_type encoding;
	if (path.rfind(L".ogg") == path.size() - 4) {
		encoding = ISound::ENCODING_OGG_VORBIS;
	} else {
		encoding = ISound::ENCODING_WAV;
	}
	return encoding;
}

ISound& SoundManager::createSound(const char* filepath, bool loop, bool stream, unsigned looppos, ISound::encoding_type encoding) {
	IFileReader* reader = NULL;
	for (std::list<IFileFactory*>::iterator itr = _fileFactories.begin(); itr != _fileFactories.end(); ++itr) {
		if (reader = (*itr)->createFileReader(filepath)) break;
	}
	if (reader == NULL) {
		throw Error("Cannot find available file reader");
	}
	if (encoding == ISound::ENCODING_AUTO) {
		encoding = detectEncodingType(std::string(filepath));
	}
	Sound *sound = new Sound(boost::shared_ptr<IFileReader>(reader), loop, stream, looppos, encoding);
	_sounds.push_back(sound);
	return *sound;
}

ISound& SoundManager::createSound(const wchar_t* filepath, bool loop, bool stream, unsigned looppos, ISound::encoding_type encoding) {
	IFileReader* reader = NULL;
	for (std::list<IFileFactory*>::iterator itr = _fileFactories.begin(); itr != _fileFactories.end(); ++itr) {
		if (reader = (*itr)->createFileReader(filepath)) break;
	}
	if (reader == NULL) {
		throw Error("Cannot find available file reader");
	}
	if (encoding == ISound::ENCODING_AUTO) {
		encoding = detectEncodingType(std::wstring(filepath));
	}
	Sound *sound = new Sound(boost::shared_ptr<IFileReader>(reader), loop, stream, looppos, encoding);
	_sounds.push_back(sound);
	return *sound;
}

IListener& SoundManager::getListener() const {
	return *_listener;
}

void SoundManager::addFileFactory(IFileFactory* factory) {
	if (std::find(_fileFactories.begin(), _fileFactories.end(), factory) != _fileFactories.end()) {
		throw Error("The FileFactory has already added");
	}
	_fileFactories.push_front(factory);
}

void SoundManager::removeFileFactory(IFileFactory* factory) {
	std::list<IFileFactory*>::iterator itr;
	itr = std::find(_fileFactories.begin(), _fileFactories.end(), factory);
	if (itr == _fileFactories.end()) {
		throw Error("The FileFactory is not added");
	}
	_fileFactories.erase(itr);
}