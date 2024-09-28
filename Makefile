TARGET_EXEC := app

BUILD_DIR := ./build
SRC_DIRS := ./test ./src


SRCS := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

DEPS := $(OBJS:.o=.d)
CC := clang

INC_DIRS := ./include

INC_FLAGS := $(addprefix -I, $(INC_DIRS))

CFLAGS := $(INC_FLAGS) -g -MMD -MP

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -g -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -fPIE $< -o $@

.PHONY: clean test static mem validate

clean:
	rm -r $(BUILD_DIR)

test: $(BUILD_DIR)/$(TARGET_EXEC)
	compiledb -n make
	./$<

static:
	clang-tidy $(SRCS)

mem: $(BUILD_DIR)/$(TARGET_EXEC)
	valgrind $(BUILD_DIR)/$(TARGET_EXEC)

validate: mem static
