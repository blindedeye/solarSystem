SRCPATH=src
BUILDPATH=build
EXE=triangle.exe
SRC=${SRCPATH}/triangle.cpp
LIBS=-lGL -lGLU -lglut -lGLEW

${BUILDPATH}/${EXE}: ${SRC}
	g++ ${SRC} -o $@ ${LIBS}

clean:
	rm ${BUILDPATH}/${EXE}

run: ${BUILDPATH}/${EXE}
	./${BUILDPATH}/${EXE}