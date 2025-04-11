#!/bin/bash

CC=gcc
CFLAGS="-Wall -Wextra -g"

rm -f *.o pipe_program

echo "compiling child.c..."
$CC $CFLAGS -c child.c -o child.o
if [ $? -ne 0 ]; then
    exit 1
fi

echo "compiling parent.c..."
$CC $CFLAGS -c parent.c -o parent.o
if [ $? -ne 0 ]; then
    exit 1
fi

echo "compiling main.c..."
$CC $CFLAGS -c main.c -o main.o
if [ $? -ne 0 ]; then
    exit 1
fi

echo "linking objects..."
$CC $CFLAGS -o pipe_program main.o parent.o child.o
if [ $? -ne 0 ]; then
    exit 1
fi

echo "successfully compiled and linked all files"
