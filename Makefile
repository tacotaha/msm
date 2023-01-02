CC=gcc
CFLAGS=-O3 -Wall -Wextra -Werror -D__ADX__ -march=native -mtune=native -fopenmp #-fno-omit-frame-pointer -g
LDFLAGS=-lgomp
BLST_ROOT=$(realpath ./blst)
OBJ=msm.o libblst.a
EXEC=msm

all: $(EXEC)

msm: libblst.a msm.o main.o
	$(CC) $(LDFLAGS) $(OBJ) main.o -o msm

main.o: main.c
	$(CC) $(CFLAGS) -I$(BLST_ROOT) -c main.c -o main.o

msm.o: msm.c
	$(CC) $(CFLAGS) -I$(BLST_ROOT) -c msm.c -o msm.o

libblst.a: $(BLST_ROOT)
	$(BLST_ROOT)/build.sh CC=$(CC)

clean:
	rm -rf $(OBJ) $(EXEC) *.o *.a *~

format:
	find . -maxdepth 1 -regextype posix-extended -regex ".*\.(c|h)" | xargs indent -par -br -brf -brs -kr -ci2 -cli2 -i2 -l120 -nut
