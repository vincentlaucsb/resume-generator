set(TINYXML2_DIR ${CMAKE_CURRENT_LIST_DIR}/tinyxml2)
add_library(tinyxml2 STATIC ${TINYXML2_DIR}/tinyxml2.cpp ${TINYXML2_DIR}/tinyxml2.h)

set(PUGIXML_DIR ${CMAKE_CURRENT_LIST_DIR}/pugixml)
add_subdirectory(pugixml)
add_subdirectory(fmt)
add_subdirectory(internal)