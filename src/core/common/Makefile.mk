################################################################################
# Library OpenThread (OT) - SRC/CORE/COMMON Rules
################################################################################

LIB_CPPSRC								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_COMMON_DIR)/logging.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_COMMON_DIR)/message.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_COMMON_DIR)/tasklet.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_COMMON_DIR)/timer.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_COMMON_DIR)/trickle_timer.cpp

INC_PATH								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_COMMON_DIR)
