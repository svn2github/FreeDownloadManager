/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FormatInfoProvider.h"

std::vector<FormatInfo> FormatInfoProvider::getValues(){
	std::vector<FormatInfo> formats;

	

	formats.push_back(FormatInfo(L"5", L"flv", 400, 240));
	formats.push_back(FormatInfo(L"6", L"flv", 450, 270));
	formats.push_back(FormatInfo(L"13", L"3gp", 0, 0));
	formats.push_back(FormatInfo(L"17", L"3gp", 176, 144));
	formats.push_back(FormatInfo(L"18", L"mp4", 640, 360));
	formats.push_back(FormatInfo(L"22", L"mp4", 1280, 720));
	formats.push_back(FormatInfo(L"34", L"flv", 640, 360));
	formats.push_back(FormatInfo(L"35", L"flv", 854, 480));
	formats.push_back(FormatInfo(L"36", L"3gp", 320, 240));
	formats.push_back(FormatInfo(L"37", L"mp4", 1920, 1080));
	formats.push_back(FormatInfo(L"38", L"mp4", 4096, 3072));
	formats.push_back(FormatInfo(L"43", L"webm", 640, 360));
	formats.push_back(FormatInfo(L"44", L"webm", 854, 480));
	formats.push_back(FormatInfo(L"45", L"webm", 1280, 720));
	formats.push_back(FormatInfo(L"46", L"webm", 1920, 1080));

	
	formats.push_back(FormatInfo(L"82", L"mp4", 0, 360));
	formats.push_back(FormatInfo(L"83", L"mp4", 0, 480));
	formats.push_back(FormatInfo(L"84", L"mp4", 0, 720));
	formats.push_back(FormatInfo(L"85", L"mp4", 0, 1080));
	formats.push_back(FormatInfo(L"100", L"webm", 0, 360));
	formats.push_back(FormatInfo(L"101", L"webm", 0, 480));
	formats.push_back(FormatInfo(L"102", L"webm", 0, 720));

	
	formats.push_back(FormatInfo(L"92", L"mp4", 0, 240));
	formats.push_back(FormatInfo(L"93", L"mp4", 0, 360));
	formats.push_back(FormatInfo(L"94", L"mp4", 0, 480));
	formats.push_back(FormatInfo(L"95", L"mp4", 0, 720));
	formats.push_back(FormatInfo(L"96", L"mp4", 0, 1080));
	formats.push_back(FormatInfo(L"132", L"mp4", 0, 240));
	formats.push_back(FormatInfo(L"151", L"mp4", 0, 72));

	
	formats.push_back(FormatInfo(L"133", L"mp4", 0, 240));
	formats.push_back(FormatInfo(L"134", L"mp4", 0, 360));
	formats.push_back(FormatInfo(L"135", L"mp4", 0, 480));
	formats.push_back(FormatInfo(L"136", L"mp4", 0, 720));
	formats.push_back(FormatInfo(L"137", L"mp4", 0, 1080));
	formats.push_back(FormatInfo(L"138", L"mp4", 0, 2160));
	formats.push_back(FormatInfo(L"160", L"mp4", 0, 144));
	formats.push_back(FormatInfo(L"264", L"mp4", 0, 1440));

	
	formats.push_back(FormatInfo(L"139", L"m4a", 0, 0));
	formats.push_back(FormatInfo(L"140", L"m4a", 0, 0));
	formats.push_back(FormatInfo(L"141", L"m4a", 0, 0));

	
	formats.push_back(FormatInfo(L"167", L"webm", 640, 360));
	formats.push_back(FormatInfo(L"168", L"webm", 854, 480));
	formats.push_back(FormatInfo(L"169", L"webm", 1280, 720));
	formats.push_back(FormatInfo(L"170", L"webm", 1920, 1080));
	formats.push_back(FormatInfo(L"218", L"webm", 854, 480));
	formats.push_back(FormatInfo(L"219", L"webm", 854, 480));
	formats.push_back(FormatInfo(L"242", L"webm", 0, 240));
	formats.push_back(FormatInfo(L"243", L"webm", 0, 360));
	formats.push_back(FormatInfo(L"244", L"webm", 0, 480));
	formats.push_back(FormatInfo(L"245", L"webm", 0, 480));
	formats.push_back(FormatInfo(L"246", L"webm", 0, 480));
	formats.push_back(FormatInfo(L"247", L"webm", 0, 720));
	formats.push_back(FormatInfo(L"248", L"webm", 0, 1080));
	formats.push_back(FormatInfo(L"271", L"webm", 0, 1440));
	formats.push_back(FormatInfo(L"272", L"webm", 0, 2160));

	
	formats.push_back(FormatInfo(L"171", L"webm", 0, 0));
	formats.push_back(FormatInfo(L"172", L"webm", 0, 0));

	return formats;
}