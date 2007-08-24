/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

    

#include "stdafx.h"
#include "vmsBtFileImpl.h"
#include "common.h"
#include <fstream>

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

