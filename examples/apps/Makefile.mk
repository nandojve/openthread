################################################################################
# Library OpenThread (OT) - Examples/Apps Rules
################################################################################

LIBRARY_STACK_OPENTHREAD_EXAMPLES_APPS_x_DIR	= $(LIBRARY_STACK_OPENTHREAD_EXAMPLES_APPS_DIR)/$(CONFIG_OPENTHREAD_APPLICATION)

include $(LIBRARY_STACK_OPENTHREAD_EXAMPLES_APPS_x_DIR)/Makefile.mk
