###
### Makefile for Zap Project
###
### Author:  Harry Lynch

CXX = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow -fPIC
LDFLAGS = -g3 -no-pie

zap: main.o HuffmanCoder.o HuffmanTreeNode.o ZapUtil.o
	${CXX} ${LDFLAGS} -o zap main.o HuffmanCoder.o HuffmanTreeNode.o ZapUtil.o
main: main.cpp HuffmanCoder.h
	${CXX} ${CXXFLAGS} -c main.cpp
HuffmanCoder: HuffmanCoder.cpp HuffmanCoder.h
	${CXX} ${CXXFLAGS} -c HuffmanCoder.cpp

clean:
	@find . -type f \( \
		-name '*.o' ! -name 'HuffmanTreeNode.o' ! -name 'BinaryIO.o' \
		! -name 'ZapUtil.o' \) -exec rm -f {} \;
	@rm -f *~ a.out

