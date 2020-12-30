XBE_TITLE = coccoiMannu
GEN_XISO = $(XBE_TITLE).iso
SRCS = $(CURDIR)/main.cpp
#NXDK_DIR = $(CURDIR)/../../..
NXDK_PBGL = y
NXDK_CXX = y
NXDK_SDL = y
NXDK_SDL_MIXER=y

CXXFLAGS += -I$(CURDIR)/include

include $(NXDK_DIR)/Makefile


TARGET += $(OUTPUT_DIR)/res
$(GEN_XISO): $(OUTPUT_DIR)/res
$(OUTPUT_DIR)/res: $(CURDIR)/res $(OUTPUT_DIR)
	rm -rf $(OUTPUT_DIR)/res
	$(VE)cp -rf '$<' '$@'

.PHONY: clean 
clean: $(CLEANRULES)
	$(VE)rm -rf $(TARGET) \
	           main.exe main.exe.manifest main.lib \
	           $(OBJS) $(SHADER_OBJS) $(DEPS) \
	           $(GEN_XISO)