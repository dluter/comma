SET( PROJECT "math" )
SET( TARGET_NAME comma${PROJECT} )

FILE( GLOB source   ${SOURCE_CODE_BASE_DIR}/${PROJECT}/*.cpp)
FILE( GLOB includes ${SOURCE_CODE_BASE_DIR}/${PROJECT}/*.h)

SOURCE_GROUP( ${TARGET_NAME} FILES ${source} ${includes} )

add_custom_target( ${TARGET_NAME} ${source} ${includes} ) # ADD_LIBRARY( ${TARGET_NAME} ${source} ${includes} )
#SET_TARGET_PROPERTIES( ${TARGET_NAME} PROPERTIES ${comma_LIBRARY_PROPERTIES} )
#TARGET_LINK_LIBRARIES( ${TARGET_NAME} comma_base )

INSTALL( FILES ${includes} DESTINATION ${comma_INSTALL_INCLUDE_DIR}/${PROJECT} )
#INSTALL(
#    TARGETS ${TARGET_NAME}
#    RUNTIME DESTINATION ${comma_INSTALL_BIN_DIR} COMPONENT Runtime     # .exe, .dll
#    LIBRARY DESTINATION ${comma_INSTALL_LIB_DIR} COMPONENT Runtime     # .so, mod.dll
#    ARCHIVE DESTINATION ${comma_INSTALL_LIB_DIR} COMPONENT Development # .a, .lib
#)

IF( comma_BUILD_TESTS )
    ADD_SUBDIRECTORY( test )
ENDIF( comma_BUILD_TESTS )
