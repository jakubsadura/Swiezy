# Define settings for Intel Compiler in Altix 350
OPTION(INTEL_ALTIX350
  "Use Intel Compiler C/C++ in SGI Altix 350" 
  OFF
)

IF(INTEL_ALTIX350)
  SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)

  SET(CMAKE_CXX_COMPILER "icpc" CACHE STRING "Using Intel C++ Compiler" FORCE)
  SET(CMAKE_CXX_FLAGS "-mcpu=itanium2 -tpp2" CACHE STRING "Flag to generate code for Intel Itanium 64" FORCE)
  SET(CMAKE_CXX_FLAGS_DEBUG "-g" CACHE STRING "Flag to generate debug builds" FORCE)
  SET(CMAKE_CXX_FLAGS_MINSIZEREL "-Os" CACHE STRING "Flag to generate minsize builds" FORCE)
  SET(CMAKE_CXX_FLAGS_RELEASE "-O3" CACHE STRING "Flag to generate release builds" FORCE)
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g" CACHE STRING "Flag to generate release builds with debug information" FORCE)

  SET(CMAKE_C_COMPILER "icc" CACHE STRING "Using Intel C Compiler" FORCE)
  SET(CMAKE_C_FLAGS "-mcpu=itanium2 -tpp2" CACHE STRING "Flag to generate code for Intel Itanium 64" FORCE)
  SET(CMAKE_C_FLAGS_DEBUG "-g" CACHE STRING "Flag to generate debug builds" FORCE)
  SET(CMAKE_C_FLAGS_MINSIZEREL "-Os" CACHE STRING "Flag to generate minsize builds" FORCE)
  SET(CMAKE_C_FLAGS_RELEASE "-O3" CACHE STRING "Flag to generate release builds" FORCE)
  SET(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g" CACHE STRING "Flag to generate release builds with debug information" FORCE)
ENDIF ( INTEL_ALTIX350 )

MARK_AS_ADVANCED(INTEL_ALTIX350)

# We have compiled the windows binaries with the MSCV2003. We give the 
# MSCV2003 runtime libraries so it will run in machines that doesn't 
# have it installed or have a different version.
IF(WIN32)
  INSTALL(FILES
    "${CILAB_TOOLKIT_SOURCE_DIR}/cilabResources/DLLs/MSVC7.1/msvcr71.dll"
    "${CILAB_TOOLKIT_SOURCE_DIR}/cilabResources/DLLs/MSVC7.1/msvcp71.dll"
    "${CILAB_TOOLKIT_SOURCE_DIR}/cilabResources/DLLs/MSVC7.1/msvcr71d.dll"
    "${CILAB_TOOLKIT_SOURCE_DIR}/cilabResources/DLLs/MSVC7.1/msvcp71d.dll"
    "${CILAB_TOOLKIT_SOURCE_DIR}/cilabResources/DLLs/MSVC7.1/msvcrtd.dll"
    DESTINATION
    "${CMAKE_INSTALL_PREFIX}/cilabApps/GIMIAS"
  )
ENDIF(WIN32)