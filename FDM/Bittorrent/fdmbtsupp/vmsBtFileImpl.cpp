/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsBtFileImpl.h"
#include "common.h"
#include <fstream>
#include <boost/filesystem/operations.hpp>
using namespace boost::filesystem;
#include <libtorrent/file_pool.hpp>
#include <libtorrent/storage.hpp>
#include <libtorrent/hasher.hpp>  

__declspec(dllexport) vmsBtFile* WINAPI vmsBt_CreateTorrentFileObject ()
{
	return new vmsBtFileImpl;
}

vmsBtFileImpl::vmsBtFileImpl(void)
{
	m_torrent = NULL;
	m_cRefs = 1;
}

vmsBtFileImpl::~vmsBtFileImpl(void)
{
	if (m_torrent)
		delete m_torrent;
}

BOOL vmsBtFileImpl::LoadFromFile (LPCSTR pszTorrentFile)
{
	SAFE_DELETE (m_torrent);

	HANDLE hFile = CreateFileA (pszTorrentFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD dwSize = GetFileSize (hFile, NULL);

	LPBYTE pbTorrent = new BYTE [dwSize];
	
	BOOL b = ReadFile (hFile, pbTorrent, dwSize, &dwSize, NULL);

	CloseHandle (hFile);

	if (b)
		b = LoadFromBuffer (pbTorrent, dwSize);

	delete [] pbTorrent;

	return b;
}

BOOL vmsBtFileImpl::LoadFromBuffer (LPBYTE pbTorrent, DWORD dwTorrentSize)
{
	SAFE_DELETE (m_torrent);

	try {
		m_torrent = new torrent_info (
			bdecode (pbTorrent, pbTorrent + dwTorrentSize));
	}
	catch (...) {
		return FALSE;
	}

	return TRUE;
}

void vmsBtFileImpl::Release ()
{
	if (InterlockedDecrement (&m_cRefs) == 0)
		delete this;
}

void vmsBtFileImpl::AddRef ()
{
	InterlockedIncrement (&m_cRefs);
}

int vmsBtFileImpl::get_FileCount ()
{
	return m_torrent->num_files ();
}

void vmsBtFileImpl::get_FileName (int nIndex, LPSTR pszRes)
{
	strcpy (pszRes, m_torrent->file_at (nIndex).path.string ().c_str ());
}

UINT64 vmsBtFileImpl::get_FileSize (int nIndex)
{
	return m_torrent->file_at (nIndex).size;
}

UINT64 vmsBtFileImpl::get_TotalFilesSize ()
{
	return m_torrent->total_size ();
}

void vmsBtFileImpl::get_TorrentName (LPSTR pszRes)
{
	strcpy (pszRes, m_torrent->name ().c_str ());
}

BOOL vmsBtFileImpl::get_TorrentBuffer (LPBYTE pbRes, DWORD dwSize, DWORD *pdwTorrentSize)
{
	entry e = m_torrent->create_torrent ();

	std::vector <char> v;
	bencode (std::back_inserter (v), e);

	*pdwTorrentSize = v.size ();

	if (pbRes == NULL)
		return TRUE;

	if (dwSize < v.size ())
		return FALSE;

	for (size_t i = 0; i < v.size (); i++)
		*pbRes++ = v [i];

	return TRUE;
}

void vmsBtFileImpl::get_InfoHash (LPSTR pszRes)
{
	const sha1_hash& hash = m_torrent->info_hash ();
	for (size_t i = 0; i < hash.size; i++)
	{
		sprintf (pszRes, "%02x", (int)hash [i]);
		pszRes += strlen (pszRes);
	}
}

void vmsBtFileImpl::get_TrackerUrl (LPSTR pszRes, int nIndex)
{
	const std::vector <announce_entry> &v = m_torrent->trackers ();
	strcpy (pszRes, size_t(nIndex) < v.size () ? v [nIndex].url.c_str () : "");
}

int vmsBtFileImpl::get_TrackerCount ()
{
	return m_torrent->trackers ().size ();
}

void vmsBtFileImpl::get_TorrentComment(LPSTR pszRes)
{
	strcpy (pszRes, m_torrent->comment ().c_str ());
}

int vmsBtFileImpl::get_PieceCount ()
{
	return m_torrent->num_pieces ();
}

int vmsBtFileImpl::get_PieceSize ()
{
	return (int)m_torrent->piece_size (0);
}

void add_files (torrent_info& t, path const& p, path const& l, LPBOOL pbNeedCancel)
{
	if (l.leaf ()[0] == '.') 
		return;

	path f(p / l);

	if (is_directory (f))
	{
		for (directory_iterator i (f), end; i != end; ++i)
		{
			add_files (t, p, l / i->leaf (), pbNeedCancel);
			if (pbNeedCancel && *pbNeedCancel)
				return;
		}
	}
	else
	{
		t.add_file (l, file_size (f));
	}
}

UINT64 get_files_size (path const& p, path const& l, LPBOOL pbNeedCancel)
{
	if (l.leaf ()[0] == '.') 
		return 0;

	path f(p / l);

	UINT64 s = 0;

	if (is_directory (f))
	{
		for (directory_iterator i (f), end; i != end; ++i)
		{
			s += get_files_size (p, l / i->leaf (), pbNeedCancel);
			if (pbNeedCancel && *pbNeedCancel)
				return 0;
		}
	}
	else
	{
		s += file_size (f);
	}

	return s;
}

BOOL vmsBtFileImpl::CreateNewTorrent (LPCSTR pszSrcPath, LPCSTR pszTrackers, LPCSTR pszUrlSeeds, 
	LPCSTR pszCreator, LPCSTR pszComment, int iPieceSize, int *pnCreateProgress, BOOL *pbNeedCancel)
{
	if (pnCreateProgress)
		*pnCreateProgress = 0;

	try
	{
		if (m_torrent)
			delete m_torrent;

		m_torrent = new torrent_info;

		path full_path = complete (path (pszSrcPath));

		if (iPieceSize == 0)
		{
			UINT64 uTotalSize = get_files_size (full_path.branch_path (), full_path.leaf (), pbNeedCancel);

			if (uTotalSize > 2 * 1024*1024*1024ui64)
				iPieceSize = 4*1024*1024;
			else if (uTotalSize > 1 * 1024*1024*1024ui64)
				iPieceSize = 2*1024*1024;
			else if (uTotalSize > 500*1024*1024ui64)
				iPieceSize = 1024*1024;
			else if (uTotalSize > 200*1024*1024ui64)
				iPieceSize = 512*1024;
			else
				iPieceSize = 256*1024;
		}

		add_files (*m_torrent, full_path.branch_path (), full_path.leaf (), pbNeedCancel);
		m_torrent->set_piece_size (iPieceSize);

		file_pool fp;
		boost::scoped_ptr <storage_interface> st (
			default_storage_constructor (m_torrent, full_path.branch_path (), fp));

		while (*pszTrackers)
		{
			m_torrent->add_tracker(pszTrackers);
			pszTrackers += strlen (pszTrackers) + 1;
			if (*pszTrackers && FALSE == IsCharAlphaA (*pszTrackers))
				return FALSE; 
		}

		
		int num = m_torrent->num_pieces ();
		std::vector <char> buf (iPieceSize);
		for (int i = 0; i < num; ++i)
		{
			st->read (&buf [0], i, 0, (int)m_torrent->piece_size (i));
			hasher h (&buf [0], (int)m_torrent->piece_size (i));
			m_torrent->set_hash (i, h.final ());
			if (pnCreateProgress)
				*pnCreateProgress = MulDiv (i+1, 100, num);
			if (pbNeedCancel && *pbNeedCancel)
				return FALSE;
		}

		if (pszCreator)
			m_torrent->set_creator(pszCreator);

		if (pszComment)
			m_torrent->set_comment (pszComment);

		if (pszUrlSeeds)
		{
			while (*pszUrlSeeds)
			{
				m_torrent->add_url_seed (pszUrlSeeds);
				pszUrlSeeds += strlen (pszUrlSeeds) + 1;
				if (!IsCharAlphaA (*pszUrlSeeds))
					return FALSE; 
			}
		}

		
		
		
		LPBYTE pb; DWORD dw; 
		if (FALSE == get_TorrentBuffer (NULL, 0, &dw))
		{
			delete m_torrent;
			return FALSE;
		}
		pb = new BYTE [dw];
		get_TorrentBuffer (pb, dw, &dw);
		delete m_torrent;
		m_torrent = NULL;

		LoadFromBuffer (pb, dw);

		delete [] pb;
	}
	catch (std::exception&)
	{
		if (m_torrent)
			delete m_torrent;
		return FALSE;
	}                

	return TRUE;
}

BOOL vmsBtFileImpl::GenerateFastResumeDataForSeed (LPCSTR pszSrcFolderOrFile, LPBYTE pbData, DWORD dwSize, LPDWORD pdwSize)
{
	entry ret (entry::dictionary_t);

	ret ["file-format"] = "libtorrent resume file";
	ret ["file-version"] = 1;

	ret ["allocation"] = "full";

	ret ["info-hash"] = std::string ((char*)m_torrent->info_hash ().begin(), (char*)m_torrent->info_hash ().end());

	int block_size = 1024; 
	if (m_torrent->piece_length () < block_size)
		block_size = (int)m_torrent->piece_length ();
	int num_blocks_per_piece =
		static_cast<int>(m_torrent->piece_length ()) / block_size;

	ret ["blocks per piece"] = num_blocks_per_piece;

	std::vector<int> piece_index;
	piece_index.reserve (m_torrent->num_pieces ());
	for (int i = 0; i < m_torrent->num_pieces(); ++i)
		piece_index.push_back (i);
	entry::list_type& slots = ret ["slots"].list ();
	std::copy (piece_index.begin (), piece_index.end (), std::back_inserter (slots));  

	std::vector<std::pair<size_type, std::time_t> > file_sizes = 
		get_filesizes (*m_torrent, fs::complete (pszSrcFolderOrFile));

	ret ["file sizes"] = entry::list_type ();
	entry::list_type& fl = ret ["file sizes"].list ();
	for (std::vector<std::pair<size_type, std::time_t> >::iterator i
			= file_sizes.begin (), end (file_sizes.end ()); i != end; ++i)
	{
		entry::list_type p;
		p.push_back (entry(i->first));
		p.push_back (entry(i->second));
		fl.push_back (entry(p));
	}

	

	std::vector <char> v;
	bencode (std::back_inserter (v), ret);

	*pdwSize = v.size ();

	if (pbData == NULL)
		return TRUE;

	if (dwSize < v.size ())
		return FALSE;

	for (size_t i = 0; i < v.size (); i++)
		*pbData++ = v [i];

	return TRUE;
}

void vmsBtFileImpl::get_FileName2 (int nIndex, LPSTR pszRes, DWORD dwBuffSize)
{
	strncpy (pszRes, m_torrent->file_at (nIndex).path.string ().c_str (), dwBuffSize - 1);
	pszRes [dwBuffSize - 1] = 0;
}

void vmsBtFileImpl::get_TorrentName2 (LPSTR pszRes, DWORD dwBuffSize)
{
	strncpy (pszRes, m_torrent->name ().c_str (), dwBuffSize - 1);
	pszRes [dwBuffSize - 1] = 0;
}

void vmsBtFileImpl::get_TrackerUrl2 (LPSTR pszRes, int nIndex, DWORD dwBuffSize)
{
	const std::vector <announce_entry> &v = m_torrent->trackers ();
	strncpy (pszRes, size_t(nIndex) < v.size () ? v [nIndex].url.c_str () : "", dwBuffSize - 1);
	pszRes [dwBuffSize - 1] = 0;
}

void vmsBtFileImpl::get_TorrentComment2 (LPSTR pszRes, DWORD dwBuffSize)
{
	strncpy (pszRes, m_torrent->comment ().c_str (), dwBuffSize - 1);
	pszRes [dwBuffSize - 1] = 0;
}