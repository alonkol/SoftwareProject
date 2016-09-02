/** feature extraction and tree construction */
#include <stdio.h>
#include <stdlib.h>
#include "SPConfig.h"
#include "SPPoint.h"
#include "SP_aux.h"

#define MAXLINESIZE 1024
#define featsSuff ".feats"

SPPoint* spUpdateAndSaveFeats(SPPoint* allFeats,SPPoint* imgFeats,int totalSize,int numFeats,int index,SPConfig config)
{
    int j,k;
    SP_CONFIG_MSG msg;
    char featsFileName[MAXLINESIZE];
    allFeats=(SPPoint*)realloc(allFeats,(totalSize+numFeats)*sizeof(SPPoint));
    if(allFeats==NULL){
        printf("NULLLLL");
        return NULL;
    }
    msg=spConfigGetPath(featsFileName,config,index,featsSuff);
    if(msg!=SP_CONFIG_SUCCESS){
        printf("PROBLEM!!!");
        return NULL;
    }
    FILE *fp= fopen(featsFileName, "w");
    fprintf(fp,"%d\n%d\n%d\n",index,numFeats,spConfigGetPCADim(config,&msg));
    for(j=0; j<numFeats; j++)
    {
        allFeats[totalSize+j] = spPointCopy(imgFeats[j]);
        for(k=0; k<spConfigGetPCADim(config,&msg); k++)
        {
            fprintf(fp,"%lf ",spPointGetAxisCoor(imgFeats[j],k));
        }
        spPointDestroy(imgFeats[j]);
        fprintf(fp,"\n");
    }
    fclose(fp);
    free(imgFeats);
    return allFeats;
}


SPPoint* spLoadImgFeats(const SPConfig config,int numImages,int *totalSize)
{
    SPPoint* allFeats=NULL;
    SP_CONFIG_MSG msg;
    int i,j,k,imgIndex,numFeats,savedDim,dim = spConfigGetPCADim(config,&msg);
    *totalSize=0;
    char featsFileName[MAXLINESIZE],buff[MAXLINESIZE];
    for (i=0;i<numImages;i++){
        msg=spConfigGetPath(featsFileName,config,i,featsSuff);
        FILE *fo = fopen(featsFileName, "r");
        fscanf(fo,"%d",&imgIndex);
        fscanf(fo,"%d",&numFeats);
        fscanf(fo,"%d",&savedDim);
        if(i!=imgIndex){
            printf("ERROR");
            return NULL;
        }
        if(savedDim>dim){
            printf("Warning,took only %d dim instead of %d\n",dim,savedDim);

        }
        if(savedDim<dim){
            printf("Error");
            return NULL;
        }
        allFeats=(SPPoint*)realloc(allFeats,(*totalSize+numFeats)*sizeof(SPPoint));
        for(j=0;j<numFeats;j++){
            double* data=(double*)malloc(sizeof(double)*dim);
            for(k=0;k<dim;k++){
                fscanf(fo,"%lf",&data[k]);
            }
            fgets(buff,MAXLINESIZE,fo);//skip to next line
            allFeats[*totalSize+j] = spPointCreate(data,dim,imgIndex);
        }
        *totalSize+=numFeats;
        fclose(fo);
        printf("Finished loading img %d \n",imgIndex);
    }

    return allFeats;


}

/*
SPPoint* spLoadImgFeats(const SPConfig config,int index){
    SPImgFeats* imgFeats = (SPImgFeats*)malloc(sizeof(SPImgFeats));
    char fileName[MAXLINESIZE],buff[MAXLINESIZE];
    spConfigGetImageFeatsPath(fileName,config,imgFeats->imgIndex); //changeeee
    FILE *fo;
    fo = fopen(fileName, "r");
    fscanf(fo,"%d",&imgFeats->imgIndex);
    fscanf(fo,"%d",&imgFeats->numFeats);
    imgFeats->feats=(SPPoint*)malloc(sizeof(SPPoint*)*imgFeats->numFeats);
    for(int i=0;i<imgFeats->numFeats;i++){
        int dim;
        fscanf(fo,"%d",&dim);
        double* vals = (double*)malloc(sizeof(double)*dim);
        for(int j=0;j<dim;j++){
            fscanf(fo,"%f",&vals[j]);
        }
        fgets(buff,MAXLINESIZE,fo);//skip to next line
        imgFeats->feats[i] = spPointCreate(vals,dim,index);
    }
    return imgFeats;
}

*/



