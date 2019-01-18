TARGET=cerrari
LIBS=-lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio
#SRC=main.cpp road.cpp math.cpp
SRC=src/*.cpp

$(TARGET): $(SRC) 
	g++ $(LIBS) -o $(TARGET) $(SRC) -std=c++17

test: $(TARGET)
	./$(TARGET)
