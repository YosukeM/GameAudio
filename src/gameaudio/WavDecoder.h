#ifndef GAMEAUDIO_WAVDECODER_H_
#define GAMEAUDIO_WAVDECODER_H_

#include "IDecoder.h"

namespace gameaudio {
	class WavDecoder : public IDecoder {
		uint64 _length;
		unsigned _frequency, _channelsNum, _bitNum;
		unsigned _initialDataPos;
		boost::shared_ptr<IFileReader> _reader;
	public:
		WavDecoder();
		~WavDecoder();

		virtual bool init(boost::shared_ptr<IFileReader>);

		// header infomations
		virtual uint64 getLengthBySamples() const;
		virtual unsigned getFrequency() const;
		virtual unsigned getChannelsNum() const;
		virtual unsigned getBitNum() const;

		// read
		virtual unsigned read(void*, uint64 position_by_samples, unsigned bytes_to_read);
	};
};

#endif