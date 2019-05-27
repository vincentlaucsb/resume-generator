set(PUGIXML_DIR ${CMAKE_CURRENT_LIST_DIR}/pugixml)
set(FMTLIB_DIR ${CMAKE_CURRENT_LIST_DIR}/fmt)
set(CTML_DIR ${CMAKE_CURRENT_LIST_DIR}/CTML)

add_subdirectory(pugixml)
add_subdirectory(fmt)
add_subdirectory(CTML)
add_subdirectory(internal)