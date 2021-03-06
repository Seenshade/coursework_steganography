#pragma once

#include <iostream>
#include <vector>

struct WavHeader {
	char chunk_id[5];
	char format[5];
	uint32_t chunk_size;
	char subchunk1id[5];
	uint32_t subchunk1size;
	uint32_t audio_format;
	uint32_t num_channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint32_t block_align;
	uint32_t bits_per_sample;
	char subchunk2id[5];
	uint32_t subchunk2size;
	
	uint32_t minutes_duration;
	uint16_t seconds_duration;
};

template<typename T>
std::ostream &operator<<(std::ostream &output, const std::vector<std::pair<T, T>> &v) {
	for (const auto&[left, right] : v) {
		output << "(" << left << ", " << right << ")\n";
	}
	return output;
}

class WAV {
public:
	~WAV();
	void ReadHeader(std::ifstream &input);
	void PrintWavInfo();
	void PrintWavData();
	void WriteModified(std::ofstream &output);
	void WriteMessageToWAV(const std::string &message);
	std::string GetMessageFromWAV(std::ifstream &input, size_t n);
	void ReadWavData(std::ifstream &input);
	uint32_t GetWavDataSize() const;
	
private:
	void WriteData(std::ofstream &output);
	void ToLittleEndianHeaderFrom2BytesBigEndian(char *buf, uint16_t value);
	void ToLittleEndianHeaderFrom4BytesBigEndian(char *buf, uint32_t value);
	void ToLittleEndianDataFrom2BytesNumberFromData(char *buf, int16_t value);
	int16_t Get2BytesNumberFromData(char *buf);
	uint16_t Get2BytesNumberFromHeader(char *buf);
	uint32_t Get4BytesNumberFromHeader(char *buf);
	
	WavHeader header;
	std::vector<std::pair<int16_t, int16_t>> wav_data;
};