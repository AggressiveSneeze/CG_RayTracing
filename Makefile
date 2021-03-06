#################################################################
#
#				Warning!!!!
#				============
#	
# this make file compiles all the sources with implicit rules
# therefore in order to make sure all the changes were made 
# run it with "make clean all"
#
##################################################################

EXEC = 

# list of all the sources files
SRCS	=  	ex5.cpp		\
		bimage.cpp \
		scene.cpp \
		camera.cpp \
		MyMeshObject.cpp \
		polygon.cpp \
		triangle.cpp \
 	object.cpp \
 	sphere.cpp\

# compiler 
CC	= g++

#OpenMesh definitions
#CG_HOME = /usr/local/Cellar/open-mesh/4.1/
#OM_DIR = $(CG_HOME)/OpenMesh
#OM_LIBS = -L$(OM_DIR)/lib -lOpenMeshCore
#OM_INCLUDE=  -I$(OM_DIR)/include

#INCLUDE = -I/usr/include -I/usr/X11R6/include -I/cs/course/current/cg/include $(OM_INCLUDE)

#OpenMesh definitions
CG_HOME = /usr/local/Cellar/open-mesh/4.1
OM_DIR = $(CG_HOME)/OpenMesh
OM_LIBS = -L$(OM_DIR)/lib -lOpenMeshCore
OM_INCLUDE=  -I$(OM_DIR)/include

INCLUDE =-I/usr/local/include -I/usr/X11R6/include -I/usr/local/Cellar/glew/1.12.0/include $(OM_INCLUDE)

OBJS = $(subst .cpp,.o,$(SRCS)) 		#substituting .cc -> .o

# in order to run in debug mode use the command "make DBG=1"
ifdef DBG

	CFLAGS = 	\
	    -std=c++11\
	     -O3\
		-Wall 	\
		-Werror \
		-DDEBUG	\
		-O0		\
		-g3		\
		-Wno-deprecated
					
						
else

	CFLAGS = 		\
		-Wall		\
		-DNDEBUG    \
		-O2
				 
endif


# linker
LD	= $(CC)
LDFLAGS	=


# the name of the lib to create 
APP = ex5

# each lib should add with -l flag
#LIBS =  -L/usr/lib -lm -lGLEW -lglut -lGL -lGLU -ldl -L/usr/X11R6/lib $(OM_LIBS)  -L/cs/course/2013/cg/lib -lbimage
LIBS = -L/usr/local/lib -lm -lGLEW -L/usr/lib -framework GLUT -framework OpenGL -ldl -L/usr/X11R6/lib $(OM_LIBS)

# rule to change the suffix of the sources files
OBJS	= ${SRCS:.cpp=.o}

# clear out all suffixes
.SUFFIXES:
# list only those we use
.SUFFIXES: .o .cpp

# define a suffix rule for .c -> .o
.cpp.o :
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

##################################
# 	default target 		##
##################################
all : $(APP)

$(APP) : $(OBJS)
	$(LD)  $(OBJS) $(LIBS) -o $(APP)

$(OBJS) : $(SRCS)


tar:
	tar cvf $(EXEC).tar Makefile $(SRCS) $(HDRS) README
	

.PHONY:
clean :
	rm -f $(OBJS) $(EXEC) *~ $(APP)
