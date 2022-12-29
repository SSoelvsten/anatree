cmake_minimum_required(VERSION 3.12)

enable_language(CXX)

project (anatree
  VERSION 1.0.0
  DESCRIPTION "Anatree: A Fast Data Structure for Anagrams"
  HOMEPAGE_URL "https://github.com/ssoelvsten/anatree/"
  LANGUAGES CXX
)

# ============================================================================ #
# Library Source Code
# ============================================================================ #
add_subdirectory (src)

# ============================================================================ #
# Unit Tests
# ============================================================================ #
# add_subdirectory (test)
