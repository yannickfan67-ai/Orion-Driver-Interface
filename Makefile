CC ?= cc
CXX ?= c++
CLANG ?= clang
CFLAGS := -std=c11 -Wall -Wextra -Werror -Iinclude
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -Iinclude
BUILD := build

.PHONY: check check-native check-cross clean
check: check-native check-cross

$(BUILD):
	mkdir -p $(BUILD)

check-native: | $(BUILD)
	$(CC) $(CFLAGS) tests/abi_smoke.c -o $(BUILD)/abi-smoke
	./$(BUILD)/abi-smoke
	$(CXX) $(CXXFLAGS) -c tests/header_smoke.cpp -o $(BUILD)/header-smoke.o

check-cross: | $(BUILD)
	$(CLANG) -target i386-none-elf -ffreestanding $(CFLAGS) -c tests/abi_smoke.c -o $(BUILD)/abi-i686.o
	$(CLANG) -target x86_64-none-elf -ffreestanding $(CFLAGS) -c tests/abi_smoke.c -o $(BUILD)/abi-x86_64.o
	$(CLANG) -target aarch64-none-elf -ffreestanding $(CFLAGS) -c tests/abi_smoke.c -o $(BUILD)/abi-aarch64.o
	$(CLANG) -target riscv64-none-elf -ffreestanding $(CFLAGS) -c tests/abi_smoke.c -o $(BUILD)/abi-riscv64.o

clean:
	rm -rf $(BUILD)
