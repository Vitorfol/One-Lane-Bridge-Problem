CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -pthread
LDFLAGS  := -pthread

SRC_DIR  := src
OBJ_DIR  := build
TARGET   := ponte

SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS     := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	@if [ -z "$(OBJS)" ]; then \
		echo "Nenhum arquivo .cpp encontrado em $(SRC_DIR)/."; \
		exit 1; \
	fi
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
