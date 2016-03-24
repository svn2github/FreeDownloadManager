/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

enum text_file_encoding
{
	tfe_unknown,
	tfe_utf8,
	tfe_utf16,
	tfe_utf16le
};

inline bool get_text_encoding (const unsigned char* buffer, size_t buffer_size, text_file_encoding& result)
{
	if (buffer_size >= 3 && buffer [0] == 0xEF && buffer [1] == 0xBB && buffer [2] == 0xBF)
		result = tfe_utf8;

	else if (buffer_size >= 2 && buffer [0] == 0xFE && buffer [1] == 0xFF)
		result = tfe_utf16;

	else if (buffer_size >= 2 && buffer [0] == 0xFF && buffer [1] == 0xFE)
		result = tfe_utf16le;

	else
		result = tfe_unknown;

	return true;
}

inline bool get_text_file_encoding (const tstring& name, text_file_encoding& result)
{
	HANDLE file = CreateFile (name.c_str (), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
		0, nullptr);
	if (file == INVALID_HANDLE_VALUE)
		return false;

	BYTE abc [3];
	DWORD dw;
	bool ok = ReadFile (file, abc, sizeof (abc), &dw, nullptr) != FALSE;

	CloseHandle (file);

	if (!ok)
		return false;

	return get_text_encoding (abc, dw, result);
}

inline FILE* open_text_file (const tstring& name)
{
	text_file_encoding tfe;
	if (!get_text_file_encoding (name, tfe))
		return nullptr;

	static std::map <text_file_encoding, tstring> enc_map;
	if (enc_map.empty ())
	{
		enc_map [tfe_utf8] = _T ("UTF-8");
		enc_map [tfe_utf16] = _T ("UNICODE");
		enc_map [tfe_utf16le] = _T ("UTF-16LE");
	}

	tstringstream tss;
	tss << _T ("rt");
	if (tfe != tfe_unknown)
		tss << _T (", ccs=") << enc_map [tfe];

	FILE *f;
	errno_t err = _tfopen_s (&f, name.c_str (), tss.str ().c_str ());
	if (err)
		return nullptr;

	return f;
}