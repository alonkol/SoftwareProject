#include <stdio.h>
#include <stdlib.h>
#include "SPConfig.h"
#include "SPImageProc.h"
#include "SPPoint.h"
#include "SPInit.h"

#define MAXPATHSIZE 1024;

/** feature extraction and tree construction */
struct sp_img_feats
{
    int imgIndex;
    int numFeats;
    SPPoint* feats;
}

/** produces and saves features */
SPImgFeats** spProduceFeats(const SPConfig config)
{
    SP_CONFIG_MSG msg;
    int numImages = spConfigGetNumOfImages(config,msg);
    int numFeatures = spConfigGetNumOfFeatures(config,msg);
    SPImgFeats** allFeats = (SPImgFeats**)malloc(sizeof(SPImgFeats*)*numImages);
    if (imgFeats==NULL)
    {
        printf("ALLOCATION\n");
        return NULL;
    }
    for(i=0; i<numImages; i++)
    {
        if(spConfigIsExtractionMode(config,msg))
        {
            allFeats[i]=spExtractImgFeats(i);
            spWriteImgFeatsFile(allFeats[i]);
        }
        else
        {
            allFeats[i]=spLoadImgFeats(config,i);
        }
        return allFeats;
    }

    SPImgFeats* spExtractImgFeats(const SPConfig config,int index)
    {
        //reads ans stores all features for image
        char imagePath[MAXPATHSIZE];
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
        char fileName[MAXPATHSIZE];
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

    SPImgFeats* spLoadImgFeats(const SPConfig config,int index){
        SPImgFeats* imgFeats = (SPImgFeats*)malloc(sizeof(SPImgFeats));
        char fileName[MAXPATHSIZE],buff[MAXPATHSIZE];
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
            fgets(buff,MAXPATHSIZE,fo);//skip to next line
            imgFeats->feats[i] = spPointCreate(vals,dim,index);
        }
        return imgFeats;
    }





