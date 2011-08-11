#ifndef GAMEAUDIO_STREAMSOUND_H_
#define GAMEAUDIO_STREAMSOUND_H_

#include <boost/circular_buffer.hpp>
#include <al.h>
#include "../../include/gameaudio/IFileReader.h"
#include "SoundAbstract.h"
#include <boost/thread/mutex.hpp>

namespace gameaudio {
	class Sound;
	class IDecoder;

	class StreamSound : public SoundAbstract {
		Sound *_sound;
		IDecoder *_decoder;
		ALuint _buffers[2];
		typedef enum {
			NOT_LOADED,
			NOT_PLAYED,
			PLAYING,
			PLAYED,
		} state_type;
		volatile state_type _status[2];
		int _statePosition;
		uint64 _loadedPosition, _queuedPosition;
		bool _isLooped;
		unsigned _loopPosition;
		boost::mutex *_mutex;

		void _updateStatus();
		void _waitUntilLoaded();
	public:
		StreamSound(boost::shared_ptr<IFileReader> reader, encoding_type encode, bool is_looped, unsigned looppos);
		~StreamSound();
		void update();

		virtual void play();

		virtual void stop();
		virtual void rewind();

		virtual void setPlayPositionBySamples(uint64);
		virtual void setPlayPositionBySecs(float);

		virtual uint64 getPlayPositionBySamples() const;
		virtual float getPlayPositionBySecs() const;
	};
};

#endif