################################################################################
# Library OpenThread (OT) - Examples/Platform Rules
################################################################################

LIBRARY_STACK_OPENTHREAD_EXAMPLES_PLATFORM_x_DIR		= $(LIBRARY_STACK_OPENTHREAD_EXAMPLES_PLATFORM_DIR)/$(CONFIG_OPENTHREAD_PLATFORM)

include $(LIBRARY_STACK_OPENTHREAD_EXAMPLES_PLATFORM_x_DIR)/Makefile.mk
