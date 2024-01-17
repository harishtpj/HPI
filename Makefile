# Makefile program to build HPI project
CPPFLAGS = /nologo /EHsc /std:c++latest
WASMFLAGS = -std=c++23 -s WASM=1 -sNO_DISABLE_EXCEPTION_CATCHING -sEXPORTED_RUNTIME_METHODS=cwrap \
	-sEXPORTED_FUNCTIONS=_runHPI -sALLOW_MEMORY_GROWTH
SRC_DIR = src
BIN_DIR = bin

SRCS = $(SRC_DIR)\hpi.cpp $(SRC_DIR)\scanner\scanner.cpp $(SRC_DIR)\scanner\token.cpp  \
		$(SRC_DIR)\parser\parser.cpp $(SRC_DIR)\interpreter\environment.cpp \
		$(SRC_DIR)\interpreter\HPIFunction.cpp $(SRC_DIR)\interpreter\interpreter.cpp \
		$(SRC_DIR)\resolve\resolver.cpp
EXE = $(BIN_DIR)\hpi.exe

all: refresh $(EXE)

run: refresh $(EXE)
	@$(MAKE) clean
	@echo "---> Running..."
	@$(EXE)

runweb: refresh $(EXE)
	@$(MAKE) clean
	@echo "---> Running web server..."
	@python -m http.server 8080

$(EXE):
	@echo "---> Compiling..."
	@$(CPP) $(CPPFLAGS) $(SRCS) /Fe$@
	@$(MAKE) clean
	@echo "---> Built program"

wasm:
	@echo "---> Compiling WASM Binary..."
	@em++ $(SRCS) $(WASMFLAGS)  -o hpi.js
	@echo "---> Built WASM binary"

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
