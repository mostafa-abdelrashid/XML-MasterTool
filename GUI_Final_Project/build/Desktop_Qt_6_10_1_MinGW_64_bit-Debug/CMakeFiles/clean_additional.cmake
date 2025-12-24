# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\XML_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\XML_autogen.dir\\ParseCache.txt"
  "XML_autogen"
  )
endif()
