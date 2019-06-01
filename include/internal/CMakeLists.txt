add_executable(resume "")
target_sources(resume
	PRIVATE
		common.h
		processor.cpp
		processor.h
		resume.cpp
		resume.h
		xml_node.h
)

target_link_libraries(resume pugixml fmt)

target_include_directories(resume
	PRIVATE
		${PUGIXML_DIR}/src
		${FMTLIB_DIR}/include
		${CTML_DIR}/include
)