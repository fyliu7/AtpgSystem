$(BIN_TOP)/$(MODE)/atpg : $(PKG_DIR)/atpg/$(BIN_DIR)/$(MODE)/atpg
	@if [ ! -d $(dir $@) ]; then \
		mkdir -p $(dir $@); \
	fi
	$(AT)rm -f $@
	@echo "    CP $(notdir $@) $(PRT_MODE)"
	$(AT)cp -f $< $@

$(BIN_TOP)/$(MODE)/netlist_test : $(PKG_DIR)/interface/$(BIN_DIR)/$(MODE)/netlist_test
	@if [ ! -d $(dir $@) ]; then \
		mkdir -p $(dir $@); \
	fi
	$(AT)rm -f $@
	@echo "    CP $(notdir $@) $(PRT_MODE)"
	$(AT)cp -f $< $@

