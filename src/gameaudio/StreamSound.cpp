#include "StreamSound.h"
#include "IDecoder.h"

#include <al.h>
#include <cassert>
#include <boost/thread/thread_time.hpp>
#include <boost/thread.hpp>

using namespace gameaudio;

namespace {
	const unsigned CHUNK_SEC_SIZE = 5;
}

StreamSound::StreamSound(boost::shared_ptr<IFileReader> reader, encoding_type encode, bool is_looped, unsigned looppos)
: _decoder(0), _statePosition(0), _isLooped(is_looped), _loopPosition(looppos), _loadedPosition(0), _queuedPosition(0), _mutex(0)
{
	_mutex = new boost::mutex();
	_status[0] = _status[1] = NOT_LOADED;
	_decoder = createDecoder(reader, encode);
	alGenBuffers(2, _buffers);
}

StreamSound::~StreamSound() {
	alDeleteBuffers(2, _buffers);
	delete _mutex;
}

void StreamSound::_updateStatus() {
	ALint processed_num;
	alGetSourcei(_alSource, AL_BUFFERS_PROCESSED, &processed_num);

	if (processed_num >= 1) {
		_status[_statePosition] = PLAYED;
		_statePosition = 1 - _statePosition;	// next pos
		if (_status[_statePosition] == NOT_PLAYED) {
			_status[_statePosition] = PLAYING;
		} else {
			_statePosition = 1 - _statePosition;	// prev pos
		}
	}
}

void StreamSound::update() {
	boost::mutex::scoped_lock lock(*_mutex);

	_updateStatus();

	for (int i = 0; i < 2; ++i) {
		int pos = (_statePosition + i) % 2;
		switch (_status[pos]) {
		case PLAYED:
			if (_status[1 - pos] == PLAYING) {
				ALuint buffer;
				alSourceUnqueueBuffers(_alSource, 1, &buffer);
				assert(buffer == _buffers[pos]);
				ALint size;
				alGetBufferi(buffer, AL_SIZE, &size);
				_queuedPosition += size / (_decoder->getBitNum() * _decoder->getChannelsNum() / 8);
				_queuedPosition %= _decoder->getLengthBySamples();
				// continues
			} else {
				break;
			}
		case NOT_LOADED:
			{
				const unsigned bytes_par_sample = _decoder->getBitNum() * _decoder->getChannelsNum() / 8;
				unsigned size = CHUNK_SEC_SIZE * bytes_par_sample * _decoder->getFrequency();
				// if sound length > CHUK_SEC_SIZE * 2
				if (size > _decoder->getSizeByBytes()) {
					if (pos == 0) {
						size = _decoder->getLengthBySamples() / 2 * bytes_par_sample;
					} else {
						size = (_decoder->getLengthBySamples() - _decoder->getLengthBySamples() / 2) * bytes_par_sample;
					}
				}
				// endif

				char* data = new char[size];
				unsigned total_read_size = 0;

				if (_isLooped) {
					const unsigned SEEM_AS_END_OF_SOUND = 5;
					const unsigned LOOP_LIMIT = 5;
					unsigned count = 0;
					unsigned eofcount = 0;
					unsigned read_size = 0;
					do {
						read_size = _decoder->read(data + total_read_size, _loadedPosition, size - read_size);
						if (read_size == 0) {
							eofcount++;
							if (eofcount > SEEM_AS_END_OF_SOUND) {
								_loadedPosition = _loopPosition;
								eofcount = 0;
								count++;
								if (count > LOOP_LIMIT) break;
							}
						} else {
							total_read_size += read_size;
							_loadedPosition = (_loadedPosition + read_size / bytes_par_sample);
							if (_loadedPosition >= _decoder->getLengthBySamples()) {
								_loadedPosition -= _decoder->getLengthBySamples() - _loopPosition;
							}
						}
					} while (total_read_size < size);
				} else {
					total_read_size = _decoder->read(data, _loadedPosition, size);
					_loadedPosition = _loadedPosition + total_read_size / bytes_par_sample;
				}

				alBufferData(_buffers[pos], _decoder->getFormat(), data, total_read_size, _decoder->getFrequency());
				alSourceQueueBuffers(_alSource, 1, _buffers + pos);
				_status[pos] = NOT_PLAYED;

				delete[] data;
			}
			break;
		case NOT_PLAYED:
		case PLAYING:
			// nothing to do
			break;
		}
	}
}

void StreamSound::_waitUntilLoaded() {
	while (_status[0] == NOT_LOADED && _status[1] == NOT_LOADED) {
		boost::thread::sleep(boost::get_system_time()+boost::posix_time::milliseconds(1));
	}
}

void StreamSound::play() {
	if (!isPlaying()) {
		_waitUntilLoaded();
		SoundAbstract::play();
	}
}

void StreamSound::stop() {
	pause();
	setPlayPositionBySamples(0);
}

void StreamSound::rewind() {
	setPlayPositionBySamples(0);
}

uint64 StreamSound::getLengthBySamples() const {
	return _decoder->getLengthBySamples();
}

float StreamSound::getLengthBySecs() const {
	return (float)((double)_decoder->getLengthBySamples() / (double)_decoder->getFrequency());
}

void StreamSound::setPlayPositionBySamples(uint64 pos) {
	boost::mutex::scoped_lock lock(*_mutex);
	_status[0] = _status[1] = NOT_LOADED;
	_loadedPosition = _queuedPosition = pos;
}

void StreamSound::setPlayPositionBySecs(float pos) {
	boost::mutex::scoped_lock lock(*_mutex);
	_status[0] = _status[1] = NOT_LOADED;
	_loadedPosition = _queuedPosition = (uint64)((double)pos * (double)_decoder->getFrequency());
}

uint64 StreamSound::getPlayPositionBySamples() const {
	boost::mutex::scoped_lock lock(*_mutex);
	ALint processed_num;
	alGetSourcei(_alSource, AL_BUFFERS_PROCESSED, &processed_num);
	ALint size = 0;
	if (processed_num) {
		alGetBufferi(_buffers[1-_statePosition], AL_SIZE, &size);
		size /= _decoder->getFrequency() * _decoder->getChannelsNum() * _decoder->getBitNum() / 8;
	}
	ALint offset;
	alGetSourcei(_alSource, AL_SAMPLE_OFFSET, &offset);
	return (_queuedPosition + (offset - size)) % _decoder->getLengthBySamples();
}

float StreamSound::getPlayPositionBySecs() const {
	return (float)getPlayPositionBySamples() / (float)_decoder->getFrequency();
}