# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\calenderProject_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\calenderProject_autogen.dir\\ParseCache.txt"
  "calenderProject_autogen"
  )
endif()
