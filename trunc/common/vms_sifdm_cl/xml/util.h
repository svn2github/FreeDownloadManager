/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

template <class TObject>
std::string vmsXmlSerializeObject (TObject& obj, const std::wstring& root_node_name, 
	unsigned flags = 0, unsigned xmlosFlags = 0)
{
	auto ss = std::make_shared <std::stringstream> ();
	vmsXmlSerializationOutputStream xml (root_node_name, xmlosFlags);
	xml.BindToStream (ss);
	if (!obj.Serialize (&vmsSerializationIoStream (&xml), flags))
		return std::string ();
	xml.Flush ();
	return ss->str ();
}