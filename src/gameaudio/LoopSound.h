#ifndef GAMEAUDIO_LOOPSOUND_H_
#define GAMEAUDIO_LOOPSOUND_H_

#include "SoundAbstract.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

namespace gameaudio {
	class IFileReader;

	class LoopSound : public SoundAbstract {
		ALuint _initialBuffer, _loopBuffer;
		unsigned _loopPosition, _frequency;
		volatile bool _initialBufferProcessed;
		boost::mutex *_mutex;
		void _initBuffers();
		void _resetBuffers();
	public:
		LoopSound(boost::shared_ptr<IFileReader> reader, encoding_type encoding, unsigned looppos);
		~LoopSound();

		void update();

		virtual void stop();
		virtual void rewind();

		virtual void setPlayPositionBySamples(uint64);
		virtual void setPlayPositionBySecs(float);

		virtual uint64 getPlayPositionBySamples() const;
		virtual float getPlayPositionBySecs() const;
	};
}

#endif