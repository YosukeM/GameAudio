#include "OggVorbisDecoder.h"

#include "../../include/gameaudio/IFileReader.h"

using namespace gameaudio;

namespace {
	size_t read_func(void *ptr, size_t size, size_t nmemb, void *datasource) {
		IFileReader* reader = (IFileReader*)datasource;
		return reader->read(ptr, nmemb);
	}

	int seek_func(void *datasource, ogg_int64_t offset, int whence) {
		IFileReader* reader = (IFileReader*)datasource;

		IFileReader::seek_type st;
		switch (whence) {
		case SEEK_SET: st = IFileReader::E_SEEK_BEG; break;
		case SEEK_CUR: st = IFileReader::E_SEEK_CUR; break;
		case SEEK_END: st = IFileReader::E_SEEK_END; break;
		}

		if (reader->seek(offset, st)) {
			return 0;
		} else {
			return -1;
		}
	}

	long tell_func(void *datasource) {
		IFileReader* reader = (IFileReader*)datasource;
		return reader->tell();
	}

	ov_callbacks callback = {
		&read_func,
		&seek_func,
		NULL,
		&tell_func,
	};
}

bool OggVorbisDecoder::init(boost::shared_ptr<IFileReader> reader) {
	_reader = reader;
	if (ov_open_callbacks(reader.get(), &_vf, NULL, 0, callback) != 0) return false;

	_length = ov_pcm_total(&_vf, -1);
	vorbis_info *vi = ov_info(&_vf, -1);
	_frequency = vi->rate;
	_channelsNum = vi->channels;

	return true;
}

// header infomations
uint64 OggVorbisDecoder::getLengthBySamples() const {
	return _length;
}

unsigned OggVorbisDecoder::getFrequency() const {
	return _frequency;
}

unsigned OggVorbisDecoder::getChannelsNum() const {
	return _channelsNum;
}

unsigned OggVorbisDecoder::getBitNum() const {
	return 16;
}

unsigned OggVorbisDecoder::read(void* buf, uint64 position_by_samples, unsigned bytes_to_read) {
	if (ov_pcm_seek(&_vf, position_by_samples) != 0) return 0;

	int current_stream = 0;
	char* buffer = (char*)buf;
	uint64 total = 0;

	while (total < bytes_to_read) {
		int v = ov_read(&_vf, buffer, bytes_to_read, 0, 2, 1, &current_stream);
		switch (v) {
		case OV_HOLE:
			return 0;
		case OV_EBADLINK:
			return 0;
		case OV_EINVAL:
			return 0;
		case 0:
			return total;
		}
		total += v;
		buffer += v;
	}
	return total;
}