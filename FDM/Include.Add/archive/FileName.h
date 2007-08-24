/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#ifndef __WINDOWS_FILENAME_H
#define __WINDOWS_FILENAME_H

#include "String.h"

namespace NWindows {
namespace NFile {
namespace NName {

const TCHAR kDirDelimiter = '\\';
const TCHAR kAnyStringWildcard = '*';

void NormalizeDirPathPrefix(CSysString &dirPath); 
#ifndef _UNICODE
void NormalizeDirPathPrefix(UString &dirPath); 
#endif

namespace NPathType
{
  enum EEnum
  {
    kLocal,
    kUNC
  };
  EEnum GetPathType(const UString &path);
}

struct CParsedPath
{
  UString Prefix; 
  UStringVector PathParts;
  void ParsePath(const UString &path);
  UString MergePath() const;
};

void SplitNameToPureNameAndExtension(const UString &fullName, 
    UString &pureName, UString &extensionDelimiter, UString &extension); 

}}}

#endif
