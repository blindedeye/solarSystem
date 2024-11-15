SRCPATH=src
BUILDPATH=build
EXE=solar.exe
SRC=${SRCPATH}/main.cpp ${SRCPATH}/planet.cpp ${SRCPATH}/orbit.cpp
LIBS=-lGL -lGLU -lglut -lGLEW

${BUILDPATH}/${EXE}: ${SRC}
	g++ ${SRC} -o $@ ${LIBS}

clean:
	rm ${BUILDPATH}/${EXE}

run: ${BUILDPATH}/${EXE}
	./${BUILDPATH}/${EXE}