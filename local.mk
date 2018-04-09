# If SPINN_DIRS is not defined, this is an error!
ifndef SPINN_DIRS
    $(error SPINN_DIRS is not set.  Please define SPINN_DIRS (possibly by running "source setup" in the spinnaker package folder))
endif

ifndef APP_OUTPUT_DIR
    $(error APP_OUTPUT_DIR is not set.  Please define APP_OUTPUT_DIR)
endif

ifndef BUILD_DIR
    $(error BUILD_DIR is not set.  Please define BUILD_DIR)
endif

all: $(APP_OUTPUT_DIR)$(APP).aplx $(APP_OUTPUT_DIR)$(APP).dict

# Convert the objs into the correct format to work here
OBJS := $(SOURCES)
$(eval OBJS := $(OBJS:%.c=$(BUILD_DIR)%.o))
OBJECTS += $(OBJS)

LIBRARIES += -lspinn_frontend_common -lspinn_common -lm
FEC_DEBUG := PRODUCTION_CODE
PROFILER := PROFILER_DISABLED

# Run md5sum on application name and extract first 8 bytes
SHELL = bash
APPLICATION_NAME_HASH = $(shell echo -n "$(APP)" | (md5sum 2>/dev/null || md5) | cut -c 1-8)

CFLAGS += -Wall -Wextra -D$(FEC_DEBUG) -D$(PROFILER) $(OTIME) -DAPPLICATION_NAME_HASH=0x$(APPLICATION_NAME_HASH)

MODIFIED_DIR = modified/

$(BUILD_DIR)%.o: $(MODIFIED_DIR)%.c
	-mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -D__FILENAME__=\"$(notdir $*.c)\" -o $@ $<

include $(SPINN_DIRS)/make/convert.mk
include $(SPINN_DIRS)/make/Makefile.common

# Tidy and cleaning dependencies
clean:
	$(RM) $(OBJECTS) $(BUILD_DIR)$(APP).elf $(BUILD_DIR)$(APP).txt $(APP_OUTPUT_DIR)$(APP).aplx
