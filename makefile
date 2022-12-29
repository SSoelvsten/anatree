.PHONY: help install uninstall test docs

help:
	@echo "Anatree : A Fast Data Structure for Anagrams"
	@echo "--------------------------------------------"
	@echo ""
	@echo "Targets"
	@echo "- help      : Prints this list of targets"
	@echo "- install   : Install the library"
	@echo "- uninstall : Uninstalls the library"
	@echo "- test      : Compile and run unit tests"
	@echo "- docs      : Generate Doxygen documentation"

install:
	@mkdir -p build/
	@cd build/ && cmake -D CMAKE_BUILD_TYPE=Release ..
	@cd build/ && make install

uninstall:
	@xargs -a build/install_manifest.txt rm

test:
	@mkdir -p build/
	@cd build/ && cmake -D CMAKE_BUILD_TYPE=Debug ..
	@cd build/ && make anatree_test
	@build/test/anatree_test

docs:
	@echo "No Doxygen documentation supported"
