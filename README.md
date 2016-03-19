# UE4_MP3Importer
Allows the use of MP3 files in Unreal Engine 4
##Use
Unstable, does not do much, probably does not handle any error at all, only Win64.

Still allows to drag&drop MP3 files into Unreal Engine 4.

Works by decoding the MP3 into a WAV buffer, which then gets passed to UE4.


##License
Code licensed under the MIT license, see LICENSE file.

Uses the libmg123 library (http://www.mpg123.de/), which is under the LPGL 1.2 license:

	Copyright (c) 1995-2013 by Michael Hipp and others,
	free software under the terms of the LGPL v2.1

##How to use
Either unzip a release file into the <UnrealEngineRoot>/Engine/Plugins folder, 
or build the Project yourself and copy the files. 

__MP3 Importer is only looking for the libmg123 library in <UnrealEngineRoot>/Engine/Plugins/MP3Importer/Lib/x64 right now__

Make sure to use that folder, or change the path in the source code.