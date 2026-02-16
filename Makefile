BUILD_DIR := _build
BINARY    := hello
BIN_PATH  := $(BUILD_DIR)/$(BINARY)

DEBUGGER  := gdb --quiet
CHECKER   := cppcheck --force --enable=warning --enable=portability
FORMATTER := clang-format -i

BLUE   := \033[34m
GREEN  := \033[32m
YELLOW := \033[33m
RESET  := \033[0m

.PHONY: all configure build clean run size sstrip debug check format help

all: build

$(BUILD_DIR): CMakeLists.txt
	@echo "$(BLUE)Running CMake:$(RESET)"
	cmake -B $(BUILD_DIR) -S .

configure: $(BUILD_DIR)


MAKEFLAGS := --no-print-directory 
export MAKEFLAGS

FORCE:

$(BIN_PATH): $(BUILD_DIR) FORCE
	@echo "$(BLUE)Building the project:$(RESET)"
	cmake --build $(BUILD_DIR) -j $(shell nproc)

build: $(BIN_PATH)

run: build
	@echo "$(GREEN)Executing the binary:$(RESET)"
	@./$(BIN_PATH)

clean:
	@echo "$(YELLOW)Cleaning the build directory: $(BUILD_DIR)$(RESET)"
	rm -rf $(BUILD_DIR)

debug: build
	@echo "$(YELLOW)Launching Debugger:$(RESET)"
	$(DEBUGGER) $(BIN_PATH)

size: build
	@echo "-------------------------------------------"
	@echo "$(YELLOW)ELF sections headers:$(RESET)"
	@readelf -S $(BIN_PATH)
	@echo "-------------------------------------------"
	@echo "$(YELLOW) Runtime sections $(RESET)"
	@size -A $(BIN_PATH)
	@echo "-------------------------------------------"
	@echo "$(YELLOW)Total binary size:$(RESET)" `stat -c %s $(BIN_PATH)` "bytes"
	@echo "-------------------------------------------"


CPP_HPP_FILES := find . -path ./$(BUILD_DIR) -prune -o \( -iname '*.cpp' -o -iname '*.hpp' \) -print

check:
	@echo "$(BLUE)Running Static Analysis:$(RESET)"
	$(CPP_HPP_FILES) | $(CHECKER) --file-list=-

format:
	@echo "$(YELLOW)Formatting source code...$(RESET)"
	$(CPP_HPP_FILES) |xargs $(FORMATTER)


sstrip: build
	@echo "$(BLUE)Remove all non essintail headers$(RESET)"
	sstrip $(BIN_PATH)
	@echo "-------------------------------------------"
	@echo "$(YELLOW)Final binary size:$(RESET)" `stat -c %s $(BIN_PATH)` "bytes"
	@echo "-------------------------------------------"

compcheck: clean
	@echo "$(BLUE) Checking compilation compatibility$(BLUE)"
	cmake -B $(BUILD_DIR) -S .\
		-DCMAKE_SYSTEM_NAME=Generic\
		-DCMAKE_SYSTEM_PROCESSOR=x86_64 \
		-DFORCE_NO_LTO=0
	$(MAKE) build
	$(MAKE) size
	@echo "$(BLUE) RUN 'make clean' TO RESET $(BLUE)"

help:
	@echo "make build       - Compile the project"
	@echo "make run         - Build and execute"
	@echo "make clean       - Delete buid artifacts"
	@echo "make size        - Binary size statistics"
	@echo "make sstrip      - Run sstrip for aggressive ELF size reduction"
	@echo "make debug       - Open in debugger ($(DEBUGGER))"
	@echo "make check       - Run static analysis ($(CHECKER))"
	@echo "make format      - Auto-format cpp/hpp files ($(FORMATTER))"
	@echo "make compcheck   - Check compliation for generic system"
