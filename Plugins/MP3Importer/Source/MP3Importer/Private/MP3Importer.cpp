// Distributed under the MIT License (MIT) (See accompanying file LICENSE
// or copy at http://opensource.org/licenses/MIT)

#include "MP3Importer.h"

IMPLEMENT_MODULE(FMP3ImporterModule, MP3Importer)
DEFINE_LOG_CATEGORY(MP3ImporterLog);




int(*mpg123_init)(void);
mpg123_handle* (*mpg123_new)(const char* decoder, int *error);
size_t(*mpg123_outblock)(mpg123_handle *mh);
int(*mpg123_open_feed)(mpg123_handle *mh);
int(*mpg123_feed)(mpg123_handle *mh, const unsigned char *in, size_t size);
int(*mpg123_getformat)(mpg123_handle *mh, long *rate, int *channels, int *encoding);
int(*mpg123_encsize)(int encoding);
int(*mpg123_read)(mpg123_handle *mh, unsigned char *outmemory, size_t outmemsize, size_t *done);
void(*mpg123_delete)(mpg123_handle *mh);
int(*mpg123_close)(mpg123_handle *mh);
void(*mpg123_exit)(void);


void FMP3ImporterModule::StartupModule()
{
	UE_LOG(MP3ImporterLog, Display, TEXT("StartupModule"));
	UE_LOG(MP3ImporterLog, Warning, TEXT("MP3Importer uses libmpg123, downloaded from: http://mpg123.org"));
	UE_LOG(MP3ImporterLog, Warning, TEXT("libmpg123: Copyright(c) 1995 - 2013 by Michael Hipp and others,"));
	UE_LOG(MP3ImporterLog, Warning, TEXT("libmpg123: free software under the terms of the LGPL v2.1"));

	DLLHandle = nullptr;
	LoadLibrary();
}
 
void FMP3ImporterModule::ShutdownModule()
{
	UE_LOG(MP3ImporterLog, Display, TEXT("ShutdownModule"));

	if (DLLHandle != nullptr)
		FPlatformProcess::FreeDllHandle(DLLHandle);
}

void FMP3ImporterModule::LoadLibrary()
{
	FString filePath = FPaths::Combine(*FPaths::EnginePluginsDir(), TEXT("MP3Importer"), TEXT("Lib"), TEXT("x64"),  DLL_NAME);
	if (!FPaths::FileExists(filePath))
	{
		UE_LOG(MP3ImporterLog, Error, TEXT("Could not find %s in %s"), DLL_NAME, *filePath);
	}

	DLLHandle = FPlatformProcess::GetDllHandle(*filePath); // Retrieve the DLL.
	if (DLLHandle != NULL)
	{
		mpg123_init =		(int(__cdecl *)(void))												FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_init"));
		mpg123_new =		(mpg123_handle* (__cdecl *)(const char*, int *))					FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_new"));
		mpg123_outblock =	(size_t(__cdecl *)(mpg123_handle*))									FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_outblock"));
		mpg123_open_feed =	(int(__cdecl *)(mpg123_handle*))									FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_open_feed"));
		mpg123_feed =		(int(__cdecl *)(mpg123_handle*, const unsigned char*, size_t))		FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_feed"));
		mpg123_getformat =	(int(__cdecl *)(mpg123_handle*, long*, int*, int*))					FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_getformat"));
		mpg123_encsize =	(int(__cdecl *)(int))												FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_encsize"));
		mpg123_read =		(int(__cdecl *)(mpg123_handle*, unsigned char*, size_t, size_t*))	FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_read"));
		mpg123_delete =		(void(__cdecl *)(mpg123_handle*))									FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_delete"));
		mpg123_close =		(int(__cdecl *)(mpg123_handle*))									FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_close"));
		mpg123_exit =		(void(__cdecl *)(void))												FPlatformProcess::GetDllExport(DLLHandle, *FString("mpg123_exit"));

		//TODO: check each for null?
	}
	else
	{
		UE_LOG(MP3ImporterLog, Error, TEXT("Could not GetDLLExports."));
	}
 }

