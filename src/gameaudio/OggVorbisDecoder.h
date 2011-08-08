#ifndef GAMEAUDIO_OGGVORBISDECODER_H_
#define GAMEAUDIO_OGGVORBISDECODER_H_

#include "IDecoder.h"
#include <vorbis/vorbisfile.h>
#include "../../include/gameaudio/IFileReader.h"

namespace gameaudio {

	class IFileReader;

	class OggVorbisDecoder : public IDecoder {
		unsigned _frequency, _channelsNum;
		uint64 _length;

		OggVorbis_File _vf;
		boost::shared_ptr<IFileReader> _reader;

	public:
		OggVorbisDecoder() {};
		~OggVorbisDecoder() {};

		virtual bool init(boost::shared_ptr<IFileReader>);

		// header infomations
		uint64 getLengthBySamples() const;
		virtual unsigned getFrequency() const;
		virtual unsigned getChannelsNum() const;
		virtual unsigned getBitNum() const;

		// read
		virtual unsigned read(void*, uint64 position_by_samples, unsigned bytes_to_read);
	};
};

#endif