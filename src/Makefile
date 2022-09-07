CC=g++
CFLAGS=-framework ApplicationServices -framework Carbon -framework Cocoa -std=c++20 -oFast
SOURCES=main.cpp KeyboardInputMacOS.cpp Keys.cpp ProcessNameFetcherMacOS.mm
EXECUTABLE=ExtendedKeyboard
all: $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) -o $(EXECUTABLE)