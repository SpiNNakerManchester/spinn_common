# Set to 1 for GNU tools, 0 for ARM
GNU = 1

# Prefix for GNU tool binaries
GP  = arm-none-eabi


# If SPINN_DIRS is not defined, this is an error!
ifndef SPINN_DIRS
    $(error SPINN_DIRS is not set.  Please define SPINN_DIRS (possibly by running "source setup" in the spinnaker tools folder))
endif
SPINN_LIB_DIR = $(SPINN_DIRS)/lib
SPINN_INC_DIR = $(SPINN_DIRS)/include


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


# General tool setup
CAT = cat
RM  = rm -f
LS  = ls -l
MKDIR = mkdir -p
CP  = cp

# GNU tool setup
ifeq ($(GNU),1)
    AS = $(GP)-as --defsym GNU=1 -mthumb-interwork -march=armv5te

    CC = $(GP)-gcc -mthumb-interwork -march=armv5te -std=gnu99 -I $(SPINN_INC_DIR) -c
    CFLAGS += -fdata-sections -ffunction-sections -static
    
    LD = $(GP)-gcc -T$(SCRIPT) -Wl,-e,cpu_reset -Wl,-static -fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,--use-blx -nostartfiles -static -L $(SPINN_LIB_DIR)
    AR = $(GP)-ar -rcs
    OC = $(GP)-objcopy
    OD = $(GP)-objdump -dxt > $(APP).txt
    NM = $(GP)-nm

# ARM tool setup
else
    AS = armasm --keep --cpu=5te --apcs /interwork

    CC = armcc -c --c99 --cpu=5te --apcs /interwork --min_array_alignment=4 -I $(INC_DIR)
    CFLAGS += --split_sections

    LD = armlink --scatter=$(SCRIPT) --remove --entry cpu_reset

    AR = armar -rcs
    OC = fromelf
    OD = fromelf -cds --output $(APP).txt
    NM = nm
endif


# Libraries
LIBS = $(SPINN_LIB_DIR)/libspin1_api.a

# Objects
OBJS = bit_field.o random.o stdfix-exp.o

# Headers
HEADERS = arm_acle.h arm.h bit_field.h cmsis.h debug.h random.h spin-print.h static-assert.h stdfix-exp.h stdfix-full-iso.h utils.h

$(SPINN_COMMON_LIBS)/libspinn_common.a:	$(OBJS:%.o=$(SPINN_COMMON_BUILD)/%.o) $(HEADERS:%.h=$(SPINN_COMMON_INCLUDE)/%.h)
	$(RM) $@
	$(AR) $@ $<


$(SPINN_COMMON_INCLUDE)/%.h:	src/%.h
	$(CP) $< $@

$(SPINN_COMMON_BUILD)/%.o:	src/%.c $(SPINN_COMMON_BUILD)
	$(CC) $(CFLAGS) -o $@ $<

$(SPINN_COMMON_BUILD):
	$(MKDIR) $@
