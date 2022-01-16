SET( lib1name calicesim )

##########################################################
# include directories                                    #
##########################################################
#SET(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)

INCLUDE_DIRECTORIES( BEFORE "${${PROJECT_NAME}_SOURCE_DIR}/digitization/digitime/include" )
INCLUDE_DIRECTORIES( BEFORE "${${PROJECT_NAME}_SOURCE_DIR}/digitization/digihcal/include" )
INCLUDE_DIRECTORIES( BEFORE "${${PROJECT_NAME}_SOURCE_DIR}/digitization/digiecal/include" )
INCLUDE_DIRECTORIES( BEFORE "${${PROJECT_NAME}_SOURCE_DIR}/digitization/digitrack/include" )
INCLUDE_DIRECTORIES( BEFORE "${${PROJECT_NAME}_SOURCE_DIR}/digitization/ganging/include" )
INCLUDE_DIRECTORIES( BEFORE "${${PROJECT_NAME}_SOURCE_DIR}/digitization/digisim/include" )
INCLUDE_DIRECTORIES( BEFORE "${${PROJECT_NAME}_SOURCE_DIR}/digitization/sim/include" )
INCLUDE_DIRECTORIES( BEFORE "${${PROJECT_NAME}_SOURCE_DIR}/digitization/digiAhc2/include" )
# Test SiW ECAL
INCLUDE_DIRECTORIES( BEFORE "${${PROJECT_NAME}_SOURCE_DIR}/digitization/digiSiWECAL/include" )
INCLUDE_DIRECTORIES( BEFORE "${${PROJECT_NAME}_SOURCE_DIR}/digitization/FLATv01/include" )
INCLUDE_DIRECTORIES( BEFORE "${${PROJECT_NAME}_SOURCE_DIR}/digitization/SCH2ROOT/include" )
INCLUDE_DIRECTORIES( BEFORE "${${PROJECT_NAME}_SOURCE_DIR}/digitization/CH2ROOT/include" )
INCLUDE_DIRECTORIES( BEFORE "${${PROJECT_NAME}_SOURCE_DIR}/digitization/ShapingSiWECAL/include" )
INCLUDE_DIRECTORIES( BEFORE "${${PROJECT_NAME}_SOURCE_DIR}/digitization/ConversionSiWECAL/include" )
INCLUDE_DIRECTORIES( BEFORE "${${PROJECT_NAME}_SOURCE_DIR}/digitization/ShapedConversionSiWECAL/include" )

INCLUDE_DIRECTORIES( ${LCIO_INCLUDE_DIRS}
                     ${Marlin_INCLUDE_DIRS}
		     ${GEAR_INCLUDE_DIRS}
		     ${CLHEP_INCLUDE_DIRS}
		     ${ROOT_INCLUDE_DIRS}
		     ${RAW2CALOHIT_INCLUDE_DIRS}
		     ${RECOSIPM_INCLUDE_DIRS}
		     ${CELLGEOMETRY_INCLUDE_DIRS}
                     ${CALICE_USERLIB_INCLUDE_DIRS} )

IF( BUILD_WITH_USE_LCCD )
  INCLUDE_DIRECTORIES( ${LCCD_INCLUDE_DIRS}
                       ${CondDBMySQL_INCLUDE_DIRS} )
ENDIF()

IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  INCLUDE_DIRECTORIES( ${streamlog_INCLUDE_DIRS} )
ENDIF()

##########################################################
# sources                                                #
##########################################################
ADD_DEFINITIONS( ${${PROJECT_NAME}_DEFINITIONS}        )
ADD_DEFINITIONS( ${${PROJECT_NAME}_EXPORT_DEFINITIONS} )

AUX_SOURCE_DIRECTORY( ./digitime/src all_srcs )
AUX_SOURCE_DIRECTORY( ./digihcal/src all_srcs )
AUX_SOURCE_DIRECTORY( ./digihcal/src/NeighbourRelations all_srcs )
AUX_SOURCE_DIRECTORY( ./digihcal/src/TileInhomogeneities all_srcs )
AUX_SOURCE_DIRECTORY( ./digiecal/src all_srcs )
AUX_SOURCE_DIRECTORY( ./digitrack/src all_srcs )
AUX_SOURCE_DIRECTORY( ./ganging/src all_srcs )
AUX_SOURCE_DIRECTORY( ./digisim/src all_srcs )
AUX_SOURCE_DIRECTORY( ./sim/src all_srcs )
AUX_SOURCE_DIRECTORY( ./digiAhc2/src all_srcs )
# Test SiW ECAL
AUX_SOURCE_DIRECTORY( ./digiSiWECAL/src all_srcs )
AUX_SOURCE_DIRECTORY( ./SCH2ROOT/src all_srcs )
AUX_SOURCE_DIRECTORY( ./CH2ROOT/src all_srcs )
AUX_SOURCE_DIRECTORY( ./FLATv01/src all_srcs )
AUX_SOURCE_DIRECTORY( ./ShapingSiWECAL/src all_srcs )
AUX_SOURCE_DIRECTORY( ./ConversionSiWECAL/src all_srcs )
AUX_SOURCE_DIRECTORY( ./ShapedConversionSiWECAL/src all_srcs )

# remove "./" from the list of source files
STRING( REPLACE "./" "" all_srcs  "${all_srcs}" )

# library sources
SET( ${lib1name}_srcs ${all_srcs} )

##########################################################
# what to build                                          #
##########################################################
# require proper c++
SET_SOURCE_FILES_PROPERTIES(  ${${lib1name}_srcs} PROPERTIES
  COMPILE_FLAGS "${CMAKE_CXX_FLAGS}" )

ADD_LIBRARY( ${lib1name} ${${lib1name}_srcs} )

TARGET_LINK_LIBRARIES(${lib1name} ${LCIO_LIBRARIES}
${Marlin_LIBRARIES} ${GEAR_LIBRARIES} ${CLHEP_LIBRARIES}
${ROOT_LIBRARIES} ${RAW2CALOHIT_LIBRARIES} ${RECOSIPM_LIBRARIES}
${CELLGEOMETRY_LIBRARIES} ${CALICE_USERLIB_LIBRARIES} ${LCCD_LIBRARIES})

SET_TARGET_PROPERTIES( ${lib1name} PROPERTIES
		       VERSION ${${PROJECT_NAME}_VERSION}
                       SOVERSION ${${PROJECT_NAME}_SOVERSION}
                        # not needed at the moment (no build of
                        # static and dynamic lib at the same time)
                        # CLEAN_DIRECT_OUTPUT 1
                      )


#########################################################
# install                                               #
#########################################################
INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/digitization/digitime/include/"
         DESTINATION ${INCLUDE_INSTALL_DIR}
         PATTERN "*~" EXCLUDE
         PATTERN "*CVS*" EXCLUDE )

INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/digitization/digihcal/include/"
         DESTINATION ${INCLUDE_INSTALL_DIR}
         PATTERN "*~" EXCLUDE
         PATTERN "*CVS*" EXCLUDE )

INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/digitization/digiecal/include/"
         DESTINATION ${INCLUDE_INSTALL_DIR}
         PATTERN "*~" EXCLUDE
         PATTERN "*CVS*" EXCLUDE )

INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/digitization/digitrack/include/"
         DESTINATION ${INCLUDE_INSTALL_DIR}
         PATTERN "*~" EXCLUDE
         PATTERN "*CVS*" EXCLUDE )

INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/digitization/digisim/include/"
         DESTINATION ${INCLUDE_INSTALL_DIR}
         PATTERN "*~" EXCLUDE
         PATTERN "*CVS*" EXCLUDE )

INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/digitization/ganging/include/"
         DESTINATION ${INCLUDE_INSTALL_DIR}
         PATTERN "*~" EXCLUDE
         PATTERN "*CVS*" EXCLUDE )

INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/digitization/digiAhc2/include/"
         DESTINATION ${INCLUDE_INSTALL_DIR}
         PATTERN "*~" EXCLUDE
         PATTERN "*CVS*" EXCLUDE )

# Test SiW ECAL
INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/digitization/digiSiWECAL/include/"
         DESTINATION ${INCLUDE_INSTALL_DIR}
         PATTERN "*~" EXCLUDE
         PATTERN "*CVS*" EXCLUDE )

# Test SCH2ROOT 
INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/digitization/SCH2ROOT/include/"
         DESTINATION ${INCLUDE_INSTALL_DIR}
         PATTERN "*~" EXCLUDE
         PATTERN "*CVS*" EXCLUDE )

# Test CH2ROOT 
INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/digitization/CH2ROOT/include/"
         DESTINATION ${INCLUDE_INSTALL_DIR}
         PATTERN "*~" EXCLUDE
         PATTERN "*CVS*" EXCLUDE )

# Test FLATv01 
INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/digitization/FLATv01/include/"
         DESTINATION ${INCLUDE_INSTALL_DIR}
         PATTERN "*~" EXCLUDE
         PATTERN "*CVS*" EXCLUDE )

# Test Shaping
INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/digitization/ShapingSiWECAL/include/"
         DESTINATION ${INCLUDE_INSTALL_DIR}
         PATTERN "*~" EXCLUDE
         PATTERN "*CVS*" EXCLUDE )

# Test Shaping
INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/digitization/ConversionSiWECAL/include/"
         DESTINATION ${INCLUDE_INSTALL_DIR}
         PATTERN "*~" EXCLUDE
         PATTERN "*CVS*" EXCLUDE )

# Test Shaping
INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/digitization/ShapedConversionSiWECAL/include/"
         DESTINATION ${INCLUDE_INSTALL_DIR}
         PATTERN "*~" EXCLUDE
         PATTERN "*CVS*" EXCLUDE )



 INSTALL( TARGETS ${lib1name}
 	 DESTINATION ${LIB_INSTALL_DIR}
 	 PERMISSIONS
 	 OWNER_READ OWNER_WRITE OWNER_EXECUTE
 	 GROUP_READ GROUP_EXECUTE
 	 WORLD_READ WORLD_EXECUTE  )


#############################################################
# prepare for XXXXConfig.cmake                              #
#############################################################
GET_TARGET_PROPERTY(${Lib1name}_prefix ${lib1name} PREFIX)
GET_TARGET_PROPERTY(${Lib1name}_suffix ${lib1name} SUFFIX)
SET(lib_${lib1name}_name ${${Lib1name}_prefix}${Lib1name}${${Lib1name}_suffix})
