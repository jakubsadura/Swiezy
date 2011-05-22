# InstallRequiredSystemLibraries is a standard cmake include file
INCLUDE(InstallRequiredSystemLibraries)


SET(CPACK_PACKAGE_FILE_NAME
  "${PROJECT_APPLICATION_TITLE}"
)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY
  "Gimias"
)
SET(CPACK_PACKAGE_VENDOR
  "${STUDIO_NAME}"
)
SET(CPACK_PACKAGE_DESCRIPTION_FILE
  "${CILAB_TOOLKIT_SOURCE_DIR}/cilabResources/documentation/buildToolkit.txt"
)
# SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "7")
SET(CPACK_PACKAGE_VERSION_PATCH "3")

SET(CPACK_PACKAGE_INSTALL_DIRECTORY
  "CILab"
)

IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  #SET(CPACK_PACKAGE_ICON
  #  "${CILAB_TOOLKIT_SOURCE_DIR}/cilabResources\\\\headbg.jpg"
  #)
  SET(CPACK_NSIS_DISPLAY_NAME
    "${CPACK_PACKAGE_INSTALL_DIRECTORY} CILab Laboratory"
  )
  SET(CPACK_NSIS_HELP_LINK
    "http:\\\\\\\\www.cilab.upf.edu\\\\gimias"
  )
  SET(CPACK_NSIS_URL_INFO_ABOUT
    "http:\\\\\\\\www.cilab.upf.edu\\\\gimias"
  )
  SET(CPACK_NSIS_CONTACT
    "developers@cilab.upf.edu"
  )
  SET(CPACK_NSIS_MODIFY_PATH
    ON
  )
  SET(CPACK_GENERATOR
    "NSIS"
  )
ELSE(WIN32 AND NOT UNIX)

  SET(CPACK_SOURCE_STRIP_FILES "")
  SET(CPACK_GENERATOR
    "STGZ"
  )
ENDIF(WIN32 AND NOT UNIX)

SET(CPACK_PACKAGE_EXECUTABLES
  "gimias"
  "Icon64"
)

INCLUDE(CPack)
