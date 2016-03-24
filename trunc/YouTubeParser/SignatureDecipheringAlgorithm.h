/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef SIGNATUREDECIPHERINGALGORITHMH
#define SIGNATUREDECIPHERINGALGORITHMH
struct SignatureDecipheringAlgorithm{
	tstring id, algorithm;
	SignatureDecipheringAlgorithm( tstring _id, tstring _algorithm ){
		id = _id;
		algorithm = _algorithm;
	}
};
#endif