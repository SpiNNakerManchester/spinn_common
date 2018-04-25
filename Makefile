# If SPINN_DIRS is not defined, this is an error!
ifndef SPINN_DIRS
    $(error SPINN_DIRS is not set.  Please define SPINN_DIRS (possibly by running "source setup" in the spinnaker tools folder))
endif

SPINN_COMMON_BUILD = build

# Objects
OBJS = bit_field.o circular_buffer.o normal.o random.o stdfix-exp.o log.o \
	sincos.o sqrt.o
BUILD_OBJS = $(OBJS:%.o=$(SPINN_COMMON_BUILD)/%.o)

SPINN_COMMON_DEBUG := PRODUCTION_CODE

# Include our own include directory
CFLAGS += -I include $(OTIME) -D$(SPINN_COMMON_DEBUG)

# Headers
HEADERS = arm_acle_gcc.h arm_acle.h arm.h bit_field.h circular_buffer.h \
	cmsis.h core_v5te.h debug.h log.h normal.h pair.h polynomial.h random.h \
	sincos.h spin-print.h sqrt.h static-assert.h stdfix-exp.h \
	stdfix-full-iso.h utils.h
INSTALL_HEADERS = $(HEADERS:%.h=$(SPINN_INC_DIR)/%.h)

INSTALL ?= install

RAW_DIR = src/
MODIFIED_DIR = modified_src/
APP = libspinn_common
LOG_DICT_FILE = $(SPINN_COMMON_BUILD)/$(APP).dict

# Build rules (default)
all: $(SPINN_COMMON_BUILD)/$(APP).a $(LOG_DICT_FILE)

$(SPINN_COMMON_BUILD)/libspinn_common.a: $(BUILD_OBJS) 
	$(RM) $@
	$(AR) $@ $(BUILD_OBJS)

override LIB = 1
include $(SPINN_DIRS)/make/spinnaker_tools.mk

$(MODIFIED_DIR)%.c: $(RAW_DIR)
	python -m spinn_utilities.make_tools.convertor $(RAW_DIR) $(MODIFIED_DIR) $(LOG_DICT_FILE)

$(LOG_DICT_FILE): $(RAW_DIR)
	python -m spinn_utilities.make_tools.convertor $(RAW_DIR) $(MODIFIED_DIR) $(LOG_DICT_FILE)

$(SPINN_COMMON_BUILD)/%.o: $(MODIFIED_DIR)%.c $(SPINN_COMMON_BUILD)
	$(CC) $(CFLAGS) -o $@ $<

$(SPINN_COMMON_BUILD):
	$(MKDIR) $@

#install: $(SPINN_COMMON_BUILD)/$(APP).a $(LOG_DICT_FILE)
#	$(INSTALL) -c -m644 $< $(SPINN_LIB_DIR)
#	$(INSTALL) -c -m644 $(HEADERS:%.h=include/%.h) $(SPINN_INC_DIR)

# Installing rules

# Makefile
MAKEFILES = local.mk
INSTALL_MAKEFILES = $(MAKEFILES:%=$(SPINN_MAKE_LIB_DIR)/%)

# Installing rules
install: install-libraries install-headers install-makefiles
install-libraries: $(SPINN_LIB_DIR)/$(APP).a $(SPINN_LIB_DIR)/$(APP).dict
install-headers: $(INSTALL_HEADERS)
install-makefiles: $(INSTALL_MAKEFILES)

$(SPINN_LIB_DIR)/%.a: $(SPINN_COMMON_BUILD)/%.a
	$(INSTALL) -c -m644 $< $(SPINN_LIB_DIR)

$(SPINN_LIB_DIR)/%.dict: $(SPINN_COMMON_BUILD)/%.dict
	$(INSTALL) -c -m644 $< $(SPINN_LIB_DIR)

$(SPINN_INC_DIR)/%.h: include/%.h
	$(INSTALL) -c -m644 $< $(SPINN_INC_DIR)

$(SPINN_MAKE_LIB_DIR)/%: %
	$(INSTALL) -c -m644 $< $(SPINN_MAKE_LIB_DIR)

clean:
	$(RM) $(SPINN_COMMON_BUILD)/$(APP).a $(BUILD_OBJS) $(OLD_CONVERT_FILES) $(LOG_DICT_FILE)
	rm -rf $(MODIFIED_DIR)

install-clean:
	$(RM) $(SPINN_LIB_DIR)/$(APP).a $(SPINN_LIB_DIR)/*.dict $(SPINN_LIB_DIR)/*.ranges $(INSTALL_LIBS) $(INSTALL_HEADERS) $(INSTALL_MAKEFILES) 

.PHONY: all install clean

test:
	# $(BUILD_OBJS) 
