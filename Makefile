OBJS = GPT.cpp

CC = g++

INCLUDE_PATHS = -ICurl\include -IOpenSSL\include

LIBRARY_PATHS = -LCurl\lib

LINKER_FLAGS = -lws2_32 -llibssl -llibcrypto -llibcurl

OBJ_NAME = GPT

all : $(OBJS)
	$(CC) -g $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME)
