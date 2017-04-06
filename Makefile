# If SPINN_DIRS is not defined, this is an error!
ifndef SPINN_DIRS
    $(error SPINN_DIRS is not set.  Please define SPINN_DIRS (possibly by running "source setup" in the spinnaker tools folder))
endif

override LIB = 1
SPINN_COMMON_BUILD = build
include $(SPINN_DIRS)/make/Makefile.common

SPINN_COMMON_DEBUG := PRODUCTION_CODE

# Include our own include directory
CFLAGS += -I include $(OTIME) -D$(SPINN_COMMON_DEBUG)

# Objects
OBJS = bit_field.o circular_buffer.o normal.o random.o stdfix-exp.o log.o \
	sincos.o sqrt.o
BUILD_OBJS = $(OBJS:%.o=$(SPINN_COMMON_BUILD)/%.o)

# Headers
HEADERS = arm_acle_gcc.h arm_acle.h arm.h bit_field.h circular_buffer.h \
	cmsis.h core_v5te.h debug.h log.h normal.h pair.h polynomial.h random.h \
	sincos.h spin-print.h static-assert.h stdfix-exp.h stdfix-full-iso.h \
	utils.h
INSTALL_HEADERS = $(HEADERS:%.h=$(SPINN_INC_DIR)/%.h)

all: $(SPINN_COMMON_BUILD)/libspinn_common.a

# Build rules (default)
$(SPINN_COMMON_BUILD)/libspinn_common.a: $(BUILD_OBJS) 
	$(RM) $@
	$(AR) $@ $(BUILD_OBJS)

$(SPINN_COMMON_BUILD)/%.o: src/%.c $(SPINN_COMMON_BUILD)
	$(CC) $(CFLAGS) -o $@ $<

$(SPINN_COMMON_BUILD):
	$(MKDIR) $@

# Installing rules
install: $(SPINN_LIB_DIR)/libspinn_common.a $(INSTALL_HEADERS)

$(SPINN_LIB_DIR)/libspinn_common.a: $(SPINN_COMMON_BUILD)/libspinn_common.a
	$(CP) $< $@

$(SPINN_INC_DIR)/%.h: include/%.h
	$(CP) $< $@

clean:
	$(RM) $(SPINN_COMMON_BUILD)/libspinn_common.a $(BUILD_OBJS)

.PHONY: all install clean
