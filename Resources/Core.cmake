MESSAGE(STATUS
  "Building platform ${PROJECT_NAME}"
)

SET(PROJECT_EXECUTABLE_NAME "gimias")

##########################################################
#   Begin By Nacho ..
##########################################################
SET(LIBRARY_OUTPUT_PATH
  "${CILAB_TOOLKIT_BINARY_DIR}/bin"
  CACHE
  INTERNAL
  "Single output directory for building all libraries."
)

SET(EXECUTABLE_OUTPUT_PATH
  "${CILAB_TOOLKIT_BINARY_DIR}/bin"
  CACHE
  INTERNAL
  "Single output directory for building all libraries."
)

##########################################################
#   End By Nacho ..
##########################################################

SET(${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES
  ${${PROJECT_NAME}_SOURCE_DIR}/Core
  ${${PROJECT_NAME}_BINARY_DIR}/Core # to include configured files
  ${${PROJECT_NAME}_BINARY_DIR}
)

SET(CoreLib_DIR
  ${${PROJECT_NAME}_BINARY_DIR}
  CACHE
  INTERNAL
  ""
)

# Collect all the widgets in the subdirs
# Note that ${PLUGIN_NAME} is set in the cmakelists.txt of the core, via the macro NEW_CILAB_APPLICATION_START.
FILE(GLOB
  ${PLUGIN_NAME}_subdirContents
  "Core/Widgets/*"
)
SET(${PLUGIN_NAME}_moduleList "")
FOREACH(${PLUGIN_NAME}_DIRECTORY ${${PLUGIN_NAME}_subdirContents})
  IF(IS_DIRECTORY ${${PLUGIN_NAME}_DIRECTORY})
    STRING(REGEX MATCH "CVS$" ${PLUGIN_NAME}_IS_CVS ${${PLUGIN_NAME}_DIRECTORY})
    IF(NOT ${PLUGIN_NAME}_IS_CVS)
      SET(${PLUGIN_NAME}_moduleList
        ${${PLUGIN_NAME}_moduleList}
        ${${PLUGIN_NAME}_DIRECTORY}
      )
      SET(${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES
        ${${PLUGIN_NAME}_DIRECTORY}
        ${${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES}
      )
    ENDIF(NOT ${PLUGIN_NAME}_IS_CVS)
  ENDIF(IS_DIRECTORY ${${PLUGIN_NAME}_DIRECTORY})
ENDFOREACH(${PLUGIN_NAME}_DIRECTORY)

# This macro parses the ${PROJECT_NAME}_REQUIRED_PACKAGES variable
# and for each <package> name found there tries to include its
# <package>config.cmake & Use<package>.cmake file. If package is found
# this macro reads up the values ot the <package>_INCLUDE_DIRS,
# <package>_LIBRARY_DIRS and <package>_LIBRARIES to the 
# ${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES,
# ${PROJECT_NAME_CAPITAL}_LIBRARY_DIRECTORIES
# and ${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES respectively.
INCLUDE_EXTERNAL_PACKAGES()

SET(${PROJECT_NAME_CAPITAL}_OTHER_LIBRARIES
  ${${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES}
)

SET(${PROJECT_NAME_CAPITAL}_LIBRARY_DIRS
  "${CILAB_TOOLKIT_BINARY_DIR}/bin"
  ${${PROJECT_NAME_CAPITAL}_LIBRARY_DIRECTORIES}
)

#IF(WIN32)
  ADD_DEFINITIONS(${QT_DEFINITIONS})
#ENDIF(WIN32)

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

  # Find the directory within the path ${${PLUGIN_NAME}_SUBDIRECTORY}
  # For example: D:\Users\Maarten\Code\cvs_toolkit\cilabApps\LCore\Core\Widgets\DataEntityBrowser becomes
  # DataEntityBrowser
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
    "${${PLUGIN_NAME}_SUBDIRECTORY}/resource/*.txt"
  )
  # Copy all resource files to the current build
  INSTALL(FILES
    ${RESOURCE_FILES}
    DESTINATION "cilabApps/GIMIAS/resource"
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

#------------------------------------------------------------
# Configure cmake-generated files (no longer used)
#------------------------------------------------------------
#FILE(GLOB
#  ${PROJECT_NAME}_CMAKECONFIGURABLE_FILES
#  "Core/*.cxx.in"
#)
# GENERATE_CONFIGURABLE_FILES(${PROJECT_NAME}_CMAKECONFIGURATED_FILES ${${PROJECT_NAME}_CMAKECONFIGURABLE_FILES})

#------------------------------------------------------------
# Build library
#------------------------------------------------------------
INCLUDE_DIRECTORIES(${${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES})
LINK_DIRECTORIES(${${PROJECT_NAME_CAPITAL}_LIBRARY_DIRECTORIES})

#ADD_LIBRARY(${PLUGIN_NAME}_Widgets
#  STATIC
#  ${${PLUGIN_NAME}_FORM_FILES}
#  ${${PLUGIN_NAME}_SRC_FILES}
#  ${${PLUGIN_NAME}_H_FILES}
#  ${${PLUGIN_NAME}_UI_H_FILES}
#  ${${PLUGIN_NAME}_MOC_WRAPPED_SRC_FILES}
#  ${${PLUGIN_NAME}_UIWRAPPED_SRC_FILES}
#  ${${PLUGIN_NAME}_UIWRAPPED_H_FILES}
#)

SET(${PLUGIN_NAME}_Widgets_Sources
  ${${PLUGIN_NAME}_FORM_FILES}
  ${${PLUGIN_NAME}_SRC_FILES}
  ${${PLUGIN_NAME}_H_FILES}
  ${${PLUGIN_NAME}_UI_H_FILES}
  ${${PLUGIN_NAME}_MOC_WRAPPED_SRC_FILES}
  ${${PLUGIN_NAME}_UIWRAPPED_SRC_FILES}
  ${${PLUGIN_NAME}_UIWRAPPED_H_FILES}
)


# Add this dependency so that Qmitk gets compiled before CoreLib_Widgets
# We need the include directories of the Qmitk where UIC parser creates some
# include files that CoreLib_Widgets needs.
#ADD_DEPENDENCIES(${PLUGIN_NAME}_Widgets
#  Qmitk
#)

#Add the current directory also as there might be also cpp, or h files to be processed...
FILE(GLOB
  ${PLUGIN_NAME}_GIMIA_PLUGIN_SRCS
  "Core/*.cpp"
  "Core/*.cxx"
  "Core/*.txx"
)

#Add the current directory also as there might be also .ui, cpp, or h files to be processed...
FILE(GLOB
  ${PLUGIN_NAME}_GIMIA_PLUGIN_H
  "Core/*.h"
)

# Collect H files from all subdirectories
FILE(GLOB
  ${PLUGIN_NAME}_CORE_QOBJ_H
  "Core/*_QOBJ.h"
)

# wrap all _QOBJ.h files
IF(${PLUGIN_NAME}_CORE_QOBJ_H)
  QT_WRAP_CPP(${PLUGIN_NAME}
    ${PLUGIN_NAME}_GIMIA_PLUGIN_WRAPPED_SRC_FILES
    ${${PLUGIN_NAME}_CORE_QOBJ_H}
  )
ENDIF(${PLUGIN_NAME}_CORE_QOBJ_H)

ADD_LIBRARY(${PLUGIN_NAME}
  SHARED
  ${${PLUGIN_NAME}_Widgets_Sources}
  ${${PROJECT_NAME}_CMAKECONFIGURATED_FILES}
  ${${PROJECT_NAME}_CMAKECONFIGURABLE_FILES}
  ${${PLUGIN_NAME}_GIMIA_PLUGIN_SRCS}
  ${${PLUGIN_NAME}_GIMIA_PLUGIN_H}
  ${${PLUGIN_NAME}_GIMIA_PLUGIN_WRAPPED_SRC_FILES}
)

# Add this dependency so that Qmitk gets compiled before CoreLib_Widgets
# We need the include directories of the Qmitk where UIC parser creates some
# include files that CoreLib_Widgets needs.
ADD_DEPENDENCIES(${PLUGIN_NAME}
  Qmitk
)

# Linking the Core
FOREACH(DEBUG_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES})
	TARGET_LINK_LIBRARIES(${PLUGIN_NAME}
    debug ${DEBUG_LINK_LIBRARY}
  )
ENDFOREACH(DEBUG_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES})

FOREACH(RELEASE_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_RELEASE_LIBRARIES})
	TARGET_LINK_LIBRARIES(${PLUGIN_NAME}
    optimized ${RELEASE_LINK_LIBRARY}
  )
ENDFOREACH(RELEASE_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_RELEASE_LIBRARIES})


TARGET_LINK_LIBRARIES(${PLUGIN_NAME}
#  ${PLUGIN_NAME}_Widgets
  ${${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES}
)

SET(${PROJECT_NAME_CAPITAL}_LIBRARIES
  ${PLUGIN_NAME}
#  ${PLUGIN_NAME}_Widgets
)

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

      FOREACH(DEBUG_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES})
	      TARGET_LINK_LIBRARIES(${SUBDIRECTORY_NAME}_QTDesignerPlugin
          debug ${DEBUG_LINK_LIBRARY}
        )
      ENDFOREACH(DEBUG_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES})

      TARGET_LINK_LIBRARIES(${SUBDIRECTORY_NAME}_QTDesignerPlugin
	      ${${PROJECT_NAME_CAPITAL}_LIBRARIES}
#	      ${PLUGIN_NAME}_Widgets
	      ${${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES}
      )
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
    DESTINATION "cilabApps/GIMIAS"
  )
ELSE(WIN32)
  INSTALL(TARGETS
    ${PLUGIN_NAME}
    LIBRARY
    DESTINATION "cilabApps/GIMIAS/lib"
  )
ENDIF(WIN32)

SET(${PROJECT_NAME_CAPITAL}_PLUGINS_DIR
  "cilabApps/GIMIAS/plugins"
)

SET(${PROJECT_NAME_CAPITAL}_USE_FILE
  "${${PROJECT_NAME}_BINARY_DIR}/Use${PROJECT_NAME}.cmake"
)

CONFIGURE_FILE("${${PROJECT_NAME}_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
  "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  @ONLY
  IMMEDIATE
)

CONFIGURE_FILE("${${PROJECT_NAME}_SOURCE_DIR}/Use${PROJECT_NAME}.cmake.in"
  "${${PROJECT_NAME}_BINARY_DIR}/Use${PROJECT_NAME}.cmake"
  COPYONLY
  IMMEDIATE
)

#-------------------------------------
# Prepare sources for Main Application
#-------------------------------------
FILE(GLOB
  MAINAPP_SRC_FILES
  "Applications/*.cxx"
  "Applications/*.cpp"
)

FILE(GLOB
  MAINAPP_H_FILES
  "Applications/*.h"
  "Applications/*.txx"
  "Applications/*.ui"
  "Applications/*.ui.h"
)

# Build main
ADD_EXECUTABLE(${PROJECT_EXECUTABLE_NAME}
  ${MAINAPP_SRC_FILES}
  ${MAINAPP_H_FILES}
)
# For a STRANGE STRANGE reason is the only way to make it link with CoreLib under linux!
# Need futher investigation to find out why...
IF(NOT WIN32)
  TARGET_LINK_LIBRARIES(${PROJECT_EXECUTABLE_NAME}
    optimized CoreLib
#    optimized CoreLib_Widgets
    optimized qt-mt
    optimized Qmitk
    optimized mitkDataStructures
  )
  TARGET_LINK_LIBRARIES(${PROJECT_EXECUTABLE_NAME}
    debug CoreLib
#    debug CoreLib_Widgets
    debug qt-mt
    debug Qmitk
    debug mitkDataStructures
  )
ENDIF(NOT WIN32)

FOREACH(DEBUG_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES})
  TARGET_LINK_LIBRARIES(${PROJECT_EXECUTABLE_NAME}
    debug ${DEBUG_LINK_LIBRARY}
  )
ENDFOREACH(DEBUG_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES})

# The debug libraries must be included BEFORE the release (walkaround for Python)
TARGET_LINK_LIBRARIES(${PROJECT_EXECUTABLE_NAME}
  ${${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES}
  ${${PROJECT_NAME_CAPITAL}_LIBRARIES}
)

INSTALL(TARGETS
  ${PROJECT_EXECUTABLE_NAME}
  RUNTIME
  DESTINATION
  "cilabApps/GIMIAS"
)


#----------------------
# Install GIMIAS scripts
#----------------------

FILE(GLOB
  ${PLUGIN_NAME}_SCRIPT_FILES
  "Core/scripts/*.xpy"
  "Core/scripts/*.py"
)

IF(${PLUGIN_NAME}_SCRIPT_FILES)
  # Copy all resource files to the current build
  INSTALL(FILES
    ${${PLUGIN_NAME}_SCRIPT_FILES}
    DESTINATION
    "cilabApps/GIMIAS/scripts"
  )

ENDIF(${PLUGIN_NAME}_SCRIPT_FILES)

#------------------------------------------------------------
# Copyrights files to be installed in the /Copyrights dir.
#------------------------------------------------------------

# Get all .txt files
FILE(GLOB
  ${PROJECT_NAME}_COPYRIGHT_FILES
  "Copyrights/*Copyright.txt"
)

IF(${PROJECT_NAME}_COPYRIGHT_FILES)
  # Copy all Copyright files to the current build
  INSTALL(FILES
    ${${PROJECT_NAME}_COPYRIGHT_FILES}
    DESTINATION
    "cilabApps/GIMIAS/Copyrights"
  )
ENDIF(${PROJECT_NAME}_COPYRIGHT_FILES)

#------------------------------------------------------------
# Resource files (icons, images, sounds...) to be installed 
# in the release dir /resource
#------------------------------------------------------------

# Get all resource files
FILE(GLOB
  ${PROJECT_NAME}_RESOURCE_FILES
  "resource/*.bmp"
  "resource/*.jpg"
  "resource/*.png"
  "resource/*.ttf"
  "resource/*.dtd"
  "resource/*.xml"
  "resource/*.txt"
)

IF(${PROJECT_NAME}_RESOURCE_FILES)
  # Copy all resource files to the current build
  INSTALL(FILES
    ${${PROJECT_NAME}_RESOURCE_FILES}
    DESTINATION
    "cilabApps/GIMIAS/resource"
  )

ENDIF(${PROJECT_NAME}_RESOURCE_FILES)

# Get all demo data files
  FILE(GLOB ${PROJECT_NAME}_DEMO_DATA_FILES
    "data/*.vtk"
  )
  
# Copy all demo data files to the current build
IF(${PROJECT_NAME}_DEMO_DATA_FILES)
  INSTALL(FILES
    ${${PROJECT_NAME}_DEMO_DATA_FILES}
    DESTINATION "cilabApps/GIMIAS/data"
  )
ENDIF(${PROJECT_NAME}_DEMO_DATA_FILES)

#------------------------------------------------------------
# Test scripts and examples of the scripting interface
# to be installed in the release dir /scripts 
#------------------------------------------------------------

# Get all .xpy files
FILE(GLOB
  ${PROJECT_NAME}_TEST_FILES
  "*.xpy"
)

IF(${PROJECT_NAME}_TEST_FILES)
  # Copy all resource files to the current build
  INSTALL(FILES
    ${${PROJECT_NAME}_TEST_FILES}
    DESTINATION
    "cilabApps/GIMIAS/scripts"
  )

ENDIF(${PROJECT_NAME}_TEST_FILES)

# Get all extra files
FILE(GLOB
  EXTRA_FILES
  "Applications/*.xml"
)

# Copy all extra files to the current build
IF(EXTRA_FILES)
  INSTALL(FILES
    ${EXTRA_FILES}
    DESTINATION
    "cilabApps/GIMIAS"
  )
ENDIF(EXTRA_FILES)

INSTALL(FILES
  "Applications/_about_this_dir_3dparty.txt"
  DESTINATION
  "cilabApps/GIMIAS/thirdParty"
  RENAME "_about_this_dir.txt"
)
INSTALL(FILES
  "Applications/_about_this_dir_lib.txt"
  DESTINATION
  "cilabApps/GIMIAS/lib"
  RENAME "_about_this_dir.txt"
)
INSTALL(FILES
  "Applications/_about_this_dir_plugins.txt"
  DESTINATION
  "cilabApps/GIMIAS/plugins"
  RENAME "_about_this_dir.txt"
)
INSTALL(FILES
  "Applications/_about_this_dir_plugins_modules.txt"
  DESTINATION
  "cilabApps/GIMIAS/modules"
  RENAME "_about_this_dir.txt"
)

# Create the Python modules for the Core
#HANDLE_SWIG_WRAPPING()

# Generate LCore documentation
GENERATE_CILAB_PROJECT_DOCUMENTATION()

IF (WIN32)
  SOURCE_GROUP("Generated MOC Files" REGULAR_EXPRESSION moc_[a-zA-Z0-9_]*[.]cxx$)
  SOURCE_GROUP("Source Files" REGULAR_EXPRESSION [.]cxx$|[.]cpp$)
  SOURCE_GROUP("Input for source Files" REGULAR_EXPRESSION [.]in$)
  SOURCE_GROUP("Forms" REGULAR_EXPRESSION [.]ui$)
  SOURCE_GROUP("Forms Headers" REGULAR_EXPRESSION [.]ui[.]h$)
  SOURCE_GROUP("SWIG Interfaces" REGULAR_EXPRESSION [.]i$)
ENDIF(WIN32)
