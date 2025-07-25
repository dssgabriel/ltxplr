cmake_minimum_required(VERSION 3.25)

include(FetchContent)

if(NOT LTXPLR_LOCKFILE)
  set(LTXPLR_LOCKFILE "lockfile.json" CACHE FILEPATH "Path to the dependency lockfile for ltxplr.")
endif()

set(LTXPLR_projectDir "${CMAKE_CURRENT_LIST_DIR}/..")
message(TRACE "LTXPLR_projectDir=\"${LTXPLR_projectDir}\"")

message(TRACE "LTXPLR_LOCKFILE=\"${LTXPLR_LOCKFILE}\"")
file(REAL_PATH "${LTXPLR_LOCKFILE}" LTXPLR_lockfile BASE_DIRECTORY "${LTXPLR_projectDir}" EXPAND_TILDE)
message(DEBUG "Using lockfile: \"${LTXPLR_lockfile}\"")

# Force CMake to reconfigure the project if the lockfile changes
set_property(
  DIRECTORY
    "${LTXPLR_projectDir}"
  APPEND
  PROPERTY
    CMAKE_CONFIGURE_DEPENDS
      "${LTXPLR_lockfile}"
)

# For more on the protocol for this function, see:
# https://cmake.org/cmake/help/latest/command/cmake_language.html#provider-commands
function(LTXPLR_provideDependency method package_name)
  # Read the lockfile
  file(READ "${LTXPLR_lockfile}" LTXPLR_rootObj)

  # Get the "dependencies" field and store it in LTXPLR_dependenciesObj
  string(JSON LTXPLR_dependenciesObj ERROR_VARIABLE LTXPLR_error GET "${LTXPLR_rootObj}" "dependencies")
  if(LTXPLR_error)
    message(FATAL_ERROR "${LTXPLR_lockfile}: ${LTXPLR_error}")
  endif()

  # Get the length of the libraries array and store it in LTXPLR_dependenciesObj
  string(JSON LTXPLR_numDependencies ERROR_VARIABLE LTXPLR_error LENGTH "${LTXPLR_dependenciesObj}")
  if(LTXPLR_error)
    message(FATAL_ERROR "${LTXPLR_lockfile}: ${LTXPLR_error}")
  endif()

  if(LTXPLR_numDependencies EQUAL 0)
    return()
  endif()

  # Loop over each dependency object
  math(EXPR LTXPLR_maxIndex "${LTXPLR_numDependencies} - 1")
  foreach(LTXPLR_index RANGE "${LTXPLR_maxIndex}")
    set(LTXPLR_errorPrefix "${LTXPLR_lockfile}, dependency ${LTXPLR_index}")

    # Get the dependency object at LTXPLR_index
    # and store it in LTXPLR_depObj
    string(JSON LTXPLR_depObj ERROR_VARIABLE LTXPLR_error GET "${LTXPLR_dependenciesObj}" "${LTXPLR_index}")
    if(LTXPLR_error)
      message(FATAL_ERROR "${LTXPLR_errorPrefix}: ${LTXPLR_error}")
    endif()

    # Get the "name" field and store it in LTXPLR_name
    string(JSON LTXPLR_name ERROR_VARIABLE LTXPLR_error GET "${LTXPLR_depObj}" "name")
    if(LTXPLR_error)
      message(FATAL_ERROR "${LTXPLR_errorPrefix}: ${LTXPLR_error}")
    endif()

    # Get the "package_name" field and store it in LTXPLR_pkgName
    string(JSON LTXPLR_pkgName ERROR_VARIABLE LTXPLR_error GET "${LTXPLR_depObj}" "package_name")
    if(LTXPLR_error)
      message(FATAL_ERROR "${LTXPLR_errorPrefix}: ${LTXPLR_error}")
    endif()

    # Get the "git_repository" field and store it in LTXPLR_repo
    string(JSON LTXPLR_repo ERROR_VARIABLE LTXPLR_error GET "${LTXPLR_depObj}" "git_repository")
    if(LTXPLR_error)
      message(FATAL_ERROR "${LTXPLR_errorPrefix}: ${LTXPLR_error}")
    endif()

    # Get the "git_tag" field and store it in LTXPLR_tag
    string(JSON LTXPLR_tag ERROR_VARIABLE LTXPLR_error GET "${LTXPLR_depObj}" "git_tag")
    if(LTXPLR_error)
      message(FATAL_ERROR "${LTXPLR_errorPrefix}: ${LTXPLR_error}")
    endif()

    if(method STREQUAL "FIND_PACKAGE")
      if(package_name STREQUAL LTXPLR_pkgName)
        string(
          APPEND
          LTXPLR_debug
          "Redirecting find_package calls for ${LTXPLR_pkgName} "
          "to FetchContent logic fetching ${LTXPLR_repo} at "
          "${LTXPLR_tag} according to ${LTXPLR_lockfile}."
        )
        message(DEBUG "${LTXPLR_debug}")
        FetchContent_Declare("${LTXPLR_name}" GIT_REPOSITORY "${LTXPLR_repo}" GIT_TAG "${LTXPLR_tag}" EXCLUDE_FROM_ALL)
        set(INSTALL_GTEST OFF) # Disable GoogleTest installation
        FetchContent_MakeAvailable("${LTXPLR_name}")

        # Important! <PackageName>_FOUND tells CMake that `find_package` is
        # not needed for this package anymore
        set("${LTXPLR_pkgName}_FOUND" TRUE PARENT_SCOPE)
      endif()
    endif()
  endforeach()
endfunction()

cmake_language(SET_DEPENDENCY_PROVIDER LTXPLR_provideDependency SUPPORTED_METHODS FIND_PACKAGE)
