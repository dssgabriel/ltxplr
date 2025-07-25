add_library(ltxplr INTERFACE)
add_library(ltxplr::ltxplr ALIAS ltxplr)

target_sources(
  ltxplr
  INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include
    FILES ltxplr.hpp ltl.hpp
)
