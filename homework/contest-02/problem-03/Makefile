#===============================#
# List of tests for the problem #
#===============================#

# List of tests for current problem:
TESTS = \
	array-overflow-gen

# Include common Makefile:
include ../../common.mk

tests/array-overflow-gen.input: FORCE
	@mkdir -p tests
	@seq 10000000 20000000 > $@
