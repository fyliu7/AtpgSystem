$(BIN_TOP)/$(MODE)/atpg : $(PKG_DIR)/atpg/$(BIN_DIR)/$(MODE)/atpg
	@if [ ! -d $(dir $@) ]; then \
		mkdir -p $(dir $@); \
	fi
	$(AT)rm -f $@
	@echo "    CP $(notdir $@) $(PRT_MODE)"
	$(AT)cp -f $< $@

$(BIN_TOP)/$(MODE)/cli_test : $(PKG_DIR)/cli/$(BIN_DIR)/$(MODE)/cli_test
	@if [ ! -d $(dir $@) ]; then \
		mkdir -p $(dir $@); \
	fi
	$(AT)rm -f $@
	@echo "    CP $(notdir $@) $(PRT_MODE)"
	$(AT)cp -f $< $@

