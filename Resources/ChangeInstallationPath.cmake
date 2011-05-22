# This cmake file gets installed in the "share" directory of
# the installation prefix and gets renamed as CMakeLists.txt.
# When a distribution is created and the installation
# directory gets moved to another machine (or other
# directory in the same one), this CMake should be ran
# to reconfigure the paths of the Use<ProjectName>.cmake
# & <ProjectName>Cofig.cmake files to reflect the correct
# new installation prefix.

# CMake file used to configure the distribution
# to the new installation location. User should
# run the ccmake on this file and the distribution
# will be configured by it self.

PROJECT(CONFIGURE)

# Include file which indicates the path to the old
# installation prefix in order to update the
# EvoAPIConfig.cmake file information.
INCLUDE(old_installation_directory.txt)

# Returns the parents directory
STRING(REGEX
  REPLACE
  "/[^/]*$"
  ""
  NEW_INSTALLATION_DIRECTORY
  "${CONFIGURE_SOURCE_DIR}"
)

# Read all the projects <ProjectName>Config.cmake
# file names in a variable
FILE(GLOB
  PROJECT_CONFIG_CMAKE_FILES
  "../*Config.cmake"
)

FOREACH(CONFIG_CMAKE_FILE ${PROJECT_CONFIG_CMAKE_FILES})
  # Reads the <ProjectName>config.cmake file to the
  # <ProjectName>config.cmake_Contents_String variable.
  FILE(READ
    "${CONFIG_CMAKE_FILE}"
    ${CONFIG_CMAKE_FILE}_Contents_String
  )

  # Substitute the old installation prefix with
  # the new installation prefix
  STRING(REGEX
    REPLACE
    "${OLD_INSTALLATION_DIRECTORY}"
    "${NEW_INSTALLATION_DIRECTORY}"
    ${CONFIG_CMAKE_FILE}_Contents_String
    "${${CONFIG_CMAKE_FILE}_Contents_String}"
  )

  # Write back the file with the correct paths to the
  # <ProjectName> include and library directories.
  FILE(WRITE
    "${CONFIG_CMAKE_FILE}"
    "${${CONFIG_CMAKE_FILE}_Contents_String}"
  )
ENDFOREACH(CONFIG_CMAKE_FILE)

# Notify user about the new installation prefix!
MESSAGE(STATUS
  "New installation prefix set to ${NEW_INSTALLATION_DIRECTORY} ..."
)

# Rewrite the old_installation_directory.txt to contain the
# new directory for the next time the library will be moved.
FILE(WRITE
  "${CONFIGURE_SOURCE_DIR}/old_installation_directory.txt"
  "SET(OLD_INSTALLATION_DIRECTORY ${NEW_INSTALLATION_DIRECTORY})"
)
