ifeq ($(OS),Windows_NT)
SHELL = cmd.exe
MKDIR = md "$(@D)" 2> nul || :
RMDIR = rmdir /S /Q
CLEAN = $(RMDIR) $(BUILD) 2> nul || :
else
MKDIR = mkdir -p "$(@D)" 2> /dev/null || :
RMDIR = rm -rf
CLEAN = $(RMDIR) $(BUILD) 2> /dev/null || :
endif

CXX     = C:/MinGW/bin/g++.exe
BUILD     = build
SRC_A  = $(wildcard src/*.cpp) 

OBJ = $(addprefix $(BUILD)/, $(addsuffix .o, $(basename $(SRC_A))) )

SRC_H  = $(wildcard include/*.h)


all:main.exe
main.exe: $(OBJ)
	$(MKDIR)
	$(CXX) -o  $@ $? -L. -lsqlite3

$(BUILD)/%.o: %.cpp $(SRC_H) 
	$(MKDIR)
	$(CXX) -c -o $@ $< 

clean:
	$(CLEAN)
