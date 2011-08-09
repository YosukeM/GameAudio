#include "IDecoder.h"
#include "OggVorbisDecoder.h"
#include "WavDecoder.h"
#include "../../include/gameaudio/Error.h"

using namespace gameaudio;

IDecoder* gameaudio::createDecoder(boost::shared_ptr<IFileReader> reader, ISound::encoding_type encoding) {
	IDecoder *decoder = 0;
	switch (encoding) {
	case ISound::ENCODING_OGG_VORBIS:
		decoder = new OggVorbisDecoder();
		break;
	case ISound::ENCODING_WAV:
		decoder = new WavDecoder();
		break;
	default:
		throw Error("This file format is not supported.");
	}
	if (!decoder->init(reader)) {
		throw Error("Unkown wav format");
	}
	return decoder;
}

ALenum IDecoder::getFormat() const {
	ALenum format = -1;
	if (getChannelsNum() == 1) {
		if (getBitNum() == 8) {
			format = AL_FORMAT_MONO8;
		} else if (getBitNum() == 16) {
			format = AL_FORMAT_MONO16;
		}
	} else if (getChannelsNum() == 2) {
		if (getBitNum() == 8) {
			format = AL_FORMAT_STEREO8;
		} else if (getBitNum() == 16) {
			format = AL_FORMAT_STEREO16;
		}
	}
	return format;
};