CXX := g++
AR := ar rvs
CXXFLAGS := -Wall -g

TARGET := utils.a

SRCS := $(wildcard *.c++)
OBJS := $(patsubst %.c++,%.o,$(SRCS))

$(TARGET): $(OBJS)
	$(AR) $@ $^
%.o: %.c++
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf $(TARGET) *.o

.PHONY: all clean