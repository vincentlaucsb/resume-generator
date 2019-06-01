add_executable(resume "")
target_sources(resume
	PRIVATE
		common.h
		processor.cpp
		processor.h
		resume.cpp
		resume.h
		xml_node.h
		xml_node.cpp
)

target_link_libraries(resume pugixml fmt)

target_include_directories(resume
	PRIVATE
		${PUGIXML_DIR}/src
		${FMTLIB_DIR}/include
		${CTML_DIR}/include
)

# Copy examples into build directory for easier debugging
add_custom_target(copy_examples
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${RESUME_ROOT_DIR}/examples $<TARGET_FILE_DIR:resume>
)

add_custom_command(
	TARGET resume POST_BUILD
	COMMAND ${copy_examples}
)
