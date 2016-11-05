################################################################################
# Library OpenThread (OT) - SRC Rules
################################################################################

LIBRARY_STACK_OPENTHREAD_SRC_CLI_DIR		= $(LIBRARY_STACK_OPENTHREAD_SRC_DIR)/cli
LIBRARY_STACK_OPENTHREAD_SRC_CORE_DIR		= $(LIBRARY_STACK_OPENTHREAD_SRC_DIR)/core
LIBRARY_STACK_OPENTHREAD_SRC_DIAG_DIR		= $(LIBRARY_STACK_OPENTHREAD_SRC_DIR)/diag
LIBRARY_STACK_OPENTHREAD_SRC_MISSING_DIR	= $(LIBRARY_STACK_OPENTHREAD_SRC_DIR)/missing
LIBRARY_STACK_OPENTHREAD_SRC_NCP_DIR		= $(LIBRARY_STACK_OPENTHREAD_SRC_DIR)/ncp

ifneq (,$(filter $(CONFIG_OT_IFACE_CLI),y yes YES 1))
	include $(LIBRARY_STACK_OPENTHREAD_SRC_CLI_DIR)/Makefile.mk
endif
ifneq (,$(filter $(CONFIG_OT_IFACE_NCP),y yes YES 1))
	include $(LIBRARY_STACK_OPENTHREAD_SRC_NCP_DIR)/Makefile.mk
endif
ifneq (,$(filter $(CONFIG_OT_DIAG),y yes YES 1))
	LIB_CFLAGS	+= -DOPENTHREAD_ENABLE_DIAG=1
	include $(LIBRARY_STACK_OPENTHREAD_SRC_DIAG_DIR)/Makefile.mk
else
	LIB_CFLAGS	+= -DOPENTHREAD_ENABLE_DIAG=0
endif
ifneq (,$(filter $(CONFIG_OT_MULTIPLE_INSTANCE),y yes YES 1))
	LIB_CFLAGS	+= -DOPENTHREAD_MULTIPLE_INSTANCE=1
else
	LIB_CFLAGS	+= -DOPENTHREAD_MULTIPLE_INSTANCE=0
endif
#include $(LIBRARY_STACK_OPENTHREAD_SRC_MISSING_DIR)/Makefile.mk
include $(LIBRARY_STACK_OPENTHREAD_SRC_CORE_DIR)/Makefile.mk

INC_PATH								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_DIR)