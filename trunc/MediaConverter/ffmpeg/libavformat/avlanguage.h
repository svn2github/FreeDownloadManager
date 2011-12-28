/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_AVLANGUAGE_H
#define AVFORMAT_AVLANGUAGE_H


enum AVLangCodespace {
    AV_LANG_ISO639_2_BIBL, 
    AV_LANG_ISO639_2_TERM, 
    AV_LANG_ISO639_1       
};


const char *av_convert_lang_to(const char *lang, enum AVLangCodespace target_codespace);

#endif 
