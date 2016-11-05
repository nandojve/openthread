################################################################################
# Library OpenThread (OT) - SRC/CORE/MESHCOP Rules
################################################################################

ifneq (,$(filter $(CONFIG_OPENTHREAD_ENABLE_COMMISSIONER),y yes YES 1))
	LIB_CPPSRC							+=										\
		$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MESHCOP_DIR)/commissioner.cpp
endif
ifneq (,$(filter $(CONFIG_OPENTHREAD_ENABLE_DTLS),y yes YES 1))
	LIB_CPPSRC							+=										\
		$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MESHCOP_DIR)/dtls.cpp
endif
ifneq (,$(filter $(CONFIG_OPENTHREAD_ENABLE_JOINER),y yes YES 1))
	LIB_CPPSRC							+=										\
		$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MESHCOP_DIR)/joiner.cpp
endif
LIB_CPPSRC								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MESHCOP_DIR)/energy_scan_client.cpp		\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MESHCOP_DIR)/joiner_router.cpp			\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MESHCOP_DIR)/leader.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MESHCOP_DIR)/panid_query_client.cpp

INC_PATH								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_MESHCOP_DIR)
