ifndef CONVERT_DIR
    ifndef MODIFIED_DIR
        $(error neither CONVERT_DIR nor MODIFIED_DIR is set.)
    endif
    CONVERT_DIR := $(MODIFIED_DIR)
endif

ifndef SUMMARY_DICT
    ifndef APP_OUTPUT_DIR
        $(error neither SUMMARY_DICT nor APP_OUTPUT_DIR is set.)
    endif
    ifndef APP
        $(error neither SUMMARY_DICT nor APP is not set.)
    endif
    SUMMARY_DICT := $(APP_OUTPUT_DIR)$(APP).dict
endif

ifndef RANGE_FILE
    RANGE_FILE = $(abspath $(MODIFIED_DIR))/log_ranges.txt
endif

ifndef RANGE_START
    RANGE_START = 9000
endif


ifdef RAW_DIR
    # Recursive including sub dirs
	_RAW_FILES = $(shell find $(RAW_DIR) -name '*.c')
	_RAW_FILES += $(shell find $(RAW_DIR) -name '*.h')
else
    # so use sources and all local files NONE recursive (modified my be sub dir)
	_RAW_FILES = $(SOURCES)
	_RAW_FILES += $(wildcard *.c)
	_RAW_FILES += $(wildcard *.h)
endif

# sort to remove duplicates
_MODIFIED_FILES = $(sort $(_RAW_FILES))
$(eval _MODIFIED_FILES := $(_MODIFIED_FILES:$(RAW_DIR)%=$(MODIFIED_DIR)%))

_DICT_FILES = $(sort $(_RAW_FILES))
$(eval _DICT_FILES := $(_DICT_FILES:$(RAW_DIR)%.c=$(MODIFIED_DIR)%.cdict))
$(eval _DICT_FILES := $(_DICT_FILES:$(RAW_DIR)%.h=$(MODIFIED_DIR)%.hdict))

# _DICT_FILES += $(shell find $(SPINN_DIRS)/lib -name '*.csv')

#List the existing
OLD_CONVERT_FILES = $(RANGE_FILE)
OLD_CONVERT_FILES += $(shell find $(MODIFIED_DIR) -name '*.c')
OLD_CONVERT_FILES += $(shell find $(MODIFIED_DIR) -name '*.h')
OLD_CONVERT_FILES += $(shell find $(MODIFIED_DIR) -name '*.cdict')
OLD_CONVERT_FILES += $(shell find $(MODIFIED_DIR) -name '*.hdict')

# Rule to create all the modified c files
$(MODIFIED_DIR)%.c $(MODIFIED_DIR)%.cdict: $(RAW_DIR)%.c
	python -m spinn_utilities.make_tools.file_convertor $< $(MODIFIED_DIR)$*.c $(RANGE_FILE) $(RANGE_START)

# Rule to create all the modified h files
$(MODIFIED_DIR)%.h $(MODIFIED_DIR)%.hdict: $(RAW_DIR)%.h
	python -m spinn_utilities.make_tools.file_convertor $< $(MODIFIED_DIR)$*.h $(RANGE_FILE) $(RANGE_START)

# At the end we want all the csv files merged
#$(SUMMARY_DICT): $(_DICT_FILES)
$(SUMMARY_DICT):
	head -n 2 $(firstword $(_DICT_FILES)) > $(SUMMARY_DICT)
	$(foreach dict, $(_DICT_FILES), tail -n+3 $(dict) >> $(SUMMARY_DICT);)

# Files to be kept after build
PRECIOUS_FILES += $(_MODIFIED_FILES) $(_DICT_FILES) $(SUMMARY_DICT)

# Files to be required by default rule
ALL_TARGETS += $(SUMMARY_DICT)