# If SPINN_COMMON_LIBS is not defined, this is an error!
ifndef SPINN_COMMON_LIBS
    $(error SPINN_COMMON_LIBS is not set.  Please define SPINN_COMMON_LIBS (possibly by running "source setup" in the root of this package))
endif

# If SPINN_COMMON_INCLUDE is not defined, this is an error!
ifndef SPINN_COMMON_INCLUDE
    $(error SPINN_COMMON_INCLUDE is not set.  Please define SPINN_COMMON_INCLUDE (possibly by running "source setup" in the root of this package))
endif

# If SPINN_COMMON_BUILD is not defined, this is an error!
ifndef SPINN_COMMON_BUILD
    $(error SPINN_COMMON_BUILD is not set.  Please define SPINN_COMMON_BUILD (possibly by running "source setup" in the root of this package))
endif


# If SPINN_DIRS is not defined, this is an error!
ifndef SPINN_DIRS
    $(error SPINN_DIRS is not set.  Please define SPINN_DIRS (possibly by running "source setup" in the spinnaker tools folder))
endif

include $(SPINN_DIRS)/Makefile.common

# Include our own include directory
CFLAGS += -I $(SPINN_COMMON_INCLUDE)

# General tool setup
CAT = cat
LS  = ls -l
MKDIR = mkdir -p
CP  = cp

# Libraries
ifneq ($(GNU), 1)
    LIBS = $(SPINN_LIB_DIR)/libspin1_api.a
else
    LIBS = $(SPINN_LIB_DIR)/spin1_api.a
endif

# Objects
OBJS = bit_field.o random.o stdfix-exp.o

# Headers
HEADERS = arm_acle.h arm.h bit_field.h cmsis.h debug.h random.h spin-print.h static-assert.h stdfix-exp.h stdfix-full-iso.h utils.h

$(SPINN_COMMON_LIBS)/libspinn_common.a:	$(OBJS:%.o=$(SPINN_COMMON_BUILD)/%.o) $(HEADERS:%.h=$(SPINN_COMMON_INCLUDE)/%.h)
	$(RM) $@
	$(AR) $@ $(OBJS:%.o=$(SPINN_COMMON_BUILD)/%.o)

$(SPINN_COMMON_INCLUDE)/%.h: src/%.h
	$(CP) $< $@

$(SPINN_COMMON_BUILD)/%.o: src/%.c $(SPINN_COMMON_BUILD)
	$(CC) $(CFLAGS) -o $@ $<

$(SPINN_COMMON_BUILD):
	$(MKDIR) $@

clean:
	$(RM) $(SPINN_COMMON_LIBS)/libspinn_common.a $(OBJS:%.o=$(SPINN_COMMON_BUILD)/%.o)
