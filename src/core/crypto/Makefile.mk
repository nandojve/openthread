################################################################################
# Library OpenThread (OT) - SRC/CORE/CRYPTO Rules
################################################################################

LIB_CPPSRC								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_CRYPTO_DIR)/aes_ccm.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_CRYPTO_DIR)/aes_ecb.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_CRYPTO_DIR)/hmac_sha256.cpp				\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_CRYPTO_DIR)/mbedtls.cpp					\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_CRYPTO_DIR)/sha256.cpp

INC_PATH								+=										\
	$(LIBRARY_STACK_OPENTHREAD_SRC_CORE_CRYPTO_DIR)
