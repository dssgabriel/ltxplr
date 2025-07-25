# ltxplr

Exploring various implementation of multi-dimensional offset computations for tiled layouts.

### Requirements

- CMake 3.25+
- LLVM clang v19+ (w/ libc++)

### Build

Using CMake presets:
```sh
cmake --workflow --preset llvm-release
```

Manually:
```sh
cmake -B <BUILD_DIR> \
      -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=cmake/use-fetch-content.cmake \
      -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_CXX_STANDARD=26 \
      -DCMAKE_CXX_FLAGS='-stdlib=libc++'
cmake --build <BUILD_DIR>
```

### Execute

Run with a given number of dimensions:
```sh
./<BUILD_DIR>/bin/main <DIM>
```

Supported dimensions (as of 2025-06-25): `2`, `3`
