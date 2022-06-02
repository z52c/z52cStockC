CXX     = C:/MinGW/bin/g++.exe
BUILD     = build
SRC_A  = $(wildcard src/*.cpp) 

OBJ = $(addprefix $(BUILD)/, $(addsuffix .o, $(basename $(SRC_A))) )

SRC_H  = $(wildcard include/*.h)


all:main.exe
main.exe: $(OBJ)
	$(CXX) -o  $@ $? -L. -lsqlite3

$(BUILD)/%.o: %.cpp $(SRC_H) 
	$(CXX) -c -o $@ $< 
