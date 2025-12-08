# Copyright (c) 2014 The University of Manchester
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

# Locations of tag files
TAGFILES=sllt.tag
SLLT_TAG=https://spinnakermanchester.github.io/spinnaker_tools/sllt.tag

DOXYGEN ?= doxygen
WGET ?= wget

all:
	$(MAKE) -f spinn_common_lib.mk all

install:
	$(MAKE) -f spinn_common_lib.mk install

clean:
	$(MAKE) -f spinn_common_lib.mk clean
	-$(RM) $(TAGFILES)

sllt.tag: .sllt_template.tag
	cp .sllt_template.tag sllt.tag
ifneq (, $(shell which $(WGET)))
	-$(WGET) -q -O sllt.tag $(SLLT_TAG)
endif

doxygen: $(TAGFILES)
	$(DOXYGEN)

.PHONY: all clean doxygen
