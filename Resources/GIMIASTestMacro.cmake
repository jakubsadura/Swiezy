
MACRO(TEST_GIMIAS _site_name _build_name _python_path _console_path _svn_app _csnake_config _resource_path _bin_path _source_path )

#############################
# INIT
set(CTEST_PROJECT_NAME "GIMIAS")
set(CTEST_SITE  "${_site_name}")
set(CTEST_BUILD_NAME "${_build_name}")
set(PYTHON_PATH "${_python_path}")
set(CSNAKE_CONSOLE_PATH "${_console_path}")
set(CSNAKE_GIMIAS_CONFIG "${_csnake_config}")

#############################
# START
set(CTEST_SOURCE_DIRECTORY "${_resource_path}")
set(CTEST_BINARY_DIRECTORY "${_bin_path}")
ctest_start (Nightly)

#############################
# UPDATE
set(CTEST_SOURCE_DIRECTORY "${_source_path}")
set(CTEST_UPDATE_COMMAND "${_svn_app}")
SET(CTEST_CHECKOUT_COMMAND "\"${CTEST_UPDATE_COMMAND}\" update --non-interactive")
ctest_update (SOURCE "${CTEST_SOURCE_DIRECTORY}")

#############################
# CONFIGURE
set(CTEST_CONFIGURE_COMMAND "${PYTHON_PATH} ${CSNAKE_CONSOLE_PATH} ${CSNAKE_GIMIAS_CONFIG} -c" )
ctest_configure(BUILD "${CTEST_BINARY_DIRECTORY}")

#############################
# BUILD
set(CTEST_BUILD_COMMAND "${PYTHON_PATH} ${CSNAKE_CONSOLE_PATH} ${CSNAKE_GIMIAS_CONFIG} -b" )
ctest_build(BUILD "${CTEST_BINARY_DIRECTORY}")

#############################
# SUBMIT
ctest_submit()

ENDMACRO(TEST_GIMIAS)
