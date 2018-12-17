TARGET=cerrari
LIBS=-lsfml-system -lsfml-graphics -lsfml-window

$(TARGET): main.cpp
	g++ $(LIBS) -o $(TARGET) main.cpp

test: $(TARGET)
	./$(TARGET)
