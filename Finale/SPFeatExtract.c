/** feature extraction */
#include <stdio.h>
#include <stdlib.h>
#include "SPFeatExtract.h"

#define MAXLINESIZE 1024
#define FEATSSUFF ".feats"
#define ALLOC_ERROR_MSG "Memory Allocation Error."
#define FILE_PROBLEM_ERROR "Problem with reading file."
#define FILE_INDEX_ERROR "File image index isn't corresponding to image name."

SPPoint* spLoadImgFeats(const SPConfig config,int numImages,int *totalSize)
{
    char featsFileName[MAXLINESIZE],buff[MAXLINESIZE];
    SPPoint *allFeats=NULL,*tmpPtr,tmpPoint;
    SP_CONFIG_MSG msg;
    FILE *fo;
    double* data;
    int i,j,k,imgIndex,numFeats,savedDim,dim = spConfigGetPCADim(config,&msg);
    *totalSize=0;

    for (i=0; i<numImages; i++)
    {
        /*  The format of a feats file is: three lines containing the image index, number of features saved
        and the dimension of every feature. after which each feature is printed in a line seperated by spaces. */
        msg=spConfigGetPath(featsFileName,config,i,FEATSSUFF);
        fo = fopen(featsFileName, "r");
        if(fo==NULL){
            spLoggerPrintError(FILE_PROBLEM_ERROR,__FILE__,__func__,__LINE__);
            destroyPointsArr(allFeats,*totalSize);
            return NULL;
        }
        fscanf(fo,"%d",&imgIndex);
        fscanf(fo,"%d",&numFeats);
        fscanf(fo,"%d",&savedDim);
        if(i!=imgIndex)
        {
            spLoggerPrintError(FILE_INDEX_ERROR,__FILE__,__func__,__LINE__);
            destroyPointsArr(allFeats,*totalSize);
            return NULL;
        }
        if(savedDim>dim)
        {
            sprintf(buff,"Took only %d dim from images saved with %d dimension.",dim,savedDim);
            destroyPointsArr(allFeats,*totalSize);
            spLoggerPrintWarning(buff,__FILE__,__func__,__LINE__);

        }
        if(savedDim<dim)
        {
            sprintf(buff,"User wants %d dim from images while saved with %d dimension.",dim,savedDim);
            destroyPointsArr(allFeats,*totalSize);
            spLoggerPrintError(buff,__FILE__,__func__,__LINE__);
            return NULL;
        }
        tmpPtr=(SPPoint*)realloc(allFeats,(*totalSize+numFeats)*sizeof(SPPoint));
        if(tmpPtr==NULL){
            destroyPointsArr(allFeats,*totalSize);
            return NULL;
        }
        allFeats=tmpPtr;

        for(j=0; j<numFeats; j++)
        {
            data=(double*)malloc(sizeof(double)*dim);
            if(data==NULL){
                destroyPointsArr(allFeats,*totalSize+j);
                return NULL;
            }
            for(k=0; k<dim; k++)
            {
                fscanf(fo,"%lf",&data[k]);
            }
            fgets(buff,MAXLINESIZE,fo);//skip to next line
            tmpPoint = spPointCreate(data,dim,imgIndex);
            free(data);
            if (tmpPoint==NULL){
                destroyPointsArr(allFeats,*totalSize+j);
                return NULL;
            }
            allFeats[*totalSize+j] = tmpPoint;

        }
        *totalSize+=numFeats;
        fclose(fo);
        sprintf(buff,"Finished loading image %d ",imgIndex);
        spLoggerPrintInfo(buff);
    }

    return allFeats;

}

SPPoint* spUpdateAndSaveFeats(SPPoint* allFeats,SPPoint* imgFeats,int totalSize,int numFeats,int index,SPConfig config)
{
    int j,k;
    SP_CONFIG_MSG msg;
    FILE *fp;
    char featsFileName[MAXLINESIZE],buff[MAXLINESIZE];;
    allFeats=(SPPoint*)realloc(allFeats,(totalSize+numFeats)*sizeof(SPPoint));
    if(allFeats==NULL)
    {
        spLoggerPrintError(ALLOC_ERROR_MSG,__FILE__,__func__,__LINE__ );
        return NULL;
    }
    msg=spConfigGetPath(featsFileName,config,index,FEATSSUFF);
    if(msg!=SP_CONFIG_SUCCESS)
    {
        spLoggerPrintError(FILE_PROBLEM_ERROR,__FILE__,__func__,__LINE__ );
        return NULL;
    }
    fp = fopen(featsFileName, "w");
    if(fp==NULL){
            spLoggerPrintError(FILE_PROBLEM_ERROR,__FILE__,__func__,__LINE__);
            destroyPointsArr(allFeats,totalSize);
            return NULL;
    }
    /*  The format of a feats file is: three lines containing the image index, number of features saved
        and the dimension of every feature. after which each feature is printed in a line seperated by spaces. */
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
    sprintf(buff,"Img %d features were saved to file %s.",index,featsFileName);
    spLoggerPrintInfo(buff);
    return allFeats;
}

void destroyPointsArr(SPPoint* pointsArr,int arrSize)
{
    int i;
    if(pointsArr!=NULL)
    {
        for(i=0; i<arrSize; i++)
        {
            spPointDestroy(pointsArr[i]);
        }
        free(pointsArr);
    }
}








