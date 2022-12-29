add_library(anatree INTERFACE)

target_include_directories(anatree INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(anatree INTERFACE cxx_std_17)

target_include_directories(
  anatree INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>)

install(TARGETS anatree EXPORT anatree_targets)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include)
install(EXPORT anatree_targets
  FILE anatree_config.cmake
  DESTINATION lib/cmake/anatree)
