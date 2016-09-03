/** feature extraction */
#include <stdio.h>
#include <stdlib.h>
#include "SPConfig.h"
#include "SPPoint.h"
#include "SPFeatExtract.h"
#include "SPBPriorityQueue.h"

#define MAXLINESIZE 1024
#define featsSuff ".feats"
#define ALLOC_ERROR_MSG "Memory Allocation Error."

SPPoint* spUpdateAndSaveFeats(SPPoint* allFeats,SPPoint* imgFeats,int totalSize,int numFeats,int index,SPConfig config)
{
    int j,k;
    SP_CONFIG_MSG msg;
    FILE *fp;
    char featsFileName[MAXLINESIZE],buff[MAXLINESIZE];;
    allFeats=(SPPoint*)realloc(allFeats,(totalSize+numFeats)*sizeof(SPPoint));
    if(allFeats==NULL){
        spLoggerPrintError(ALLOC_ERROR_MSG,__FILE__,__func__,__LINE__ );
        return NULL;
    }
    msg=spConfigGetPath(featsFileName,config,index,featsSuff);
    if(msg!=SP_CONFIG_SUCCESS){
        spLoggerPrintError("problem with filepath",__FILE__,__func__,__LINE__ );
        return NULL;
    }
    fp= fopen(featsFileName, "w");
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


SPPoint* spLoadImgFeats(const SPConfig config,int numImages,int *totalSize)
{
    char featsFileName[MAXLINESIZE],buff[MAXLINESIZE];
    SPPoint* allFeats=NULL;
    SP_CONFIG_MSG msg;
    FILE *fo;
    int i,j,k,imgIndex,numFeats,savedDim,dim = spConfigGetPCADim(config,&msg);
    *totalSize=0;

    for (i=0;i<numImages;i++){
        msg=spConfigGetPath(featsFileName,config,i,featsSuff);
        fo = fopen(featsFileName, "r");
        fscanf(fo,"%d",&imgIndex);
        fscanf(fo,"%d",&numFeats);
        fscanf(fo,"%d",&savedDim);
        if(i!=imgIndex){
            spLoggerPrintError("File image index isn't corresponding to image name.",__FILE__,__func__,__LINE__);
            return NULL;
        }
        if(savedDim>dim){
            sprintf(buff,"Took only %d dim from images saved with %d dimension.",dim,savedDim);
            spLoggerPrintWarning(buff,__FILE__,__func__,__LINE__);

        }
        if(savedDim<dim){
            sprintf(buff,"User wants %d dim from images while saved with %d dimension.",dim,savedDim);
            spLoggerPrintError(buff,__FILE__,__func__,__LINE__);
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
        sprintf(buff,"Finished loading image %d ",imgIndex);
        spLoggerPrintInfo(buff);
    }

    return allFeats;

}








