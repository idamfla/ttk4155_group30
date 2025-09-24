# List all source files to be compiled; separate with space
SOURCE_DIR := source
SOURCE_FILES := $(shell find $(SOURCE_DIR) -name '*.c' \
  $(foreach f,$(EXCLUDE_FILES),! -name $(f)) \
  $(foreach d,$(EXCLUDE_DIRS),! -path '$(d)/*'))

# Set this flag to "yes" (no quotes) to use JTAG; otherwise ISP (SPI) is used
PROGRAM_WITH_JTAG := yes

# Feel free to ignore anything below this line
PROGRAMMER := atmelice_isp
ifeq ($(PROGRAM_WITH_JTAG), yes)
	PROGRAMMER := atmelice
endif

BUILD_DIR := build
TARGET_CPU := atmega162
TARGET_DEVICE := m162

CC := avr-gcc
CFLAGS := -O -std=c11 -mmcu=$(TARGET_CPU) -ggdb -Wall -Wextra -Wpedantic
LDFLAGS := -Wl,-Map,$(BUILD_DIR)/output.map

OBJECT_FILES = $(patsubst $(SOURCE_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCE_FILES))

.DEFAULT_GOAL := $(BUILD_DIR)/main.hex

DIRS := $(sort $(dir $(OBJECT_FILES)))

# Make sure BUILD_DIR and subdirectories exist
$(DIRS):
	mkdir -p $@

# Compile rule
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c Makefile | $(DIRS)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.hex: $(OBJECT_FILES) | $(DIRS)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $(BUILD_DIR)/a.out
	avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/a.out $(BUILD_DIR)/main.hex

.PHONY: flash
flash: $(BUILD_DIR)/main.hex
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)/main.hex:i

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: erase
erase:
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -e
	
.PHONY: debug
debug:
	if pgrep avarice; then pkill avarice; fi
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)/main.hex:i
	x-terminal-emulator -e avarice --edbg --ignore-intr :4242
	sleep 2
	avr-gdb -tui -iex "target remote localhost:4242" $(BUILD_DIR)/a.out
	killall -s 9 avarice	

.PHONY: docs
docs:
	mkdir -p build/doxygen
	doxygen Doxyfile
	echo ""
	echo "Open build/doxygen/html/index.html to view the documentation"
