OBJ_NAME=dbproxy
EXE=lib$(OBJ_NAME).so
CC=g++ -std=c++11

#编译选项
FLAG=-Wall -fPIC -MMD -shared -g -O2 -o

#包含的头文件
INCLUDES=-I/usr/include/libxml2/ -I$(XR_PATH)/include -I$(XR_SERVER_PATH)/include -I../include

LINK=-lxml2 -lxr -Wl,-rpath,$(XR_PATH)

LIBS=-L/usr/lib64 -L/usr/lib -L$(XR_PATH)

#编译的代码文件
LOCAL_SRCS =$(wildcard ./*.cpp)

$(EXE):
	rm -fr $(EXE)
	rm -f lib$(OBJ_NAME).d
	$(CC) -ldl -lrt $(LOCAL_SRCS) $(INCLUDES) $(LIBS) $(LINK) $(FLAG) $@ 
	rm -f lib$(OBJ_NAME).d
	cp $(EXE) $(XR_SERVER_PATH)
	rm -f $(EXE)
