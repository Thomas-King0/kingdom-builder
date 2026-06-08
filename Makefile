CXX= g++
CXXFLAGS= -Wall -Wextra

OBJS=main.o land.o major.o minor.o mine.o pasture.o quarry.o sawmill.o garden.o castle.o fort.o village.o road.o caravan.o helpful.o barbarian.o menu.o objectWriter.o

TARGET=kingdom-builder_alpha

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
