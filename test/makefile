# Copyright (c) 2016 National Central University, Studio215 ALL RIGHTS RESERVED
#
# Makefile for Timer_Pool
#
# ver 0.0.2
#
# Author: Hsien-Wen, Hu
#
# Date: 2016/9/7
#

CC = gcc

EXE_NAME = test_timer_pool

CFLAGS += -Wall
CFLAGS += -g

INC =
INC += -I../

LIB =
LIB += -L/usr/local/bin

SRC = 
SRC += ../Timer_Pool
SRC += ../funclog
SRC += ./test_timer_pool

LDFLAG = 
LDFLAG += -lev

SRC_FILES = ${addsuffix .c, ${SRC}}
OBJ_FILES = ${addsuffix .o, ${SRC}}


	
all: ${EXE_NAME} ${TEST}

${EXE_NAME}: ${OBJ_FILES}
	${CC} ${CFLAGS} ${INC} ${LIB} ${MAIN} ${OBJ_FILES} -o $@ ${LDFLAG}
${OBJ_FILES} : %.o : %.c
	${CC} ${CFLAGS} -c ${INC} $< -o $@ ${LDFLAG}
clean:
	rm -rf ${OBJ_FILES} ${EXE_NAME} ${TEST}
