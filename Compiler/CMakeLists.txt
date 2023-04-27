set(APP_SRCS "")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} APP_SRCS)

add_executable(Compiler ${APP_SRCS})
target_precompile_headers(Compiler PRIVATE ${PCH_DIR})
target_include_directories(Compiler PRIVATE ${PROJECT_DIR} PRIVATE ${LEX_DIR} PRIVATE ${PARSE_DIR})
target_link_directories(Compiler PRIVATE ${LEX_BIN_DIR} ${PARSE_BIN_DIR})
target_link_libraries(Compiler PRIVATE ParseLib LexLib)

#Post Build
#add_custom_command(TARGET Compiler POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LEX_BIN_DIR}/LexLib.dll ${APP_BIN_DIR}/LexLib.dll)
#add_custom_command(TARGET Compiler POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PARSE_BIN_DIR}/ParseLib.dll ${APP_BIN_DIR}/ParseLib.dll)