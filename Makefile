OBJS = GPT.cpp

CC = g++

INCLUDE_PATHS = -ICurl\include -IOpenSSL\include

LIBRARY_PATHS = -LCurl\lib

COMPILER_FLAGS = -w

LINKER_FLAGS = -lws2_32 -llibssl -llibcrypto -llibcurl

OBJ_NAME = GPT

all : $(OBJS)
	$(CC) -g $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
