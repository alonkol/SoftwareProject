#include "SPPoint.h"

/** feature extraction and tree construction */

/** loads features  from file */
SPPoint* spLoadImgFeats(const SPConfig config,int numImages,int* totalSize);

/** Extracts features of certain image into array of points and saves them into file */
SPPoint* spUpdateAndSaveFeats(SPPoint* allFeats,SPPoint* imgFeats,int totalSize,int numFeats,int index,SPConfig config);

