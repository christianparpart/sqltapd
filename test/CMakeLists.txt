if(ENABLE_TESTS)

  add_executable(sqltap-test sqltap-test.cpp
    Tokenizer-test.cpp
  )
  target_link_libraries(sqltap-test gtest sqltap)

  add_custom_target(test-sqltap
    DEPENDS sqltap-test
    COMMAND ./test-sqltap ${CMAKE_CURRENT_SOURCE_DIR}
  )

endif()
