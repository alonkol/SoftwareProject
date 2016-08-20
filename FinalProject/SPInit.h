/** feature extraction and tree construction */


/** struct designed to hold for each image its features */
typedef struct sp_img_feats SPImgFeats;

/** Extracts/loads features into array of  SPImgFeats according to config*/
SPImgFeats** spProduceFeats(const SPConfig config);

/** Extracts features of a certain image into SPImgFeat */
SPImgFeats* spExtractImgFeats(const SPConfig config,int index);

/** save features of a certain image into file */
void spWriteImgFeatsFile(SPImgFeats* imgFeats);

/** loads features of a certain image from file */
SPImgFeats* spLoadImgFeats(const SPConfig config,int index);

