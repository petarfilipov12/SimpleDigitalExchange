CC = g++
CFLAGS = -std=c++11 -g #-D JSON_DIAGNOSTICS=1 #gdb -g3 -O0
CFLAGS_END = -lssl -lcrypto

PROGRAM_NAME = MatchingEngine_exec

BUILD_DIR = build
STABLE_DIR = stable
CODE_DIR = code

INCLUDES = $(sort $(shell find $(CODE_DIR) -type f -name '*.h' -printf "%h\n"))
INC_FLAGS = $(patsubst %, -I%, $(INCLUDES))

CODE_SOURCES = $(shell find $(CODE_DIR) -type f -name '*.cpp')
OBJS = $(subst $(CODE_DIR), $(BUILD_DIR), $(patsubst %.cpp, %.o, $(CODE_SOURCES)))
DEPENDS = $(patsubst %.cpp, %.o, $(OBJS))


$(PROGRAM_NAME): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(PROGRAM_NAME) $^ $(CFLAGS_END)

$(BUILD_DIR):
	mkdir -pv $(BUILD_DIR)

-include $(patsubst %.o, %.d, $(OBJS))

$(BUILD_DIR)/%.o: $(CODE_DIR)/%.cpp
	mkdir -pv $(dir $@)
	$(CC) $(CFLAGS) $(INC_FLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/%.o: $(CORE_DIR)/%.c
	mkdir -pv $(dir $@)
	$(CC) $(CFLAGS) $(INC_FLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean $(PROGRAM_NAME)

run:
	./$(BUILD_DIR)/$(PROGRAM_NAME)

stable_run:
	./$(STABLE_DIR)/$(PROGRAM_NAME)

print:
	echo $(INCLUDES)
	echo $(OBJS)
