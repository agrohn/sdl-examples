# - Find tmxparser source/include folder
# This module finds tmxparser if it is installed and determines where
# the files are. This code sets the following variables:
#
#  TMXPARSER_FOUND             - have tmxparser been found
#  TMXPARSER_INCLUDE_DIR       - path to where tmxparser.h is found
#
FIND_PATH(TMXPARSER_INCLUDE_DIR Tmx.h
  HINTS $ENV{TMXPARSERDIR}
  PATH_SUFFIXES include/tmxparser
  PATHS ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  ${CMAKE_SOURCE_DIR}/deps
  )
find_library(TMXPARSER_LIBRARY
        tmxparser
        HINTS $ENV{TINYXML2DIR}
        PATH_SUFFIXES lib64 lib
        PATHS ~/Library/Frameworks
         /Library/Frameworks
         /usr/local
         /usr
	 ${CMAKE_SOURCE_DIR}/deps
)
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TmxParser DEFAULT_MSG TMXPARSER_LIBRARY)

mark_as_advanced(TMXPARSER_LIBRARY TMXPARSER_INCLUDE_DIR)

