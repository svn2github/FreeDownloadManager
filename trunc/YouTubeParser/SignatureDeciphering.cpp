/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "SignatureDeciphering.h"
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <regex>

SignatureDeciphering::SignatureDeciphering( std::shared_ptr<ValuesProviderBase<SignatureDecipheringAlgorithm> > algorithmProvider ){
	_algorithmProvider = algorithmProvider;
	fillDecipheringAlgorithms();
}

void SignatureDeciphering::fillDecipheringAlgorithms(){
	if ( !_algorithmProvider )
		return;
	decipheringAlgorithms.clear();
	std::vector<SignatureDecipheringAlgorithm> algosVector = _algorithmProvider->getValues();
	for ( auto it = algosVector.begin(); it != algosVector.end(); ++it ){
		SignatureDecipheringAlgorithm algo = *it;
		decipheringAlgorithms[algo.id] = algo.algorithm;
	}	       
}

void SignatureDeciphering::addDecipheringAlgorithm( tstring id, tstring algorithm ){
	decipheringAlgorithms[id] = algorithm;
}

tstring SignatureDeciphering::getDecipherAlgorithm( const tstring& playerId, const tstring& playerText ){	
	if ( decipheringAlgorithms.find( playerId ) != decipheringAlgorithms.end() ){
		return decipheringAlgorithms[playerId];
	}
	return getDecipherAlgorithmFromPlayer( playerText );	
}

tstring SignatureDeciphering::decipherSignature( const tstring &signature, const tstring& playerId, const tstring& playerText ){
	tstring result = signature;
	std::vector<tstring> commands;
	if ((m_playerId != playerId) || m_decipherText.empty() )
		m_decipherText = getDecipherAlgorithm(playerId, playerText);
	m_playerId = playerId;
	boost::split(commands, m_decipherText, boost::is_any_of(" "));
	for (auto it_command = commands.begin(); it_command != commands.end(); ++it_command)
	{
		tstring command = *it_command;
		if ( boost::starts_with( command, _T("r") ) ){
			tstring temp( result.rbegin(), result.rend() );
			result = temp;
		}
		else if ( boost::starts_with( command, _T("s") ) ){
			int startIndex = -1;
			try{
				startIndex = std::stoi(command.substr( 1 ) );
			}
			catch (std::exception const&  ex) {
				OutputDebugString( _T("Deciphering signature: cannot get an index for slice") );
				return _T("");
			}
			if ( ( startIndex >= 0 ) && ( startIndex < result.length() ) ){
				result = result.substr( startIndex );
			}
		}
		else if ( boost::starts_with( command, _T("w") ) ){
			int index = -1;
			try{
				index = std::stoi(command.substr( 1 ) );
			}
			catch (std::exception const&  ex) {
				OutputDebugString( _T("Deciphering signature: cannot get an index for swap") );
				return _T("");
			}
			if ( !result.empty() && ( index >= 0 ) ){				
				index = index % result.length();
				std::swap(result[0], result[index]);
			}
		}
		else return _T("");
	}

	return result;
}

tstring SignatureDeciphering::getShortNameForFunc( const tstring& func ){
	if ( boost::contains( func, _T("split") ) )
		return _T("");
	if ( boost::contains( func, _T("join") ) )
		return _T("");
	if ( boost::contains( func, _T("splice") ) )
		return _T("s");
	if ( boost::contains( func, _T("reverse") ) )
		return _T("r");
	if ( boost::contains( func, _T("length]") ) )
		return _T("w");

	return _T("");
}

tstring SignatureDeciphering::analyzeJSFunctionLine(  const tstring& playerText, const tstring& line ){

	tstring _line = line;
	auto dotPos = _line.find( _T(".") );
	if ( ( dotPos != _line.npos ) && ( dotPos + 1 < _line.length() )  )
		_line = _line.substr( dotPos + 1 );
	std::wregex funcPattern(_T("([^(]+)\\(([^)]*)"));
	std::wsmatch match;
	tstring funcName;
	tstring funcArguments;
	if(std::regex_search(_line, match, funcPattern)) {
		if(match[1].matched) {
			funcName = match[1].str();
		}
		if ( match[2].matched ){
			funcArguments = match[2].str();
		}
	}
	if ( !funcName.empty() ){		
		tstring shortName = getShortNameForFunc( funcName );
		if ( shortName.empty() ){
			std::wstring patternString = funcName;
			patternString += _T(":function[^\\{]+\\{([^}]+)");
			std::wregex subFuncPattern(patternString);
			std::wsmatch subFuncMatch;
			tstring subFunc;
			if(std::regex_search(playerText, subFuncMatch, subFuncPattern)) {
				if(subFuncMatch[1].matched) {
					subFunc = subFuncMatch[1].str();
					shortName = getShortNameForFunc( subFunc );					
				}
			}			
		}
		if ( !shortName.empty() ){
			tstring numberParam;
			std::vector<tstring> args;
			boost::split(args, funcArguments, boost::is_any_of(","));
			for (auto it = args.begin(); it != args.end(); ++it)
			{
				tstring arg = *it;
				if ( !arg.empty() && ( arg.find_first_not_of(_T("0123456789") ) == tstring::npos ) ){
					numberParam = arg;
					break;
				}
			}
			return shortName + numberParam;
		}
	}
	return _T("");
}

tstring SignatureDeciphering::getDecipherAlgorithmFromJSFunction(  const tstring& playerText, const tstring& functionBody ){
	tstring result;

	
	std::vector<tstring> elems;
	boost::split(elems, functionBody, boost::is_any_of(";"));
	for (auto it = elems.begin(); it != elems.end(); ++it)
	{
		tstring subResult = analyzeJSFunctionLine( playerText, *it );
		if ( !result.empty() && !subResult.empty() )
			result += _T( " " );
		result += subResult;
	}

	OutputDebugString( result.c_str() );

	return result;
}

tstring SignatureDeciphering::getInitialFunctionName(const tstring& playerText){
	std::wregex initialFuncNamePattern(_T("(signature=)([^(]+)"));
	std::wsmatch match;
	std::wstring funcName;
	if (std::regex_search(playerText, match, initialFuncNamePattern) && (match.size() > 2 ) && match[2].matched) {
		funcName = match[2].str();
	}
	else{
		initialFuncNamePattern = _T("(\"signature\",)([^(]+)");		
		if (std::regex_search(playerText, match, initialFuncNamePattern) && (match.size() > 2) && match[2].matched) {
			funcName = match[2].str();
		}
	}

	return funcName;
}

tstring SignatureDeciphering::escapeSpecialSymbols(const tstring& str){
	tstring result = str;
	tstring escapedChars = _T(".^$|()[]{}*+?");
	for (auto it = escapedChars.begin(); it != escapedChars.end(); ++it){
		tstring search(1, (*it));
		tstring replacement = L"\\";
		replacement += search;
		boost::replace_all(result, search, replacement);
	}

	return result;
}

tstring SignatureDeciphering::getDecipherAlgorithmFromPlayer(  const tstring& playerText ){
	tstring func;
	tstring funcName;

	
	funcName = getInitialFunctionName( playerText );
	funcName = escapeSpecialSymbols(funcName);
	
	if (!funcName.empty()){
		std::wstringstream tstr;
		
		tstr << _T("(function ") << funcName << _T(")\\([^)]+\\)\\{([^\\}]+)");
		std::wregex initialFuncPattern(tstr.str());
		std::wsmatch funcMatch;
		if (std::regex_search(playerText, funcMatch, initialFuncPattern) && ( funcMatch.size() > 2 ) && funcMatch[2].matched){
			func = funcMatch[2].str();
			
		}
		return getDecipherAlgorithmFromJSFunction(playerText, func);
	}
	return _T( "" );
}