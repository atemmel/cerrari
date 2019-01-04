TARGET=cerrari
LIBS=-lsfml-system -lsfml-graphics -lsfml-window
#SRC=main.cpp road.cpp math.cpp
SRC=*.cpp

$(TARGET): $(SRC) 
	g++ $(LIBS) -o $(TARGET) $(SRC)

test: $(TARGET)
	./$(TARGET)
