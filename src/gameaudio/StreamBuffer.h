#ifndef GAMEAUDIO_STREAMBUFFER_H_
#define GAMEAUDIO_STREAMBUFFER_H_

#include <boost/circular_buffer.hpp>
#include <al.h>

// スレッドセーフではない
// Soundクラスで整合性を取る

namespace gameaudio {
	class Sound;
	
	class StreamBuffer {
		Sound *_sound;
		IDecoder *_decoder;
		boost::circular_buffer<ALuint> _buffers;
	public:
		StreamBuffer(Sound *sound, IDecoder *decoder);
		~StreamBuffer();
		void update();
	};
};

#endif