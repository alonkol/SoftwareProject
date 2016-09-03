#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "SPLogger.h"
#include "SPConfig.h"
#define MAXLINESIZE 1024
#define MAX_PARAM_LEN 64

#define MEMORY_ALLOC_FAILURE_MSG "Memory allocation failure\n"
#define INVALID_CONFIG_LINE_MSG "Invalid configuration line"
#define CONSTRAINT_NOT_MET_MSG "Invalid value - constraint not met"


/**
 * A data-structure which is used for configuring the system.
 */


struct sp_config_t{
    char spImagesDirectory[MAXLINESIZE];
    char spImagesPrefix[MAXLINESIZE];
    char spImagesSuffix[MAXLINESIZE];
    int spNumOfImages;
    int spPCADimension;
    char spPCAFilename[MAXLINESIZE];
    int spNumOfFeatures;
    bool spExtractionMode;
    int spNumOfSimilarImages;
    SPLIT_METHOD spKDTreeSplitMethod;
    int spKNN;
    bool spMinimalGUI;
    int spLoggerLevel;
    char spLoggerFilename[MAXLINESIZE];
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
    char str[MAXLINESIZE];
    char variableName[MAXLINESIZE];
    char variableValue[MAXLINESIZE];
    char param[MAX_PARAM_LEN];
    int i;
    int j;
    int line = 0;
    FILE* fp;

    SPConfig cfg = (SPConfig)malloc(sizeof(struct sp_config_t));
    if (cfg == NULL){
        printf(MEMORY_ALLOC_FAILURE_MSG);
        return NULL;
    }

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

    //to check if it was initialized
    cfg->spNumOfImages = -1;
    strcpy(cfg->spImagesSuffix,"");
    strcpy(cfg->spImagesPrefix,"");
    strcpy(cfg->spImagesDirectory,"");
    strcpy(param,"");

    // read cfg from file
    fp = fopen(filename, "r");

    if(fp == NULL)
    {
        printf("The configuration file %s couldn't be opened\n",filename);
        free(cfg);
        return NULL;
    }

    while( fgets (str, MAXLINESIZE, fp) != NULL )
    {
        line++;
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
        while (str[i] == ' '){
            i++;
        }
        if (str[i] != '='){
            printConfigError(filename, line, INVALID_CONFIG_LINE_MSG);
            free(cfg);
            return NULL;
        }
        i++;
        while (str[i] == ' '){
            i++;
        }
        j = 0;
        while (str[i] != ' ' && str[i] != '\0' && str[i] != '\n' && str[i] != '#'){
            variableValue[j] = str[i];
            i++;
            j++;
        }
        variableValue[j] = '\0';

        // check if invalid end of row
        while (str[i] == ' '){
            i++;
        }
        if (str[i] != '\0' && str[i] != '\n' && str[i] != '#'){
            printConfigError(filename, line, INVALID_CONFIG_LINE_MSG);
            free(cfg);
            return NULL;
        }

        if (strcmp(variableName,"spImagesDirectory") == 0){
            strcpy(cfg->spImagesDirectory,variableValue);
        } else if (strcmp(variableName,"spImagesPrefix") == 0){
            strcpy(cfg->spImagesPrefix,variableValue);
        } else if (strcmp(variableName,"spImagesSuffix") == 0){
            if (strcmp(variableValue, ".jpg") != 0 &&
                strcmp(variableValue, ".png") != 0 &&
                strcmp(variableValue, ".bmp") != 0 &&
                strcmp(variableValue, ".gif") != 0){
                printConfigError(filename, line, CONSTRAINT_NOT_MET_MSG);
                free(cfg);
                return NULL;
            }
            strcpy(cfg->spImagesSuffix,variableValue);
        } else if (strcmp(variableName,"spNumOfImages") == 0){
            cfg->spNumOfImages = atoi(variableValue);
            if (cfg->spNumOfImages <= 0){
                printConfigError(filename, line, CONSTRAINT_NOT_MET_MSG);
                free(cfg);
                return NULL;
            }
        } else if (strcmp(variableName,"spPCADimension") == 0){
            cfg->spPCADimension = atoi(variableValue);
            if (cfg->spPCADimension < 10 || cfg->spPCADimension > 28){
                printConfigError(filename, line, CONSTRAINT_NOT_MET_MSG);
                free(cfg);
                return NULL;
            }
        } else if (strcmp(variableName,"spPCAFilename") == 0){
            strcpy(cfg->spPCAFilename,variableValue);
        } else if (strcmp(variableName,"spNumOfFeatures") == 0){
            cfg->spNumOfFeatures = atoi(variableValue);
            if (cfg->spNumOfFeatures <= 0){
                printConfigError(filename, line, CONSTRAINT_NOT_MET_MSG);
                free(cfg);
                return NULL;
            }
        } else if (strcmp(variableName,"spExtractionMode") == 0){
            if (strcmp(variableValue,"true") != 0 && strcmp(variableValue,"false") != 0){
                printConfigError(filename, line, CONSTRAINT_NOT_MET_MSG);
                free(cfg);
                return NULL;
            }
            cfg->spExtractionMode = (strcmp(variableValue,"true")==0);
        } else if (strcmp(variableName,"spNumOfSimilarImages") == 0){
            cfg->spNumOfSimilarImages = atoi(variableValue);
            if (cfg->spNumOfSimilarImages <= 0){
                printConfigError(filename, line, CONSTRAINT_NOT_MET_MSG);
                free(cfg);
                return NULL;
            }
        } else if (strcmp(variableName,"spKDTreeSplitMethod") == 0){
            if (strcmp(variableValue, "RANDOM") == 0){
                cfg->spKDTreeSplitMethod = RANDOM;
            } else if (strcmp(variableValue, "INCREMENTAL") == 0){
                cfg->spKDTreeSplitMethod = INCREMENTAL;
            } else if (strcmp(variableValue, "MAX_SPREAD") == 0){
                cfg->spKDTreeSplitMethod = MAX_SPREAD;
            } else {
                printConfigError(filename, line, CONSTRAINT_NOT_MET_MSG);
                free(cfg);
                return NULL;
            }
        } else if (strcmp(variableName,"spKNN") == 0){
            cfg->spKNN = atoi(variableValue);
            if (cfg->spKNN <= 0){
                printConfigError(filename, line, CONSTRAINT_NOT_MET_MSG);
                free(cfg);
                return NULL;
            }
        } else if (strcmp(variableName,"spMinimalGUI") == 0){
            if (strcmp(variableValue,"true") != 0 && strcmp(variableValue,"false") != 0){
                printConfigError(filename, line, CONSTRAINT_NOT_MET_MSG);
                free(cfg);
                return NULL;
            }
            cfg->spMinimalGUI = (strcmp(variableValue,"true")==0);
        } else if (strcmp(variableName,"spLoggerLevel") == 0){
            cfg->spLoggerLevel = atoi(variableValue);
            if (cfg->spLoggerLevel <= 0 || cfg->spLoggerLevel > 4){
                printConfigError(filename, line, CONSTRAINT_NOT_MET_MSG);
                free(cfg);
                return NULL;
            }
        } else if (strcmp(variableName,"spLoggerFilename") == 0){
            strcpy(cfg->spLoggerFilename,variableValue);
        }
    }

    fclose(fp);

    // check if any non-default parameter is missing

    if (cfg->spNumOfImages == -1){
        strcpy(param,"spNumOfImages");
    }
    if (strcmp(cfg->spImagesSuffix, "")==0){
        strcpy(param,"spImageSuffix");
    }
    if (strcmp(cfg->spImagesPrefix, "")==0){
        strcpy(param,"spImagePrefix");
    }
    if (strcmp(cfg->spImagesDirectory, "")==0){
        strcpy(param,"spImageDirectory");
    }
    if (strcmp(param,"")!=0){
        printMissingParamError(filename, line, param);
        return NULL;
    }

    return cfg;
}


// auxiliary functions for printing config error messages

void printConfigError(const char* filename, int line, char* msg){
    printf("File: %s\n", filename);
    printf("Line: %d\n", line);
    printf("Message: %s\n", msg);
}

void printMissingParamError(const char* filename, int line, char* param){
    printf("File: %s\n", filename);
    printf("Line: %d\n", line);
    printf("Message: Parameter %s is not set\n", param);
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
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return NULL;
    }
    *msg = SP_CONFIG_SUCCESS;
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
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return false;
    }
    *msg = SP_CONFIG_SUCCESS;
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
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    }
    *msg = SP_CONFIG_SUCCESS;
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
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    }
    *msg = SP_CONFIG_SUCCESS;
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
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    }
    *msg = SP_CONFIG_SUCCESS;
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
    return spConfigGetPath(imagePath,config,index,config->spImagesSuffix);
}

SP_CONFIG_MSG spConfigGetPath(char* filePath, const SPConfig config,
		int index,char* suffix){
    int nDigits = floor(log10(config->spNumOfImages))+1;
    char *strI=(char*)malloc(sizeof(char)*nDigits);
    sprintf(strI,"%d",index);

	if (filePath == NULL || config == NULL){
        return SP_CONFIG_INVALID_ARGUMENT;
    }
    if (index >= config->spNumOfImages){
        return SP_CONFIG_INDEX_OUT_OF_RANGE;
    }
    strcpy(filePath,config->spImagesDirectory);
    strcat(filePath, config->spImagesPrefix);
    strcat(filePath, strI);
    strcat(filePath, suffix);
    free(strI);
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
    strcpy(pcaPath,config->spImagesDirectory);
    strcat(pcaPath, config->spPCAFilename);
    return SP_CONFIG_SUCCESS;
}

/**
 * Frees all memory resources associate with config.
 * If config == NULL nothing is done.
 */
void spConfigDestroy(SPConfig config){
    free(config);
}

SPLIT_METHOD getSplitMethod(const SPConfig config){
    return config->spKDTreeSplitMethod;
}

int spConfigGetKNN(SPConfig config){
    return config->spKNN;
}

int spConfigGetNumOfSimilar(SPConfig config){
    return config->spNumOfSimilarImages;

}
char* spConfigGetLoggerFile(SPConfig config){
    return config->spLoggerFilename;
}
SP_LOGGER_LEVEL spConfigGetLoggerLevel(SPConfig config){
    return config->spLoggerLevel;
}

/**************************/
/**************************/
/**************************/
/**************************/
/** printing for testing **/
void spConfigPrint(SPConfig config){
    printf("image directory: %s\n",config->spImagesDirectory);
    printf("image pre: %s\n",config->spImagesPrefix);
    printf("image suf: %s\n",config->spImagesSuffix);
    printf("num images: %d\n",config->spNumOfImages);
    printf("spPCADimension: %d\n",config->spPCADimension);
    printf("spPCAFilename: %s\n",config->spPCAFilename);
    printf("spExtractionMode: %d\n",config->spExtractionMode);
    printf("spNumOfSimilarImages: %d\n",config->spNumOfSimilarImages);
    printf("spNumOfFeatures: %d\n",config->spNumOfFeatures);
    printf("spKDTreeSplitMethod: %d\n",config->spKDTreeSplitMethod);
    printf("spKNN: %d\n",config->spKNN);
    printf("spMinimalGUI: %d\n",config->spMinimalGUI);
    printf("spLoggerLevel: %d\n",config->spLoggerLevel);
    printf("spLoggerFilename: %s\n",config->spLoggerFilename);
}
/**************************/
/**************************/
/**************************/
/**************************/

