/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsExeProgramData
{
public:
	static std::string construct_label (const std::string& label_base)
	{
		return std::string ("###") + label_base + std::string ("###");
	}

	vmsExeProgramData(const std::string& label, unsigned max_size = 100*1024, const std::wstring& exe_path = vmsGetModuleFileName ())
	{
		assert (!exe_path.empty ());

		HANDLE hFile = CreateFile (exe_path.c_str (), GENERIC_READ, FILE_SHARE_READ, nullptr, 
			OPEN_EXISTING, 0, nullptr);
		assert (hFile != INVALID_HANDLE_VALUE);

		LARGE_INTEGER file_size; file_size.QuadPart = 0;
		verify (GetFileSizeEx (hFile, &file_size));

		unsigned to_read = max_size > file_size.QuadPart ? max_size : (unsigned)file_size.QuadPart;

		LARGE_INTEGER pos; pos.QuadPart = - (int64_t)to_read;
		verify (SetFilePointerEx (hFile, pos, &pos, FILE_END));

		std::vector <unsigned char> data (to_read);
		DWORD read = 0;
		verify (ReadFile (hFile, &data.front (), to_read, &read, nullptr));

		CloseHandle (hFile);

		auto itLabelBegin = std::search (data.begin (), data.end (), label.begin (), label.end ());
		if (itLabelBegin == data.end ())
			return;

		itLabelBegin += label.size ();

		auto itLabelEnd = std::search (itLabelBegin, data.end (), label.begin (), label.end ());

		m_data.assign (itLabelBegin, itLabelEnd);
	}

public:
	std::vector <unsigned char> m_data;
};

