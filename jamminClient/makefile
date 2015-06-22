CC=gcc
FMOD_LIBPATH=lib/FMOD
INCLUDEPATH=-I include/
LIBPATH=
OBJPATH=obj
BINPATH=bin
SRCPATH=src
TARGET=$(BINPATH)/jamminClient
TEST_COMMAND_HANDLER=$(BINPATH)/testCommandHandler
TEST_CONNECTION=$(BINPATH)/testConnection
TEST_OUTPUT=$(BINPATH)/testOutput
TEST_TOOLS=$(BINPATH)/testTools
TEST_SOUND_CAPTURE=$(BINPATH)/testSoundCapture
TEST_SOUND_PLAYBACK=$(BINPATH)/testSoundPlayback
BINARIES=$(TEST_TOOLS) $(TEST_COMMAND_HANDLER) $(TEST_CONNECTION) $(TEST_OUTPUT) $(TEST_SOUND_CAPTURE) $(TEST_SOUND_PLAYBACK) $(TARGET)
OBJECT= $(OBJPATH)/output.o $(OBJPATH)/connection.o $(OBJPATH)/sound.o $(OBJPATH)/tools.o $(OBJPATH)/input.o $(OBJPATH)/command_handler.o $(OBJPATH)/core.o $(OBJPATH)/session.o
CFLAGS=$(INCLUDEPATH) -Wall
CLINKS=-lasound -lpthread

all: main testCommandHandler testConnection testCore testInput testOutput testTools testSoundCapture testSoundPlayback

main: $(TARGET)

testCommandHandler: $(TEST_COMMAND_HANDLER)

testConnection: $(TEST_CONNECTION)

testCore: $(TEST_CORE)

testInput: $(TEST_INPUT)

testOutput: $(TEST_OUTPUT)

testTools: $(TEST_TOOLS)

testSoundCapture: $(TEST_SOUND_CAPTURE)
 
testSoundPlayback: $(TEST_SOUND_PLAYBACK)

$(TARGET): $(OBJECT) $(OBJPATH)/main.o 
	$(CC) -o $@ $^ $(CLINKS)

$(TEST_COMMAND_HANDLER): $(OBJECT) $(OBJPATH)/testCommandHandler.o
	$(CC) -o $@ $^ $(CLINKS)

$(TEST_CONNECTION): $(OBJECT) $(OBJPATH)/testConnection.o
	$(CC) -o $@ $^ $(CLINKS)

$(TEST_OUTPUT): $(OBJECT) $(OBJPATH)/testOutput.o
	$(CC) -o $@ $^ $(CLINKS)

$(TEST_TOOLS): $(OBJECT) $(OBJPATH)/testTools.o
	$(CC) -o $@ $^ $(CLINKS)
		
$(TEST_SOUND_CAPTURE): $(OBJECT) $(OBJPATH)/testSoundCapture.o
	$(CC) -o $@ $^ $(CLINKS)

$(TEST_SOUND_PLAYBACK): $(OBJECT) $(OBJPATH)/testSoundPlayback.o
	$(CC) -o $@ $^ $(CLINKS)

$(OBJPATH)/%.o: $(SRCPATH)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)
	
clean:
	rm -rf $(OBJECT) $(BINARIES) $(OBJPATH)/main.o $(OBJPATH)/testCommandHandler.o $(OBJPATH)/testConnection.o  $(OBJPATH)/testOutput.o $(OBJPATH)/testTools.o $(OBJPATH)/testSoundPlayback.o $(OBJPATH)/testSoundCapture.o
