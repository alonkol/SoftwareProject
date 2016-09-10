#include "SPPoint.h"
#include "SPConfig.h"

/** This file contains functions related to feature extraction **/

/**
* loads features from .feat file into array of features
* @param    config  - configuration file.
* @param    numImages - the number of images to be loaded from file.
* @param    totalSize - pointer to int used to return the size of the array created.
* @return   SPPoint* - the array of features in success,
*           Null in case of allocation error or no matching between saved files to config file
*           such as feature dimension problem.
*/
SPPoint* spLoadImgFeats(const SPConfig config,int numImages,int* totalSize);

/**
* Gets array of extracted features of certain image, adds them to an array containing all the features,
* and saved those features into file.
* @param    allfeats  - array of points to which the new points(features) are added.
* @param    imgFeats - array of points(features) extracted from a single image.
* @param    totalSize - the size of the array before adding the features.
* @param    index - the index of the image its features are added.
* @param    config - the configuration file.
* @return   SPPoint* - the array of features in success,
            Null in case of allocation error or problem with files.
*
*/
SPPoint* spUpdateAndSaveFeats(SPPoint* allFeats,SPPoint* imgFeats,int totalSize,int numFeats,int index,SPConfig config);

/**
 * The function destroys an array consisted of SPPoint.
 * @param   pointsArr - pointer to array of points.
 * @param   arrSize - size of the array.
 * @return  void
**/
void destroyPointsArr(SPPoint* pointsArr,int arrSize);
