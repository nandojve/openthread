################################################################################
# Library OpenThread (OT) - SRC/CORE/MAC Rules
################################################################################

LIB_CPPSRC								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MAC_DIR)/mac.cpp						\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MAC_DIR)/mac_blacklist.cpp				\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MAC_DIR)/mac_frame.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MAC_DIR)/mac_whitelist.cpp

INC_PATH								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MAC_DIR)
