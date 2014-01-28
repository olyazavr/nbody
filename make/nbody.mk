# Virtual path directive - find all 
vpath %.cpp $(DEV_DIR)/src

PROJ_NAME := nbody

## The actual result of building this project:
## a static library named 'libnbody.a'.
TARGET_PROJ := $(INSTALL_DIR)/lib/lib$(PROJ_NAME).a
#$(TARGET_PROJ) : | $(INSTALL_DIR)/include/$(PROJ_NAME)

## Where to put all of the compiled object files
BUILD_DIR_PROJ := $(BUILD_DIR)/$(PROJ_NAME)

## List of object files to produce (from the similarly
## named .cpp files). Add any extra files you want to have
## compiled here.
OBJECTS_PROJ := \
	$(BUILD_DIR_PROJ)/Body.o \
	$(BUILD_DIR_PROJ)/System.o \
	$(BUILD_DIR_PROJ)/Simulation.o \
	$(BUILD_DIR_PROJ)/constants.o \
	#$(BUILD_DIR_PROJ)/add.o \
	#$(BUILD_DIR_PROJ)/any.o \
	#$(BUILD_DIR_PROJ)/other.o \
	#$(BUILD_DIR_PROJ)/files.o \
	#$(BUILD_DIR_PROJ)/here.o \
## Add more object files above as you write them!
$(OBJECTS_PROJ) : | $(INSTALL_DIR)/include/$(PROJ_NAME) \
										$(INSTALL_DIR)/bin/resources/$(PROJ_NAME) \
										$(INSTALL_DIR)/test/resources/$(PROJ_NAME)

## Copies over the public header files to install/include
## See http://www.gnu.org/software/make/manual/html_node/Double_002dColon.html
$(INSTALL_DIR)/include/$(PROJ_NAME) ::
	$(MKDIR) $@
	$(RSYNC) $(DEV_DIR)/include/$(PROJ_NAME)/ $@/

$(INSTALL_DIR)/bin/resources/$(PROJ_NAME) ::
	$(MKDIR) $@
	$(RSYNC) $(DEV_DIR)/resources/$(PROJ_NAME)/ $@/

$(INSTALL_DIR)/test/resources/$(PROJ_NAME) ::
	$(MKDIR) $@
	$(RSYNC) $(DEV_DIR)/resources/$(PROJ_NAME)/ $@/

## Add the public include files to the list of artifacts
## to be cleaned up on a 'make clean'
ARTIFACTS += $(INSTALL_DIR)/include/$(PROJ_NAME)
## Add the resource files to the list of artifacts
## to be cleaned up on a 'make clean'
ARTIFACTS += $(INSTALL_DIR)/bin/resources/$(PROJ_NAME)
ARTIFACTS += $(INSTALL_DIR)/test/resources/$(PROJ_NAME)
