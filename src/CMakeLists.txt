add_definitions(
  -pthread
  -DPACKAGE_NAME="${PACKAGE_NAME}"
  -DPACKAGE_VERSION="${PACKAGE_VERSION}"
  -DPACKAGE_HOMEPAGE_URL="${PACKAGE_HOMEPAGE_URL}"
  -DSYSCONFDIR="${SYSCONFDIR}"
  -DLOGDIR="${LOGDIR}"
  -DPLUGINDIR="${PLUGINDIR}"
  -DVERSION="${PACKAGE_VERSION}"
)

set(sqltap_SRC
  Manifest.cpp
  Token.cpp
  Tokenizer.cpp
  Parser.cpp
  Query.cpp
  Parameter.cpp
  Field.cpp
  FieldValue.cpp
  AllFields.cpp
  DependantQuery.cpp
)

add_executable(sqltapd main.cpp ${sqltap_SRC})
target_link_libraries(sqltapd ${LIBXML_LIBRARIES} ${XZERO_LIBRARIES} ${SD_LIBRARIES} ${TCMALLOC_LIBRARIES})
set_target_properties(sqltapd PROPERTIES COMPILE_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden -DBUILD_X0D=1")
#set_target_properties(sqltapd PROPERTIES LINK_FLAGS "${BASE_LDFLAGS}")
install(TARGETS sqltapd RUNTIME DESTINATION sbin)
#install(FILES sqltapd.conf-dist DESTINATION ${SYSCONFDIR})

#add_subdirectory(plugins)

