#include "SPLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//File open mode
#define SP_LOGGER_OPEN_MODE "w"

// Global variable holding the logger
SPLogger logger = NULL;

struct sp_logger_t {
	FILE* outputChannel; //The logger file
	bool isStdOut; //Indicates if the logger is stdout
	SP_LOGGER_LEVEL level; //Indicates the level
};

SP_LOGGER_MSG spLoggerCreate(const char* filename, SP_LOGGER_LEVEL level) {
	if (logger != NULL) { //Already defined
		return SP_LOGGER_DEFINED;
	}
	logger = (SPLogger) malloc(sizeof(*logger));
	if (logger == NULL) { //Allocation failure
		return SP_LOGGER_OUT_OF_MEMORY;
	}
	logger->level = level; //Set the level of the logger
	if (filename == NULL || strcmp(filename,"stdout")==0) { //In case the filename is not set use stdout
		logger->outputChannel = stdout;
		logger->isStdOut = true;
	} else { //Otherwise open the file in write mode
		logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
		if (logger->outputChannel == NULL) { //Open failed
			free(logger);
			logger = NULL;
			return SP_LOGGER_CANNOT_OPEN_FILE;
		}
		logger->isStdOut = false;
	}
	return SP_LOGGER_SUCCESS;
}

void spLoggerDestroy() {
	if (!logger) {
		return;
	}
	if (!logger->isStdOut) {//Close file only if not stdout
		fclose(logger->outputChannel);
	}
	free(logger);//free allocation
	logger = NULL;
}

SP_LOGGER_MSG spLoggerPrintError(const char* msg, const char* file,
		const char* function, const int line){
	int success;


    if (!msg || !file || !function || line < 0){
        return SP_LOGGER_INVAlID_ARGUMENT;
    }

    if (!logger || !logger->outputChannel){
        return SP_LOGGER_UNDIFINED;
    }

    success = fprintf(logger->outputChannel, \
                          "---ERROR---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n", \
                          file, function, line, msg);

    if (success < 0){
        return SP_LOGGER_WRITE_FAIL;
    }

    return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintWarning(const char* msg, const char* file,
		const char* function, const int line){
    int success;

    if (!msg || !file || !function || line < 0){
        return SP_LOGGER_INVAlID_ARGUMENT;
    }

    if (!logger || !logger->outputChannel){
        return SP_LOGGER_UNDIFINED;
    }

    if (logger->level != SP_LOGGER_ERROR_LEVEL){
        success = fprintf(logger->outputChannel,
            "---WARNING---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",
             file, function, line, msg);

        if (success < 0){
            return SP_LOGGER_WRITE_FAIL;
        }
    }

    return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintInfo(const char* msg){
    int success;

    if (!msg){
        return SP_LOGGER_INVAlID_ARGUMENT;
    }

    if (!logger || !logger->outputChannel){
        return SP_LOGGER_UNDIFINED;
    }

    if (logger->level == SP_LOGGER_INFO_WARNING_ERROR_LEVEL ||
        logger->level == SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL){

        success = fprintf(logger->outputChannel,
            "---INFO---\n- message: %s\n",
             msg);

        if (success < 0){
            return SP_LOGGER_WRITE_FAIL;
        }
    }

    return SP_LOGGER_SUCCESS;
}


SP_LOGGER_MSG spLoggerPrintDebug(const char* msg, const char* file,
		const char* function, const int line){
    int success;

    if (!msg || !file || !function || line < 0){
        return SP_LOGGER_INVAlID_ARGUMENT;
    }

    if (!logger || !logger->outputChannel){
        return SP_LOGGER_UNDIFINED;
    }

    if (logger->level == SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL){
        success = fprintf(logger->outputChannel,
            "---DEBUG---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",
             file, function, line, msg);

        if (success < 0){
            return SP_LOGGER_WRITE_FAIL;
        }
    }

    return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintMsg(const char* msg){
    int success;

    if (!msg){
        return SP_LOGGER_INVAlID_ARGUMENT;
    }

    if (!logger || !logger->outputChannel){
        return SP_LOGGER_UNDIFINED;
    }

    if (logger->level == SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL){
        success = fprintf(logger->outputChannel, "%s\n", msg);

        if (success < 0){
            return SP_LOGGER_WRITE_FAIL;
        }
    }

    return SP_LOGGER_SUCCESS;
}

