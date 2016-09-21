################################################################################
# Library OpenThread (OT) - Examples Rules
################################################################################

LIBRARY_STACK_OPENTHREAD_EXAMPLES_APPS_DIR		= $(LIBRARY_STACK_OPENTHREAD_EXAMPLES_DIR)/apps
LIBRARY_STACK_OPENTHREAD_EXAMPLES_PLATFORM_DIR	= $(LIBRARY_STACK_OPENTHREAD_EXAMPLES_DIR)/platforms

include $(LIBRARY_STACK_OPENTHREAD_EXAMPLES_APPS_DIR)/Makefile.mk
include $(LIBRARY_STACK_OPENTHREAD_EXAMPLES_PLATFORM_DIR)/Makefile.mk
