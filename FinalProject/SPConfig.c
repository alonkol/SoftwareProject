#include <stdbool.h>
#include <stdio.h>
#include "SPLogger.h"
#include "SPConfig.h"


/**
 * A data-structure which is used for configuring the system.
 */


typedef enum tree_split_method{
	RANDOM,
	MAX_SPREAD,
	INCREMENTAL
} SPLIT_METHOD;

struct sp_config_t{
    char spImagesDirectory[1024];
    char spImagesPrefix[1024];
    char spImagesSuffix[1024];
    int spNumOfImages;
    int spPCADimension;
    char spPCAFilename[1024];
    int spNumOfFeatures;
    bool spExtractionMode;
    int spNumOfSimilarImages;
    SPLIT_METHOD spKDTreeSplitMethod;
    int spKNN;
    bool spMinimalGUI;
    int spLoggerLevel;
    char spLoggerFilename[1024];
};

typedef struct sp_config_t* SPConfig;

/**
 * Creates a new system configuration struct. The configuration struct
 * is initialized based on the configuration file given by 'filename'.
 *
 * @param filename - the name of the configuration file
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return NULL in case an error occurs. Otherwise, a pointer to a struct which
 * 		   contains all system configuration.
 *
 * The resulting value stored in msg is as follow:
 * - SP_CONFIG_INVALID_ARGUMENT - if filename == NULL
 * - SP_CONFIG_CANNOT_OPEN_FILE - if the configuration file given by filename cannot be open
 * - SP_CONFIG_ALLOC_FAIL - if an allocation failure occurred
 * - SP_CONFIG_INVALID_INTEGER - if a line in the config file contains invalid integer
 * - SP_CONFIG_INVALID_STRING - if a line in the config file contains invalid string
 * - SP_CONFIG_MISSING_DIR - if spImagesDirectory is missing
 * - SP_CONFIG_MISSING_PREFIX - if spImagesPrefix is missing
 * - SP_CONFIG_MISSING_SUFFIX - if spImagesSuffix is missing
 * - SP_CONFIG_MISSING_NUM_IMAGES - if spNumOfImages is missing
 * - SP_CONFIG_SUCCESS - in case of success
 *
 *
 */
SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg){
    // declarations
    char str[1024];
    char variableName[1024];
    char variableValue[1024];
    int i;
    int j;
    FILE* fp;

    SPConfig cfg = (SPConfig)malloc(sizeof(struct sp_config_t));
    // TODO: handle allocation failure

    // init
    cfg->spPCADimension = 20;
    strcpy(cfg->spPCAFilename,"pca.yml");
    cfg->spNumOfFeatures = 100;
    cfg->spExtractionMode = true;
    cfg->spNumOfSimilarImages = 1;
    cfg->spKDTreeSplitMethod = MAX_SPREAD;
    cfg->spKNN = 1;
    cfg->spMinimalGUI = false;
    cfg->spLoggerLevel = 3;
    strcpy(cfg->spLoggerFilename,"stdout");


    // read cfg from file
    fp = fopen(filename, "r");

    if(fp == NULL)
    {
      // ERROR opening the file
    }

    while( fgets (str, 1024, fp) != NULL )
    {
        i = 0;
        while (str[i] == ' '){
            i++;
        }
        if (str[i] == '#'){
            continue;
        }
        j = 0;
        while (str[i] != '=' && str[i] != ' '){
            variableName[j] = str[i];
            i++;
            j++;
        }
        variableName[j] = '\0';
        while (str[i] == '=' || str[i] == ' '){
            i++;
        }
        j = 0;
        while (str[i] != ' ' && str[i] != '\0'){
            variableValue[j] = str[i];
            i++;
            j++;
        }
        variableValue[j] = '\0';


        // TODO: Verify inputs and log errors

        if (strcmp(variableName,"spImagesDirectory") == 0){
            strcpy(cfg->spImagesDirectory,variableValue);
        } else if (strcmp(variableName,"spImagesPrefix") == 0){
            strcpy(cfg->spImagesPrefix,variableValue);
        } else if (strcmp(variableName,"spImagesSuffix") == 0){
            strcpy(cfg->spImagesSuffix,variableValue);
        } else if (strcmp(variableName,"spNumOfImages") == 0){
            cfg->spNumOfImages = atoi(variableValue);
        } else if (strcmp(variableName,"spPCADimension") == 0){
            cfg->spPCADimension = atoi(variableValue);
        } else if (strcmp(variableName,"spPCAFilename") == 0){
            strcpy(cfg->spPCAFilename,variableValue);
        } else if (strcmp(variableName,"spNumOfFeatures") == 0){
            cfg->spNumOfFeatures = atoi(variableValue);
        } else if (strcmp(variableName,"spExtractionMode") == 0){
            cfg->spExtractionMode = (variableValue == "true");
        } else if (strcmp(variableName,"spNumOfSimilarImages") == 0){
            cfg->spNumOfSimilarImages = atoi(variableValue);
        } else if (strcmp(variableName,"spKDTreeSplitMethod") == 0){
            if (strcmp(variableValue, "RANDOM") == 0){
                cfg->spKDTreeSplitMethod = RANDOM;
            } else if (strcmp(variableValue, "INCREMENTAL") == 0){
                cfg->spKDTreeSplitMethod = INCREMENTAL;
            }
        } else if (strcmp(variableName,"spKNN") == 0){
            cfg->spKNN = atoi(variableValue);
        } else if (strcmp(variableName,"spMinimalGUI") == 0){
            cfg->spMinimalGUI = (variableValue == "true");
        } else if (strcmp(variableName,"spLoggerLevel") == 0){
            cfg->spLoggerLevel = atoi(variableValue);
        } else if (strcmp(variableName,"spLoggerFilename") == 0){
            strcpy(cfg->spLoggerFilename,variableValue);
        }

    }

    fclose(fp);

    return cfg;
}

/*
 * Returns true if spExtractionMode = true, false otherwise.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return true if spExtractionMode = true, false otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg){
    if (config == NULL){
        msg = SP_CONFIG_INVALID_ARGUMENT;
        return;
    }
    msg = SP_CONFIG_SUCCESS;
    return config->spExtractionMode;
}

/*
 * Returns true if spMinimalGUI = true, false otherwise.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return true if spExtractionMode = true, false otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg){
    if (config == NULL){
        msg = SP_CONFIG_INVALID_ARGUMENT;
        return;
    }
    msg = SP_CONFIG_SUCCESS;
    return config->spMinimalGUI;
}

/*
 * Returns the number of images set in the configuration file, i.e the value
 * of spNumOfImages.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg){
    if (config == NULL){
        msg = SP_CONFIG_INVALID_ARGUMENT;
        return;
    }
    if (config->spNumOfImages == NULL){
        return -1;
    }
    msg = SP_CONFIG_SUCCESS;
    return config->spNumOfImages;
}

/*
 * Returns the number of features to be extracted. i.e the value
 * of spNumOfFeatures.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg){
   if (config == NULL){
        msg = SP_CONFIG_INVALID_ARGUMENT;
        return;
    }
    if (config->spNumOfFeatures == NULL){
        return -1;
    }
    msg = SP_CONFIG_SUCCESS;
    return config->spNumOfFeatures;
}

/**
 * Returns the dimension of the PCA. i.e the value of spPCADimension.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg){
   if (config == NULL){
        msg = SP_CONFIG_INVALID_ARGUMENT;
        return;
    }
    if (config->spPCADimension == NULL){
        return -1;
    }
    msg = SP_CONFIG_SUCCESS;
    return config->spPCADimension;
}

/**
 * Given an index 'index' the function stores in imagePath the full path of the
 * ith image.
 *
 * For example:
 * Given that the value of:
 *  spImagesDirectory = "./images/"
 *  spImagesPrefix = "img"
 *  spImagesSuffix = ".png"
 *  spNumOfImages = 17
 *  index = 10
 *
 * The functions stores "./images/img10.png" to the address given by imagePath.
 * Thus the address given by imagePath must contain enough space to
 * store the resulting string.
 *
 * @param imagePath - an address to store the result in, it must contain enough space.
 * @param config - the configuration structure
 * @param index - the index of the image.
 *
 * @return
 * - SP_CONFIG_INVALID_ARGUMENT - if imagePath == NULL or config == NULL
 * - SP_CONFIG_INDEX_OUT_OF_RANGE - if index >= spNumOfImages
 * - SP_CONFIG_SUCCESS - in case of success
 */
SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,
		int index){
	if (imagePath == NULL || config == NULL){
        return SP_CONFIG_INVALID_ARGUMENT;
    }
    if (index >= config->spNumOfImages){
        return SP_CONFIG_INDEX_OUT_OF_RANGE;
    }
    imagePath = config->spImagesDirectory;
    strcat(imagePath, config->spImagesPrefix);
    strcat(imagePath, index);
    strcat(imagePath, config->spImagesSuffix);

    return SP_CONFIG_SUCCESS;
}

/**
 * The function stores in pcaPath the full path of the pca file.
 * For example given the values of:
 *  spImagesDirectory = "./images/"
 *  spPcaFilename = "pca.yml"
 *
 * The functions stores "./images/pca.yml" to the address given by pcaPath.
 * Thus the address given by pcaPath must contain enough space to
 * store the resulting string.
 *
 * @param imagePath - an address to store the result in, it must contain enough space.
 * @param config - the configuration structure
 * @return
 *  - SP_CONFIG_INVALID_ARGUMENT - if imagePath == NULL or config == NULL
 *  - SP_CONFIG_SUCCESS - in case of success
 */
SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config){
    if (pcaPath == NULL || config == NULL){
        return SP_CONFIG_INVALID_ARGUMENT;
    }
    pcaPath = config->spImagesDirectory;
    strcat(pcaPath, config->spPCAFilename);
    return SP_CONFIG_SUCCESS;
}

/**
 * Frees all memory resources associate with config.
 * If config == NULL nothig is done.
 */
void spConfigDestroy(SPConfig config){
    free(config);
}



