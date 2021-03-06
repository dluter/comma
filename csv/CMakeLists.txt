SET( PROJECT "csv" )
SET( TARGET_NAME comma_${PROJECT} )
SET( dir ${SOURCE_CODE_BASE_DIR}/${PROJECT} )

FILE( GLOB source ${dir}/*.cpp )
FILE( GLOB includes ${dir}/*.h )
FILE( GLOB impl_includes ${dir}/impl/*.h )

SOURCE_GROUP( ${TARGET_NAME} FILES ${source} ${includes} ${impl_includes} )

ADD_LIBRARY( ${TARGET_NAME} ${source} ${includes} ${impl_includes} )
SET_TARGET_PROPERTIES( ${TARGET_NAME} PROPERTIES ${comma_LIBRARY_PROPERTIES} )
TARGET_LINK_LIBRARIES( ${TARGET_NAME} comma_application ${comma_ALL_EXTERNAL_LIBRARIES} )

INSTALL( FILES ${includes} DESTINATION ${comma_INSTALL_INCLUDE_DIR}/${PROJECT}/ )
INSTALL( FILES ${impl_includes} DESTINATION ${comma_INSTALL_INCLUDE_DIR}/${PROJECT}/impl )
INSTALL(
    TARGETS ${TARGET_NAME}
    RUNTIME DESTINATION ${comma_INSTALL_BIN_DIR} COMPONENT Runtime     # .exe, .dll
    LIBRARY DESTINATION ${comma_INSTALL_LIB_DIR} COMPONENT Runtime     # .so, mod.dll
    ARCHIVE DESTINATION ${comma_INSTALL_LIB_DIR} COMPONENT Development # .a, .lib
)

IF( comma_BUILD_APPLICATIONS )
    ADD_SUBDIRECTORY( applications )
ENDIF( comma_BUILD_APPLICATIONS )

IF( comma_BUILD_TESTS )
    ADD_SUBDIRECTORY( test )
ENDIF( comma_BUILD_TESTS )
