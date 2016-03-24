/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "vmsAVMergerFFMPEG.h"
#include "vmsFFMPEG.h"
#include <vector>
#include <tchar.h>

int vmsAVMergerFFMPEG::merge( std::wstring _inputFileName1, std::wstring _inputFileName2,
		std::wstring _outputFileName ){
	if ( !FFMPEG->Init() )
		return -1;

	std::string inputFileName1;
	inputFileName1 = utf8FromWide(_inputFileName1);	

	std::string inputFileName2;
	inputFileName2 = utf8FromWide(_inputFileName2);	

	std::string outputFileName;
	outputFileName = utf8FromWide(_outputFileName);	

	char *szInputFileName1 = new char[inputFileName1.length()+1];
	strcpy( szInputFileName1, inputFileName1.c_str() );
	char *szInputFileName2 = new char[inputFileName2.length()+1];
	strcpy( szInputFileName2, inputFileName2.c_str() );
	char *szOutputFileName = new char[outputFileName.length()+1];
	strcpy( szOutputFileName, outputFileName.c_str() );

	int result = FFMPEG->launchFFMPEG(&m_bStop, "ffmpeg.exe", 
#ifdef _DEBUG
		"-v", "debug",
#endif
		"-i", szInputFileName1, "-i", szInputFileName2, "-vcodec", "copy", "-acodec", "copy", szOutputFileName, NULL);
	tstringstream tstrStream;
	tstrStream<<L"\nFFMPEG Result "<<result<<L"\n";
	OutputDebugString( tstrStream.str().c_str() );

	delete[] szInputFileName1;
	delete[] szInputFileName2;
	delete[] szOutputFileName;

	return result;
}