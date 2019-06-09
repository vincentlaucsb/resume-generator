set(PUGIXML_DIR ${CMAKE_CURRENT_LIST_DIR}/pugixml)
set(FMTLIB_DIR ${CMAKE_CURRENT_LIST_DIR}/fmt)
set(CTML_DIR ${CMAKE_CURRENT_LIST_DIR}/CTML)
set(CXXOPTS_DIR ${CMAKE_CURRENT_LIST_DIR}/cxxopts)

add_subdirectory(pugixml)
add_subdirectory(fmt)
add_subdirectory(CTML)
add_subdirectory(cxxopts)

add_executable(resume "")
target_sources(resume
	PRIVATE
        common.h
        custom_tag.h
        processor.cpp
        processor.h
        resume.cpp
        resume.h
        resume_parser.cpp
        text.h
        text.cpp
        xml_node.h
        xml_node.cpp
)

target_link_libraries(resume pugixml fmt)

target_include_directories(resume
	PRIVATE
		${PUGIXML_DIR}/src
		${FMTLIB_DIR}/include
		${CTML_DIR}/include
        ${CXXOPTS_DIR}/include
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

install(
    TARGETS resume
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
)