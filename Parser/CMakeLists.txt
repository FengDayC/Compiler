set(PARSE_SRCS "")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PARSE_SRCS)

add_library(ParseLib STATIC ${PARSE_SRCS})
target_precompile_headers(ParseLib PRIVATE ${PCH_DIR})
target_include_directories(ParseLib PRIVATE ${PROJECT_DIR})
target_include_directories(ParseLib PRIVATE ${LEX_DIR})