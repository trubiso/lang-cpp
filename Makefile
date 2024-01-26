OBJS := main.o Util.o Diagnostic.o Debug.o \
	Tokenizer/Token.o Tokenizer/Tokenizer.o \
	Parser/Primitive.o Parser/Identifier.o Parser/Type.o Parser/Expression.o Parser/Statement.o
DEPS := $(OBJS:.o=.d)
TARGET := ./out

EXTRA_FLAGS ?= -O2

CPPFLAGS := -MMD -MP
CXXFLAGS := -Wall -Wextra -Wpedantic $(EXTRA_FLAGS) -std=c++20
CFLAGS := $(CXXFLAGS)
LDFLAGS :=

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: clean run gdb

clean:
	-rm -f *.d
	-rm -f **/*.d
	-rm -f *.o
	-rm -f **/*.o
	-rm -f $(TARGET)

run: $(TARGET)
	$(TARGET)

gdb:
	make clean
	EXTRA_FLAGS=-g make $(TARGET)
	gdb $(TARGET)

-include $(DEPS)
