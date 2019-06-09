add_executable(resume_test "")
add_test(resume_test resume_test)
target_sources(resume_test
    PRIVATE
    catch.hpp
    main.cpp
    test_text.cpp
    test_custom_xml_processor.cpp
)
target_link_libraries(resume_test resume_lib)
target_include_directories(resume_test PRIVATE ${RESUME_ROOT_DIR}/include)