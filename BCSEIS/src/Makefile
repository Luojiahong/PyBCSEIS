CC	= g++
FC	= g77
CFLAGS	= -g -DDEBUG -Wall
FFLAGS	= -Wall

OBJS1	= bcseis.o \
	  sacHeader.o sacSeisgram.o distaz.o readSacData.o \
	  mathFuncs.o fourier.o complex.o \
	  stas.o evData.o seisData.o tmDelay.o \
	  getMaxShiftLag.o calcTmDelays.o calcCCTmDelay.o calcSubTmDelay.o calcBSTmDelay.o bsTaper.o \
	  calcTravTm.o ttime.o direct1.o refract.o vmodel.o tiddid.o

OBJS2	= getsachdr.o \
	  sacHeader.o sacSeisgram.o distaz.o

OBJS3	= setsachdr.o \
	  sacHeader.o sacSeisgram.o distaz.o

BIN	= ../linuxbin
PROG	= bcseis getsachdr setsachdr

.c.o:
	${CC} $(CFLAGS) -c $<

.f.o:
	${FC} $(FFLAGS) -c $<

all:	${PROG}

bcseis:	${OBJS1}
	${CC} ${CFLAGS} -lm -o $@ ${OBJS1}
	mv $@ ${BIN}

getsachdr:	${OBJS2}
	${CC} ${CFLAGS} -lm -o $@ ${OBJS2}
	mv $@ ${BIN}

setsachdr:	${OBJS3}
	${CC} ${CFLAGS} -lm -o $@ ${OBJS3}
	mv $@ ${BIN}

clean:
	rm *.o
