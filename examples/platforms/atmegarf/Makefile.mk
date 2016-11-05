################################################################################
# Library OpenThread (OT) - Examples/Platform/atmegarf Rules
################################################################################

LIB_CSRC							+=											\
	$(LIBRARY_STACK_OPENTHREAD_EXAMPLES_PLATFORM_x_DIR)/alarm.c					\
	$(LIBRARY_STACK_OPENTHREAD_EXAMPLES_PLATFORM_x_DIR)/diag.c					\
	$(LIBRARY_STACK_OPENTHREAD_EXAMPLES_PLATFORM_x_DIR)/logging.c				\
	$(LIBRARY_STACK_OPENTHREAD_EXAMPLES_PLATFORM_x_DIR)/misc.c					\
	$(LIBRARY_STACK_OPENTHREAD_EXAMPLES_PLATFORM_x_DIR)/platform.c				\
	$(LIBRARY_STACK_OPENTHREAD_EXAMPLES_PLATFORM_x_DIR)/radio.c					\
	$(LIBRARY_STACK_OPENTHREAD_EXAMPLES_PLATFORM_x_DIR)/random.c				\
	$(LIBRARY_STACK_OPENTHREAD_EXAMPLES_PLATFORM_x_DIR)/uart.c

INC_PATH								+=										\
	$(LIBRARY_STACK_OPENTHREAD_EXAMPLES_PLATFORM_x_DIR)
