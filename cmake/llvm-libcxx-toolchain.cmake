include(${CMAKE_CURRENT_LIST_DIR}/llvm-toolchain.cmake)

if(NOT CMAKE_CXX_FLAGS MATCHES "-stdlib=libc\\+\\+")
  string(APPEND CMAKE_CXX_FLAGS " -stdlib=libc++")
endif()
