/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "Utils.h"
#include "tstring.h"

 struct HTMLReplace {
	 tstring match;
	 tstring replace;
 } codes[] = {
	 {_T("&quot;"), _T("\"")},
	 {_T("&apos;"), _T("'")},
	 {_T("&amp;"), _T("&")},
	 {_T("&lt;"), _T("<")},
	 {_T("&gt;"), _T(">")},
	 {_T("&nbsp;"), _T(" ")},
	 {_T("&#32;"), _T(" ")},
	 {_T("&#33;"), _T("!")},
	 {_T("&#34;"), _T("\"")},
	 {_T("&#35;"), _T("#")},
	 {_T("&#36;"), _T("$")},
	 {_T("&#37;"), _T("%")},
	 {_T("&#38;"), _T("&")},
 	 {_T("&#39;"), _T("'")},
	 {_T("&#40;"), _T("(")},
	 {_T("&#41;"), _T(")")},
	 {_T("&#42;"), _T("*")},
	 {_T("&#43;"), _T("+")},
	 {_T("&#44;"), _T(",")},
	 {_T("&#45;"), _T("-")},
	 {_T("&#46;"), _T(".")},
	 {_T("&#47;"), _T("/")},
	 {_T("&#58;"), _T(":")},	 
	 {_T("&#59;"), _T(";")},
	 {_T("&#60;"), _T("<")},
     {_T("&#61;"), _T("=")},
	 {_T("&#62;"), _T(">")},
     {_T("&#63;"), _T("?")},
 };

tstring HTMLDecode( const tstring& s )
 {
	 tstring rs = s;
	 
	 for ( size_t i = 0; i < array_length( codes ); i++ ) {
		 
		 const tstring& match = codes[i].match;
		 const tstring& repl = codes[i].replace;
		 tstring::size_type start = rs.find( match );
		 
		 while ( start != tstring::npos ) {
			 rs.replace( start, match.size(), repl );
			 
			 start = rs.find( match, start + repl.size() );
		 }
	 }
	 return rs;
 }

bool hasEnding (tstring const &fullString, tstring const &ending)
{
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

std::wstring with_quotes(const std::wstring& s) { return _T("\"") + s + _T("\""); }

void print_tree(const boost::property_tree::wptree& pt, int level)
{
    const std::wstring sep(2 * level, _T(' '));
	tstringstream tstr;
    BOOST_FOREACH(const boost::property_tree::wptree::value_type &v, pt) {
        tstr << sep
            << with_quotes(v.first) << _T(" : ") << with_quotes(v.second.data()) << _T("\n");
        print_tree(v.second, level + 1);
    }
	OutputDebugString( tstr.str().c_str() );
}

void print_tree(const boost::property_tree::wptree& pt) { print_tree(pt, 0); }

