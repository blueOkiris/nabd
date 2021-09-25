# Build settings

## Compiler options
CPPC :=				g++
CPPFLAGS :=			-Wall -Werror -O2 -std=c++17
LD :=				g++
LDFLAGS :=		
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
					TruthMachineTest
LIB_SRC :=			$(wildcard lib/src/*.cpp)
LIB_HFILES :=		$(wildcard lib/include/*.hpp)
LIB_OBJS :=			$(addprefix $(OBJFLDR)/lib/,$(subst .cpp,.o,$(foreach file,$(LIB_SRC),$(notdir $(file)))))
LIB_INC :=			-Ilib/include

# Targets

## Helper Targets

.PHONY : all
ifeq ($(OS),Windows_NT)
all : $(BUILDFLDR)\\$(OBJNAME) tests
else
all : $(BUILDFLDR)/$(OBJNAME) tests
endif

.PHONY : clean
clean:
ifeq ($(OS),Windows_NT)
	cmd /k "rmdir /s /q $(BUILDFLDR) & exit"
	cmd /k "rmdir /s /q $(OBJFLDR) & exit"
else
	rm -rf $(BUILDFLDR)
	rm -rf $(OBJFLDR)
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
else
$(BUILDFLDR)/$(OBJNAME) : $(OBJS)
endif
	-mkdir $(BUILDFLDR)
	$(LD) -o $@ $^ $(LDFLAGS)

.PHONY : tests
ifeq ($(OS),Windows_NT)
tests : $(addprefix $(BUILDFLDR)\\,$(TEST_OBJNAMES))
else
tests : $(addprefix $(BUILDFLDR)/,$(TEST_OBJNAMES))
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
