# ltxplr

Exploration repository for various implementation of multi-dimensional offset calculations.

### Build

Using CMake presets:
```sh
cmake --workflow --preset llvm-release
./build/llvm-release/bin/main <DIM>
```

Manually:
```sh
cmake -B build \
      -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=cmake/use-fetch-content.cmake \
      -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_CXX_STANDARD=26 \
      -DCMAKE_CXX_FLAGS='-stdlib=libc++'
cmake --build build
./build/bin/main <DIM>
```
