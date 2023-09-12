This directory exists because CMake poses arbitrary limitation to what one can
do inside the main CMakeLists.txt files.
- It allows to control input/output of external scripts (e.g. to quiet the
compilation of python).
- It also permits to execute basic code (if/then/else, foreach, etc.) using
CMake syntax when calling "make <target>", instead of writing shell scripts
which tend to be less portable.

Each file is used as an external function that may be called in main files as follows.
  ${CMAKE_COMMAND} -D "<arg1_name>=<arg1_value>" -D "<arg2_name>=<arg2_value>"  ...
                   -P "${CMAKE_SOURCE_DIR}/CMakeScripts/<file_name>"
