# If SPINN_DIRS is not defined, this is an error!
ifndef SPINN_DIRS
    $(error SPINN_DIRS is not set.  Please define SPINN_DIRS (possibly by running "source setup" in the spinnaker tools folder))
endif

SPINN_COMMON_BUILD = build

# Objects
OBJS = bit_field.o circular_buffer.o normal.o random.o stdfix-exp.o log.o \
	sincos.o sqrt.o
BUILD_OBJS = $(OBJS:%.o=$(SPINN_COMMON_BUILD)/%.o)

# Build rules (default)
all: $(SPINN_COMMON_BUILD)/libspinn_common.a

$(SPINN_COMMON_BUILD)/libspinn_common.a: $(BUILD_OBJS)
	$(RM) $@
	$(AR) $@ $(BUILD_OBJS)

override LIB = 1
include $(SPINN_DIRS)/make/spinnaker_tools.mk

SPINN_COMMON_DEBUG := PRODUCTION_CODE

# Include our own include directory
CFLAGS += -I include $(OTIME) -D$(SPINN_COMMON_DEBUG)

# Headers
HEADERS = arm_acle_gcc.h arm_acle.h arm.h assert.h bit_field.h \
    circular_buffer.h cmsis.h core_v5te.h log.h normal.h pair.h polynomial.h \
    random.h sincos.h spin-print.h sqrt.h static-assert.h stdfix-exp.h \
	stdfix-full-iso.h utils.h
INSTALL_HEADERS = $(HEADERS:%.h=$(SPINN_INC_DIR)/%.h)

INSTALL ?= install

$(SPINN_COMMON_BUILD)/%.o: src/%.c $(SPINN_COMMON_BUILD)
	$(CC) $(CFLAGS) -o $@ $<

$(SPINN_COMMON_BUILD):
	$(MKDIR) $@

# Installing rules
install: $(SPINN_COMMON_BUILD)/libspinn_common.a
	$(INSTALL) -c -m644 $< $(SPINN_LIB_DIR)
	$(INSTALL) -c -m644 $(HEADERS:%.h=include/%.h) $(SPINN_INC_DIR)
	cp CITATION.cff $(SPINN_DIRS)/spinn_common.citation.cff

clean:
	$(RM) $(SPINN_COMMON_BUILD)/libspinn_common.a $(BUILD_OBJS) \
	$(SPINN_LIB_DIR)/*.dict $(SPINN_LIB_DIR)/*.ranges $(INSTALL_HEADERS)

install-clean:
	$(RM) $(INSTALL_HEADERS)

.PHONY: all install clean install-clean
