# Build options

## Compiler settings
ifeq ($(OS),Windows_NT)
NABC :=			..\..\build\nabc.exe
LD :=			..\..\build\nabc.exe
INC :=			-I ..\..\lib\include
else
NABC :=			../../build/nabc
LD :=			../../build/nabc
INC :=			-I ../../lib/include
endif

# Project settings
OBJNAME :=		TruthMachine
SRC :=			$(wildcard *.nabd)
OBJS :=			$(subst .nabd,.o,$(SRC))

%.o : %.nabd
	$(NABC) $< $(INC)

$(OBJNAME) : $(NABC) $(OBJS)
	$(LD) $@ -k $(OBJS)
