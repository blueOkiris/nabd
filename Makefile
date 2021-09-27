# Build settings

## Compiler options
CPPC :=				g++
CPPFLAGS :=			-Wall -Werror -g -std=c++17
LD :=				g++
LDFLAGS :=			-lm
BUILDFLDR :=		build
OBJFLDR :=			obj

## Main target settings
OBJNAME :=			nabc
SRC :=				$(wildcard src/*.cpp)
HFILES :=			$(wildcard include/*.hpp)
INC :=				-Iinclude
OBJS :=				$(addprefix $(OBJFLDR)/$(OBJNAME)/,$(subst .cpp,.o,$(foreach file,$(SRC),$(notdir $(file)))))

## Test settings
TEST_OBJNAMES :=	HelloWorldTest \
					TruthMachineTest \
					StdTest
LIB_SRC :=			$(wildcard lib/src/*.cpp)
LIB_HFILES :=		$(wildcard lib/include/*.hpp)
LIB_OBJS :=			$(addprefix $(OBJFLDR)/lib/,$(subst .cpp,.o,$(foreach file,$(LIB_SRC),$(notdir $(file)))))
LIB_INC :=			-Ilib/include

# Targets

## Helper Targets

.PHONY : all
ifeq ($(OS),Windows_NT)
all : $(BUILDFLDR)\\$(OBJNAME) tests examples
else
all : $(BUILDFLDR)/$(OBJNAME) tests examples
endif

.PHONY : clean
clean:
ifeq ($(OS),Windows_NT)
	-cmd /k "rmdir /s /q $(BUILDFLDR) & exit"
	-cmd /k "rmdir /s /q $(OBJFLDR) & exit"

	-cmd /k "rmdir /s /q examples\\truth-machine\\infLoop_nabdout & exit"
	-cmd /k "rmdir /s /q examples\\truth-machine\\main_nabdout & exit"
	-cmd /k "rmdir /s /q examples\\truth-machine\\TruthMachine_nabdout & exit"
	-cmd /k "del examples\\truth-machine\\infLoop.o & exit"
	-cmd /k "del examples\\truth-machine\\main.o & exit"
	-cmd /k "del examples\\truth-machine\\TruthMachine.exe & exit"

	-cmd /k "rmdir /s /q examples\\guess-num\\main_nabdout & exit"
	-cmd /k "rmdir /s /q examples\\guess-num\\GuessTheNumber_nabdout & exit"
	-cmd /k "del examples\\guess-num\\main.o & exit"
	-cmd /k "del examples\\guess-num\\GuessTheNumber.exe & exit"
	
	-cmd /k "rmdir /s /q examples\\ParserTest_nabdout & exit"
	-cmd /k "del examples\\ParserTest.o & exit"
else
	rm -rf $(BUILDFLDR)
	rm -rf $(OBJFLDR)

	rm -rf examples/truth-machine/*_nabdout
	rm -rf examples/truth-machine/*.o
	rm -rf examples/truth-machine/TruthMachine

	rm -rf examples/guess-num/*_nabdout
	rm -rf examples/guess-num/*.o
	rm -rf examples/guess-num/GuessTheNumber
	
	rm -rf examples/*.o
	rm -rf examples/*_nabdout

	rm -rf installers/debian/nabc/usr
	rm -rf installers/debian/nabc.deb
endif

ifeq ($(OS),Windows_NT)
$(OBJFLDR)\\nabc\\%.o : src\\%.cpp $(subst /,\\,$(HFILES))
	-mkdir $(OBJFLDR)
	-mkdir $(OBJFLDR)\\nabc
else
$(OBJFLDR)/nabc/%.o : src/%.cpp $(HFILES)
	mkdir -p $(OBJFLDR)/nabc
endif
	$(CPPC) $(CPPFLAGS) $(INC) -o $@ -c $<

ifeq ($(OS),Windows_NT)
$(OBJFLDR)\\tests\\%.o : tests\\%.cpp $(subst /,\\,$(LIB_HFILES))
	-mkdir $(OBJFLDR)
	-mkdir $(OBJFLDR)\\tests
else
$(OBJFLDR)/tests/%.o : tests/%.cpp $(LIB_HFILES)
	mkdir -p $(OBJFLDR)/tests
endif
	$(CPPC) $(CPPFLAGS) $(LIB_INC) -o $@ -c $<

ifeq ($(OS),Windows_NT)
$(OBJFLDR)\\lib\\%.o : lib\\src\\%.cpp $(subst /,\\,$(LIB_HFILES))
	-mkdir $(OBJFLDR)
	-mkdir $(OBJFLDR)\\lib
else
$(OBJFLDR)/lib/%.o : lib/src/%.cpp $(LIB_HFILES)
	mkdir -p $(OBJFLDR)/lib
endif
	$(CPPC) $(CPPFLAGS) $(LIB_INC) -o $@ -c $<

## Main targets

ifeq ($(OS),Windows_NT)
$(BUILDFLDR)\\$(OBJNAME) : $(subst /,\\,$(OBJS))
	-mkdir $(BUILDFLDR)
else
$(BUILDFLDR)/$(OBJNAME) : $(OBJS)
	mkdir -p $(BUILDFLDR)
endif
	$(LD) -o $@ $^ $(LDFLAGS)

.PHONY : tests
ifeq ($(OS),Windows_NT)
tests : $(addprefix $(BUILDFLDR)\\,$(TEST_OBJNAMES))
else
tests : $(addprefix $(BUILDFLDR)/,$(TEST_OBJNAMES))
endif

.PHONY : examples
ifeq ($(OS),Windows_NT)
examples : examples\\truth-machine\\TruthMachine.exe examples\\guess-num\\GuessTheNumber.exe
else
examples : examples/truth-machine/TruthMachine examples/guess-num/GuessTheNumber
endif

ifeq ($(OS),Windows_NT)
define test_targets
$(BUILDFLDR)\\$(1) : $(subst /,\\,$(LIB_OBJS)) $(OBJFLDR)\\tests\\$(1).o
	-mkdir $(BUILDFLDR)
	$(LD) -o $(BUILDFLDR)\\$(1) \
		$(subst /,\\,$(LIB_OBJS)) $(OBJFLDR)\\tests\\$(1).o $(LDFLAGS)
endef
else
define test_targets
$(BUILDFLDR)/$(1) : $(LIB_OBJS) $(OBJFLDR)/tests/$(1).o
	mkdir -p $(BUILDFLDR)
	$(LD) -o $(BUILDFLDR)/$(1) $(LIB_OBJS) $(OBJFLDR)/tests/$(1).o $(LDFLAGS)
endef
endif
$(foreach test,$(TEST_OBJNAMES),$(eval $(call test_targets,$(test))))

ifeq ($(OS),Windows_NT)
examples\\truth-machine\\TruthMachine.exe : $(BUILDFLDR)\\$(OBJNAME)
	mingw32-make -C examples\\truth-machine
else
examples/truth-machine/TruthMachine : $(BUILDFLDR)/$(OBJNAME)
	make -C examples/truth-machine
endif

ifeq ($(OS),Windows_NT)
examples\\guess-num\\GuessTheNumber.exe : $(BUILDFLDR)\\$(OBJNAME)
	mingw32-make -C examples\\guess-num
else
examples/guess-num/GuessTheNumber : $(BUILDFLDR)/$(OBJNAME)
	make -C examples/guess-num
endif

ifneq ($(OS),Windows_NT)
.PHONY : installers/debian/nabc.deb
installers/debian/nabc.deb : $(BUILDFLDR)/$(OBJNAME)
	mkdir -p installers/debian/nabc/usr/bin
	cp $< installers/debian/nabc/usr/bin
	
	mkdir -p installers/debian/nabc/usr/include/nabc
	cp lib/include/std.hpp installers/debian/nabc/usr/include/nabc

ifeq ($(WSL),)
	dpkg-deb --build installers/debian/nabc
else
	cp -r installers/debian/nabc ~
	dpkg-deb --build ~/nabc
	rm -rf ~/nabc
	mv ~/nabc.deb installers/debian
endif
endif
