#ifndef GAMEAUDIO_STATICSOUND_H_
#define GAMEAUDIO_STATICSOUND_H_

#include "SoundAbstract.h"
#include <boost/shared_ptr.hpp>

namespace gameaudio {
	class StaticSound : public SoundAbstract {
		ALuint _alBuffer;
		unsigned _frequency;
		uint64 _size;
	public:
		StaticSound(boost::shared_ptr<IFileReader> reader, encoding_type encoding);

		virtual uint64 getLengthBySamples() const;
		virtual float getLengthBySecs() const;
		
		virtual void update();
		
		virtual float getPlayPositionBySecs() const;
		virtual void setPlayPositionBySecs(float);
	};
}

#endif