SH = bash
BT_SCRIPT = ./batchtest.sh

# Path to the root of the tests. contains subdirectories of test inputs.
ROOT_OF_TESTS = ./resource/test/

# iterate all the test directories that starts with "test".
test_all:
	@for dir in $(ROOT_OF_TESTS)test*; do \
		$(SH) $(BT_SCRIPT) -v test $$dir; \
	done

# run batchtest.sh -v clean $$dir for all test directories.
clean_test_all:
	@for dir in $(ROOT_OF_TESTS)test*; do \
		$(SH) $(BT_SCRIPT) -v clean $$dir; \
	done
