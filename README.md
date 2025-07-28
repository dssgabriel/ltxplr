# LT-XPLR: Exploring tiled multi-dimensional offset computations for performance

### Requirements

- CMake 3.25+
- LLVM clang++ v19+
- LLVM libc++ v18+

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

Run on a given layout, with a given number of dimensions:
```sh
./<BUILD_DIR>/benchmarks/<LAYOUT> <DIM>
```

Supported layouts (as of 2025-07-29): `ltl`, `rtr` 

Supported dimensions (as of 2025-07-25): `2`, `3`, `4` (partial)
