#ifndef GAMEAUDIO_IDECODER_H_
#define GAMEAUDIO_IDECODER_H_

#include <boost/shared_ptr.hpp>
#include "../../include/gameaudio/IFileReader.h"

namespace gameaudio {

	class IFileReader;

	class IDecoder {
	public:
		IDecoder() {};
		virtual ~IDecoder() {};

		// return true when succeeded
		virtual bool init(boost::shared_ptr<IFileReader>) = 0;

		// header infomations
		virtual uint64 getLengthBySamples() const = 0;
		virtual unsigned getFrequency() const = 0;
		virtual unsigned getChannelsNum() const = 0;
		virtual unsigned getBitNum() const = 0;
		virtual unsigned getSizeByBytes() const { return getLengthBySamples() * getChannelsNum() * (getBitNum() / 8); };

		// read
		virtual unsigned read(void*, uint64 position_by_samples, unsigned bytes_to_read) = 0;
	};

};

#endif