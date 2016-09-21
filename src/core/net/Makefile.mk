################################################################################
# Library OpenThread (OT) - SRC/CORE/NET Rules
################################################################################

LIB_CPPSRC								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_NET_DIR)/icmp6.cpp						\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_NET_DIR)/ip6.cpp						\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_NET_DIR)/ip6_address.cpp				\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_NET_DIR)/ip6_filter.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_NET_DIR)/ip6_mpl.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_NET_DIR)/ip6_routes.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_NET_DIR)/netif.cpp						\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_NET_DIR)/udp6.cpp

INC_PATH								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_NET_DIR)
