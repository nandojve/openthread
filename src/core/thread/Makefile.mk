################################################################################
# Library OpenThread (OT) - SRC/CORE/THREAD Rules
################################################################################

LIB_CPPSRC								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/address_resolver.cpp		\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/energy_scan_server.cpp		\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/key_manager.cpp				\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/link_quality.cpp			\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/lowpan.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/mesh_forwarder.cpp			\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/meshcop_dataset.cpp			\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/meshcop_dataset_manager.cpp	\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/meshcop_tlvs.cpp			\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/mle.cpp						\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/mle_router.cpp				\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/mle_router.cpp				\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/network_data.cpp			\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/network_data_leader.cpp		\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/network_data_local.cpp		\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/panid_query_server.cpp		\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/thread_netif.cpp			\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)/thread_tlvs.cpp				\

INC_PATH								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_THREAD_DIR)
