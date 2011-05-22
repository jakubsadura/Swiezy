MESSAGE(STATUS
  "Building plugin ${PROJECT_NAME}"
)

INCLUDE_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR})

# Collect all the widgets in the subdirs
FILE(GLOB ${PLUGIN_NAME}_subdirContents "widgets/*")
SET(${PLUGIN_NAME}_moduleList "")
FOREACH(${PLUGIN_NAME}_DIRECTORY ${${PLUGIN_NAME}_subdirContents})
  IF(IS_DIRECTORY ${${PLUGIN_NAME}_DIRECTORY})
    STRING(REGEX MATCH "CVS$" ${PLUGIN_NAME}_IS_CVS ${${PLUGIN_NAME}_DIRECTORY})
    IF(NOT ${PLUGIN_NAME}_IS_CVS)
      SET(${PLUGIN_NAME}_moduleList
	${${PLUGIN_NAME}_moduleList}
	${${PLUGIN_NAME}_DIRECTORY}
      )
      INCLUDE_DIRECTORIES(${${PLUGIN_NAME}_DIRECTORY})
    ENDIF(NOT ${PLUGIN_NAME}_IS_CVS)
  ENDIF(IS_DIRECTORY ${${PLUGIN_NAME}_DIRECTORY})
ENDFOREACH(${PLUGIN_NAME}_DIRECTORY)

INCLUDE_DIRECTORIES(${${PROJECT_NAME}_SOURCE_DIR})

# FOREACH(PACKAGE ${${PROJECT_NAME}_REQUIRED_PACKAGES})
#   IF(${PACKAGE} MATCHES "MITK*" OR ${PACKAGE} MATCHES "CoreLib*")
#     LIST(REMOVE_ITEM ${PROJECT_NAME}_REQUIRED_PACKAGES ${PACKAGE})
#     LIST(INSERT ${PROJECT_NAME}_REQUIRED_PACKAGES 0 ${PACKAGE})
#   ENDIF(${PACKAGE} MATCHES "MITK*" OR ${PACKAGE} MATCHES "CoreLib*")
# ENDFOREACH(PACKAGE)

# This macro parses the ${PROJECT_NAME}_REQUIRED_PACKAGES variable
# and for each <package> name found there tries to include its
# <package>config.cmake & Use<package>.cmake file. If package is found
# this macro reads up the values ot the <package>_INCLUDE_DIRS,
# <package>_LIBRARY_DIRS and <package>_LIBRARIES to the 
# ${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES,
# ${PROJECT_NAME_CAPITAL}_LIBRARY_DIRECTORIES
# and ${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES respectively.
INCLUDE_EXTERNAL_PACKAGES()

#------------------------------------------------------------
# Prepare sources
#------------------------------------------------------------
FOREACH( ${PLUGIN_NAME}_SUBDIRECTORY ${${PLUGIN_NAME}_moduleList})
  # Collect UI files from current subdirectory
  FILE(GLOB
    ${PLUGIN_NAME}_FORM_FILES_TMP
    "${${PLUGIN_NAME}_SUBDIRECTORY}/*.ui"
  )
  # Collect UI.H files from current subdirectory
   FILE(GLOB
    ${PLUGIN_NAME}_UI_H_FILES_TMP
    "${${PLUGIN_NAME}_SUBDIRECTORY}/*.ui.h"
  )

  STRING(REGEX REPLACE
    "^.*/" ""
    SUBDIRECTORY_NAME
    ${${PLUGIN_NAME}_SUBDIRECTORY}
  )

  # Collect source files from all subdirectories
  FILE(GLOB
    ${PLUGIN_NAME}_SRC_FILES_TMP
    "${${PLUGIN_NAME}_SUBDIRECTORY}/*.cxx"
    "${${PLUGIN_NAME}_SUBDIRECTORY}/*.cpp"
    "${${PLUGIN_NAME}_SUBDIRECTORY}/*.txx"
  )
  # Collect H files from all subdirectories
  FILE(GLOB
    ${PLUGIN_NAME}_H_FILES_TMP
    "${${PLUGIN_NAME}_SUBDIRECTORY}/*.h"
  )
  
  # Remove UI.H files from the _H_FILES_TMP variable
  FOREACH(ITEM ${${PLUGIN_NAME}_UI_H_FILES_TMP})
  LIST(REMOVE_ITEM
    ${PLUGIN_NAME}_H_FILES_TMP
    ${ITEM}
  )
  ENDFOREACH(ITEM)

  # Collect the current widget files and attach it to a global list
  SET(${PLUGIN_NAME}_FORM_FILES
    ${${PLUGIN_NAME}_FORM_FILES}
    ${${PLUGIN_NAME}_FORM_FILES_TMP}
  )
  SET(${PLUGIN_NAME}_SRC_FILES
    ${${PLUGIN_NAME}_SRC_FILES}
    ${${PLUGIN_NAME}_SRC_FILES_TMP}
  )
  SET(${PLUGIN_NAME}_H_FILES
    ${${PLUGIN_NAME}_H_FILES}
    ${${PLUGIN_NAME}_H_FILES_TMP}
  )
  SET(${PLUGIN_NAME}_UI_H_FILES
    ${${PLUGIN_NAME}_UI_H_FILES}
    ${${PLUGIN_NAME}_UI_H_FILES_TMP}
  )

  # Get all resource files
  FILE(GLOB RESOURCE_FILES
    "${${PLUGIN_NAME}_SUBDIRECTORY}/resource/*.bmp"
    "${${PLUGIN_NAME}_SUBDIRECTORY}/resource/*.jpg"
    "${${PLUGIN_NAME}_SUBDIRECTORY}/resource/*.png"
    "${${PLUGIN_NAME}_SUBDIRECTORY}/resource/*.ttf"
    "${${PLUGIN_NAME}_SUBDIRECTORY}/resource/*.dtd"
    "${${PLUGIN_NAME}_SUBDIRECTORY}/resource/*.xml"
  )
  # Copy all resource files to the current build
  INSTALL(FILES
    ${RESOURCE_FILES}
    DESTINATION "cilabApps/GIMIAS/plugins/${PLUGIN_NAME}/resource"
  )
  SET(${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES
    ${${PLUGIN_NAME}_SUBDIRECTORY}
    ${${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES}
  )
ENDFOREACH( ${PLUGIN_NAME}_SUBDIRECTORY ${${PLUGIN_NAME}_moduleList})

#
# After collecting all widgets, wrap and generate the full list
#

IF(${PLUGIN_NAME}_FORM_FILES)
  QT_WRAP_UI(${PLUGIN_NAME}
    ${PLUGIN_NAME}_UIWRAPPED_H_FILES
    ${PLUGIN_NAME}_UIWRAPPED_SRC_FILES
    ${${PLUGIN_NAME}_FORM_FILES}
  )
ENDIF(${PLUGIN_NAME}_FORM_FILES)


# wrap all .h files
IF(${PLUGIN_NAME}_H_FILES)
  QT_WRAP_CPP(${PLUGIN_NAME}
    ${PLUGIN_NAME}_MOC_WRAPPED_SRC_FILES
    ${${PLUGIN_NAME}_H_FILES}
  )
ENDIF(${PLUGIN_NAME}_H_FILES)

IF(EXISTS ${${PROJECT_NAME}_SOURCE_DIR}/data)
  # Get all data files
  FILE(GLOB
    PLUGIN_DATA_FILES
    "${${PROJECT_NAME}_SOURCE_DIR}/data/*.*"
  )

  # Remove CVS folder from file list
  LIST(REMOVE_ITEM
    PLUGIN_DATA_FILES
    "CVS"
    ".svn"
  )
  # Copy all resource files to the current build
  INSTALL(FILES
    ${PLUGIN_DATA_FILES}
    DESTINATION "cilabApps/GIMIAS/plugins/${PLUGIN_NAME}/data"
  )
ENDIF(EXISTS ${${PROJECT_NAME}_SOURCE_DIR}/data)

#------------------------------------------------------------
# Build library
#------------------------------------------------------------
INCLUDE_DIRECTORIES(${${PROJECT_NAME_CAPITAL}_INCLUDE_DIRS})
LINK_DIRECTORIES(${${PROJECT_NAME_CAPITAL}_LIBRARY_DIRECTORIES})

# CHECK for empty widgets directory
SET(${PLUGIN_NAME}_ALL_SOURCES
  ${${PLUGIN_NAME}_FORM_FILES}
  ${${PLUGIN_NAME}_SRC_FILES}
  ${${PLUGIN_NAME}_H_FILES}
  ${${PLUGIN_NAME}_UI_H_FILES}
  ${${PLUGIN_NAME}_MOC_WRAPPED_SRC_FILES}
  ${${PLUGIN_NAME}_UIWRAPPED_SRC_FILES}
  ${${PLUGIN_NAME}_UIWRAPPED_H_FILES}
)

IF(${PLUGIN_NAME}_ALL_SOURCES)
  ADD_LIBRARY(${PLUGIN_NAME}_Widgets
    STATIC
    ${${PLUGIN_NAME}_ALL_SOURCES}
  )
  SET(${PLUGIN_NAME}_Widgets_Built
    "TRUE"
  )
ENDIF(${PLUGIN_NAME}_ALL_SOURCES)

#Add the current directory also as there might be also cpp, or h files to be processed...
FILE(GLOB
  ${PLUGIN_NAME}_GIMIA_PLUGIN_SRCS
  "*.cpp"
  "*.cxx"
  "*.txx"
)

#Add the current directory also as there might be also .ui, cpp, or h files to be processed...
FILE(GLOB
  ${PLUGIN_NAME}_GIMIA_PLUGIN_H
  "*.h"
)
IF(${PLUGIN_NAME}_GIMIA_PLUGIN_H)
  QT_WRAP_CPP(${PLUGIN_NAME}
    ${PLUGIN_NAME}_GIMIA_PLUGIN_WRAPPED_SRC_FILES
    ${${PLUGIN_NAME}_GIMIA_PLUGIN_H}
  )
ENDIF(${PLUGIN_NAME}_GIMIA_PLUGIN_H)

ADD_LIBRARY(${PLUGIN_NAME}
  SHARED
  ${${PLUGIN_NAME}_GIMIA_PLUGIN_SRCS}
  ${${PLUGIN_NAME}_GIMIA_PLUGIN_H}
  ${${PLUGIN_NAME}_GIMIA_PLUGIN_WRAPPED_SRC_FILES}
)

IF(${PLUGIN_NAME}_Widgets_Built)
  TARGET_LINK_LIBRARIES(${PLUGIN_NAME}
    ${PLUGIN_NAME}_Widgets
    ${${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES}
  )
ELSE(${PLUGIN_NAME}_Widgets_Built)
  TARGET_LINK_LIBRARIES(${PLUGIN_NAME}
    ${${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES}
  )
ENDIF(${PLUGIN_NAME}_Widgets_Built)

FOREACH(DEBUG_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES})
  TARGET_LINK_LIBRARIES(${PLUGIN_NAME}
    debug ${DEBUG_LINK_LIBRARY}
  )
ENDFOREACH(DEBUG_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES})

OPTION(BUILD_QT_DESIGNER_PLUGINS "Build wrappers for showing widgets in Qt designer" OFF)

FOREACH(RELEASE_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_RELEASE_LIBRARIES})
  TARGET_LINK_LIBRARIES(${PLUGIN_NAME}
    optimized ${RELEASE_LINK_LIBRARY}
  )
ENDFOREACH(RELEASE_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_RELEASE_LIBRARIES})

IF(BUILD_QT_DESIGNER_PLUGINS)
  FOREACH(${PLUGIN_NAME}_SUBDIRECTORY ${${PLUGIN_NAME}_moduleList})
    STRING(REGEX REPLACE
      "^.*/" ""
      SUBDIRECTORY_NAME
      ${${PLUGIN_NAME}_SUBDIRECTORY}
    )
    #Collect source files for QT designer plugins (If available and chosen by user to be built)

    FILE(GLOB
      ${PLUGIN_NAME}_QTDESIGNERPLUGIN_SRC_FILES_TMP
      "${${PLUGIN_NAME}_SUBDIRECTORY}/QTDesignerPlugin/*.cxx"
      "${${PLUGIN_NAME}_SUBDIRECTORY}/QTDesignerPlugin/*.cpp"
      "${${PLUGIN_NAME}_SUBDIRECTORY}/QTDesignerPlugin/*.h"
    )

    #If there are files in the QT designer plugin directory, compile them
    IF(${PLUGIN_NAME}_QTDESIGNERPLUGIN_SRC_FILES_TMP)
      ADD_LIBRARY(${SUBDIRECTORY_NAME}_QTDesignerPlugin
        SHARED
        ${${PLUGIN_NAME}_QTDESIGNERPLUGIN_SRC_FILES_TMP}
      )
      IF(${PLUGIN_NAME}_Widgets_Built)
        TARGET_LINK_LIBRARIES(${SUBDIRECTORY_NAME}_QTDesignerPlugin
	        ${PLUGIN_NAME}_Widgets
  	      ${${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES}
        )
      ELSE(${PLUGIN_NAME}_Widgets_Built)
        TARGET_LINK_LIBRARIES(${SUBDIRECTORY_NAME}_QTDesignerPlugin
		      ${${PROJECT_NAME_CAPITAL}_LIBRARIES}
	        ${${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES}
        )
      ENDIF(${PLUGIN_NAME}_Widgets_Built)

      FOREACH(DEBUG_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES})
        TARGET_LINK_LIBRARIES(${SUBDIRECTORY_NAME}_QTDesignerPlugin
          debug ${DEBUG_LINK_LIBRARY}
        )
      ENDFOREACH(DEBUG_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES})

      IF(WIN32)
        INSTALL(TARGETS ${SUBDIRECTORY_NAME}_QTDesignerPlugin
          RUNTIME
          DESTINATION
          "cilabApps/GIMIAS/designer"
        )
      ELSE(WIN32)
        INSTALL(TARGETS ${SUBDIRECTORY_NAME}_QTDesignerPlugin
          LIBRARY
          DESTINATION 
          "cilabApps/GIMIAS/designer"
        )
      ENDIF(WIN32)
    ENDIF(${PLUGIN_NAME}_QTDESIGNERPLUGIN_SRC_FILES_TMP)
  ENDFOREACH( ${PLUGIN_NAME}_SUBDIRECTORY ${${PLUGIN_NAME}_moduleList})
ENDIF(BUILD_QT_DESIGNER_PLUGINS)

IF(WIN32)
  INSTALL(TARGETS
    ${PLUGIN_NAME}
    RUNTIME
    DESTINATION "cilabApps/GIMIAS/plugins/${PLUGIN_NAME}/lib"
  )
ELSE(WIN32)
  INSTALL(TARGETS
    ${PLUGIN_NAME}
    LIBRARY
    DESTINATION "cilabApps/GIMIAS/plugins/${PLUGIN_NAME}/lib"
  )
ENDIF(WIN32)

# Generate each plugins documentation
GENERATE_CILAB_PROJECT_DOCUMENTATION()

IF (WIN32)
  SOURCE_GROUP("Generated MOC Files" REGULAR_EXPRESSION moc_[a-zA-Z0-9_]*[.]cxx$)
  SOURCE_GROUP("Source Files" REGULAR_EXPRESSION [.]cxx$|[.]cpp$)
  SOURCE_GROUP("Input for source Files" REGULAR_EXPRESSION [.]in$)
  SOURCE_GROUP("Forms" REGULAR_EXPRESSION [.]ui$)
  SOURCE_GROUP("Forms Headers" REGULAR_EXPRESSION [.]ui[.]h$)
  SOURCE_GROUP("SWIG Interfaces" REGULAR_EXPRESSION [.]i$)
ENDIF(WIN32)
