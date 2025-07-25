include_guard(GLOBAL)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(DEFAULT_FLAGS "-stdlib=libc++")

if(LTXPLR_BUILDSYS_SANITIZER STREQUAL "MaxSan")
  set(
    SANITIZER_FLAGS
    "-fsanitize=address -fsanitize=leak -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=undefined -fsanitize-undefined-trap-on-error"
  )
elseif(LTXPLR_BUILDSYS_SANITIZER STREQUAL "TSan")
  set(SANITIZER_FLAGS "-fsanitize=thread")
endif()

set(
  DEBUG_FLAGS
  "-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wold-style-cast -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wdouble-promotion -Wformat=2 -fno-omit-frame-pointer -fno-inline -fno-common -fstack-protector-strong -ftrapv -pipe ${DEFAULT_FLAGS} ${SANITIZER_FLAGS}"
)

set(CMAKE_C_FLAGS_DEBUG_INIT "${DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "${DEBUG_FLAGS}")

set(RELEASE_FLAGS "-O3 ${DEFAULT_FLAGS} ${SANITIZER_FLAGS}")

set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "${RELEASE_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "${RELEASE_FLAGS}")

set(CMAKE_C_FLAGS_RELEASE_INIT "${RELEASE_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "${RELEASE_FLAGS}")
