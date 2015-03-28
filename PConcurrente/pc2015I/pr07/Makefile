DIROBJ := obj/
DIREXE := exec/
DIRSRC := src/

CFLAGS := -c -Wall
LDLIBS := -lrt
CC := gcc

all : dirs manager filosofo

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

manager: $(DIROBJ)manager.o
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

filosofo: $(DIROBJ)filosofo.o
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

$(DIROBJ) %.o: $(DIRSRC) %.c
	$(CC) $(CFLAGS) $^ -o $@

clean :   
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRSRC) *~