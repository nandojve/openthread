################################################################################
# Library OpenThread (OT) - SRC/NCP Rules
################################################################################

LIB_CSRC							+=											\
	$(LIBRARY_STACK_OPENTHREAD_SRC_NCP_DIR)/spinel.c

LIB_CPPSRC							+=											\
	$(LIBRARY_STACK_OPENTHREAD_SRC_NCP_DIR)/flen.c								\
	$(LIBRARY_STACK_OPENTHREAD_SRC_NCP_DIR)/hdlc.cpp							\
	$(LIBRARY_STACK_OPENTHREAD_SRC_NCP_DIR)/ncp_base.cpp						\
	$(LIBRARY_STACK_OPENTHREAD_SRC_NCP_DIR)/ncp_buffer.cpp						\
	$(LIBRARY_STACK_OPENTHREAD_SRC_NCP_DIR)/ncp_spi.cpp							\
	$(LIBRARY_STACK_OPENTHREAD_SRC_NCP_DIR)/ncp_uart.cpp

INC_PATH								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_NCP_DIR)
