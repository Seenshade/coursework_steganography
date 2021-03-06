#include "wav.h"
#include "metrics.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <bitset>
#include <exception>
#include <iomanip>
#include <cmath>

WAV::~WAV() {
	wav_data.clear();
}

uint32_t WAV::GetWavDataSize() const {
	return header.subchunk2size;
}

void WAV::ReadWavData(std::ifstream &input) {
	char left[2], right[2];
	for (size_t i = 0; i < header.subchunk2size; i += 4) {
		input.read(left, 2);
		input.read(right, 2);
		wav_data.push_back(
		{Get2BytesNumberFromData(left),
		 Get2BytesNumberFromData(right)});
	}
	wav_data.shrink_to_fit();
}

void WAV::PrintWavInfo() {
	std::string horizontal_line(33, char(205));
	std::string upper_horizontal_line = horizontal_line;
	std::string lower_horizontal_line = horizontal_line;
	upper_horizontal_line[0] = char(201);
	upper_horizontal_line[32] = char(187);
	upper_horizontal_line[16] = char(203);
	lower_horizontal_line[0] = char(200);
	lower_horizontal_line[16] = char(202);
	lower_horizontal_line[32] = char(188);
	horizontal_line[0] = char(204);
	horizontal_line[16] = char(206);
	horizontal_line[32] = char(185);
	std::cout << std::left << upper_horizontal_line << "\n"
	          << char(186) << std::setw(15) << "Type" << char(186) << std::setw(15) << header.chunk_id << char(186)
	          << "\n"
	          << horizontal_line << "\n"
	          << char(186) << std::setw(15) << "Chunk size" << char(186) << std::setw(15)
	          << std::to_string(header.chunk_size) + " bytes" << char(186)
	          << "\n"
	          << horizontal_line << "\n"
	          << char(186) << std::setw(15) << "Format" << char(186) << std::setw(15) << header.format << char(186)
	          << "\n"
	          << horizontal_line << "\n"
	          << char(186) << std::setw(15) << "Subchank1id" << char(186) << std::setw(15) << header.subchunk1id
	          << char(186) << "\n"
	          << horizontal_line << "\n"
	          << char(186) << std::setw(15) << "subchank1size" << char(186) << std::setw(15)
	          << std::to_string(header.subchunk1size) + " bytes" << char(186) << "\n"
	          << horizontal_line << "\n"
	          << char(186) << std::setw(15) << "Audio format" << char(186) << std::setw(15) << header.audio_format
	          << char(186) << "\n"
	          << horizontal_line << "\n"
	          << char(186) << std::setw(15) << "Number channels" << char(186) << std::setw(15) << header.num_channels
	          << char(186) << "\n"
	          << horizontal_line << "\n"
	          << char(186) << std::setw(15) << "Sample rate" << char(186) << std::setw(15)
	          << std::to_string(header.sample_rate) + " Hz" << char(186) << "\n"
	          << horizontal_line << "\n"
	          << char(186) << std::setw(15) << "Byte rate" << char(186) << std::setw(15) << header.byte_rate << char(186)
	          << "\n"
	          << horizontal_line << "\n"
	          << char(186) << std::setw(15) << "block align" << char(186) << std::setw(15)
	          << std::to_string(header.block_align) + " bytes" << char(186) << "\n"
	          << horizontal_line << "\n"
	          << char(186) << std::setw(15) << "bits per sample" << char(186) << std::setw(15)
	          << std::to_string(header.bits_per_sample) + " bits" << char(186) << "\n"
	          << horizontal_line << "\n"
	          << char(186) << std::setw(15) << "subchunk2id" << char(186) << std::setw(15) << header.subchunk2id
	          << char(186) << "\n"
	          << horizontal_line << "\n"
	          << char(186) << std::setw(15) << "subchank2size" << char(186) << std::setw(15)
	          << std::to_string(header.subchunk2size) + " bytes" << char(186) << "\n"
	          << lower_horizontal_line << "\n";
	std::cout << upper_horizontal_line << "\n"
	          << char(186) << std::setw(15) << "duration" << char(186) << std::setw(15)
	          << std::to_string(header.minutes_duration) + "m" + std::to_string(header.seconds_duration)+"s" << char(186)
	          << "\n"
	          << lower_horizontal_line << "\n";
}

void WAV::PrintWavData() {
	for (const auto&[left, right] : wav_data) {
		std::cout << "(" << left << ", " << right << ")\n";
	}
}

void WAV::WriteModified(std::ofstream &output) {
	char buffer4bytes[4];
	char buffer2bytes[2];
	output.write(header.chunk_id, 4);
	ToLittleEndianHeaderFrom4BytesBigEndian(buffer4bytes, header.chunk_size);
	output.write(buffer4bytes, 4);
	output.write(header.format, 4);
	output.write(header.subchunk1id, 4);
	ToLittleEndianHeaderFrom4BytesBigEndian(buffer4bytes, header.subchunk1size);
	output.write(buffer4bytes, 4);
	ToLittleEndianHeaderFrom2BytesBigEndian(buffer2bytes, header.audio_format);
	output.write(buffer2bytes, 2);
	ToLittleEndianHeaderFrom2BytesBigEndian(buffer2bytes, header.num_channels);
	output.write(buffer2bytes, 2);
	ToLittleEndianHeaderFrom4BytesBigEndian(buffer4bytes, header.sample_rate);
	output.write(buffer4bytes, 4);
	ToLittleEndianHeaderFrom4BytesBigEndian(buffer4bytes, header.byte_rate);
	output.write(buffer4bytes, 4);
	ToLittleEndianHeaderFrom2BytesBigEndian(buffer2bytes, header.block_align);
	output.write(buffer2bytes, 2);
	ToLittleEndianHeaderFrom2BytesBigEndian(buffer2bytes, header.bits_per_sample);
	output.write(buffer2bytes, 2);
	output.write(header.subchunk2id, 4);
	ToLittleEndianHeaderFrom4BytesBigEndian(buffer4bytes, header.subchunk2size);
	output.write(buffer4bytes, 4);
	WriteData(output);
}

void WAV::WriteMessageToWAV(const std::string &message) {
	std::string err = "Wav file not enough bytes to hide this message\n";
	err.append("Available bytes to write: " + std::to_string(wav_data.size()) + "\n");
	err.append("Message size: " + std::to_string(message.length()) + "\n");
	if (message.length() > wav_data.size() / 8) {
		throw std::invalid_argument(err);
	}
	auto it = wav_data.begin();
	for (const char &item : message) {
		std::bitset<8> symbol(item);
		for (size_t i = 0; i < symbol.size(); i++) {
			std::bitset<16> amplitude(it->second);
			amplitude[0] = symbol[symbol.size() - 1 - i];
			it->second = amplitude.to_ulong();
			it++;
		}
	}
}

std::string WAV::GetMessageFromWAV(std::ifstream &input, size_t n) {
	std::string res = "";
	size_t count = 0;
	ReadHeader(input);
	ReadWavData(input);
	auto it = wav_data.begin();
	std::bitset<8> symbol;
	for (size_t i = 0; i < n * 8; i++) {
		std::bitset<16> amplitude(it->second);
		symbol[symbol.size() - 1 - count] = amplitude[0];
		count++;
		it++;
		if (count == 8) {
			res.push_back(static_cast<uint8_t>(symbol.to_ulong()));
			count = 0;
		}
	}
	return res;
}

void WAV::ReadHeader(std::ifstream &input) {
	char buffer[4];
	input.read(header.chunk_id, 4);
	header.chunk_id[4] = '\0';
	input.read(buffer, 4);
	header.chunk_size = Get4BytesNumberFromHeader(buffer);
	input.read(header.format, 4);
	header.format[4] = '\0';
	input.read(header.subchunk1id, 4);
	header.subchunk1id[4] = '\0';
	input.read(buffer, 4);
	header.subchunk1size = Get4BytesNumberFromHeader(buffer);
	input.read(buffer, 2);
	header.audio_format = Get2BytesNumberFromHeader(buffer);
	input.read(buffer, 2);
	header.num_channels = Get2BytesNumberFromHeader(buffer);
	input.read(buffer, 4);
	header.sample_rate = Get4BytesNumberFromHeader(buffer);
	input.read(buffer, 4);
	header.byte_rate = Get4BytesNumberFromHeader(buffer);
	input.read(buffer, 2);
	header.block_align = Get2BytesNumberFromHeader(buffer);
	input.read(buffer, 2);
	header.bits_per_sample = Get2BytesNumberFromHeader(buffer);
	input.read(header.subchunk2id, 4);
	header.subchunk2id[4] = '\0';
	input.read(buffer, 4);
	header.subchunk2size = Get4BytesNumberFromHeader(buffer);
	float fDurationSeconds = 1.f * header.subchunk2size / (header.bits_per_sample / 8) / header.num_channels / header.sample_rate;
	header.minutes_duration = (int) std::floor(fDurationSeconds) / 60;
	header.seconds_duration = fDurationSeconds - (header.minutes_duration * 60);
	wav_data.reserve(header.subchunk2size / 4);
}

void WAV::WriteData(std::ofstream &output) {
	char left_buf[2], right_buf[2];
	for (const auto&[left, right] : wav_data) {
		ToLittleEndianDataFrom2BytesNumberFromData(left_buf, left);
		ToLittleEndianDataFrom2BytesNumberFromData(right_buf, right);
		output << left_buf[0] << left_buf[1] << right_buf[0] << right_buf[1];
	}
}

void WAV::ToLittleEndianHeaderFrom2BytesBigEndian(char *buf, uint16_t value) {
	buf[1] = value >> 8;
	buf[0] = value & 0xFF;
}

void WAV::ToLittleEndianHeaderFrom4BytesBigEndian(char *buf, uint32_t value) {
	buf[3] = value >> 24;
	buf[2] = value >> 16;
	buf[1] = value >> 8;
	buf[0] = value & 0xFF;
}

void WAV::ToLittleEndianDataFrom2BytesNumberFromData(char *buf, int16_t value) {
	buf[1] = value >> 8;
	buf[0] = value & 0xFF;
}

int16_t WAV::Get2BytesNumberFromData(char *buf) {
	return static_cast<uint8_t>(buf[1]) << 8 |
	       static_cast<uint8_t>(buf[0]) << 0;
}

uint16_t WAV::Get2BytesNumberFromHeader(char *buf) {
	return static_cast<uint8_t>(buf[1]) << 8 |
	       static_cast<uint8_t>(buf[0]) << 0;
}

uint32_t WAV::Get4BytesNumberFromHeader(char *buf) {
	return static_cast<uint8_t>(buf[3]) << 24 |
	       static_cast<uint8_t>(buf[2]) << 16 |
	       static_cast<uint8_t>(buf[1]) << 8 |
	       static_cast<uint8_t>(buf[0]) << 0;
}
