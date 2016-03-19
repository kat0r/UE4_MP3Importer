// Distributed under the MIT License (MIT) (See accompanying file LICENSE
// or copy at http://opensource.org/licenses/MIT)

#pragma once
 
#include "Engine.h"
#include "ModuleManager.h"
#include "UnrealEd.h"
 
DECLARE_LOG_CATEGORY_EXTERN(MP3ImporterLog, All, All)
#define DLL_NAME TEXT("libmpg123-0.dll")

class FMP3ImporterModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private: 
	void LoadLibrary();
	void *DLLHandle;
};


typedef void* mpg123_handle;

#define MPG123_OK 0
#define MPG123_DONE -10

extern int				(*mpg123_init)(void);
extern mpg123_handle *	(*mpg123_new)(const char* decoder, int *error);
extern size_t			(*mpg123_outblock)(mpg123_handle *mh);
extern int				(*mpg123_open_feed)(mpg123_handle *mh);
extern int				(*mpg123_feed)(mpg123_handle *mh, const unsigned char *in, size_t size);
extern int				(*mpg123_getformat)(mpg123_handle *mh, long *rate, int *channels, int *encoding);
extern int				(*mpg123_encsize)(int encoding);
extern int				(*mpg123_read)(mpg123_handle *mh, unsigned char *outmemory, size_t outmemsize, size_t *done);
extern void				(*mpg123_delete)(mpg123_handle *mh);
extern int				(*mpg123_close)(mpg123_handle *mh);
extern void				(*mpg123_exit)(void);