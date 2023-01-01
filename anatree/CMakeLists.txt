cmake_minimum_required(VERSION 3.19)

project (anatree
  VERSION 1.0.0
  DESCRIPTION "Anatree: A Fast Data Structure for Anagrams"
  HOMEPAGE_URL "https://github.com/ssoelvsten/anatree/"
  LANGUAGES CXX
)

add_library(anatree INTERFACE)

target_compile_features(anatree INTERFACE cxx_std_20)

target_include_directories(anatree INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>)

install(TARGETS anatree EXPORT anatree_targets)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include)
install(EXPORT anatree_targets
  FILE anatree_config.cmake
  DESTINATION lib/cmake/anatree)
