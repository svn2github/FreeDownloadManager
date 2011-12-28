/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVUTIL_PCA_H
#define AVUTIL_PCA_H

struct PCA *ff_pca_init(int n);
void ff_pca_free(struct PCA *pca);
void ff_pca_add(struct PCA *pca, double *v);
int ff_pca(struct PCA *pca, double *eigenvector, double *eigenvalue);

#endif 
