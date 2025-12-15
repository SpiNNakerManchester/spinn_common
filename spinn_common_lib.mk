# Copyright (c) 2025 The University of Manchester
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

SPINN_COMMON_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
SPINN_COMMON_BUILD = $(SPINN_COMMON_DIR)/build

ifndef SPINN_INSTALL_DIR:
    # assume parallel clone
    SPINN_INSTALL_DIR = $(SPINN_COMMON_DIR)/../spinnaker_tools
endif

override LIB = 1
include $(SPINN_INSTALL_DIR)/make/spinnaker_tools.mk

# Objects
OBJS = bit_field.o circular_buffer.o normal.o random.o stdfix-exp.o log.o \
	sincos.o sqrt.o
BUILD_OBJS = $(OBJS:%.o=$(SPINN_COMMON_BUILD)/%.o)

SPINN_COMMON_DEBUG := PRODUCTION_CODE

# Include our own include directory
CFLAGS += -I $(SPINN_COMMON_DIR)/include $(OTIME) -D$(SPINN_COMMON_DEBUG)

# Headers
HEADERS = arm_acle_gcc.h arm_acle.h arm.h assert.h bit_field.h \
    circular_buffer.h cmsis.h core_v5te.h log.h normal.h pair.h polynomial.h \
    random.h sincos.h spin-print.h sqrt.h static-assert.h stdfix-exp.h \
	stdfix-full-iso.h utils.h round.h

INSTALL ?= install

# Build rules (default)
all: $(SPINN_COMMON_BUILD)/libspinn_common.a

$(SPINN_COMMON_BUILD)/libspinn_common.a: $(BUILD_OBJS)
	$(MKDIR) $(SPINN_COMMON_BUILD)
	$(RM) $@
	$(AR) $@ $(BUILD_OBJS)

$(SPINN_COMMON_BUILD)/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ $<

# Installing rules
install: $(SPINN_COMMON_BUILD)/libspinn_common.a
	$(eval SPINN_COMMON_INSTALL_DIR := $(strip $(if $(SPINN_COMMON_INSTALL_DIR), $(SPINN_COMMON_INSTALL_DIR), $(if $(SPINN_DIRS), $(SPINN_DIRS)/spinn_common_install, $(error SPINN_COMMON_INSTALL_DIR or SPINN_DIRS is not set.  Please define SPINN_COMMON_INSTALL_DIR or SPINN_DIRS)))))
	$(MKDIR) $(SPINN_COMMON_INSTALL_DIR)/lib
	$(MKDIR) $(SPINN_COMMON_INSTALL_DIR)/include
	$(MKDIR) $(SPINN_COMMON_INSTALL_DIR)/make
	$(INSTALL) -m644 $< $(SPINN_COMMON_INSTALL_DIR)/lib
	$(INSTALL) -m644 $(HEADERS:%=$(SPINN_COMMON_DIR)/include/%) $(SPINN_COMMON_INSTALL_DIR)/include
	$(INSTALL) -m644 $(SPINN_COMMON_DIR)/make/spinn_common.mk $(SPINN_COMMON_INSTALL_DIR)/make

clean:
	$(RM) $(SPINN_COMMON_BUILD)/libspinn_common.a $(BUILD_OBJS)

.PHONY: all clean
