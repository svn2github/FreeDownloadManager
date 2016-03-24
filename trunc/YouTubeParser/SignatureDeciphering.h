/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef SIGNATUREDECIPHERINGH
#define SIGNATUREDECIPHERINGH
#include <vector>
#include <map>
#include <memory>
#include "ValuesProviderBase.h"
#include "SignatureDecipheringAlgorithm.h"

class SignatureDeciphering{	
	std::shared_ptr<ValuesProviderBase<SignatureDecipheringAlgorithm> > _algorithmProvider;
public:
	SignatureDeciphering( std::shared_ptr<ValuesProviderBase<SignatureDecipheringAlgorithm> > algorithmProvider );
	virtual ~SignatureDeciphering(){};	
	void addDecipheringAlgorithm( tstring id, tstring algorithm );
	virtual tstring getDecipherAlgorithm( const tstring& playerId, const tstring& playerText );
	
	virtual tstring decipherSignature( const tstring& signature, const tstring& playerId, const tstring& playerText );
	virtual tstring getDecipherAlgorithmFromPlayer( const tstring& playerText );
	virtual tstring getInitialFunctionName(const tstring& playerText);
protected:
	virtual void fillDecipheringAlgorithms();
	virtual tstring analyzeJSFunctionLine(  const tstring& playerText, const tstring& line );
	virtual tstring getDecipherAlgorithmFromJSFunction(  const tstring& playerText, const tstring& functionBody );
	virtual tstring getShortNameForFunc( const tstring& functionName );
	virtual tstring escapeSpecialSymbols( const tstring& str );
	std::map<tstring, tstring> decipheringAlgorithms;
	std::wstring m_decipherText;
	std::wstring m_playerId;
};
#endif