/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef FORMATINFOH
#define FORMATINFOH
struct FormatInfo{
	FormatInfo( std::wstring _itag, std::wstring _extension, int _width, int _height ){
		itag = _itag;
		height = _height;
		width = _width;
		extension = _extension;
	}
	std::wstring itag;
	int width, height;
	std::wstring extension;
};
#endif