find_package(fmt)
find_package(nanobench)

add_executable(main)
target_sources(main PRIVATE main.cpp)
target_compile_options(main PRIVATE -march=native)
target_link_libraries(
  main
  PRIVATE
    ltxplr::ltxplr
    fmt::fmt
    nanobench
)
if(LTXPLR_ENABLE_OMP)
  target_compile_definitions(main PRIVATE -DLTXPLR_ENABLE_OMP)
  target_link_libraries(main PRIVATE OpenMP::OpenMP_CXX)
endif()
