/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#ifndef __EXTRACT_H
#define __EXTRACT_H

#include "Wildcard.h"
#include "FileFind.h"

#include "IArchive.h"

#include "ArchiveExtractCallback.h"
#include "ArchiveOpenCallback.h"
#include "ExtractMode.h"

class CExtractOptions
{
public:
  bool StdOutMode;
  bool TestMode;
  NExtract::NPathMode::EEnum PathMode;

  UString OutputDir;
  bool YesToAll;
  UString DefaultItemName;
  NWindows::NFile::NFind::CFileInfoW ArchiveFileInfo;
  
  
  
  

  NExtract::NOverwriteMode::EEnum OverwriteMode;

  CExtractOptions(): 
      StdOutMode(false), 
      YesToAll(false), 
      TestMode(false),
      PathMode(NExtract::NPathMode::kFullPathnames),
      OverwriteMode(NExtract::NOverwriteMode::kAskBefore)
      {}

  
};

HRESULT DecompressArchives(
    UStringVector &archivePaths, UStringVector &archivePathsFull,
    const NWildcard::CCensorNode &wildcardCensor,
    const CExtractOptions &options,
    IOpenCallbackUI *openCallback,
    IExtractCallbackUI *extractCallback);

#endif
