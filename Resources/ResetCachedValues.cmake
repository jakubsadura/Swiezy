# Here are the variables that we want to be reseted
# in each CMake configuration. If not reseted here,
# there values will be appended in each configuration

# Cleanup cached variables between consecutive invocations.
FOREACH(CILAB_MODULE ${CILAB_PROJECTS_NAME} ${THIRDPARTY_PROJECTS_NAME})
  CLEAR_CACHED_VARIABLE(${CILAB_MODULE}_DEPENDS_FROM)
ENDFOREACH(CILAB_MODULE)

FOREACH(CILAB_PLUGIN ${GIMIAS-PLATFORM_PROJECTS_CHOOSEN})
  CLEAR_CACHED_VARIABLE(${CILAB_PLUGIN}_DEPENDS_FROM)
ENDFOREACH(CILAB_PLUGIN)

CLEAR_CACHED_VARIABLE(CILAB_PROJECTS_CHOOSEN)
CLEAR_CACHED_VARIABLE(CoreLib_DEPENDS_FROM)
CLEAR_CACHED_VARIABLE(GIMIAS-PLATFORM_PROJECTS_CHOOSEN)
CLEAR_CACHED_VARIABLE(CILAB_TOOLKIT_TO_BE_COMPILED)

# Clean this variable so it doesn't get appended troughout different 
# configurations. This is setted by ITK & VTK to the path that the dynamic
# libraries will be installed. The toolkit will configure Gimias launcher 
# to export the LD_LIBRARY_PATH with this paths and then launch Gimias.
CLEAR_CACHED_VARIABLE(LD_GIMIAS_PATH_BIN)
CLEAR_CACHED_VARIABLE(LD_GIMIAS_PATH_INST)

# Clear the AVAILABLE_CILAB_PROJECTS holding names and
# description of the available projects on the toolkit.
CLEAR_CACHED_VARIABLE(AVAILABLE_CILAB_PROJECTS)
CLEAR_CACHED_VARIABLE(CILAB_PROJECTS_TYPE)
CLEAR_CACHED_VARIABLE(AVAILABLE_THIRDPARTY_PROJECTS)
CLEAR_CACHED_VARIABLE(CILAB_TOOLKIT_HTML_DOC_INSTALL_DIRS)
CLEAR_CACHED_VARIABLE(LINKS_TO_CILAB_PROJECTS_DOCUMENTATION)
SET(CILABTOOLKIT_PACKAGES
  OFF
  CACHE
  BOOL
  "See packages available in the cilab toolkit."
  FORCE
)