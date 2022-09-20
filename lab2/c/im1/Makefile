CC        = gcc
SRCFILES  = common.c backup.c
OBJFILES  = common.o backup.o
TARGET    = main

build:
	$(CC) -c $(SRCFILES)
	$(CC) $(OBJFILES) -o $(TARGET)

clean:
	rm $(TARGET) *.o