################################################################################
# Library OpenThread (OT) - SRC/CORE/COAP Rules
################################################################################

LIB_CPPSRC								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_COAP_DIR)/coap_header.cpp				\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_COAP_DIR)/coap_server.cpp

INC_PATH								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_COAP_DIR)
