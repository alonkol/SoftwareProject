/** feature extraction and tree construction */
#include <stdio.h>
#include <stdlib.h>
#include "SPConfig.h"
#include "SPImageProc.h"
#include "SPPoint.h"
#include "SP_aux.h"

#define MAXLINESIZE 1024
#define featsSuff ".feats"


/** produces and saves features */
SPPoint* spProduceFeats(const SPConfig config)
{
    SP_CONFIG_MSG msg;
    int numImages = spConfigGetNumOfImages(config,&msg);
    int numFeatures = spConfigGetNumOfFeatures(config,&msg);
    SPPoint* allFeats;
    if(spConfigIsExtractionMode(config,msg))
    {
        allFeats=spExtractSaveImgFeats(config);
    }
    else
    {
        allFeats=spLoadImgFeats(config);
    }
    return allFeats;
}

SPPoint* spExtractSaveImgFeats(const SPConfig config)
{
    SP_CONFIG_MSG msg;
    SPPoint *imgFeats,*allFeats;
    char imagePath[MAXLINESIZE],featsFileName[MAXLINESIZE];
    int numFeats,i,j,totalSize=0;
    for(i = 0; i<spConfigGetNumOfImages(config,msg);i++)
    {
        msg = spConfigGetImagePath(imagePath,config,index);
        if(msg!=SP_CONFIG_SUCCESS)
        {
            //declare error
            return -1;
        }
        imgFeats = getImageFeatures(imagePath,index,&numFeats);
        allFeats=(SPPoint*)realloc(allFeats,totalSize+numFeats);
        msg=spConfigGetPath(featsFileName,config,index,featsSuff);
        FILE *fp;
        fp = fopen(featsFileName, "w");
        fprintf(fp,"%d\n%d\n",i,numFeats);
        for(j=0;j<numFeats;j++){
            allFeats[totalSize+j] = spPointCopy(imgFeats[j]);
            for(k=0; k<spConfigGetNumOfFeatures(config,msg); k++)
            {
                fprintf(fp,"%f ",spPointGetAxisCoor(imgFeats[j],k));
                spPointDestroy(imgFeats[j])
            }
            fprintf(fp,"\n");
        }
        fclose(fp);
        totalSize += numFeats;
        free(imgFeats);
    }

}









/*
SPPoint* spExtractSaveImgFeats(const SPConfig config,int index)
{
    //reads ans stores all features for image
    char imagePath[MAXLINESIZE];
    SP_CONFIG_MSG msg;

    SPImgFeats *imgFeats = (SPImgFeats*)malloc(sizeof(SPImgFeats));
    if (imgFeats==NULL)
    {
        printf("ALLOCATION\n");
        return NULL;
    }
    imgFeats->imgIndex=index;
    msg = spConfigGetImagePath(imagePath,config,index);
    if(msg==SP_CONFIG_SUCCESS)
    {
        imgFeats->feats = getImageFeatures(imagePath,index,&imgFeats->numFeats);
    }
    return imgFeats;    //possibly NULL
}

void spWriteImgFeatsFile(SPImgFeats* imgFeats)
{
    //// if failes write to logger
    char fileName[MAXLINESIZE];
    spConfigGetImageFeatsPath(fileName,config,imgFeats->imgIndex); //changeeee
    FILE *fp;
    fp = fopen(fileName, "w");
    fprintf(fp,"%d\n%d\n",imgFeats->imgIndex,imgFeats->numFeats);
    for(int i=0; i<imgFeats->numFeats; i++)
    {
        int dim = spPointGetDimension(imgFeats->feats[i]);
        fprintf(fp,"%d ",dim);
        for(int j=0; j<spPointGetDimension(dim); j++)
        {
            fprintf(fp,"%f ",spPointGetAxisCoor(imgFeats->feats[i],j));
        }
        fprintf(fp,"\n");
    }
    fclose(fp);

}

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



