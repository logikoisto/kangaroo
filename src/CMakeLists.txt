###################################################################
#SRC CMAKELISTS
###################################################################

file(GLOB_RECURSE srcs ${PROJECT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/*.h)

add_library(kangaroo STATIC ${srcs})

