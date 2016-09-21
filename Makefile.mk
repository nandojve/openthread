################################################################################
# Library OpenThread (OT) Rules
################################################################################

LIBRARY_STACK_OPENTHREAD_SRC_DIR		= $(LIBRARY_STACK_OPENTHREAD_DIR)/src
LIBRARY_STACK_OPENTHREAD_EXAMPLES_DIR	= $(LIBRARY_STACK_OPENTHREAD_DIR)/examples

include $(LIBRARY_STACK_OPENTHREAD_SRC_DIR)/Makefile.mk
include $(LIBRARY_STACK_OPENTHREAD_EXAMPLES_DIR)/Makefile.mk

INC_PATH								+=										\
	$(LIBRARY_STACK_OPENTHREAD_DIR)/include										\
	$(LIBRARY_STACK_OPENTHREAD_DIR)/include/cli									\
	$(LIBRARY_STACK_OPENTHREAD_DIR)/include/commissioning						\
	$(LIBRARY_STACK_OPENTHREAD_DIR)/include/ncp									\
	$(LIBRARY_STACK_OPENTHREAD_DIR)/include/platform