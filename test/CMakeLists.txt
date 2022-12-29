add_executable (anatree_test test.cpp)

# Link to Bandit
set(BANDIT_FLAGS "--reporter=info --colorizer=light")
target_include_directories (anatree_test PRIVATE ${PROJECT_SOURCE_DIR}/external)

# Link to Anatree
target_link_libraries(anatree_test anatree)
