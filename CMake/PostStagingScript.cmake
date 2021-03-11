
set(STAGING_DIR "${CPACK_OUTPUT_FILE_PREFIX}/${CPACK_PACKAGE_FILE_NAME}-files")

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

# The JSON file is not yet created, but we can copy all the staging files to
# a stable directory to be later zipped with the JSON file.
execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CPACK_TEMPORARY_DIRECTORY}/" "${STAGING_DIR}"
)
