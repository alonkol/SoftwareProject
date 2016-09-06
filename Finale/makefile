CC = gcc
CPP = g++
# put your object files here
OBJS = main.o SPImageProc.o SPPoint.o SPConfig.o SPLogger.o SPBPriorityQueue.o SPListElement.o SPFeatExtract.o SPKDTree.o SPKDArray.o SPSearch.o
# The executabel filename
EXEC = SPCBIR
INCLUDEPATH=/usr/local/lib/opencv-3.1.0/include/
LIBPATH=/usr/local/lib/opencv-3.1.0/lib/
LIBS=-lopencv_xfeatures2d -lopencv_features2d \
-lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core

CPP_COMP_FLAG = -std=c++11 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

C_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(CPP) $(OBJS) -L$(LIBPATH) $(LIBS) -o $@
main.o: main.cpp SPFeatExtract.h SPImageProc.h SPConfig.h SPLogger.h SPPoint.h SPListElement.h SPKDTree.h SPSearch.h SPKDArray.h SPBPriorityQueue.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPImageProc.o: SPImageProc.cpp SPImageProc.h SPConfig.h SPPoint.h SPLogger.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
# a rule for building a simple c souorce file
# use gcc -MM SPPoint.c to see the dependencies

SPPoint.o: SPPoint.c SPPoint.h
	$(CC) $(C_COMP_FLAG) -c $*.c

SPConfig.o: SPConfig.c SPConfig.h SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c

SPLogger.o: SPLogger.c SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c

SPBPriorityQueue.o: SPBPriorityQueue.c SPListElement.h SPBPriorityQueue.h
	$(CC) $(C_COMP_FLAG) -c $*.c

SPListElement.o: SPListElement.c SPListElement.h
	$(CC) $(C_COMP_FLAG) -c $*.c

SPFeatExtract.o: SPFeatExtract.c SPFeatExtract.h
	$(CC) $(C_COMP_FLAG) -c $*.c

SPKDArray.o: SPKDArray.c SPKDArray.h
	$(CC) $(C_COMP_FLAG) -c $*.c

SPKDTree.o: SPKDTree.c SPKDTree.h
	$(CC) $(C_COMP_FLAG) -c $*.c

SPSearch.o: SPSearch.c SPSearch.h
	$(CC) $(C_COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)