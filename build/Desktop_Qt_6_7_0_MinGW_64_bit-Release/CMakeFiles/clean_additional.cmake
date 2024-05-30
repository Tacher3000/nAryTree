# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\nAryTree_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\nAryTree_autogen.dir\\ParseCache.txt"
  "nAryTree_autogen"
  )
endif()
