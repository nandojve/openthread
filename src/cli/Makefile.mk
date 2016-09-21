################################################################################
# Library OpenThread (OT) - SRC/CLI Rules
################################################################################

LIB_CPPSRC							+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CLI_DIR)/cli.cpp							\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CLI_DIR)/cli_console.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CLI_DIR)/cli_dataset.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CLI_DIR)/cli_uart.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CLI_DIR)/cli_udp.cpp

INC_PATH								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CLI_DIR)
