// Distributed under the MIT License (MIT) (See accompanying file LICENSE
// or copy at http://opensource.org/licenses/MIT)

#include "MP3Importer.h"

#include "MP3Decoder.h"
#include "UnrealEd.h"
#include "Factories.h"
#include "Editor.h"
#include "AudioDeviceManager.h"


UMP3Decoder::UMP3Decoder(FFeedbackContext* WarnContext)
{
	Warn = WarnContext;
	Handle = NULL;
}

UMP3Decoder::~UMP3Decoder()
{
	if (Handle)
	{
		mpg123_close(Handle);
		mpg123_delete(Handle);
		mpg123_exit();
	}
}

void UMP3Decoder::Init(const uint8*& Buffer, const uint8* BufferEnd)
{
	int encoding;
	int channels;
	long samplerate;

	mpg123_init();
	Handle = mpg123_new(NULL, &ErrorHandle);
	BlockBufferSize = mpg123_outblock(Handle);
	

	mpg123_open_feed(Handle);
	mpg123_feed(Handle, Buffer, BufferEnd - Buffer);
	mpg123_getformat(Handle, &samplerate, &channels, &encoding);
	uint32 bytesPerSample = mpg123_encsize(encoding);

	BitsPerSample = bytesPerSample * 8;
	Channels = channels;
	Samplerate = samplerate;

	UE_LOG(MP3ImporterLog, Display, TEXT("Initialized: Samplerate: %u, Channels: %u"), Samplerate, Channels);
}

bool UMP3Decoder::Decode(TArray<uint8> &OutBuffer)
{
	check(OutBuffer.Num() == 0);
	check(OutBuffer.GetAllocatedSize() >= WAV_HEADER_SIZE);
	OutBuffer.AddZeroed(WAV_HEADER_SIZE / OutBuffer.GetTypeSize());

	FDateTime tStart = FDateTime::Now();

	unsigned char* BlockBuffer = (unsigned char*)FMemory::Malloc(BlockBufferSize);
	size_t bytesRead = 0;
	size_t done = 0;
	int result;

	do
	{
		result = mpg123_read(Handle, BlockBuffer, BlockBufferSize, &done);
		bytesRead += done;
		OutBuffer.Append(BlockBuffer, done);
	} while (result == MPG123_OK);

	uint8 header[WAV_HEADER_SIZE];
	
	WriteWaveHeader(header, bytesRead, Samplerate, Channels);
	
	FMemory::Memcpy(OutBuffer.GetData(), header, WAV_HEADER_SIZE);
	FMemory::Free(BlockBuffer);

	SizeInBytes = bytesRead;
	bool bSuccess = result == MPG123_OK || result == MPG123_DONE;

	UE_LOG(MP3ImporterLog, Display, TEXT("Decoding finished. %s bytes in %d ms. Success: %s"), 
		*FString::FormatAsNumber((int32)bytesRead), (int32)(FDateTime::Now() - tStart).GetTotalMilliseconds(), bSuccess ? TEXT("True") : TEXT("False"));

	return bSuccess;
}

void UMP3Decoder::WriteWaveHeader(uint8* header, uint32 dataLength, uint32 sampleRate, uint32 channels)
{
	uint32 buff = 0;
	uint16 buff16 = 0;

	FMemory::Memcpy(header, "RIFF", 4);
	buff = dataLength + WAV_HEADER_SIZE - 8;
	FMemory::Memcpy(header + 4, &buff, 4);

	FMemory::Memcpy(header + 8, "WAVE", 4);
	FMemory::Memcpy(header + 12, "fmt ", 4);

	buff = 16; 
	FMemory::Memcpy(header + 16, &buff, 4); //<fmt length>

	buff16 = 1;
	FMemory::Memcpy(header + 20, &buff16, 2); //<format tag> == pcm

	FMemory::Memcpy(header + 22, &channels, 2); //<channels> == pcm

	FMemory::Memcpy(header + 24, &sampleRate, 4); //<sample rate>

	buff = sampleRate * 16 * 2;
	FMemory::Memcpy(header + 28, &buff, 4); //<bytes / second>

	buff16 = channels * (16 + 7) / 8;
	FMemory::Memcpy(header + 32, &buff16, 2); //<block align> Frame-Groese = <Anzahl der Kanäle> * ((<Bits/Sample (eines Kanals)> * 7) / 8) 

	buff16 = 16;
	FMemory::Memcpy(header + 34, &buff16, 2); //<bits / sample>

	FMemory::Memcpy(header + 36, "data", 4);

	FMemory::Memcpy(header + 40, &dataLength, 4); //<length>
}