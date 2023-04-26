set(LEX_SRCS "")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} LEX_SRCS)

add_library(LexLib STATIC ${LEX_SRCS})
target_precompile_headers(LexLib PRIVATE ${PCH_DIR})
target_include_directories(LexLib PRIVATE ${PROJECT_DIR})