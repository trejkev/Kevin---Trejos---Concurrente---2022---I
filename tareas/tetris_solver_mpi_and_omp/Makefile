CXX=mpic++
FLAGS=$(strip -Wall -Wextra -std=gnu++2a -lstdc++)
# FLAGS=$(strip -Wall -Wextra -lstdc++)
# FLAGS=$(strip -Wall -Wextra -std=gnu++2a)
LIBS=-pthread
LINTFILTERS=$\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-runtime/references

BIN_DIR=bin
OBJ_DIR=build
SRC_DIR=src

DIRS=$(shell find $(SRC_DIR) -type d)
APPNAME=tetris_solver_mpi_and_omp
HEADERS=$(wildcard $(DIRS:%=%/*.hpp))
SOURCES=$(wildcard $(DIRS:%=%/*.cpp))
OBJECTS=$(SOURCES:src/%.cpp=build/%.o)
INCLUDE=$(DIRS:%=-I%)
IGNORES=$(BIN_DIR) $(OBJ_DIR)
EXEFILE=$(BIN_DIR)/$(APPNAME)
APPARGS=8080 25

default: debug

debug: FLAGS += -g -fopenmp
debug: $(EXEFILE)
release: FLAGS += -O3 -DNDEBUG -fopenmp
release: $(EXEFILE)
asan: FLAGS += -fsanitize=address -fno-omit-frame-pointer -fopenmp
asan: debug
msan: FLAGS += -fsanitize=memory -fopenmp
msan: CXX = clang++
msan: debug
tsan: FLAGS += -fsanitize=thread -fopenmp
tsan: debug
ubsan: FLAGS += -fsanitize=undefined -fopenmp
ubsan: debug

.SECONDEXPANSION:

$(EXEFILE): $(OBJECTS) | $$(@D)/.
	$(CXX) $(FLAGS) $(INCLUDE) $^ -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $$(@D)/.
	$(CXX) -c $(FLAGS) $(INCLUDE) $< -o $@ $(LIBS)

.PRECIOUS: %/.
%/.:
	mkdir -p $(dir $@)

all: bin/$(APPNAME) lint memcheck helgrind

.PHONY: lint
lint:
	cpplint --filter=$(LINTFILTERS) $(HEADERS) $(SOURCES)

.PHONY: memcheck
memcheck:
	valgrind --tool=memcheck bin/$(APPNAME) $(APPARGS)

.PHONY: helgrind
helgrind:
	valgrind --quiet --tool=helgrind bin/$(APPNAME) $(APPARGS)

.PHONY: gitignore
gitignore:
	echo $(IGNORES) > .gitignore

.PHONY: clean
clean:
	rm -rf $(IGNORES)