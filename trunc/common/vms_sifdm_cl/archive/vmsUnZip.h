#pragma once
#include "unzip.h"
#include "errors/unzip_error.h"
class vmsUnZip  
{
public:
	static BOOL Unpack(LPCTSTR ptszFileName, LPCTSTR ptszDstFolder, vmsError& error)
	{
		USES_CONVERSION;

		error.clear();

		BOOL result = FALSE;

		unzFile zipFile = unzOpen(T2A((LPTSTR)ptszFileName));

		if (NULL != zipFile)
		{
			if (UNZ_OK == unzGoToFirstFile(zipFile))
			{
				BOOL bContinue = TRUE;
				while (bContinue)
				{
					result = FALSE;
					unz_file_info fi;
					char filename[MAX_PATH] = { 0 };
					if (UNZ_OK == unzGetCurrentFileInfo(zipFile, &fi,
						filename, sizeof(filename), 0, 0, 0, 0))
					{
						if (*filename && (filename [strlen (filename) - 1] == '/' || filename [strlen (filename) - 1] == '\\'))
						{
							assert (fi.uncompressed_size == 0);
							tstring tstr = ptszDstFolder;
							tstr += '\\';
							tstr += CA2TEX<> (filename);
							tstr += '1';
							vmsBuildPathToFile (tstr.c_str ());
							result = TRUE;
						}
						else if (UNZ_OK == unzOpenCurrentFile(zipFile))
						{
							UINT dataLen = fi.uncompressed_size;
							std::vector <BYTE> fileData (dataLen ? dataLen : 1);
							if (dataLen == unzReadCurrentFile(zipFile, &fileData.front (), dataLen))
							{
								char filePathName[MAX_PATH] = { 0 };
								strcat(filePathName, T2A((LPTSTR)ptszDstFolder));
								strcat(filePathName, "\\");
								strcat(filePathName, filename);
								FILE* pFile = fopen(filePathName, "wb");
								if (!pFile)
								{
									vmsBuildPathToFile (CA2TEX<> (filePathName));
									pFile = fopen (filePathName, "wb");
								}
								if (pFile)
								{
									result = (dataLen == fwrite(&fileData.front (), 1, dataLen, pFile));
									result = result && (0 == fclose(pFile));
								}
								else
								{
									result = FALSE;
								}

								if (!result)
									error = std::error_code(errno, std::generic_category());
							}
							result = result && (UNZ_OK == unzCloseCurrentFile(zipFile));
						}
					}
					if (!result)
						break;
					if (UNZ_END_OF_LIST_OF_FILE == unzGoToNextFile(zipFile))
						bContinue = FALSE;
				}
			}
			result = result && (UNZ_OK == unzClose(zipFile));
		}

		if (!result && !error)
			error = unzip_error::unknown_error;

		return result;
	}
};
