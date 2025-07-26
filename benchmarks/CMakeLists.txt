find_package(fmt)
find_package(nanobench)

set(
  ALL_BENCHMARKS
  ltl
  rtr
)

foreach(bench ${ALL_BENCHMARKS})
  add_executable(${bench})
  target_sources(${bench} PRIVATE ${bench}.cpp)
  target_compile_options(${bench} PRIVATE -march=native)
  target_link_libraries(
    ${bench}
    PRIVATE
      ltxplr::ltxplr
      fmt::fmt
      nanobench
  )
  if(LTXPLR_ENABLE_OMP)
    target_compile_definitions(${bench} PRIVATE -DLTXPLR_ENABLE_OMP)
    target_link_libraries(${bench} PRIVATE OpenMP::OpenMP_CXX)
  endif()
endforeach()
