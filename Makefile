# Makefile program to build HPI project
CPPFLAGS = /nologo /EHsc /std:c++latest
SRC_DIR = src
BIN_DIR = bin

SRCS = $(SRC_DIR)\*.cpp $(SRC_DIR)\scanner\*.cpp
EXE = $(BIN_DIR)\hpi.exe

all: $(EXE) clean

run: $(EXE) clean
	@echo "---> Running..."
	@$(EXE)

$(EXE): $(OBJS)
	@echo "---> Compiling..."
	@$(CPP) $(CPPFLAGS) $(SRCS) /Fe$@
	@echo "---> Built program"

clean:
	@IF EXIST "*.obj" del /s /q *.obj

refresh: clean
	@IF EXIST $(BIN_DIR)\*.exe del /s /q $(BIN_DIR)\*.exe
	@echo "---> Refreshed directory"
