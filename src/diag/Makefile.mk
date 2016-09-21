################################################################################
# Library OpenThread (OT) - SRC/DIAG Rules
################################################################################

LIB_CPPSRC							+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_DIAG_DIR)/diag_process.cpp				\
	$(LIBRARY_STACK_OPENTHREAD_SRC_DIAG_DIR)/openthread-diag.cpp

INC_PATH							+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_DIAG_DIR)
