#########################################################
# Hopefully we don't have to touch below this point!
#########################################################

# Turn on/off all the options of the package
OPTION(SEE_${PROJECT_NAME_CAPITAL}_LIB_OPTIONS
  "See the configuration options of the ${PROJECT_NAME} cilab module."
  OFF
)
#------------------------------------------------------------
# VTK library
#------------------------------------------------------------

# This macro parses the ${PROJECT_NAME}_REQUIRED_PACKAGES variable
# and for each <package> name found there tries to include its
# <package>config.cmake & Use<package>.cmake file. If package is found
# this macro reads up the values ot the <package>_INCLUDE_DIRS,
# <package>_LIBRARY_DIRS and <package>_LIBRARIES to the 
# ${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES,
# ${PROJECT_NAME_CAPITAL}_LIBRARY_DIRECTORIES
# and ${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES respectively.
INCLUDE_EXTERNAL_PACKAGES()

# Propagate the libraries that we depend on to the that will inlucde us.
SET(${PROJECT_NAME_CAPITAL}_OTHER_LIBRARIES
  ${${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES}
)

# Fix a problem on starting the cmake procedure with a 
# fresh list of LIBRARY_MODULES_ACTUALLY_BUILT
SET("${PROJECT_NAME}_DIR"
  "${${PROJECT_NAME}_BINARY_DIR}"
   CACHE INTERNAL
  "Path to the ${PROJECT_NAME}"
)

# These directories are always needed.
SET(${PROJECT_NAME}_INCLUDE_DIRS_BUILD_TREE
  ${${PROJECT_NAME}_BINARY_DIR}
  ${${PROJECT_NAME}_SOURCE_DIR}
)

# Foreach directory in the libmodules directory
# include sources in the include directories.
FOREACH(LIBRARY_MODULE ${${PROJECT_NAME}_LIBRARY_MODULES})
  SET(${PROJECT_NAME}_INCLUDE_DIRS_BUILD_TREE
    "${${PROJECT_NAME}_INCLUDE_DIRS_BUILD_TREE}"
    ${${PROJECT_NAME}_SOURCE_DIR}/libmodules/${LIBRARY_MODULE}/include
    ${${PROJECT_NAME}_SOURCE_DIR}/libmodules/${LIBRARY_MODULE}/src
  )
ENDFOREACH(LIBRARY_MODULE)

# ADD new include directories
INCLUDE_DIRECTORIES(${${PROJECT_NAME}_INCLUDE_DIRS_BUILD_TREE})

#---------------------------------------
# Selecting Static vs Shared compilation
#---------------------------------------
IF(SEE_${PROJECT_NAME_CAPITAL}_LIB_OPTIONS)
  OPTION(${PROJECT_NAME_CAPITAL}_BUILD_SHARED_LIBS
    "Build as shared library"
    OFF
  )
ENDIF(SEE_${PROJECT_NAME_CAPITAL}_LIB_OPTIONS)

IF(${PROJECT_NAME_CAPITAL}_BUILD_SHARED_LIBS)
  SET(${PROJECT_NAME_CAPITAL}_BUILD_TYPE "SHARED")
  IF(WIN32)
  	# Note:: This actually is done automatically from the CMake. That means that if we want to generate
  	# for example the evoIO, evoCommno ... DLL libraries, CMake will generate each of this projects with
  	# evoIO_EXPORTS, evoCommno_EXPORTS ... defined for each project. The problem is that all classes are 
  	# prepended by the EvoLib_EXPORTS macro... This issue is in all CILab modules and it occurs because
  	# originally each library was not modular so EvoLib would generate EvoLib.lib instead of generating
  	# a lib for each of it's modules.
  	ADD_DEFINITIONS(/D${PROJECT_NAME}_EXPORTS)
  ENDIF(WIN32)
ELSE(${PROJECT_NAME_CAPITAL}_BUILD_SHARED_LIBS)
  SET(${PROJECT_NAME_CAPITAL}_BUILD_TYPE "STATIC")
ENDIF(${PROJECT_NAME_CAPITAL}_BUILD_SHARED_LIBS)


#--------------------
# LIBRARY OUTPUT PATH
#--------------------

# In windows put libraries in the same subdirectory with the executables.
# In case of libraries compiled in DLL will make the application work
# (DLLs should be in the same folder with the executable or be in the path)
# In linux, i separate libraries in /lib while executables in /bin. If the 
# libraries are compiled in shared (.so), before executing the applications
# the LD_LIBRARY_PATH environmet variable should be set to the shared libraries 
# directory (/lib)
IF(WIN32)
  SET(LIBRARY_OUTPUT_PATH
    ${${PROJECT_NAME}_BINARY_DIR}/bin
    CACHE
    INTERNAL
    "Single output directory for building all libraries."
  )
  SET(EXECUTABLE_OUTPUT_PATH
    "${${PROJECT_NAME}_BINARY_DIR}/bin"
    CACHE
    INTERNAL
    "Single output directory for building all libraries."
  )
ELSE(WIN32)
  SET(LIBRARY_OUTPUT_PATH
    ${${PROJECT_NAME}_BINARY_DIR}/lib
    CACHE
    INTERNAL
    "Single output directory for building all libraries."
  )
  SET(EXECUTABLE_OUTPUT_PATH
    "${${PROJECT_NAME}_BINARY_DIR}/bin"
    CACHE
    INTERNAL
    "Single output directory for building all libraries."
  )
ENDIF(WIN32)



# The project will create libraries in the LIBRARY_OUTPUT_PATH. However, if the project creates two libraries (libA and libB)
# then libA may be an input to libB. Therefore, the LIBRARY_OUTPUT_PATH is also a linker input path.
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

#----------------------------------------------------------
# Build library modules found in the "libmodules" directory
#----------------------------------------------------------
# Foreach directory in the libmodules directory, compile the
# sources to provide an object file (static or dynamic library)
# for the code of each directory
FOREACH(MODULE_NAME ${${PROJECT_NAME}_LIBRARY_MODULES})
  #The the name of the library module directory in capital
  STRING(TOUPPER
    ${MODULE_NAME}
    MODULE_NAME_CAPITAL
  )

  ##FIXME :: HACE ALGO ESTO????#####
  SET(${MODULE_NAME_CAPITAL}_PATH
    "${${PROJECT_NAME}_SOURCE_DIR}/libmodules/${MODULE_NAME}"
  )
  ###################################

  # Read up all header files to the ${MODULE_NAME_CAPITAL}_SOURCES_H variable
  FILE(GLOB
    ${MODULE_NAME_CAPITAL}_SOURCES_H
    "${${PROJECT_NAME}_SOURCE_DIR}/libmodules/${MODULE_NAME}/include/*.h"
  )
  # Read up all template files to the ${MODULE_NAME_CAPITAL}_SOURCES_TXX variable
  # We consider them having a .txx extension
  FILE(GLOB
    ${MODULE_NAME_CAPITAL}_SOURCES_TXX
    "${${PROJECT_NAME}_SOURCE_DIR}/libmodules/${MODULE_NAME}/src/*.txx"
  )
  # Read up all source files to the ${MODULE_NAME_CAPITAL}_SOURCES_CPP variable
  # All .cxx, cpp & cc files are valid extensions for a C++ source file.
  FILE(GLOB
    ${MODULE_NAME_CAPITAL}_SOURCES_CPP
    "${${PROJECT_NAME}_SOURCE_DIR}/libmodules/${MODULE_NAME}/src/*.cpp"
    "${${PROJECT_NAME}_SOURCE_DIR}/libmodules/${MODULE_NAME}/src/*.cc"
    "${${PROJECT_NAME}_SOURCE_DIR}/libmodules/${MODULE_NAME}/src/*.cxx"
  )

  # Add the three variables defined above to the ${MODULE_NAME_CAPITAL}_SOURCES variable
  SET(${MODULE_NAME_CAPITAL}_SOURCES
    "${${MODULE_NAME_CAPITAL}_SOURCES_H}"
    "${${MODULE_NAME_CAPITAL}_SOURCES_TXX}"
    "${${MODULE_NAME_CAPITAL}_SOURCES_CPP}"
  )

  # If there are sources that need to be compiled,
  # compile, build the library module and make the install rule.
  # No every directory builds a library, this happens in the case of a
  # library module that consists on template and header files only.
  # In that case nothing needs to be compiled.
  IF(${MODULE_NAME_CAPITAL}_SOURCES_CPP)
    # Build the library
    ADD_LIBRARY(${MODULE_NAME}
      ${${PROJECT_NAME_CAPITAL}_BUILD_TYPE}
      ${${MODULE_NAME_CAPITAL}_SOURCES}
      ${${MODULE_NAME_CAPITAL}_MOC_SOURCES}
    )
      
    IF(WIN32)
      IF(${${PROJECT_NAME_CAPITAL}_BUILD_TYPE} STREQUAL "SHARED")
        # Add inter-dependencies of libraries inside a module
        FOREACH(DEPENDENCY ${${PROJECT_NAME_CAPITAL}_LIBRARY_MODULE_${MODULE_NAME}_DEPENDENCIES})
		      TARGET_LINK_LIBRARIES(${MODULE_NAME} ${DEPENDENCY})
        ENDFOREACH(DEPENDENCY ${${PROJECT_NAME_CAPITAL}_LIBRARY_MODULE_${MODULE_NAME}_DEPENDENCIES})
		   
		    
      	# Some debug libraries are named differently to the release version
				IF(${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES)
	
	        FOREACH(DEBUG_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES})
		        TARGET_LINK_LIBRARIES(${MODULE_NAME}
	            debug ${DEBUG_LINK_LIBRARY}
	          )
	        ENDFOREACH(DEBUG_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES})

	      ENDIF(${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES)
	      
	      # Some release libraries are named differently to the debug version
				IF(${PROJECT_NAME_CAPITAL}_RELEASE_LIBRARIES)
	
	      FOREACH(RELEASE_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_RELEASE_LIBRARIES})
        	TARGET_LINK_LIBRARIES(${PLUGIN_NAME}
            optimized ${RELEASE_LINK_LIBRARY}
          )
        ENDFOREACH(RELEASE_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_RELEASE_LIBRARIES})

        ENDIF(${PROJECT_NAME_CAPITAL}_RELEASE_LIBRARIES)
        
	      
	      # Link any other library
        TARGET_LINK_LIBRARIES(${MODULE_NAME}
            ${${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES}
#            ${${PROJECT_NAME}_LIBRARY_MODULES_ACTUALLY_BUILT} #Do not use this, use ADD_MODULE_INTERDEPENDENCY macro instead
        )
      ENDIF(${${PROJECT_NAME_CAPITAL}_BUILD_TYPE} STREQUAL "SHARED")
    ENDIF(WIN32)

    # Install the produced library under the ${CMAKE_INSTALL_PREFIX}/lib/cilabModules/${PROJECT_NAME}
    INSTALL(TARGETS
      ${MODULE_NAME}
      LIBRARY DESTINATION "lib/cilabModules/${PROJECT_NAME}"
      ARCHIVE DESTINATION "lib/cilabModules/${PROJECT_NAME}"
    )
    IF(${PROJECT_NAME_CAPITAL}_INSTALL_DIR_FOR_THE_MODULE)
      INSTALL(TARGETS ${MODULE_NAME} RUNTIME DESTINATION 
        ${${PROJECT_NAME_CAPITAL}_INSTALL_DIR_FOR_THE_MODULE}
      )
    ENDIF(${PROJECT_NAME_CAPITAL}_INSTALL_DIR_FOR_THE_MODULE)

    # Set the module name to the list of all modules beeing built
    # and produced a library. This is done for the modules that only
    # have headers and templates and do not actually produce a library.
    SET(${PROJECT_NAME}_LIBRARY_MODULES_ACTUALLY_BUILT
      ${MODULE_NAME}
      ${${PROJECT_NAME}_LIBRARY_MODULES_ACTUALLY_BUILT}
    )
  ENDIF(${MODULE_NAME_CAPITAL}_SOURCES_CPP)

  # Install the header & template files under the ${CMAKE_INSTALL_PREFIX}/include/cilabModules/${PROJECT_NAME}
  INSTALL(FILES
    ${${MODULE_NAME_CAPITAL}_SOURCES_H}
    ${${MODULE_NAME_CAPITAL}_SOURCES_TXX}
    DESTINATION
    "include/cilabModules/${PROJECT_NAME}"
  )
ENDFOREACH(MODULE_NAME)

# Link applications with the libraries that the required packages
# link, plus the libraries were produced by this project.
SET(APPLICATION_LINK_LIBRARIES
  ${${PROJECT_NAME}_LIBRARY_MODULES_ACTUALLY_BUILT}
  ${${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES}
)
# Link demos with the same libraries as the applications.
SET(DEMO_LINK_LIBRARIES
  ${APPLICATION_LINK_LIBRARIES}
)


#----------------------------------
# The libraries are built. Now remove the EXPORTS definition or you may create
# incorrect builds for the applications (because using dllexport instead of dllimport)
#----------------------------------
IF(${PROJECT_NAME_CAPITAL}_BUILD_SHARED_LIBS)
  IF(WIN32)
  	REMOVE_DEFINITIONS("/D${PROJECT_NAME}_EXPORTS")
  ENDIF(WIN32)
ENDIF(${PROJECT_NAME_CAPITAL}_BUILD_SHARED_LIBS)

#----------------------------------
# Applications, demos & api options
#----------------------------------
IF(SEE_${PROJECT_NAME_CAPITAL}_LIB_OPTIONS)
  # Check if exists "Application" directory
  IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Applications)
    OPTION(${PROJECT_NAME_CAPITAL}_BUILD_APPLICATIONS
      "Build applications." 
      OFF
    )
  ENDIF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Applications)

  # Check if exists "demos" directory
  IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/demos)
    OPTION(${PROJECT_NAME_CAPITAL}_BUILD_DEMOS
      "Build demo applications."
      OFF
    )
  ENDIF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/demos)
ENDIF(SEE_${PROJECT_NAME_CAPITAL}_LIB_OPTIONS)


#--------------------------------------------------------
# Build applications found in the "Application" directory
#--------------------------------------------------------
IF(${PROJECT_NAME_CAPITAL}_BUILD_APPLICATIONS)
  #For each application declared with the ADD_APPLICATION_MODULES
  # macro in the user-defined macro
  FOREACH(APPLICATION_MODULE ${${PROJECT_NAME}_APPLICATION_MODULES})
    # Read source names to the APPS_SOURCES_CPP variable
    FILE(GLOB
      ${APPLICATION_MODULE}_APPS_SOURCES_CPP
      RELATIVE "${${PROJECT_NAME}_SOURCE_DIR}/Applications/${APPLICATION_MODULE}"
      "${${PROJECT_NAME}_SOURCE_DIR}/Applications/${APPLICATION_MODULE}/*.cpp"
      "${${PROJECT_NAME}_SOURCE_DIR}/Applications/${APPLICATION_MODULE}/*.cc"
      "${${PROJECT_NAME}_SOURCE_DIR}/Applications/${APPLICATION_MODULE}/*.cxx"
    )

    FOREACH(APPLICATION_NAME ${${APPLICATION_MODULE}_APPS_SOURCES_CPP})
      # Strips extentions from the source files
      STRING(REGEX REPLACE
        ".cpp|.cxx|.cc"
        ""
        APPLICATION
        ${APPLICATION_NAME}
      )

      # add an exe that consists solely of the current source file
      ADD_EXECUTABLE(${APPLICATION} "${${PROJECT_NAME}_SOURCE_DIR}/Applications/${APPLICATION_MODULE}/${APPLICATION}")

      # Some debug libraries are named differently to the release version
			IF(${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES)

        FOREACH(DEBUG_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES})
	        TARGET_LINK_LIBRARIES(${APPLICATION}
            debug ${DEBUG_LINK_LIBRARY}
          )
        ENDFOREACH(DEBUG_LINK_LIBRARY ${${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES})
      
      ENDIF(${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES)
      TARGET_LINK_LIBRARIES(${APPLICATION} ${APPLICATION_LINK_LIBRARIES})

			# Finally add definitions
      SET_TARGET_PROPERTIES(${APPLICATION} 
      PROPERTIES 
        LINK_FLAGS "${${PROJECT_NAME_CAPITAL}_APP_${APPLICATION}_LINK_FLAGS}"
      )

      INSTALL(TARGETS
        ${APPLICATION}
        RUNTIME
        DESTINATION
        "bin"
      )
    ENDFOREACH(APPLICATION_NAME)
  ENDFOREACH(APPLICATION_MODULE ${${PROJECT_NAME}_APPLICATION_MODULES})
ENDIF(${PROJECT_NAME_CAPITAL}_BUILD_APPLICATIONS)

#------------------------------------------
# Build demos found in the "demo" directory
#------------------------------------------
IF(${PROJECT_NAME_CAPITAL}_BUILD_DEMOS)
  FOREACH(DEMO_MODULE ${${PROJECT_NAME}_DEMO_MODULES})
    # Read header names to the DEMOS_SOURCES_H variable
    FILE(GLOB
      ${DEMO_MODULE}_DEMOS_SOURCES_H
      "${${PROJECT_NAME}_SOURCE_DIR}/demos/${DEMO_MODULE}/*.h"
    )
    
    # Read source names to the DEMOS_SOURCES_CPP variable
    FILE(GLOB
      ${DEMO_MODULE}_DEMOS_SOURCES_CPP
      RELATIVE "${${PROJECT_NAME}_SOURCE_DIR}/demos/${DEMO_MODULE}"
      "${${PROJECT_NAME}_SOURCE_DIR}/demos/${DEMO_MODULE}/*.cpp"
      "${${PROJECT_NAME}_SOURCE_DIR}/demos/${DEMO_MODULE}/*.cc"
      "${${PROJECT_NAME}_SOURCE_DIR}/demos/${DEMO_MODULE}/*.cxx"
    )

    FOREACH(DEMO_NAME ${${DEMO_MODULE}_DEMOS_SOURCES_CPP})
      # Strips extentions from the source files
      STRING(REGEX REPLACE
        ".cpp|.cxx|.cc"
        ""
        DEMO
        ${DEMO_NAME}
      )

      ADD_EXECUTABLE(${DEMO} "${${PROJECT_NAME}_SOURCE_DIR}/demos/${DEMO_MODULE}/${DEMO}" ${${DEMO_MODULE}_DEMOS_SOURCES_H})
      TARGET_LINK_LIBRARIES(${DEMO} ${DEMO_LINK_LIBRARIES})
      
      # HACK: if the demo is ${PROJECT_NAME}TestRunner, then add a custom rule to rebuild TestRunner.cpp
      IF( DEMO MATCHES "${PROJECT_NAME}TestRunner" )
		FIND_FILE(PYTHON_INTERPRETER NAMES python.exe python PATHS ENV PATH)
		
      	ADD_CUSTOM_COMMAND(
      		TARGET ${DEMO}
      		PRE_BUILD
      		COMMAND ${PYTHON_INTERPRETER} ../../../../thirdParty/CXXTEST/CxxTest/cxxtestgen.py --error-printer -o ${DEMO_NAME} ./*.h 
      		WORKING_DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/demos/${DEMO_MODULE}"
      		COMMENT "Regenerating test runner"
      		VERBATIM
      	)
      ENDIF( DEMO MATCHES "${PROJECT_NAME}TestRunner" )
      
      INSTALL(TARGETS
        ${DEMO}
        RUNTIME
        DESTINATION
        "bin"
      )
    ENDFOREACH(DEMO_NAME)
  ENDFOREACH(DEMO_MODULE ${${PROJECT_NAME}_DEMO_MODULES})
ENDIF(${PROJECT_NAME_CAPITAL}_BUILD_DEMOS)

#------------------------------------------------------------
# Configure files with settings for use by the build.
#------------------------------------------------------------
CONFIGURE_FILE(${${PROJECT_NAME}_SOURCE_DIR}/${PROJECT_NAME}Configure.h.in
  ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Configure.h
)

#######################################################################
# Help other projects use EvoLib
# DEFINED FOR EvoLibConfig.cmake.in file
#######################################################################

#-----------------------------------------------
# Library produced by this project
#-----------------------------------------------
CLEAR_VARIABLE(${PROJECT_NAME_CAPITAL}_LIBRARIES)
SET(${PROJECT_NAME_CAPITAL}_LIBRARIES
  ${${PROJECT_NAME}_LIBRARY_MODULES_ACTUALLY_BUILT}
)

#-----------------------------------------------
# Include directories
#-----------------------------------------------
SET(${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES
  "${${PROJECT_NAME}_INCLUDE_DIRS_BUILD_TREE}"
  "${${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES}"
)

#-----------------------------------------------
# Lbrary directories
#-----------------------------------------------
CLEAR_VARIABLE(${PROJECT_NAME_CAPITAL}_LIBRARY_DIRECTORIES)
SET(${PROJECT_NAME_CAPITAL}_LIBRARY_DIRECTORIES
  ${LIBRARY_OUTPUT_PATH}
  ${${PROJECT_NAME_CAPITAL}_LIBRARY_DIRECTORIES}
)

#-----------------------------------------------
# Creating files for external projects
# This are configured to use the build 
# directories instead the install ones
#-----------------------------------------------

# Copy the UseEvoLib.cmake file to the binary tree for backward compatability.
CONFIGURE_FILE("${${PROJECT_NAME}_SOURCE_DIR}/Use${PROJECT_NAME}.cmake.in"
  "${${PROJECT_NAME}_BINARY_DIR}/Use${PROJECT_NAME}.cmake"
  COPYONLY
  IMMEDIATE
)

# The "use" file.
SET(${PROJECT_NAME_CAPITAL}_USE_FILE 
  "${${PROJECT_NAME}_BINARY_DIR}/Use${PROJECT_NAME}.cmake"
)

# Configure EvoLibConfig.cmake for the build tree.
CONFIGURE_FILE(
  ${${PROJECT_NAME}_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in
  "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  @ONLY
  IMMEDIATE
)

#-----------------------------------------------
# Creating files for external projects
# This time configure them to use the 
# directories from the installation tree
#-----------------------------------------------
# Configure <ProjectName>Config.cmake for the install tree.
# Read the appropriate <PACKAGE>Config.cmake files
# with the paths to the installation tree
# We assume the same directory layout of the include
# directories in the SOURCE DIR and INSTALL DIR
# So we just replace the SOURCE_DIR_PREFIX with
# the INSTALL_DIR_PREFIX in the
# ${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES
# string.
CLEAR_VARIABLE(${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES)
CLEAR_VARIABLE(${PROJECT_NAME_CAPITAL}_LIBRARY_DIRECTORIES)

FOREACH(REQUIRED_PACKAGE ${${PROJECT_NAME}_REQUIRED_PACKAGES})
  # For each package in the required packages include the ${REQUIRED_PACKAGE}Config.cmake 
  # that is found in the Utilities directory in the build tree. In the utilities directory
  # is the version of the file which uses the installation directories instead of the "build" ones.
  # We are not sure if is "thirdParty", "cilabModule" or an external package required package so we 
  # look in all three places...
  INCLUDE(${CMAKE_BINARY_DIR}/thirdParty/${REQUIRED_PACKAGE}/Utilities/${REQUIRED_PACKAGE}Config.cmake
    OPTIONAL
  )
  INCLUDE(${CMAKE_BINARY_DIR}/cilabModules/${REQUIRED_PACKAGE}/Utilities/${REQUIRED_PACKAGE}Config.cmake
    OPTIONAL
  )
  INCLUDE(${${REQUIRED_PACKAGE}_DIR}/Utilities/${REQUIRED_PACKAGE}Config.cmake
    OPTIONAL
  )

  # When later someone wants to use the use-file from the install folder, he/she will also want to search the include folders
  # of the packages that are required by PROJECT_NAME_CAPITAL.
  SET(${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES
    "${${REQUIRED_PACKAGE}_INCLUDE_DIRS}"
    "${${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES}"
  )
  SET(${PROJECT_NAME_CAPITAL}_LIBRARY_DIRECTORIES
    "${${REQUIRED_PACKAGE}_LIBRARY_DIRS}"
    "${${PROJECT_NAME_CAPITAL}_LIBRARY_DIRECTORIES}"
  )
ENDFOREACH(REQUIRED_PACKAGE)

# The "use" file.
SET(${PROJECT_NAME_CAPITAL}_USE_FILE
  "${CMAKE_INSTALL_PREFIX}/Use${PROJECT_NAME}.cmake"
)

CONFIGURE_FILE(
  "${${PROJECT_NAME}_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
  "${${PROJECT_NAME}_BINARY_DIR}/Utilities/${PROJECT_NAME}Config.cmake"
  @ONLY
  IMMEDIATE
)

INSTALL(FILES
  "${${PROJECT_NAME}_BINARY_DIR}/Use${PROJECT_NAME}.cmake"
  "${${PROJECT_NAME}_BINARY_DIR}/Utilities/${PROJECT_NAME}Config.cmake"
  DESTINATION
  ""
)

# In the ${EvoLib_BINARY_DIR}/Utilities/ we have the tailored version
# of the EvoLibConfig.cmake file with the paths of the includes and 
# libraries to the installation tree instead of the binary one.
IF(EXISTS "${${PROJECT_NAME}_SOURCE_DIR}/${PROJECT_NAME}Win32Header.h")
  INSTALL(FILES
    "${PROJECT_NAME}Win32Header.h"
    DESTINATION 
    "include/cilabModules/${PROJECT_NAME}"
  )
ENDIF(EXISTS "${${PROJECT_NAME}_SOURCE_DIR}/${PROJECT_NAME}Win32Header.h")

IF(EXISTS "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Configure.h")
  INSTALL(FILES
    "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Configure.h"
    DESTINATION 
    "include/cilabModules/${PROJECT_NAME}"
  )
ENDIF(EXISTS "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Configure.h")

CLEAR_VARIABLE(${PROJECT_NAME}_LIBRARY_MODULES_ACTUALLY_BUILT)
CLEAR_VARIABLE(${PROJECT_NAME}_INCLUDE_DIRS_BUILD_TREE)
CLEAR_VARIABLE(${PROJECT_NAME_CAPITAL}_LIBRARIES)
CLEAR_VARIABLE(${PROJECT_NAME_CAPITAL}_LIBRARY_DIRECTORIES)
CLEAR_VARIABLE(${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES)
CLEAR_VARIABLE(${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES)
CLEAR_VARIABLE(${PROJECT_NAME_CAPITAL}_RELEASE_LIBRARIES)
CLEAR_VARIABLE(${PROJECT_NAME_CAPITAL}_LIBRARY_MODULE_${MODULE_NAME}_DEPENDENCIES)

# Generate each project's documentation
GENERATE_CILAB_PROJECT_DOCUMENTATION()

IF (WIN32)
  SOURCE_GROUP("Generated MOC Files" REGULAR_EXPRESSION moc_[a-zA-Z0-9_]*[.]cxx$)
  SOURCE_GROUP("Source Files" REGULAR_EXPRESSION [.]cxx$|[.]cpp$)
  SOURCE_GROUP("Input for source Files" REGULAR_EXPRESSION [.]in$)
  SOURCE_GROUP("Forms" REGULAR_EXPRESSION [.]ui$)
  SOURCE_GROUP("Forms Headers" REGULAR_EXPRESSION [.]ui[.]h$)
  SOURCE_GROUP("SWIG Interfaces" REGULAR_EXPRESSION [.]i$)
  SOURCE_GROUP("Templates" REGULAR_EXPRESSION [.]txx$)
ENDIF(WIN32)
