################################################################################
# Library OpenThread (OT) - SRC/CORE/MESHCOP Rules
################################################################################

LIB_CPPSRC								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MESHCOP_DIR)/commissioner.cpp			\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MESHCOP_DIR)/dtls.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MESHCOP_DIR)/energy_scan_client.cpp		\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MESHCOP_DIR)/joiner.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MESHCOP_DIR)/joiner_router.cpp			\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MESHCOP_DIR)/leader.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MESHCOP_DIR)/panid_query_client.cpp

INC_PATH								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MESHCOP_DIR)
