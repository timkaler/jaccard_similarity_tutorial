BUILD_DIR := build
CC := clang++
LD := clang++

CFLAGS := -Wall -Werror -fcilkplus -DCILKP
LDFLAGS := -lcilkrts

ALL: main


$(BUILD_DIR)/main.o: main.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(BUILD_DIR)/main.o
	$(LD) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(BINS) $(BUILD_DIR)/*.o
