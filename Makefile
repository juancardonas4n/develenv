
BINDIR=bin
.phony: all clean ${BINDIR}

all:
SRCDIR=src
OBJECTS=${SRCDIR}/develenv.o
CXXFLAGS=-Wall

all: ${BINDIR}/develenv

${BINDIR}/develenv: ${OBJECTS} ${BINDIR}
	g++ -o $@ ${OBJECTS}

${BINDIR}:
	mkdir bin

${SRCDIR}/develenv.o: ${SRCDIR}/develenv.cpp

install: ${BINDIR}/develenv
	install $^ ${HOME}/bin

clean:
	rm -R -f ${SRCDIR}/*.o ${SRCDIR}/*.h~ ${SRCDIR}/*.cpp~
	rm -R ${BINDIR}
