#include "WavDecoder.h"
#include "../../include/gameaudio/IFileReader.h"

using namespace gameaudio;

namespace {
	bool rescmp(const char* buf, const char* name) {
		for (unsigned i = 0; i < 4; ++i) {
			if (buf[i] != name[i]) return false;
		}
		return true;
	}
	unsigned read4(const char* buf) {
		unsigned data = 0;
		for (unsigned i = 0; i < 4; ++i) {
			data += (unsigned char)(buf[i]) << (i * 8);
		}
		return data;
	}
	unsigned short read2(const char* buf) {
		unsigned short data = 0;
		for (unsigned i = 0; i < 2; ++i) {
			data += (unsigned char)(buf[i]) << (i * 8);
		}
		return data;
	}
}

WavDecoder::WavDecoder() {};
WavDecoder::~WavDecoder() {};

bool WavDecoder::init(boost::shared_ptr<IFileReader> reader) {
	char buf4[4];
	char buf2[2];
	unsigned data_pos = 0;

#define GET4 { if (reader->read(buf4, 4) != 4) return false; data_pos += 4; }
#define GET2 { if (reader->read(buf2, 2) != 2) return false; data_pos += 2; }
	
	GET4;
	if (!rescmp(buf4, "RIFF")) return false;
	
	GET4;
	unsigned data_size = read4(buf4);
	// if (data_size != reader->getSize() - 8) return false;
	
	GET4;
	if (!rescmp(buf4, "WAVE")) return false;

	bool fmt_read = false, data_read = false;

	for (unsigned i = 0; i < 2; ++i) {
		GET4;
		if (rescmp(buf4, "fmt ")) {
			if (fmt_read) return false;

			// chunk size
			GET4;
			unsigned chunk_size = read4(buf4);
			if (chunk_size != 16) return false;

			// pcm format
			GET2;
			if (read2(buf2) != 1) return false;

			// channels num
			GET2;
			_channelsNum = read2(buf2);
			if (_channelsNum != 1 && _channelsNum != 2) return false;

			// frequency
			GET4;
			_frequency = read4(buf4);

			// byte / sec
			GET4;
			unsigned bps = read4(buf4);

			// block size
			GET2;
			unsigned block_size = read2(buf2);

			// bit num
			GET2;
			_bitNum = read2(buf2);
			if (_bitNum != 8 && _bitNum != 16) return false;

			if (_bitNum * _channelsNum / 8 != block_size) return false;
			if (block_size * _frequency != bps) return false;

			if (data_read) {
				reader->seek(_initialDataPos);
			}

			fmt_read = true;

		} else if (rescmp(buf4, "data")) {
			if (data_read) return false;

			GET4;
			unsigned data_size = read4(buf4);

			_initialDataPos = data_pos;
			reader->seek(data_pos + data_size);

			data_read = true;

		} else if (rescmp(buf4, "fact")) {
			GET4;
			GET4;
		} else {
			return false;
		}
	}

	_length = data_size / _channelsNum / (_bitNum / 8);
	_reader = reader;
	return true;
}

uint64 WavDecoder::getLengthBySamples() const {
	return _length;
}

unsigned WavDecoder::getFrequency() const {
	return _frequency;
}

unsigned WavDecoder::getChannelsNum() const {
	return _channelsNum;
}

unsigned WavDecoder::getBitNum() const {
	return _bitNum;
}

unsigned WavDecoder::read(void* data, uint64 position_by_samples, unsigned bytes_to_read) {
	unsigned bytes_par_sample = getChannelsNum() * (getBitNum() / 8);
	_reader->seek(_initialDataPos + position_by_samples * bytes_par_sample);
	unsigned read_size = _reader->read(data, bytes_to_read);
	if (read_size != bytes_to_read) {
		// end of sound
		_length = position_by_samples + read_size / bytes_par_sample;
	}
	return read_size;
}