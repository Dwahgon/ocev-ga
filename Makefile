CXX := g++
CXXFLAGS := -Wall -g
TARGET := ga
BUILDDIR := build

SRCS := $(wildcard *.c++)
OBJS := $(patsubst %.c++,%.o,$(SRCS))

all: $(BUILDDIR)/$(TARGET)
	printf "POP=30\nDIM=100\nSEED=\nINT_RANGE_START=-5\nINT_RANGE_END=10\nREAL_RANGE_START=-10\nREAL_RANGE_END=10" > $(BUILDDIR)/conf.conf

$(BUILDDIR)/$(TARGET): $(OBJS) | $(BUILDDIR)
	$(CXX) -o $@ $^
%.o: %.c++
	$(CXX) $(CXXFLAGS) -c $<
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(TARGET) *.o
	rm -rf $(BUILDDIR)

.PHONY: all clean