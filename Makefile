BUILD_DIR := build
CC := clang++
LD := clang++

CFLAGS := -Wall -O3 -g -gdwarf-3 -fcilkplus -Werror -DCILKP -std=c++14
LDFLAGS := #-lcilkrts -ldl

ALL: main



main: main.cpp
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

#.main.cilkscale: main.cpp
#	$(CC) -o $@ $(CFLAGS) -fcsi $^ $(LDFLAGS) -l:libclang_rt.cilkscale-x86_64.a


#main: $(BUILD_DIR)/main.o
#	$(LD) -o $@ $^ $(LDFLAGS) -l:libclang_rt.cilkscale-x86_64.a
#
##.main.cilkscale: main.cpp
##	$(LD) -o $@ $(CFLAGS) $^ $(LDFLAGS) -l:libclang_rt.cilkscale-x86_64.a
#
clean:
	rm -f $(BINS) $(BUILD_DIR)/*.o main .main.cilkscale
