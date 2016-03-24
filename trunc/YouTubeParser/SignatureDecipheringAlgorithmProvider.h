/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef SIGNATUREDECIPHERINGALGORITHMPROVIDERH
#define SIGNATUREDECIPHERINGALGORITHMPROVIDERH
#include "ValuesProviderBase.h"
#include "SignatureDecipheringAlgorithm.h"
class SignatureDecipheringAlgorithmProvider : public ValuesProviderBase<SignatureDecipheringAlgorithm>{
public:
	std::vector<SignatureDecipheringAlgorithm> getValues();
	~SignatureDecipheringAlgorithmProvider(){};
};
#endif