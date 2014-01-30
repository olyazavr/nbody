## Look in test/ for the unit test source files
vpath %.cpp $(DEV_DIR)/test

## The result of our nbody-test compile: an executable
## named list-test.x which will run all of our unit tests.
TARGET_PROJ_TEST := $(INSTALL_DIR)/test/$(PROJ_NAME)-test.x
BUILD_DIR_PROJ_TEST := $(BUILD_DIR)/$(PROJ_NAME)-test

## nbody-test.x depends on libnbody.a having been created first
$(TARGET_PROJ_TEST) : $(INSTALL_DIR)/lib/lib$(PROJ_NAME).a

## Link our nbody-test.x executable with libnbody.a and gtest
## XXX ATTENTION ATTENTION ATTTENTION MACOSX USERS! XXX
## If -lgov is giving you problems with clang, try replacing it with --coverage
$(TARGET_PROJ_TEST) : LDFLAGS += -lgcov $(INSTALL_DIR)/lib/lib$(PROJ_NAME).a

## Add lots more unit tests to this list!
OBJECTS_PROJ_TEST := \
	$(BUILD_DIR_PROJ_TEST)/gtest-all.o \
	$(BUILD_DIR_PROJ_TEST)/$(PROJ_NAME)-test.o \
	$(BUILD_DIR_PROJ_TEST)/vectorTest.o \
	#$(BUILD_DIR_PROJ_TEST)/add-more.o \
	#$(BUILD_DIR_PROJ_TEST)/unit-tests.o \
	#$(BUILD_DIR_PROJ_TEST)/right-here!.o \
	#$(BUILD_DIR_PROJ_TEST)/you-should.o \
	#$(BUILD_DIR_PROJ_TEST)/fill-up.o \
	#$(BUILD_DIR_PROJ_TEST)/all-these.o \
	#$(BUILD_DIR_PROJ_TEST)/lines.o \
	#$(BUILD_DIR_PROJ_TEST)/at.o \
	#$(BUILD_DIR_PROJ_TEST)/least.o \
## Add lots more unit tests to the list above!

$(OBJECTS_PROJ_TEST) : CXXFLAGS += -fprofile-arcs -ftest-coverage

## Look in GTEST_DIR/src for the gtest-all.cc file
vpath %.cc $(GTEST_DIR)/src

## Need to filter out -Weffc++ and -Wextra flags (or gtest won't compile)
$(OBJECTS_PROJ_TEST) : CXXFLAGS := $(filter-out -Weffc++,$(CXXFLAGS))
$(BUILD_DIR_PROJ_TEST)/gtest-all.o : CXXFLAGS := $(filter-out -Wextra,$(CXXFLAGS))

## Needed so gtest will compile on Cygwin.
$(OBJECTS_PROJ_TEST) : CXXFLAGS := $(filter-out -std=c++11,$(CXXFLAGS))
$(OBJECTS_PROJ_TEST) : CXXFLAGS := -std=gnu++0x $(CXXFLAGS)
