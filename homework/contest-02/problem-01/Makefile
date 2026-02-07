#===============================#
# List of tests for the problem #
#===============================#

# List of tests for current problem:
TESTS = \
	stack-overflow-gen \
	end-sequence-gen

# Include common Makefile:
include ../../common.mk

tests/stack-overflow-gen.input: FORCE
	@mkdir -p tests
	@seq 200000 400000 > $@

tests/end-sequence-gen.input: FORCE
	@mkdir -p tests
	@seq 100 200 > $@
