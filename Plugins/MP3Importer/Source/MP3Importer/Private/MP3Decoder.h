// Distributed under the MIT License (MIT) (See accompanying file LICENSE
// or copy at http://opensource.org/licenses/MIT)

#pragma once

//in byte
#define WAV_HEADER_SIZE 44
#define BITS_PER_SAMPLE 16


class UMP3Decoder
{
public:
	UMP3Decoder(FFeedbackContext* WarnContext);
	~UMP3Decoder();

	void Init(const uint8*& Buffer, const uint8* BufferEnd);
	bool Decode(TArray<uint8> &OutBuffer);


	uint32 SizeInBytes;
	uint32 Samplerate;
	uint32 Channels;
	uint32 BitsPerSample;

private:
	static void WriteWaveHeader(uint8* header, uint32 dataLength, uint32 sampleRate, uint32 channels);
	mpg123_handle* Handle;
	int ErrorHandle;
	FFeedbackContext* Warn;
	
	size_t BlockBufferSize;
};