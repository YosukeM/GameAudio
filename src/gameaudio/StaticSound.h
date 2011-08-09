#ifndef GAMEAUDIO_STATICSOUND_H_
#define GAMEAUDIO_STATICSOUND_H_

#include "SoundAbstract.h"
#include <boost/shared_ptr.hpp>

namespace gameaudio {
	class StaticSound : public SoundAbstract {
		ALuint _alBuffer;
	public:
		StaticSound(boost::shared_ptr<IFileReader> reader, encoding_type encoding);
	};
}

#endif