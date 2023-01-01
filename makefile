.PHONY: help install uninstall test docs

help:
	@echo "Anatree : A Fast Data Structure for Anagrams"
	@echo "--------------------------------------------"
	@echo ""
	@echo "Targets"
	@echo "- help      : Prints this list of targets"
	@echo "- install   : Install the library"
	@echo "- uninstall : Uninstalls the library"
	@echo "- docs      : Generate Doxygen documentation"
	@echo "- test      : Compile and run unit tests"
	@echo "- coverage  : Compile and run unit tests with coverage"

# ============================================================================ #
#  INSTALLATION INTO '/usr/local/include'
# ============================================================================ #
install:
	@mkdir -p build/
	@cd build/ && cmake -D CMAKE_BUILD_TYPE=Release ..
	@cd build/ && make install

uninstall:
	@xargs -a build/install_manifest.txt rm

# ============================================================================ #
#  DOCUMENTATION
# ============================================================================ #
docs:
	@echo "No Doxygen documentation supported"

# ============================================================================ #
#  UNIT TESTS
# ============================================================================ #
O2_FLAGS = "-g -O2"

test:
	@mkdir -p build/
	@cd build/ && cmake -D CMAKE_BUILD_TYPE=Debug \
                      -D CMAKE_C_FLAGS=$(O2_FLAGS) \
                      -D CMAKE_CXX_FLAGS=$(O2_FLAGS) \
								..

	@cd build/ && make anatree_test

	@build/test/anatree_test --reporter=info --colorizer=light

# ============================================================================ #
#  LCOV COVERAGE REPORT
# ============================================================================ #
COV_C_FLAGS = "-g -O0 -Wall -fprofile-arcs -ftest-coverage"
COV_EXE_LINKER_FLAGS = "-fprofile-arcs -ftest-coverage"

coverage:
	@mkdir -p build/
	@cd build/ && cmake -D CMAKE_BUILD_TYPE=Debug \
                      -D CMAKE_C_FLAGS=$(COV_C_FLAGS) \
                      -D CMAKE_CXX_FLAGS=$(COV_C_FLAGS) \
                      -D CMAKE_EXE_LINKER_FLAGS=$(COV_EXE_LINKER_FLAGS) \
                ..
	@cd build/ && make $(MAKE_FLAGS) anatree_test

	@lcov --directory build/anatree/ --zerocounters

	@./build/test/anatree_test || echo ""

  # create report
	@rm -rf test/report/
	@lcov --capture --directory build/ --output-file ./coverage.info
	@lcov --remove coverage.info --output-file coverage.info "/usr/*" "*/external/*" "*test*"

  # print report to console
	@lcov --list coverage.info
  # print report to html file
	@genhtml coverage.info -o test/report/
