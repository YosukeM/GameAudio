#include "SoundManager.h"

#include <boost/scoped_ptr.hpp>
#include "../../include/gameaudio/Error.h"
#include "SoundAbstract.h"
#include "Listener.h"
#include "../../include/gameaudio/IFileFactory.h"
#include <algorithm>
#include "FileFactory.h"

#include "LoopSound.h"
#include "StaticSound.h"
#include "StreamSound.h"

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace gameaudio;

namespace {
	SoundManager *gSoundManager;
	class SMDeleter {
	public:
		~SMDeleter() {
			gameaudio::releaseSoundManager();
		};
	} gSMDeleter;
}

ISoundManager& gameaudio::getSoundManager() {
	if (!gSoundManager) {
		gSoundManager = new SoundManager();
	}
	return *(gSoundManager);
}

void gameaudio::releaseSoundManager() {
	delete gSoundManager;
	gSoundManager = 0;
}

SoundManager::SoundManager()
	: _device(NULL), _context(NULL), _listener(NULL), _defaultFileFactory(NULL),
	_mutex(NULL), _thread(NULL), _threadEnd(false), _modified(false), _lastModified(NULL)
{
	_device = alcOpenDevice(NULL);
	if (!_device) throw Error("Cannot open OpenAL device");
	_context = alcCreateContext(_device, NULL);
	if (!_device) throw Error("Cannot create OpenAL context");
	alcMakeContextCurrent(_context);

	_defaultFileFactory = new FileFactory();
	addFileFactory(_defaultFileFactory);
	_listener = new Listener();

	_mutex = new boost::mutex();
	_thread = new boost::thread(boost::bind(&SoundManager::_update, this));
	
#ifdef _WIN32
	SetThreadPriority(_thread->native_handle(), THREAD_PRIORITY_TIME_CRITICAL);
#endif
}

SoundManager::~SoundManager() {
	{
		boost::mutex::scoped_lock lock(*_mutex);
		_threadEnd = true;
	}
	if (_thread->joinable()) _thread->join();
	delete _thread;
	delete _mutex;

	foreach (ISound *sound, _sounds) {
		delete sound;
	}
	_sounds.clear();

	_fileFactories.clear();
	delete _defaultFileFactory;

	alcMakeContextCurrent(NULL);
	alcDestroyContext(_context);
	alcCloseDevice(_device);
}

void SoundManager::_update() {
	std::set<SoundAbstract*>::iterator itr;
	bool is_empty = true;

	while (true) {
		{
			boost::mutex::scoped_lock lock(*_mutex);
			if (_sounds.empty()) {
				is_empty = true;
			} else {
				if (_modified) {
					itr = _sounds.upper_bound(_lastModified);
					if (itr == _sounds.end()) itr = _sounds.begin();
					_modified = false;
				}
				if (is_empty) {
					itr = _sounds.begin();
					is_empty = false;
				} else {
					++itr;
					if (itr == _sounds.end()) itr = _sounds.begin();
				}
				(*itr)->update();
			}
			if (_threadEnd) break;
		}
		
		boost::thread::sleep(boost::get_system_time()+boost::posix_time::milliseconds(1));
	}
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

SoundAbstract* SoundManager::_createSound(IFileReader* reader, ISound::encoding_type encoding, bool stream, bool loop, unsigned looppos) {
	SoundAbstract *sound;
	boost::shared_ptr<IFileReader> sreader(reader);

	if (stream) {
		sound = new StreamSound(sreader, encoding, loop, looppos);
	} else {
		if (loop) {
			sound = new LoopSound(sreader, encoding, looppos);
		} else {
			sound = new StaticSound(sreader, encoding);
		}
	}
	return sound;
}

ISound& SoundManager::createSound(const char* filepath, bool loop, bool stream, unsigned looppos, ISound::encoding_type encoding) {
	IFileReader* reader = NULL;
	foreach (IFileFactory* factory, _fileFactories) {
		if (reader = factory->createFileReader(filepath)) break;
	}
	if (reader == NULL) {
		throw Error("Cannot find available file reader");
	}
	if (encoding == ISound::ENCODING_AUTO) {
		encoding = detectEncodingType(std::string(filepath));
	}

	SoundAbstract* sound = _createSound(reader, encoding, stream, loop, looppos);
	{
		boost::mutex::scoped_lock lock(*_mutex);
		_sounds.insert(sound);
		_modified = true;
		_lastModified = sound;
	}
	return *sound;
}

ISound& SoundManager::createSound(const wchar_t* filepath, bool loop, bool stream, unsigned looppos, ISound::encoding_type encoding) {
	IFileReader* reader = NULL;
	foreach (IFileFactory* factory, _fileFactories) {
		if (reader = factory->createFileReader(filepath)) break;
	}
	if (reader == NULL) {
		throw Error("Cannot find available file reader");
	}
	if (encoding == ISound::ENCODING_AUTO) {
		encoding = detectEncodingType(std::wstring(filepath));
	}

	SoundAbstract* sound = _createSound(reader, encoding, stream, loop, looppos);
	{
		boost::mutex::scoped_lock lock(*_mutex);
		_sounds.insert(sound);
		_modified = true;
		_lastModified = sound;
	}
	return *sound;
}

void SoundManager::removeSound(ISound& s) {
	boost::mutex::scoped_lock lock(*_mutex);
	SoundAbstract *sound = dynamic_cast<SoundAbstract*>(&s);
	_sounds.erase(sound);
	_modified = true;
	_lastModified = sound;
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
	std::list<IFileFactory*>::iterator itr = std::find(_fileFactories.begin(), _fileFactories.end(), factory);
	if (itr == _fileFactories.end()) {
		throw Error("The FileFactory is not added");
	}
	_fileFactories.erase(itr);
}

void SoundManager::setDopplerFactor(float v) {
	alDopplerFactor(v);
}

float SoundManager::getDopplerFactor() {
	return alGetFloat(AL_DOPPLER_FACTOR);
}

void SoundManager::setSpeedOfSound(float v) {
	alSpeedOfSound(v);
}

float SoundManager::getSpeedOfSound() {
	return alGetFloat(AL_SPEED_OF_SOUND);
}