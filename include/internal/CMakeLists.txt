add_executable(resume "")
target_sources(resume
	PRIVATE
		resume.cpp
		resume.h
)

target_link_libraries(resume pugixml fmt)

target_include_directories(resume
	PRIVATE
		${PUGIXML_DIR}/src
		${FMTLIB_DIR}/include
)