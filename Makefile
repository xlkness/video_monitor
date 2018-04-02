
CC = arm-linux-gcc

CFLAG = -W -O
LDFLAG = -lpthread

SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin

SRC = $(wildcard ${SRCDIR}/*.c)
OBJ = $(patsubst %.c,${OBJDIR}/%.o,$(notdir ${SRC}))

monitor:${OBJ}
	$(CC) $(CFLAG) -o $@ $^ $(LDFLAG)
	cp $@ /share

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAG) -o $@ $< -I./inc -c

.PHONY:clean
clean:
	rm $(OBJDIR)/* $(BINDIR)/*  monitor -rf
