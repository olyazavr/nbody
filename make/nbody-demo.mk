## Look in src/demo for the demo main files
vpath %.cpp $(DEV_DIR)/src/demo

## The result of our nbody-demo compile: an executable
## named list-demo.x which will run our program
TARGET_PROJ_DEMO := $(INSTALL_DIR)/bin/$(PROJ_NAME)-demo.x
BUILD_DIR_PROJ_DEMO := $(BUILD_DIR)/$(PROJ_NAME)-demo

## nbody-demo.x depends on libnbody.a having been created first
$(TARGET_PROJ_DEMO) : $(INSTALL_DIR)/lib/lib$(PROJ_NAME).a

## Link our nbody-demo.x executable with libnbody.a
## (here is where you add any extra third-party library linkings needed)
$(TARGET_PROJ_DEMO) : LDFLAGS += $(INSTALL_DIR)/lib/lib$(PROJ_NAME).a

## Add any more files to this list
OBJECTS_PROJ_DEMO := \
	$(BUILD_DIR_PROJ_DEMO)/main.o \
	#$(BUILD_DIR_PROJ_DEMO)/otherFiles.o \
	#$(BUILD_DIR_PROJ_DEMO)/moreOtherFiles.o \
## Add any more files you like!

$(INSTALL_DIR)/include/$(PROJ_NAME)-demo ::
	$(MKDIR) $@
	$(RSYNC) $(DEV_DIR)/include/$(PROJ_NAME)-demo/ $@/

## Add the public include files to the list of artifacts
## to be cleaned up on a 'make clean'
ARTIFACTS += $(INSTALL_DIR)/include/$(PROJ_NAME)-demo
