MACRO(ADD_REQUIRED_PACKAGES)
  # Iterate over all values in the input argument (which is a list)
  FOREACH(REQUIRED_PACKAGE ${ARGV})
  
  	# Copy ${REQUIRED_PACKAGE} to the ${PROJECT_NAME}_REQUIRED_PACKAGES list
    IF(${PROJECT_NAME}_REQUIRED_PACKAGES)
      SET(${PROJECT_NAME}_REQUIRED_PACKAGES
        ${REQUIRED_PACKAGE}
        ${${PROJECT_NAME}_REQUIRED_PACKAGES}
      )
    ELSE(${PROJECT_NAME}_REQUIRED_PACKAGES)
      SET(${PROJECT_NAME}_REQUIRED_PACKAGES
        ${REQUIRED_PACKAGE}
      )
    ENDIF(${PROJECT_NAME}_REQUIRED_PACKAGES)
    
    # Add ${REQUIRED_PACKAGE} to the ${PROJECT_NAME}_DEPENDS_FROM list
    SET(${PROJECT_NAME}_DEPENDS_FROM
      ${REQUIRED_PACKAGE}
      ${${PROJECT_NAME}_DEPENDS_FROM}
      CACHE
      INTERNAL
      "Cilab Projects from which ${PROJECT_NAME} depends."
      FORCE
    )
  ENDFOREACH(REQUIRED_PACKAGE)
ENDMACRO(ADD_REQUIRED_PACKAGES)

MACRO(ADD_MODULE_INTERDEPENDENCY TARGET_OF_DEPENDENCY "REQUIRES" ${ARGV})

##
## Adds dependency and linking between libraries of the same module
##

IF(${PROJECT_NAME}_TO_BE_COMPILED)
 SET(DEPENDENCY_LIST ${ARGV})
 LIST(GET DEPENDENCY_LIST 1 KEYWORD)
  IF(KEYWORD STREQUAL "REQUIRES")
    CLEAR_VARIABLE(${PROJECT_NAME_CAPITAL}_LIBRARY_MODULE_${TARGET_OF_DEPENDENCY}_DEPENDENCIES)
    LIST(REMOVE_AT DEPENDENCY_LIST 0 1)
    FOREACH(DEPENDENCY ${DEPENDENCY_LIST})
      SET(${PROJECT_NAME_CAPITAL}_LIBRARY_MODULE_${TARGET_OF_DEPENDENCY}_DEPENDENCIES 
          ${${PROJECT_NAME_CAPITAL}_LIBRARY_MODULE_${TARGET_OF_DEPENDENCY}_DEPENDENCIES}
          ${DEPENDENCY}
          CACHE
          INTERNAL
          ""
          FORCE
        )
    ENDFOREACH(DEPENDENCY ${DEPENDENCY_LIST})
  ELSE(KEYWORD STREQUAL "REQUIRES")
    MESSAGE(CRITICAL "Syntax error in ADD_MODULE_INTERDEPENDENCY macro for target module ${TARGET_OF_DEPENDENCY}.\nIt was expecting the REQUIRED keyword as second parameter")
  ENDIF(KEYWORD STREQUAL "REQUIRES")  
ENDIF(${PROJECT_NAME}_TO_BE_COMPILED)
ENDMACRO(ADD_MODULE_INTERDEPENDENCY)

MACRO(ADD_LIBRARY_MODULES)
  FOREACH(LIBRARY_MODULE ${ARGV})
  
  	# append ${LIBRARY_MODULE} to the ${PROJECT_NAME}_LIBRARY_MODULES list
    IF(${PROJECT_NAME}_LIBRARY_MODULES)
      SET(${PROJECT_NAME}_LIBRARY_MODULES
        ${LIBRARY_MODULE}
        ${${PROJECT_NAME}_LIBRARY_MODULES}
      )
    ELSE(${PROJECT_NAME}_LIBRARY_MODULES)
      SET(${PROJECT_NAME}_LIBRARY_MODULES
        ${LIBRARY_MODULE}
      )
    ENDIF(${PROJECT_NAME}_LIBRARY_MODULES)
  ENDFOREACH(LIBRARY_MODULE)
ENDMACRO(ADD_LIBRARY_MODULES)


MACRO(INSTALL_LIBRARY_MODULES_AFTER_BUILD destination)
      SET(${PROJECT_NAME_CAPITAL}_INSTALL_DIR_FOR_THE_MODULE 
        ${destination}
      )
ENDMACRO(INSTALL_LIBRARY_MODULES_AFTER_BUILD destination)


MACRO(ADD_APPLICATION_MODULES)
  FOREACH(APPLICATION_MODULE ${ARGV})

   	# append ${APPLICATION_MODULE} to the ${PROJECT_NAME}_APPLICATION_MODULES list
    IF(${PROJECT_NAME}_APPLICATION_MODULES)
      SET(${PROJECT_NAME}_APPLICATION_MODULES
        ${APPLICATION_MODULE}
        ${${PROJECT_NAME}_APPLICATION_MODULES}
      )
    ELSE(${PROJECT_NAME}_APPLICATION_MODULES)
      SET(${PROJECT_NAME}_APPLICATION_MODULES
        ${APPLICATION_MODULE}
      )
    ENDIF(${PROJECT_NAME}_APPLICATION_MODULES)
  ENDFOREACH(APPLICATION_MODULE)
ENDMACRO(ADD_APPLICATION_MODULES)

MACRO(ADD_DEMO_MODULES)
  FOREACH(DEMO_MODULE ${ARGV})

   	# append ${DEMO_MODULE} to the ${PROJECT_NAME}_DEMO_MODULES list
    IF(${PROJECT_NAME}_DEMO_MODULES)
      SET(${PROJECT_NAME}_DEMO_MODULES
        ${DEMO_MODULE}
        ${${PROJECT_NAME}_DEMO_MODULES}
      )
    ELSE(${PROJECT_NAME}_DEMO_MODULES)
      SET(${PROJECT_NAME}_DEMO_MODULES
        ${DEMO_MODULE}
      )
    ENDIF(${PROJECT_NAME}_DEMO_MODULES)
  ENDFOREACH(DEMO_MODULE)
ENDMACRO(ADD_DEMO_MODULES)

MACRO(ADD_PROJECT_OPTION ARGV1 ARGV2 ARGV3)
  IF(SEE_${PROJECT_NAME_CAPITAL}_LIB_OPTIONS)
    OPTION(${ARGV1}
      ${ARGV2}
      ${ARGV3}
    )
  ENDIF(SEE_${PROJECT_NAME_CAPITAL}_LIB_OPTIONS)
ENDMACRO(ADD_PROJECT_OPTION)

MACRO(CLEAR_VARIABLE ARGV1)
  SET(${ARGV1}
    ""
  )
ENDMACRO(CLEAR_VARIABLE ARGV1)

MACRO(CLEAR_CACHED_VARIABLE ARGV1)
  SET(${ARGV1}
    ""
    CACHE
    INTERNAL
    ""
    FORCE
  )
ENDMACRO(CLEAR_CACHED_VARIABLE ARGV1)

# ARGV1 = project name
# ARGV2 = project description

MACRO(NEW_CILAB_MODULE_START ARGV1 ARGV2)
  # Define the project name
  PROJECT(${ARGV1})
  # Have the project name in capital in case you need it.
  STRING(TOUPPER
    ${PROJECT_NAME}
    PROJECT_NAME_CAPITAL
  )
  # Define the project type. It is needed for 
  # the dependency resolution mechanism  
  # Keep a comma separated list with the type of each project in CILAB_PROJECTS_TYPE.
  # Add ProjectName_TYPE : CILAB_MODULE to this list.
  IF(NOT CILAB_PROJECTS_TYPE MATCHES "${ARGV1}_TYPE : ")
    SET(CILAB_PROJECTS_TYPE
      "${ARGV1}_TYPE : CILAB_MODULE"
      ${CILAB_PROJECTS_TYPE}
      CACHE
      INTERNAL
      ""
      FORCE
    )
  ENDIF(NOT CILAB_PROJECTS_TYPE MATCHES "${ARGV1}_TYPE : ")

  # Store the projects path in the list CILAB_PROJECTS_PATH. 
  # It is needed for
  # the dependency resolution mechanism
  # to keep a list with the path to each project 
  # ${${ARGV1}_SOURCE_DIR} is the source dir var that is automatically generated by CMake.
  IF(NOT CILAB_PROJECTS_PATH MATCHES "${ARGV1}_PATH : ")
    SET(CILAB_PROJECTS_PATH
      "${ARGV1}_PATH : ${${ARGV1}_SOURCE_DIR}"
      ${CILAB_PROJECTS_PATH}
      CACHE
      INTERNAL
      ""
      FORCE
    )
  ENDIF(NOT CILAB_PROJECTS_PATH MATCHES "${ARGV1}_PATH : ")

  # Update the list CILAB_PROJECTS_NAME with {project name} entries.
  # Make sure that each package in the CILAB_PROJECTS_NAME
  # list appears only once. There is no operation in the 
  # list to search for an item (until cmake 2.4-7) so
  # we first try remove it and then we add it. That way
  # we know that appears only once. Then we put it in the cache.
  LIST(LENGTH
    CILAB_PROJECTS_NAME
    CILAB_PROJECTS_NAME_LENGTH
  )
  IF(CILAB_PROJECTS_NAME_LENGTH)
    LIST(REMOVE_ITEM
      CILAB_PROJECTS_NAME
      "${ARGV1}"
    )
  ENDIF(CILAB_PROJECTS_NAME_LENGTH)

  LIST(APPEND
    CILAB_PROJECTS_NAME
    "${ARGV1}"
  )
  # Add the CILAB_PROJECTS_NAME list to the CMake cache 
  # This means that it will be stored and can be reused, also by "parent" cmake scripts (higher in the call tree).
  SET(CILAB_PROJECTS_NAME
    ${CILAB_PROJECTS_NAME}
    CACHE
    INTERNAL
    ""
    FORCE
  )

  # Add it to the list of the available projects, containing pairs of {project name, description}.
  IF(NOT AVAILABLE_CILAB_PROJECTS MATCHES "${ARGV1} : ")
    SET(AVAILABLE_CILAB_PROJECTS
      "\n${ARGV1} : ${ARGV2}"
      ${AVAILABLE_CILAB_PROJECTS}
      CACHE
      INTERNAL
      ""
      FORCE
    )
  ENDIF(NOT AVAILABLE_CILAB_PROJECTS MATCHES "${ARGV1} : ")

  # We define for each ${CILAB_MODULE} the ${CILAB_MODULE}_DIR. When
  # someone does a FIND_PACKAGE(${CILAB_MODULE}) the macro will try to use
  # the ${CILAB_MODULE}_DIR directory. That's why we export this information.
  IF(NOT "${ARGV1}_DIR")
    SET("${ARGV1}_DIR"
      "${CILAB_TOOLKIT_BINARY_DIR}/cilabModules/${ARGV1}"
      CACHE
      INTERNAL
      "Path to the ${ARGV1}"
    )
  ENDIF(NOT "${ARGV1}_DIR")
ENDMACRO(NEW_CILAB_MODULE_START)

# Include the cmake file that actually does work: i.e. creates the sln and vcproj (in Windows) or makefile (Linux).
# Only do work if ${PROJECT_NAME}_TO_BE_COMPILED is true (i.e. if the user has selected to compile this project).

MACRO(NEW_CILAB_MODULE_END ARGV1)
  IF(${PROJECT_NAME}_TO_BE_COMPILED)
    INCLUDE("${CILAB_TOOLKIT_SOURCE_DIR}/cilabResources/Project.cmake")
  ENDIF(${PROJECT_NAME}_TO_BE_COMPILED)
ENDMACRO(NEW_CILAB_MODULE_END)

MACRO(NEW_CILAB_PLUGIN_START ARGV1 ARGV2)
  PROJECT(${ARGV1})
  # Have the project name in capital in case you need it.
  STRING(TOUPPER
    ${PROJECT_NAME}
    PROJECT_NAME_CAPITAL
  )
  # Define the project type. It is needed for 
  # the dependency resolution mechanism  
  # Keep a list with the type of each project 
  IF(NOT CILAB_PROJECTS_TYPE MATCHES "${ARGV1}_TYPE : ")
    SET(CILAB_PROJECTS_TYPE
      "${ARGV1}_TYPE : CILAB_PLUGIN"
      ${CILAB_PROJECTS_TYPE}
      CACHE
      INTERNAL
      ""
      FORCE
    )
  ENDIF(NOT CILAB_PROJECTS_TYPE MATCHES "${ARGV1}_TYPE : ")

  # Store the projects path. It is needed for
  # the dependency resolution mechanism
  # to keep a list with the path to each project 
  IF(NOT CILAB_PROJECTS_PATH MATCHES "${ARGV1}_PATH : ")
    SET(CILAB_PROJECTS_PATH
      "${ARGV1}_PATH : ${${ARGV1}_SOURCE_DIR}"
      ${CILAB_PROJECTS_PATH}
      CACHE
      INTERNAL
      ""
      FORCE
    )
  ENDIF(NOT CILAB_PROJECTS_PATH MATCHES "${ARGV1}_PATH : ")

  # Make sure that each package in the CILAB_PROJECTS_NAME
  # list appears only once. There is no operation in the 
  # list to search for an item (until cmake 2.4-7) so
  # we first try remove it and then we add it. That way
  # we know that appears only once. Then we put it in the cache.
  LIST(LENGTH
    CILAB_PROJECTS_NAME
    CILAB_PROJECTS_NAME_LENGTH
  )
  IF(CILAB_PROJECTS_NAME_LENGTH)
    LIST(REMOVE_ITEM
      CILAB_PROJECTS_NAME
      "${ARGV1}"
    )
  ENDIF(CILAB_PROJECTS_NAME_LENGTH)

  LIST(APPEND
    CILAB_PROJECTS_NAME
    "${ARGV1}"
  )
  SET(CILAB_PROJECTS_NAME
    ${CILAB_PROJECTS_NAME}
    CACHE
    INTERNAL
    ""
    FORCE
  )

  # Add it to the list of the available projects.
  IF(NOT AVAILABLE_CILAB_PROJECTS MATCHES "${ARGV1} : ")
    SET(AVAILABLE_CILAB_PROJECTS
      "\n${ARGV1} : ${ARGV2}"
      ${AVAILABLE_CILAB_PROJECTS}
      CACHE
      INTERNAL
      ""
      FORCE
    )
  ENDIF(NOT AVAILABLE_CILAB_PROJECTS MATCHES "${ARGV1} : ")
  SET(PLUGIN_NAME ${PROJECT_NAME})
ENDMACRO(NEW_CILAB_PLUGIN_START)

MACRO(NEW_CILAB_PLUGIN_END ARGV1)
  IF(${PROJECT_NAME}_TO_BE_COMPILED)
    INCLUDE("${CILAB_TOOLKIT_SOURCE_DIR}/cilabResources/Plugin.cmake")
  ENDIF(${PROJECT_NAME}_TO_BE_COMPILED)
ENDMACRO(NEW_CILAB_PLUGIN_END)

MACRO(NEW_CILAB_APPLICATION_START ARGV1 ARGV2)
  PROJECT(${ARGV1})
  # Have the project name in capital in case you need it.
  STRING(TOUPPER
    ${PROJECT_NAME}
    PROJECT_NAME_CAPITAL
  )

  # Define the project type. It is needed for 
  # the dependency resolution mechanism  
  # Keep a list with the type of each project 
  IF(NOT CILAB_PROJECTS_TYPE MATCHES "${ARGV1}_TYPE : ")
    SET(CILAB_PROJECTS_TYPE
      "${ARGV1}_TYPE : CILAB_APPLICATION"
      ${CILAB_PROJECTS_TYPE}
      CACHE
      INTERNAL
      ""
      FORCE
    )
  ENDIF(NOT CILAB_PROJECTS_TYPE MATCHES "${ARGV1}_TYPE : ")

  # Add it to the list of the available projects.
  IF(NOT AVAILABLE_CILAB_PROJECTS MATCHES "${ARGV1} : ")
    SET(AVAILABLE_CILAB_PROJECTS
      "\n${ARGV1} : ${ARGV2}"
      ${AVAILABLE_CILAB_PROJECTS}
      CACHE
      INTERNAL
      ""
      FORCE
    )
  ENDIF(NOT AVAILABLE_CILAB_PROJECTS MATCHES "${ARGV1} : ")
  SET(PLUGIN_NAME ${PROJECT_NAME})
ENDMACRO(NEW_CILAB_APPLICATION_START)

MACRO(NEW_CILAB_APPLICATION_END ARGV1)
  IF(${PROJECT_NAME}_TO_BE_COMPILED)
    INCLUDE("${CILAB_TOOLKIT_SOURCE_DIR}/cilabResources/Core.cmake")
  ENDIF(${PROJECT_NAME}_TO_BE_COMPILED)
ENDMACRO(NEW_CILAB_APPLICATION_END)

# argv1 = project name
# argv2 = project description
# This function also sets the XXX_DIR value (where XXX is the project name).

MACRO(REGISTER_THIRDPARTY_PROJECT_NAME ARGV1 ARGV2)
  # add project name to AVAILABLE_THIRDPARTY_PROJECTS
  IF(NOT AVAILABLE_THIRDPARTY_PROJECTS MATCHES "${ARGV1} : ")
    SET(AVAILABLE_THIRDPARTY_PROJECTS
      "\n${ARGV1} : ${ARGV2}"
      ${AVAILABLE_THIRDPARTY_PROJECTS}
      CACHE
      INTERNAL
      ""
      FORCE
    )
  ENDIF(NOT AVAILABLE_THIRDPARTY_PROJECTS MATCHES "${ARGV1} : ")
  
  # Add the project type to the CILAB_PROJECTS_TYPE list. It is needed for 
  # the dependency resolution mechanism  
  # Keep a list with the type of each project 
  IF(NOT CILAB_PROJECTS_TYPE MATCHES "${ARGV1}_TYPE : ")
    SET(CILAB_PROJECTS_TYPE
      "${ARGV1}_TYPE : CILAB_THIRD_PARTY"
      ${CILAB_PROJECTS_TYPE}
      CACHE
      INTERNAL
      ""
      FORCE
    )
  ENDIF(NOT CILAB_PROJECTS_TYPE MATCHES "${ARGV1}_TYPE : ")

  # Store the projects path in the CILAB_PROJECTS_PATH list. It is needed for
  # the dependency resolution mechanism
  # to keep a list with the path to each project 
  IF(NOT CILAB_PROJECTS_PATH MATCHES "${ARGV1}_PATH : ")
    SET(CILAB_PROJECTS_PATH
      "${ARGV1}_PATH : ${${ARGV1}_SOURCE_DIR}"
      ${CILAB_PROJECTS_PATH}
      CACHE
      INTERNAL
      ""
      FORCE
    )
  ENDIF(NOT CILAB_PROJECTS_PATH MATCHES "${ARGV1}_PATH : ")

  # Make sure that each package in the THIRDPARTY_PROJECTS_NAME
  # list appears only once. There is no operation in the 
  # list to search for an item (until cmake 2.4-7) so
  # we first try remove it and then we add it. That way
  # we know that appears only once. Then we put it in the cache.
  LIST(LENGTH
    THIRDPARTY_PROJECTS_NAME
    THIRDPARTY_PROJECTS_NAME_LENGTH
  )
  IF(THIRDPARTY_PROJECTS_NAME_LENGTH)
    LIST(REMOVE_ITEM
      THIRDPARTY_PROJECTS_NAME
      "${ARGV1}"
    )
  ENDIF(THIRDPARTY_PROJECTS_NAME_LENGTH)

  LIST(APPEND
    THIRDPARTY_PROJECTS_NAME
    "${ARGV1}"
  )
  
  # store THIRDPARTY_PROJECTS_NAME in the cache
  SET(THIRDPARTY_PROJECTS_NAME
    ${THIRDPARTY_PROJECTS_NAME}
    CACHE
    INTERNAL
    ""
    FORCE
  )
  # We define for each ${THIRDPARTY_MODULE} the ${THIRDPARTY_MODULE}_DIR. When
  # someone does a FIND_PACKAGE(${THIRDPARTY_MODULE}) the macro will search
  # for the ${THIRDPARTY_MODULE}_DIR directory. That's why we export this information.
  IF(NOT "${ARGV1}_DIR")
    SET("${ARGV1}_DIR"
      "${CILAB_TOOLKIT_BINARY_DIR}/thirdParty/${ARGV1}"
      CACHE
      INTERNAL
      "Path to the ${ARGV1}"
     )
  ENDIF(NOT "${ARGV1}_DIR")
ENDMACRO(REGISTER_THIRDPARTY_PROJECT_NAME ARGV1 ARGV2)

# First parameter will be the projects name, and the rest of the parameters
# will be the projects from which this project depends from.
MACRO(REGISTER_THIRDPARTY_PROJECT_DEPENDENCIES)

  # Read current project name from the parameter list
  # and take it out. The project names left on the variable
  # will belong to the projects from which it depends.
  SET(ARGUMENT_LIST
    ${ARGV}
  )
  LIST(GET
    ARGUMENT_LIST
    0
    CURRENT_PROJECT_NAME
  )
  LIST(REMOVE_AT
    ARGUMENT_LIST
    0
  )
  FOREACH(PROJECT_NAME ${ARGUMENT_LIST})
    SET(${CURRENT_PROJECT_NAME}_DEPENDS_FROM
      ${PROJECT_NAME}
      ${${CURRENT_PROJECT_NAME}_DEPENDS_FROM}
      CACHE
      INTERNAL
      "Cilab Projects from which ${CURRENT_PROJECT_NAME} depends."
      FORCE
    )
  ENDFOREACH(PROJECT_NAME ${ARGUMENT_LIST})
ENDMACRO(REGISTER_THIRDPARTY_PROJECT_DEPENDENCIES)

# Each project provided from the toolkit has a type. It should
# either be a CILAB_MODULE, a CILAB_APPLICATION, a CILAB_PLUGIN or
# a CILAB_THIRD_PARTY. This is used in order to be able to separate 
# the projects in the correct projects when working with the Visual Studio.
MACRO(FIND_PROJECT_TYPE ARGV1 ARGV2)

  SET(FINISHED
    ""
  )
  LIST(LENGTH
    CILAB_PROJECTS_TYPE
    ITER
  )
  # Iterate through the project type list
  WHILE(NOT FINISHED AND ITER )
    MATH(EXPR
      ITER
      "${ITER} - 1"
    )
    # Get first element of the projects to be compiled list
    LIST(GET
      CILAB_PROJECTS_TYPE
      ${ITER}
      NEXT_PROJECT_TYPE_ITEM
    )
    IF( NEXT_PROJECT_TYPE_ITEM MATCHES "${ARGV1}_TYPE : " )
      STRING(REGEX
        REPLACE
        "${ARGV1}_TYPE : "
        ""
        ${ARGV2}
        ${NEXT_PROJECT_TYPE_ITEM}
      )
      SET(FINISHED
        "TRUE"
      )
    ENDIF( NEXT_PROJECT_TYPE_ITEM MATCHES "${ARGV1}_TYPE : " )
  ENDWHILE(NOT FINISHED AND ITER )
  
ENDMACRO(FIND_PROJECT_TYPE ARGV1 ARGV2)

MACRO(FIND_PROJECT_PATH ARGV1 ARGV2)

  SET(FINISHED
    ""
  )
  LIST(LENGTH
    CILAB_PROJECTS_PATH
    ITER
  )
  # Iterate through the project type list
  WHILE(NOT FINISHED AND ITER )
    MATH(EXPR
      ITER
      "${ITER} - 1"
    )
    # Get first element of the projects to be compiled list
    LIST(GET
      CILAB_PROJECTS_PATH
      ${ITER}
      NEXT_PROJECTS_PATH_ITEM
    )
    IF( NEXT_PROJECTS_PATH_ITEM MATCHES "${ARGV1}_PATH : " )
      STRING(REGEX
        REPLACE
        "${ARGV1}_PATH : "
        ""
        ${ARGV2}
        ${NEXT_PROJECTS_PATH_ITEM}
      )
      SET(FINISHED
        "TRUE"
      )
    ENDIF( NEXT_PROJECTS_PATH_ITEM MATCHES "${ARGV1}_PATH : " )
  ENDWHILE(NOT FINISHED AND ITER )

ENDMACRO(FIND_PROJECT_PATH ARGV1 ARGV2)

# Accepts a project name. 
# Adds the project name to the CILAB_TOOLKIT_TO_BE_COMPILED list.
# If this project depends on other projects (${${ARGV}_DEPENDS_FROM} is not empty)
# then the macro calls itself (recursion) on each item in ${${ARGV}_DEPENDS_FROM}.

MACRO(RECURSE_DEPENDENCIES ARGV)
  IF(${ARGV}_DEPENDS_FROM)
    FOREACH(DEPENDENCY ${${ARGV}_DEPENDS_FROM})
      RECURSE_DEPENDENCIES(${DEPENDENCY})
    ENDFOREACH(DEPENDENCY ${${ARGV}_DEPENDS_FROM})
  ENDIF(${ARGV}_DEPENDS_FROM)

  LIST(APPEND
    CILAB_TOOLKIT_TO_BE_COMPILED
    ${ARGV}
  )
ENDMACRO(RECURSE_DEPENDENCIES)

# This macro processes the ${CILAB_PROJECTS_CHOOSEN}
# to decide which project to compile. To do this, every
# project defines its ${PROJECT_NAME}_DEPENDS_FROM variable
# which contains the names of the projects from which it depends.
MACRO(RESOLVE_DEPENDENCIES)

  CLEAR_VARIABLE(CILAB_TOOLKIT_TO_BE_COMPILED)
  IF(CILAB_PROJECTS_CHOOSEN)
    FOREACH(PROJECT ${CILAB_PROJECTS_CHOOSEN})
      RECURSE_DEPENDENCIES(${PROJECT})
    ENDFOREACH(PROJECT ${CILAB_PROJECTS_CHOOSEN})
  ENDIF(CILAB_PROJECTS_CHOOSEN)

  # Order packages so MITK is the first to be compiled 
  # and DCMTK be the last one to be compiled.
  ORDER_PACKAGES(CILAB_TOOLKIT_TO_BE_COMPILED)

ENDMACRO(RESOLVE_DEPENDENCIES)

# Some packages like DCMTK, ITK, VTK, MITK have strange of
# erroneous befaviour when configured before or after one 
# of the other packages in the list. After try n error 
# found out that the order to be correctly configured and
# compiled is the one forced above. Always MITK before ITK
# and always DCMTK the last package to be configured!!!!!
MACRO(ORDER_PACKAGES PACKAGE_LIST)

  # If found MITK in its include dirs
  # put it in the last position of the package list.
  FOREACH(PACKAGE ${${PACKAGE_LIST}})
  
  IF(${PACKAGE} MATCHES "ITK*")
      LIST(REMOVE_ITEM
        ${PACKAGE_LIST}
        ${PACKAGE}
      )
      LIST(APPEND
        ${PACKAGE_LIST}
        ${PACKAGE}
      )
   ENDIF(${PACKAGE} MATCHES "ITK*")
  
    
   IF(${PACKAGE} MATCHES "MITK*")
      LIST(REMOVE_ITEM
        ${PACKAGE_LIST}
        ${PACKAGE}
      )
      LIST(INSERT
        ${PACKAGE_LIST}
        0
        ${PACKAGE}
      )
    ENDIF(${PACKAGE} MATCHES "MITK*")
    

    # If found DCMTK remove it from the list and append it in
    # the last position of the package list.
    IF(${PACKAGE} MATCHES "DCMTK*")
      LIST(REMOVE_ITEM
        ${PACKAGE_LIST}
        ${PACKAGE}
      )
      LIST(APPEND
        ${PACKAGE_LIST}
        "${PACKAGE}"
      )
    ENDIF(${PACKAGE} MATCHES "DCMTK*")
  ENDFOREACH(PACKAGE)
  
ENDMACRO(ORDER_PACKAGES)

# This macro parses the ${PROJECT_NAME}_REQUIRED_PACKAGES variable
# and for each <package> name found there tries to include its
# <package>config.cmake & Use<package>.cmake file. If package is found
# this macro reads up the values ot the <package>_INCLUDE_DIRS,
# <package>_LIBRARY_DIRS and <package>_LIBRARIES to the 
# ${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES,
# ${PROJECT_NAME_CAPITAL}_LIBRARY_DIRECTORIES
# and ${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES respectively.
MACRO(INCLUDE_EXTERNAL_PACKAGES)

  ORDER_PACKAGES(${PROJECT_NAME}_REQUIRED_PACKAGES)
  FOREACH(PACKAGE ${${PROJECT_NAME}_REQUIRED_PACKAGES})
    STRING(TOUPPER
      ${PACKAGE}
      PACKAGE_CAPITAL
    )
    IF(EXISTS "${CMAKE_ROOT}/Modules/Find${PACKAGE}.cmake")
      INCLUDE("${CMAKE_ROOT}/Modules/Find${PACKAGE}.cmake")
      IF(${PACKAGE}_USE_FILE)
        INCLUDE(${${PACKAGE}_USE_FILE})
      ENDIF(${PACKAGE}_USE_FILE)
    ELSE(EXISTS "${CMAKE_ROOT}/Modules/Find${PACKAGE}.cmake")
      #Find each package in the ${${PROJECT_NAME}_REQUIRED_PACKAGES} list
      FIND_PACKAGE(${PACKAGE})
      IF(${PACKAGE}_FOUND)
        # Read file package information
        IF(${PACKAGE}_USE_FILE)
          INCLUDE(${${PACKAGE}_USE_FILE})
        ENDIF(${PACKAGE}_USE_FILE)
      ELSE(${PACKAGE}_FOUND)
        MESSAGE(SEND_ERROR "Please set ${PACKAGE}_DIR.")
      ENDIF(${PACKAGE}_FOUND)
    ENDIF(EXISTS "${CMAKE_ROOT}/Modules/Find${PACKAGE}.cmake")

    # Read package compiler/precompiler flags.
    IF(${PACKAGE}_DEFINITIONS)
      ADD_DEFINITIONS(${${PACKAGE}_DEFINITIONS})
      SET(${PROJECT_NAME_CAPITAL}_DEFINITIONS
      ${${PROJECT_NAME_CAPITAL}_DEFINITIONS}
      ${${PACKAGE_CAPITAL}_DEFINITIONS}
      ${${PACKAGE}_DEFINITIONS}
      )
    ENDIF(${PACKAGE}_DEFINITIONS)

    IF(${PACKAGE_CAPITAL}_DEFINITIONS)
      ADD_DEFINITIONS(${${PACKAGE_CAPITAL}_DEFINITIONS})
    ENDIF(${PACKAGE_CAPITAL}_DEFINITIONS)

    # Read all the libraries that we should link with
    # Try various variables just in case...
    SET(${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES
      ${${PACKAGE}_LIBRARIES}
      ${${PACKAGE}_OTHER_LIBRARIES}
      ${${PACKAGE_CAPITAL}_LIBRARIES}
      ${${PACKAGE_CAPITAL}_OTHER_LIBRARIES}
      ${${PROJECT_NAME_CAPITAL}_LINK_LIBRARIES}
    )
    # In case there are debug only libraries.
    SET(${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES
      ${${PACKAGE}_LIBRARIES_DEBUG}
      ${${PACKAGE_CAPITAL}_LIBRARIES_DEBUG}
      ${${PACKAGE}_DEBUG_LIBRARIES}
      ${${PROJECT_NAME_CAPITAL}_DEBUG_LIBRARIES}
    )
    
    # In case there are release only libraries.
    SET(${PROJECT_NAME_CAPITAL}_RELEASE_LIBRARIES
      ${${PACKAGE}_LIBRARIES_RELEASE}
      ${${PACKAGE_CAPITAL}_LIBRARIES_RELEASE}
      ${${PACKAGE}_RELEASE_LIBRARIES}
      ${${PROJECT_NAME_CAPITAL}_RELEASE_LIBRARIES}
    )

    # Read all libraries link directories.
    SET(${PROJECT_NAME_CAPITAL}_LIBRARY_DIRECTORIES
      ${${PACKAGE}_LIBRARY_DIRS}
      ${${PACKAGE_CAPITAL}_LIBRARY_DIRS}
      ${${PROJECT_NAME_CAPITAL}_LIBRARY_DIRECTORIES}
    )

    # Read all the include directories
    SET(${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES
      ${${PROJECT_NAME_CAPITAL}_INCLUDE_DIRECTORIES}
      ${${PACKAGE}_INCLUDE_DIRS}
      ${${PACKAGE_CAPITAL}_INCLUDE_DIRS}
    )
  ENDFOREACH(PACKAGE)

ENDMACRO(INCLUDE_EXTERNAL_PACKAGES)

###############################################################################################
#
# Here are some macros coded for easy use of wrappers in CMAKE for the LambdaCore Build
# You should not modify anything from this file
#
################################################################################################
#-----------------------------------------------------------------------------
# A macro for getting the current dir name
# use: GET_MODULENAME(var)
#-----------------------------------------------------------------------------
MACRO(GET_MODULENAME module_name)
  STRING(REGEX
    REPLACE
    "^.*/" ""
    ${module_name}
    ${CMAKE_CURRENT_SOURCE_DIR}
  )
ENDMACRO(GET_MODULENAME)

#-----------------------------------------------------------------------------
# A macro for listing just subdirectories of the current dir if they are CMAKE-able
# (they are appended to the list if they contain CMakeLists.txt)
# use: LIST_DIRS(output)
#-----------------------------------------------------------------------------
MACRO(LIST_DIRS returnList)
  FILE(GLOB subdirContents "*")
  FOREACH(FILE ${subdirContents})
    IF(IS_DIRECTORY ${FILE})
      STRING(REGEX MATCH "CVS$" IS_CVS ${FILE})
      IF(NOT IS_CVS)
	SET(${returnList} ${${returnList}} ${FILE})
      ENDIF(NOT IS_CVS)
    ENDIF(IS_DIRECTORY ${FILE})
  ENDFOREACH(FILE)
ENDMACRO(LIST_DIRS)

#-----------------------------------------------------------------------------
# A macro for listing just subdirectories of the current dir if they are CMAKE-able
# and it returns just the relative name of the directories
# (they are appended to the list if they contain CMakeLists.txt)
# use: LIST_MODULES(output)
#-----------------------------------------------------------------------------
MACRO(LIST_MODULES returnList)
  LIST_DIRS(rList)
  FOREACH(MODULE ${rList})
    STRING(REPLACE
      "${CMAKE_CURRENT_SOURCE_DIR}/" ""
      module_name
      ${MODULE}
    )
    SET(${returnList}
      ${${returnList}}
      ${module_name}
    )
  ENDFOREACH(MODULE)
ENDMACRO(LIST_MODULES)


#------------------------------------------------------------
# A macro for recursing directories. 
# Returns a list of all subdirs recursively
# use:  RECURSE_DIRS(output_of_src_dirs output_of_bin_dirs "initial_path")
#------------------------------------------------------------
MACRO(RECURSE_DIRS returnListSrc returnListBin currentDir)
  FILE(GLOB subdirContents "${currentDir}/*")
  FOREACH(FILE ${subdirContents})
    IF(IS_DIRECTORY ${FILE})
      STRING(REGEX
        MATCH
        "CVS$"
        IS_CVS
        ${FILE}
      )
      IF(NOT IS_CVS)
	SET(${returnListSrc}
          ${${returnListSrc}}
          ${FILE}
        )
	FILE(GLOB
          hasCmakeList
          "${FILE}/CMakeLists.txt"
        )
	IF(hasCmakeList)
	  STRING(REPLACE
	    "${${PROJECT_NAME}_SOURCE_DIR}" "${${PROJECT_NAME}_BINARY_DIR}"
            FILE_BIN
            ${FILE}
          )
	  SET(${returnListBin}${${returnListBin}} ${FILE_BIN})
        ENDIF(hasCmakeList)
        RECURSE_DIRS(${returnListSrc}
          ${returnListBin}
          ${FILE}
        )
      ENDIF(NOT IS_CVS)
    ENDIF(IS_DIRECTORY ${FILE})
  ENDFOREACH(FILE)
ENDMACRO(RECURSE_DIRS)

#------------------------------------------------------------
# A macro for configuring cmake files
# use: GENERATE_CONFIGURABLE_FILES(output input_file_list)
#------------------------------------------------------------

MACRO(GENERATE_CONFIGURABLE_FILES returnList file_list)
  # Create the output dir for CMAKE generated files
  FILE(MAKE_DIRECTORY
    ${${PROJECT_NAME}_BINARY_DIR}/CmakeGenerated
  )
  FOREACH(FILE ${file_list})
    # Get the name of the file without extension
    STRING(REGEX
      REPLACE
      "[.]in$" ""
      CMAKECONFIGURABLE_OUTPUT_FILE
      ${FILE}
    )
    STRING(REGEX REPLACE
      "^.*/" ""
      CMAKECONFIGURABLE_OUTPUT_FILE
      ${CMAKECONFIGURABLE_OUTPUT_FILE}
    )

    CONFIGURE_FILE(${FILE} ${${PROJECT_NAME}_BINARY_DIR}/CmakeGenerated/${CMAKECONFIGURABLE_OUTPUT_FILE})
    SET(${returnList}
      ${${returnList}}
      ${${PROJECT_NAME}_BINARY_DIR}/CmakeGenerated/${CMAKECONFIGURABLE_OUTPUT_FILE}
    )
  ENDFOREACH(FILE)
ENDMACRO(GENERATE_CONFIGURABLE_FILES)

#-----------------------------------------------------------------------------
# A macro for wrapping SWIG files
# use: SWIG_WRAP_FILES(output_cxx_var output_py_var input_i_files)
#-----------------------------------------------------------------------------
MACRO(SWIG_WRAP_FILES output_cxx_files output_py_files input_i_files)
  # Create the output dir for SWIG files
  FILE(MAKE_DIRECTORY ${${PROJECT_NAME}_BINARY_DIR}/SwigWrapped)
  FOREACH(SWIG_IFACE ${input_i_files})
    # Get the name of the file without extension
    STRING(REGEX REPLACE
      "[.]i$" ""
      SWIG_IFACE_OUTPUT
      ${SWIG_IFACE}
    )
    STRING(REGEX REPLACE
      "^.*/" ""
      SWIG_IFACE_OUTPUT
      ${SWIG_IFACE_OUTPUT}
    )

    # Append _wrap.cxx to the wrapped file and .py to the python object
    SET(${output_cxx_files}
      ${${output_cxx_files}}
      ${${PROJECT_NAME}_BINARY_DIR}/SwigWrapped/${SWIG_IFACE_OUTPUT}_wrap.cxx
    )
    SET(${output_py_files}
      ${${output_py_files}}
      ${${PROJECT_NAME}_BINARY_DIR}/SwigWrapped/${SWIG_IFACE_OUTPUT}.py
     )
    # Prepare arguments for SWIG
    # Exec SWIG with the configured arguments
    EXECUTE_PROCESS(COMMAND
      "${SWIG_EXECUTABLE}" "-c++" "-python" "-shadow" "-includeall" "-ignoremissing" "-outdir" "${${PROJECT_NAME}_BINARY_DIR}/SwigWrapped" "-o" "${${PROJECT_NAME}_BINARY_DIR}/SwigWrapped/${SWIG_IFACE_OUTPUT}_wrap.cxx" "${SWIG_IFACE}"
      WORKING_DIRECTORY "${SWIG_DIR}"
      OUTPUT_VARIABLE SWIG_OUTPUT
      ERROR_VARIABLE SWIG_ERROR
    )
  ENDFOREACH(SWIG_IFACE)
ENDMACRO(SWIG_WRAP_FILES)

MACRO(HANDLE_SWIG_WRAPPING)

  #------------------------------------------------------------
  # Prepare sources
  #------------------------------------------------------------

  FILE(GLOB
    ${PROJECT_NAME}_SWIG_IFACES
    "Source/Core/*.i"
  )


  #------------------------------------------------------------
  # Prepare Scripting sources
  #------------------------------------------------------------
  SWIG_WRAP_FILES(${PROJECT_NAME}_SWIG_IFACES_OUTPUT
    ${PROJECT_NAME}_PYTHONOBJECTS_OUTPUT
    ${${PROJECT_NAME}_SWIG_IFACES}
  )

  #------------------------------------------------------------
  # Build Scripting wrappers
  #------------------------------------------------------------
  # Build Core Python module
  ADD_LIBRARY(_Core SHARED ${${PROJECT_NAME}_SWIG_IFACES_OUTPUT} ${${PROJECT_NAME}_SWIG_IFACES})
  TARGET_LINK_LIBRARIES(_Core
    ${${PROJECT_NAME_CAPITAL}_LIBRARIES}
  )

  INSTALL(TARGETS
    _Core
    DESTINATION "cilabApps/GIMIAS/modules"
  )
 
  INSTALL(FILES
    ${${PROJECT_NAME}_PYTHONOBJECTS_OUTPUT}
    DESTINATION "cilabApps/GIMIAS/modules"
  )
  SET(${PROJECT_NAME_CAPITAL}_OTHER_LIBRARIES
  )
ENDMACRO(HANDLE_SWIG_WRAPPING)

MACRO(CILAB_QT_WRAP_CPP MOC_INPUT_FILES MOC_OUTPUT_FILES)

  FOREACH(MOC_INPUT_FILE ${${MOC_INPUT_FILES}})
    # Get filename without path
	  STRING(REGEX
      REPLACE
      "^.*/" ""
      MOC_INPUT_FILE_STRIPPED
      ${MOC_INPUT_FILE}
    )
	
	  # Get path without filename
    STRING(REGEX
      MATCH
      "^.*/"
      MOC_INPUT_FILEPATH
      ${MOC_INPUT_FILE}
    )
   
    # If file in source dir generate it in the binary dir
    STRING(REPLACE
      "${${PROJECT_NAME}_SOURCE_DIR}" "${${PROJECT_NAME}_BINARY_DIR}"
      MOC_INPUT_FILEPATH_BINARY
      ${MOC_INPUT_FILEPATH}
    )
	
	  # Generate the MOC_OUTPUT_FILE
  	SET(MOC_OUTPUT_FILE
	    "${${PROJECT_NAME}_BINARY_DIR}/moc_${MOC_INPUT_FILE_STRIPPED}"
  	)
  	
  	# Change .h for .cxx
  	STRING(REGEX
  	  REPLACE
  	  ".h$" ".cxx"
  	  MOC_OUTPUT_FILE
  	  ${MOC_OUTPUT_FILE}
  	)

	  EXECUTE_PROCESS(COMMAND
      "${QT_MOC_EXECUTABLE}" "-p" "${MOC_INPUT_FILEPATH}" "-o" "${MOC_OUTPUT_FILE}" "${MOC_INPUT_FILE}"
      WORKING_DIRECTORY "${${PROJECT_NAME}_BINARY_DIR}"
      OUTPUT_VARIABLE QT_MOC_OUTPUT
      ERROR_VARIABLE QT_MOC_ERROR
    )
    
    SET(${MOC_OUTPUT_FILES}
      ${${MOC_OUTPUT_FILES}}
      ${MOC_OUTPUT_FILE}
    )
      
  ENDFOREACH(MOC_INPUT_FILE ${${MOC_INPUT_FILES}})
  
ENDMACRO(CILAB_QT_WRAP_CPP MOC_INPUT_FILES MOC_OUTPUT_FILES)

MACRO(RECURSE_DOCS_DIRS DOCS_SOURCE_DIRS INITIAL_DIR)

  SET(${DOCS_SOURCE_DIRS}
	  ${INITIAL_DIR}
	  ${${DOCS_SOURCE_DIRS}}
	)
	
	FILE(GLOB
	  DIRECTORY_ENTRIES
	  "${INITIAL_DIR}/*"
	)
	
	FOREACH(DIRECTORY_ENTRY ${DIRECTORY_ENTRIES})
	  IF(IS_DIRECTORY "${DIRECTORY_ENTRY}")
	  	IF(NOT "${DIRECTORY_ENTRY}" MATCHES "/CVS$|/Original$|/resource$")
	  	  RECURSE_DOCS_DIRS(${DOCS_SOURCE_DIRS} "${DIRECTORY_ENTRY}")
	  	ENDIF(NOT "${DIRECTORY_ENTRY}" MATCHES "/CVS$|/Original$|/resource$")
	  ENDIF(IS_DIRECTORY "${DIRECTORY_ENTRY}")
  ENDFOREACH(DIRECTORY_ENTRY ${DIRECTORY_ENTRIES})

ENDMACRO(RECURSE_DOCS_DIRS DOCS_SOURCE_DIRS INITIAL_DIR)

MACRO(INITIALISE_CILAB_TOOLKIT_DOCUMENTATION)

  FIND_PACKAGE(Doxygen)
  INCLUDE(${CMAKE_ROOT}/Modules/FindDoxygen.cmake)

  OPTION(BUILD_TOOLKIT_DOCUMENTATION
    OFF
    "Generate Doxygen and Tex project documentation."
  )

  #Doxygen config file requires only the path to DOT
  IF(DOXYGEN_DOT_PATH)
    SET(HAVE_DOT "YES")
  ELSE(DOXYGEN_DOT_PATH)
    SET(HAVE_DOT "NO")
  ENDIF(DOXYGEN_DOT_PATH)

ENDMACRO(INITIALISE_CILAB_TOOLKIT_DOCUMENTATION)

#------------------------------------------------------------
# Documentation to be installed in the release dir /docs 
#------------------------------------------------------------
MACRO(GENERATE_CILAB_PROJECT_DOCUMENTATION)

# [JAM] Prefer not to install always documentation (it does also when you select no to build it)
IF(BUILD_TOOLKIT_DOCUMENTATION)
# [endJAM]
  IF(DOXYGEN_FOUND)

    # Get all resource files
    FILE(GLOB
      ${PROJECT_NAME}_DOCS_FILES
      "${${PROJECT_NAME}_SOURCE_DIR}/docs/*.dox"
    )

    RECURSE_DOCS_DIRS(${PROJECT_NAME}_DOCS_SOURCE_DIRS ${${PROJECT_NAME}_SOURCE_DIR})

    # Append to all source files as input to generate docs
    SET(${PROJECT_NAME}_DOCS_FILES
      ${${PROJECT_NAME}_DOCS_FILES}
      ${${PROJECT_NAME}_DOCS_SOURCE_DIRS}
    )

    # The INPUT_DOCS_FILES variable is inserted in the Doxygen project
    STRING(REPLACE
      ";" "\t"
      INPUT_DOCS_FILES
      "${${PROJECT_NAME}_DOCS_FILES}"
    )

    SET(DOCUMENTATION_BIN_ROOT
      "${${PROJECT_NAME}_BINARY_DIR}/docs/${PROJECT_NAME}"
    )

    SET(DOCUMENTATION_SRC_ROOT
      "${${PROJECT_NAME}_SOURCE_DIR}/docs"
    )

    # Configure Doxygen project file for the build, using the cmake 
    # variables and miscellaneous info
    CONFIGURE_FILE("${CILAB_TOOLKIT_SOURCE_DIR}/cilabResources/documentation/CilabToolkitDoxygen.cfg.in"
      "${${PROJECT_NAME}_BINARY_DIR}/docs/${PROJECT_NAME}/${PROJECT_NAME}doxygen.cfg"
    )

    # Copy the background image to the binary directory
    CONFIGURE_FILE("${CILAB_TOOLKIT_SOURCE_DIR}/cilabResources/headbg.jpg"
      "${${PROJECT_NAME}_BINARY_DIR}/docs/${PROJECT_NAME}/html/headbg.jpg"
      COPYONLY
      IMMEDIATE
    )

    # If we choose to compile the documentation of the toolkit, generate a custom rule (VS project or make rule)
    # that builds the documentation. The ALL option of the ADD_CUSTOM_TARGET command means that this target is 
    # included in the ALL target, meaning when you compile everything the documentation will be automatically 
    # compiled. In the ELSE(BUILD_TOOLKIT_DOCUMENTATION), if we have doxygen installed we add a rule but EXLUDING it
    # from the BUILD_ALL target. If somebody doesn't want to build the whole documentation will be able to build the
    # documentation of the individual projects.

# [JAM] Prefer not to install always documentation (it does also when you select no to build it)
#    IF(BUILD_TOOLKIT_DOCUMENTATION)

#      ADD_CUSTOM_TARGET(GENERATE_${PROJECT_NAME}_DOCS
#        ALL
#        ${DOXYGEN}
#        ${${PROJECT_NAME}_BINARY_DIR}/docs/${PROJECT_NAME}/${PROJECT_NAME}doxygen.cfg
#      )

#    ELSE(BUILD_TOOLKIT_DOCUMENTATION)
# [endJAM]
      ADD_CUSTOM_TARGET(GENERATE_${PROJECT_NAME}_DOCS
        ${DOXYGEN}
        ${${PROJECT_NAME}_BINARY_DIR}/docs/${PROJECT_NAME}/${PROJECT_NAME}doxygen.cfg
      )
# [JAM] Prefer not to install always documentation (it does also when you select no to build it)
#    ENDIF(BUILD_TOOLKIT_DOCUMENTATION)
# [endJAM]

		
    FILE(GLOB
      ${PROJECT_NAME}_HTML_FILES
      "${${PROJECT_NAME}_BINARY_DIR}/docs/${PROJECT_NAME}/html/*"
    )
    INSTALL(FILES
      ${${PROJECT_NAME}_HTML_FILES}
      DESTINATION "share/html/${PROJECT_NAME}"
    )
    FILE(GLOB
      ${PROJECT_NAME}_TEX_FILES
      "${${PROJECT_NAME}_BINARY_DIR}/docs/${PROJECT_NAME}/latex/*"
    )
    INSTALL(FILES
      ${${PROJECT_NAME}_HTML_FILES}
      DESTINATION "share/latex/${PROJECT_NAME}"
    )

    # Add the installation prefixes of all the libraries. This will be used later to configure an
    # custom index.html file in order to can easily access each index page of each project.
    # Each projects Doxygen documentation will be installed in the ${INSTALL_PREFIX}/share/html/${PRJECT_NAME}
    # so this index.html file will be autogenerated by our CMake scripts to the ${INSTALL_PREFIX}/share/html
    # directory. This will be acctually done in the FINALISE_CILAB_TOOLKIT_DOCUMENTATION macro.
    SET(CILAB_TOOLKIT_HTML_DOC_INSTALL_DIRS
      "${CMAKE_INSTALL_PREFIX}/share/html/${PROJECT_NAME}"
      ${CILAB_TOOLKIT_HTML_DOC_INSTALL_DIRS}
      CACHE
      INTERNAL
      ""
    )

  ENDIF(DOXYGEN_FOUND)
  
# [JAM] Prefer not to install always documentation (it does also when you select no to build it) 
ENDIF(BUILD_TOOLKIT_DOCUMENTATION)
# [endJAM]
ENDMACRO(GENERATE_CILAB_PROJECT_DOCUMENTATION)


MACRO(FINALISE_CILAB_TOOLKIT_DOCUMENTATION)

  IF(BUILD_TOOLKIT_DOCUMENTATION)
    FOREACH(CILAB_TOOLKIT_HTML_DOC_INSTALL_DIR ${CILAB_TOOLKIT_HTML_DOC_INSTALL_DIRS})
      # Read the directory name for each directory in the CILAB_TOOLKIT_HTML_DOC_INSTALL_DIRS list
      # holding the paths to the documentation of all projects to the install path.
      STRING(REGEX
        REPLACE
        "^.*/" ""
        PROJECT_DOC_NAME
        ${CILAB_TOOLKIT_HTML_DOC_INSTALL_DIR}
      )
      SET(LINKS_TO_CILAB_PROJECTS_DOCUMENTATION
        "<a href=\"file:///${CILAB_TOOLKIT_HTML_DOC_INSTALL_DIR}/index.html\">${PROJECT_DOC_NAME}</a></br>"
        ${LINKS_TO_CILAB_PROJECTS_DOCUMENTATION}
      )

    ENDFOREACH(CILAB_TOOLKIT_HTML_DOC_INSTALL_DIR ${CILAB_TOOLKIT_HTML_DOC_INSTALL_DIRS})

  ENDIF(BUILD_TOOLKIT_DOCUMENTATION)

  # CMake separates elements in an appended variable or list by commas ";". I substitute every instance of
  # the comma with a new line
  STRING(REPLACE
    ";" "\n"
    LINKS_TO_CILAB_PROJECTS_DOCUMENTATION
    "${LINKS_TO_CILAB_PROJECTS_DOCUMENTATION}"
  )

  CONFIGURE_FILE("${CILAB_TOOLKIT_SOURCE_DIR}/cilabResources/documentation/CilabDocumentationIndexPage.html.in"
    "${CILAB_TOOLKIT_BINARY_DIR}/share/docs/index.html"
    @ONLY
    IMMEDIATE
  )
  INSTALL(FILES
    "${CILAB_TOOLKIT_BINARY_DIR}/share/docs/index.html"
    "${CILAB_TOOLKIT_SOURCE_DIR}/cilabResources/cilabDoxygen.css"
    "${CILAB_TOOLKIT_SOURCE_DIR}/cilabResources/headbg.jpg"
    DESTINATION "share/html"
  )

ENDMACRO(FINALISE_CILAB_TOOLKIT_DOCUMENTATION)

# Apply qt patch to avoid configuration warning. This patch saves the QTDIR
# environment variable during the CILab Toolkit configuration and in the end,
# the END_QT_PATCH recovers the original value. This patch is only applied in
# windows machines.
MACRO(START_QT_PATCH)
  IF(WIN32)
    SET(QTDIR_TMP
      $ENV{QTDIR}
    )
    SET(ENV{QTDIR}
      ${CILAB_TOOLKIT_SOURCE_DIR}/QT-3.3.7/qt-win-free-3.3.7
    )
  ENDIF(WIN32)
ENDMACRO(START_QT_PATCH)

# End QT patch. Recover the original value of the QTDIR environment variable
# to the value before the configuration.
MACRO(END_QT_PATCH)
  IF(WIN32)
    SET(ENV{QTDIR}
      ${QTDIR_TMP}
    )
  ENDIF(WIN32)
ENDMACRO(END_QT_PATCH)


#
# Adds definitions for linkage, specific to an application of a module
#
MACRO(SET_LINK_FLAGS_FOR_MODULE APP_NAME LINK_FLAGS)

	SET(${PROJECT_NAME_CAPITAL}_APP_${APP_NAME}_LINK_FLAGS
		${LINK_FLAGS}
		 CACHE
		 INTERNAL
		 ""
		 FORCE
	)

ENDMACRO(SET_LINK_FLAGS_FOR_MODULE)


# Init the variables to store the path for temporary data in Testing 
# modules and main data root folder for Toolkit
MACRO( INITIALISE_TESTING_FOLDERS )

  SET( TESTING_DIR_TEMPORARY ${CILAB_TOOLKIT_BINARY_DIR}/Temporary )
  MAKE_DIRECTORY( ${TESTING_DIR_TEMPORARY} )

  FIND_PATH( 
    TOOLKIT_DATA_ROOT 
    Data.readme 
    ${CILAB_TOOLKIT_SOURCE_DIR}/Data 
    DOC "Path of data using for testing" )

ENDMACRO( INITIALISE_TESTING_FOLDERS )

