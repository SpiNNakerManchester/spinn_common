# If SPINN_DIRS is not defined, this is an error!
ifndef SPINN_DIRS
    $(error SPINN_DIRS is not set.  Please define SPINN_DIRS (possibly by running "source setup" in the spinnaker tools folder))
endif

SPINN_COMMON_BUILD = build

# Objects
OBJS = bit_field.o circular_buffer.o normal.o random.o stdfix-exp.o log.o \
	sincos.o sqrt.o generic-runge-kutta.o
BUILD_OBJS = $(OBJS:%.o=$(SPINN_COMMON_BUILD)/%.o)

# Build rules (default)
all: $(SPINN_COMMON_BUILD)/libspinn_common.a

$(SPINN_COMMON_BUILD)/libspinn_common.a: $(BUILD_OBJS) 
	$(RM) $@
	$(AR) $@ $(BUILD_OBJS)

override LIB = 1
include $(SPINN_DIRS)/make/Makefile.common

SPINN_COMMON_DEBUG := PRODUCTION_CODE

# Include our own include directory
CFLAGS += -I include $(OTIME) -D$(SPINN_COMMON_DEBUG)

# Headers
HEADERS = arm_acle_gcc.h arm_acle.h arm.h bit_field.h circular_buffer.h \
	cmsis.h core_v5te.h debug.h log.h normal.h pair.h polynomial.h random.h \
	sincos.h spin-print.h sqrt.h static-assert.h stdfix-exp.h \
	stdfix-full-iso.h utils.h generic-runge-kutta.h

INSTALL ?= install

$(SPINN_COMMON_BUILD)/%.o: src/%.c $(SPINN_COMMON_BUILD)
	$(CC) $(CFLAGS) -o $@ $<

$(SPINN_COMMON_BUILD):
	$(MKDIR) $@

# Installing rules
install: $(SPINN_COMMON_BUILD)/libspinn_common.a
	$(INSTALL) -c -m644 $< $(SPINN_LIB_DIR)
	$(INSTALL) -c -m644 $(HEADERS:%.h=include/%.h) $(SPINN_INC_DIR)

clean:
	$(RM) $(SPINN_COMMON_BUILD)/libspinn_common.a $(BUILD_OBJS)

.PHONY: all install clean
