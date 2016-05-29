C++ = g++
OBJS = main.o main_aux.o sp_image_proc_util.o
EXEC = ex2
INCLUDEPATH=/usr/local/lib/opencv-3.1.0/include/
LIBPATH=/usr/local/lib/opencv-3.1.0/lib/
LIBS=-lopencv_xfeatures2d -lopencv_features2d \
-lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core


COMP_FLAG = -std=c++11 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(C++) $(OBJS) -L$(LIBPATH) $(LIBS) -o $@
main.o: main.cpp main_aux.h sp_image_proc_util.h
	$(C++) $(COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
main_aux.o: main_aux.h main_aux.cpp
	$(C++) $(COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
sp_image_proc_util.o: sp_image_proc_util.h sp_image_proc_util.cpp
	$(C++) $(COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp


clean:
	rm -f $(OBJS) $(EXEC)
