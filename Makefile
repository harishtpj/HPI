# Makefile program to build HPI project
CPPFLAGS = /nologo /EHsc /std:c++latest
SRC_DIR = src
BIN_DIR = bin

SRCS = $(SRC_DIR)\*.cpp $(SRC_DIR)\scanner\*.cpp $(SRC_DIR)\parser\*.cpp  \
		$(SRC_DIR)\interpreter\*.cpp $(SRC_DIR)\resolve\*.cpp
EXE = $(BIN_DIR)\hpi.exe

all: refresh $(EXE)

run: refresh $(EXE)
	@$(MAKE) clean
	@echo "---> Running..."
	@$(EXE)

$(EXE): $(OBJS)
	@echo "---> Compiling..."
	@$(CPP) $(CPPFLAGS) $(SRCS) /Fe$@
	@$(MAKE) clean
	@echo "---> Built program"

genast:
	@echo "---> Generating AST Generator"
	@$(CPP) $(CPPFLAGS) $(SRC_DIR)\tools\astGen.cpp
	@echo "---> Built AST Generator"
	@echo "---> Running AST Generator"
	@astGen $(SRC_DIR)
	@astyle $(SRC_DIR)\Expr.hpp $(SRC_DIR)\Stmt.hpp
	@del /q $(SRC_DIR)\*.orig
	@echo "---> Built AST"
	@del /q astGen.*

clean:
	@IF EXIST "*.obj" del /s /q *.obj

refresh: clean
	@IF EXIST $(BIN_DIR)\*.exe del /s /q $(BIN_DIR)\*.exe
	@echo "---> Refreshed directory"
