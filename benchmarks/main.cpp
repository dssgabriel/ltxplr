#include <cstddef>
#include <mdspan>

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <nanobench.h>

#include <ltxplr/ltxplr.hpp>

namespace nb = ankerl::nanobench;

namespace {

template <class Exts, size_t... TExts, class... I>
auto check(I... idxs) -> void
  requires (Exts::rank() == sizeof...(TExts) and Exts::rank() == sizeof...(I))
{
  using layout_ltl = ltxplr::ltl<TExts...>::template mapping<Exts>;
  layout_ltl map{};

  constexpr size_t R = Exts::rank();
  std::array<size_t, R> idx_arr{static_cast<size_t>(idxs)...};

  fmt::println("to_off_ld  ({}) = {}", idx_arr, map.tile_outer_offset_for_direct(idxs...));
  fmt::println("to_off_lds ({}) = {}", idx_arr, map.tile_outer_offset_for_direct_strides(idxs...));
  fmt::println("to_off_lp  ({}) = {}", idx_arr, map.tile_outer_offset_for_prep(idxs...));
  fmt::println("to_off_lps ({}) = {}", idx_arr, map.tile_outer_offset_for_prep_strides(idxs...));
  fmt::println("to_off_ed  ({}) = {}", idx_arr, map.tile_outer_offset_fold_direct(idxs...));
  fmt::println("to_off_eds ({}) = {}", idx_arr, map.tile_outer_offset_fold_direct_strides(idxs...));
  fmt::println("to_off_ep  ({}) = {}", idx_arr, map.tile_outer_offset_fold_prep(idxs...));
  fmt::println("to_off_eps ({}) = {}", idx_arr, map.tile_outer_offset_fold_prep_strides(idxs...));

  fmt::println("ti_off_ld  ({}) = {}", idx_arr, map.tile_inner_offset_for_direct(idxs...));
  fmt::println("ti_off_lds ({}) = {}", idx_arr, map.tile_inner_offset_for_direct_strides(idxs...));
  fmt::println("ti_off_lp  ({}) = {}", idx_arr, map.tile_inner_offset_for_prep(idxs...));
  fmt::println("ti_off_lps ({}) = {}", idx_arr, map.tile_inner_offset_for_prep_strides(idxs...));
  fmt::println("ti_off_ed  ({}) = {}", idx_arr, map.tile_inner_offset_fold_direct(idxs...));
  fmt::println("ti_off_eds ({}) = {}", idx_arr, map.tile_inner_offset_fold_direct_strides(idxs...));
  fmt::println("ti_off_ep  ({}) = {}", idx_arr, map.tile_inner_offset_fold_prep(idxs...));
  fmt::println("ti_off_eps ({}) = {}", idx_arr, map.tile_inner_offset_fold_prep_strides(idxs...));

  fmt::println("off_l       ({}) = {}", idx_arr, map.offset_for(idxs...));
  fmt::println("off_e       ({}) = {}", idx_arr, map.offset_fold(idxs...));
}

template <class Exts, size_t... TExts, class... I>
auto eval_2d(I... idxs) -> void
  requires (2 == Exts::rank() and 2 == sizeof...(TExts) and 2 == sizeof...(I))
{
  using layout_ltl = ltxplr::ltl<TExts...>::template mapping<Exts>;
  layout_ltl map;
  auto ext = map.extents();

  // clang-format off
  nb::Bench b0;
  b0.title("tile outer offset").relative(true);
  b0.run("tile_outer_offset for direct", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_direct(idxs...)); });
  b0.run("tile_outer_offset for direct strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_direct_strides(idxs...)); });
  b0.run("tile_outer_offset for prep", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_prep(idxs...)); });
  b0.run("tile_outer_offset for prep strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_prep_strides(idxs...)); });
  b0.run("tile_outer_offset fold direct", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_direct(idxs...)); });
  b0.run("tile_outer_offset fold direct strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_direct_strides(idxs...)); });
  b0.run("tile_outer_offset fold prep", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_prep(idxs...)); });
  b0.run("tile_outer_offset fold prep strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_prep_strides(idxs...)); });

  nb::Bench b1;
  b1.title("tile inner offset").relative(true);
  b1.run("tile_inner_offset for direct", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_for_direct(idxs...)); });
  b1.run("tile_inner_offset for direct strides", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_for_direct_strides(idxs...)); });
  b1.run("tile_inner_offset for prep", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_for_prep(idxs...)); });
  b1.run("tile_inner_offset for prep strides", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_for_prep_strides(idxs...)); });
  b1.run("tile_inner_offset fold direct", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_fold_direct(idxs...)); });
  b1.run("tile_inner_offset fold direct strides", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_fold_direct_strides(idxs...)); });
  b1.run("tile_inner_offset fold prep", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_fold_prep(idxs...)); });
  b1.run("tile_inner_offset fold prep strides", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_fold_prep_strides(idxs...)); });

  nb::Bench b2;
  b2.title("full offset").relative(true);
  b2.run("ltl_offset for direct", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_direct(idxs...) + map.tile_inner_offset_for_direct(idxs...)); });
  b2.run("ltl_offset for direct strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_direct_strides(idxs...) + map.tile_inner_offset_for_direct_strides(idxs...)); });
  b2.run("ltl_offset for prep", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_prep(idxs...) + map.tile_inner_offset_for_prep(idxs...)); });
  b2.run("ltl_offset for prep strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_prep_strides(idxs...) + map.tile_inner_offset_for_prep_strides(idxs...)); });
  b2.run("ltl_offset fold direct", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_direct(idxs...) + map.tile_inner_offset_fold_direct(idxs...)); });
  b2.run("ltl_offset fold direct strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_direct_strides(idxs...) + map.tile_inner_offset_fold_direct_strides(idxs...)); });
  b2.run("ltl_offset fold prep", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_prep(idxs...) + map.tile_inner_offset_fold_prep(idxs...)); });
  b2.run("ltl_offset fold prep strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_prep_strides(idxs...) + map.tile_inner_offset_fold_prep_strides(idxs...)); });
  b2.run("offset for (Kokkos impl)", [&]() { nb::doNotOptimizeAway(map.offset_for(0, 0)); });
  b2.run("offset fold (LLVM impl)", [&]() { nb::doNotOptimizeAway(map.offset_fold(0, 0)); });
  // clang-format on

  nb::Bench b3;
  b3.title("loop full offset").relative(true);
  b3.run("ltl_offset for direct", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        tmp = map.tile_outer_offset_for_direct(i, j) + map.tile_inner_offset_for_direct(i, j);
        nb::doNotOptimizeAway(tmp);
      }
    }
  });
  b3.run("ltl_offset for direct strides", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        tmp = map.tile_outer_offset_for_direct_strides(i, j) + map.tile_inner_offset_for_direct_strides(i, j);
        nb::doNotOptimizeAway(tmp);
      }
    }
  });
  b3.run("ltl_offset for prep", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        tmp = map.tile_outer_offset_for_prep(i, j) + map.tile_inner_offset_for_prep(i, j);
        nb::doNotOptimizeAway(tmp);
      }
    }
  });
  b3.run("ltl_offset for prep strides", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        tmp = map.tile_outer_offset_for_prep_strides(i, j) + map.tile_inner_offset_for_prep_strides(i, j);
        nb::doNotOptimizeAway(tmp);
      }
    }
  });
  b3.run("ltl_offset fold direct", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        tmp = map.tile_outer_offset_fold_direct(i, j) + map.tile_inner_offset_fold_direct(i, j);
        nb::doNotOptimizeAway(tmp);
      }
    }
  });
  b3.run("ltl_offset fold direct strides", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        tmp = map.tile_outer_offset_fold_direct_strides(i, j) + map.tile_inner_offset_fold_direct_strides(i, j);
        nb::doNotOptimizeAway(tmp);
      }
    }
  });
  b3.run("ltl_offset fold prep", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        tmp = map.tile_outer_offset_fold_prep(i, j) + map.tile_inner_offset_fold_prep(i, j);
        nb::doNotOptimizeAway(tmp);
      }
    }
  });
  b3.run("ltl_offset fold prep strides", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        tmp = map.tile_outer_offset_fold_prep_strides(i, j) + map.tile_inner_offset_fold_prep_strides(i, j);
        nb::doNotOptimizeAway(tmp);
      }
    }
  });
  b3.run("offset for (Kokkos impl)", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        tmp = map.offset_for(i, j);
        nb::doNotOptimizeAway(tmp);
      }
    }
  });
  b3.run("offset fold (LLVM impl)", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        tmp = map.offset_fold(i, j);
        nb::doNotOptimizeAway(tmp);
      }
    }
  });

  nb::Bench b4;
  b4.title("block loop full offset").relative(true);
  b4.run("ltl_offset for direct", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        const size_t blk = map.tile_outer_offset_for_direct(x, y);
        for (size_t i = x; i < TExts...[0]; ++i) {
          for (size_t j = y; j < TExts...[1]; ++j) {
            tmp = blk + map.tile_inner_offset_for_direct(i, j);
            nb::doNotOptimizeAway(tmp);
          }
        }
      }
    }
  });
  b4.run("ltl_offset for direct strides", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        const size_t blk = map.tile_outer_offset_for_direct_strides(x, y);
        for (size_t i = x; i < TExts...[0]; ++i) {
          for (size_t j = y; j < TExts...[1]; ++j) {
            tmp = blk + map.tile_inner_offset_for_direct_strides(i, j);
            nb::doNotOptimizeAway(tmp);
          }
        }
      }
    }
  });
  b4.run("ltl_offset for prep", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        const size_t blk = map.tile_outer_offset_for_prep(x, y);
        for (size_t i = x; i < TExts...[0]; ++i) {
          for (size_t j = y; j < TExts...[1]; ++j) {
            tmp = blk + map.tile_inner_offset_for_prep(i, j);
            nb::doNotOptimizeAway(tmp);
          }
        }
      }
    }
  });
  b4.run("ltl_offset for prep strides", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        const size_t blk = map.tile_outer_offset_for_prep_strides(x, y);
        for (size_t i = x; i < TExts...[0]; ++i) {
          for (size_t j = y; j < TExts...[1]; ++j) {
            tmp = blk + map.tile_inner_offset_for_prep_strides(i, j);
            nb::doNotOptimizeAway(tmp);
          }
        }
      }
    }
  });
  b4.run("ltl_offset fold direct", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        const size_t blk = map.tile_outer_offset_fold_direct(x, y);
        for (size_t i = x; i < TExts...[0]; ++i) {
          for (size_t j = y; j < TExts...[1]; ++j) {
            tmp = blk + map.tile_inner_offset_fold_direct(i, j);
            nb::doNotOptimizeAway(tmp);
          }
        }
      }
    }
  });
  b4.run("ltl_offset fold direct strides", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        const size_t blk = map.tile_outer_offset_fold_direct_strides(x, y);
        for (size_t i = x; i < TExts...[0]; ++i) {
          for (size_t j = y; j < TExts...[1]; ++j) {
            tmp = blk + map.tile_inner_offset_fold_direct_strides(i, j);
            nb::doNotOptimizeAway(tmp);
          }
        }
      }
    }
  });
  b4.run("ltl_offset fold prep", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        const size_t blk = map.tile_outer_offset_fold_prep(x, y);
        for (size_t i = x; i < TExts...[0]; ++i) {
          for (size_t j = y; j < TExts...[1]; ++j) {
            tmp = blk + map.tile_inner_offset_fold_prep(i, j);
            nb::doNotOptimizeAway(tmp);
          }
        }
      }
    }
  });
  b4.run("ltl_offset fold prep strides", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        const size_t blk = map.tile_outer_offset_fold_prep_strides(x, y);
        for (size_t i = x; i < TExts...[0]; ++i) {
          for (size_t j = y; j < TExts...[1]; ++j) {
            tmp = blk + map.tile_inner_offset_fold_prep_strides(i, j);
            nb::doNotOptimizeAway(tmp);
          }
        }
      }
    }
  });
  b4.run("offset for (Kokkos impl)", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        for (size_t i = x; i < TExts...[0]; ++i) {
          for (size_t j = y; j < TExts...[1]; ++j) {
            tmp = map.offset_for(i, j);
            nb::doNotOptimizeAway(tmp);
          }
        }
      }
    }
  });
  b4.run("offset fold (LLVM impl)", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        for (size_t i = x; i < TExts...[0]; ++i) {
          for (size_t j = y; j < TExts...[1]; ++j) {
            tmp = map.offset_fold(i, j);
            nb::doNotOptimizeAway(tmp);
          }
        }
      }
    }
  });
}

template <class Exts, size_t... TExts, class... I>
auto eval_3d(I... idxs) -> void
  requires (3 == Exts::rank() and 3 == sizeof...(TExts) and 3 == sizeof...(I))
{
  using layout_ltl = ltxplr::ltl<TExts...>::template mapping<Exts>;
  layout_ltl map;
  auto ext = map.extents();

  // clang-format off
  nb::Bench b0;
  b0.title("tile outer offset").relative(true);
  b0.run("tile_outer_offset for direct", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_direct(idxs...)); });
  b0.run("tile_outer_offset for direct strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_direct_strides(idxs...)); });
  b0.run("tile_outer_offset for prep", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_prep(idxs...)); });
  b0.run("tile_outer_offset for prep strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_prep_strides(idxs...)); });
  b0.run("tile_outer_offset fold direct", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_direct(idxs...)); });
  b0.run("tile_outer_offset fold direct strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_direct_strides(idxs...)); });
  b0.run("tile_outer_offset fold prep", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_prep(idxs...)); });
  b0.run("tile_outer_offset fold prep strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_prep_strides(idxs...)); });

  nb::Bench b1;
  b1.title("tile inner offset").relative(true);
  b1.run("tile_inner_offset for direct", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_for_direct(idxs...)); });
  b1.run("tile_inner_offset for direct strides", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_for_direct_strides(idxs...)); });
  b1.run("tile_inner_offset for prep", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_for_prep(idxs...)); });
  b1.run("tile_inner_offset for prep strides", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_for_prep_strides(idxs...)); });
  b1.run("tile_inner_offset fold direct", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_fold_direct(idxs...)); });
  b1.run("tile_inner_offset fold direct strides", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_fold_direct_strides(idxs...)); });
  b1.run("tile_inner_offset fold prep", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_fold_prep(idxs...)); });
  b1.run("tile_inner_offset fold prep strides", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_fold_prep_strides(idxs...)); });

  nb::Bench b2;
  b2.title("full offset").relative(true);
  b2.run("ltl_offset for direct", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_direct(idxs...) + map.tile_inner_offset_for_direct(idxs...)); });
  b2.run("ltl_offset for direct strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_direct_strides(idxs...) + map.tile_inner_offset_for_direct_strides(idxs...)); });
  b2.run("ltl_offset for prep", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_prep(idxs...) + map.tile_inner_offset_for_prep(idxs...)); });
  b2.run("ltl_offset for prep strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_prep_strides(idxs...) + map.tile_inner_offset_for_prep_strides(idxs...)); });
  b2.run("ltl_offset fold direct", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_direct(idxs...) + map.tile_inner_offset_fold_direct(idxs...)); });
  b2.run("ltl_offset fold direct strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_direct_strides(idxs...) + map.tile_inner_offset_fold_direct_strides(idxs...)); });
  b2.run("ltl_offset fold prep", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_prep(idxs...) + map.tile_inner_offset_fold_prep(idxs...)); });
  b2.run("ltl_offset fold prep strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_prep_strides(idxs...) + map.tile_inner_offset_fold_prep_strides(idxs...)); });
  b2.run("offset for (Kokkos impl)", [&]() { nb::doNotOptimizeAway(map.offset_for(idxs...)); });
  b2.run("offset fold (LLVM impl)", [&]() { nb::doNotOptimizeAway(map.offset_fold(idxs...)); });
  // clang-format on

  nb::Bench b3;
  b3.title("loop full offset").relative(true);
  b3.run("ltl_offset for direct", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        for (size_t k = 0; k < ext.extent(2); ++k) {
          tmp = map.tile_outer_offset_for_direct(i, j, k) + map.tile_inner_offset_for_direct(i, j, k);
          nb::doNotOptimizeAway(tmp);
        }
      }
    }
  });
  b3.run("ltl_offset for direct strides", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        for (size_t k = 0; k < ext.extent(2); ++k) {
          tmp = map.tile_outer_offset_for_direct_strides(i, j, k) + map.tile_inner_offset_for_direct_strides(i, j, k);
          nb::doNotOptimizeAway(tmp);
        }
      }
    }
  });
  b3.run("ltl_offset for prep", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        for (size_t k = 0; k < ext.extent(2); ++k) {
          tmp = map.tile_outer_offset_for_prep(i, j, k) + map.tile_inner_offset_for_prep(i, j, k);
          nb::doNotOptimizeAway(tmp);
        }
      }
    }
  });
  b3.run("ltl_offset for prep strides", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        for (size_t k = 0; k < ext.extent(2); ++k) {
          tmp = map.tile_outer_offset_for_prep_strides(i, j, k) + map.tile_inner_offset_for_prep_strides(i, j, k);
          nb::doNotOptimizeAway(tmp);
        }
      }
    }
  });
  b3.run("ltl_offset fold direct", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        for (size_t k = 0; k < ext.extent(2); ++k) {
          tmp = map.tile_outer_offset_fold_direct(i, j, k) + map.tile_inner_offset_fold_direct(i, j, k);
          nb::doNotOptimizeAway(tmp);
        }
      }
    }
  });
  b3.run("ltl_offset fold direct strides", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        for (size_t k = 0; k < ext.extent(2); ++k) {
          tmp
            = map.tile_outer_offset_fold_direct_strides(i, j, k) + map.tile_inner_offset_fold_direct_strides(i, j, k);
          nb::doNotOptimizeAway(tmp);
        }
      }
    }
  });
  b3.run("ltl_offset fold prep", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        for (size_t k = 0; k < ext.extent(2); ++k) {
          tmp = map.tile_outer_offset_fold_prep(i, j, k) + map.tile_inner_offset_fold_prep(i, j, k);
          nb::doNotOptimizeAway(tmp);
        }
      }
    }
  });
  b3.run("ltl_offset fold prep strides", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        for (size_t k = 0; k < ext.extent(2); ++k) {
          tmp = map.tile_outer_offset_fold_prep_strides(i, j, k) + map.tile_inner_offset_fold_prep_strides(i, j, k);
          nb::doNotOptimizeAway(tmp);
        }
      }
    }
  });
  b3.run("offset for (Kokkos impl)", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        for (size_t k = 0; k < ext.extent(2); ++k) {
          tmp = map.offset_for(i, j, k);
          nb::doNotOptimizeAway(tmp);
        }
      }
    }
  });
  b3.run("offset fold (LLVM impl)", [&]() {
    auto tmp = 0UL;
    for (size_t i = 0; i < ext.extent(0); ++i) {
      for (size_t j = 0; j < ext.extent(1); ++j) {
        for (size_t k = 0; k < ext.extent(2); ++k) {
          tmp = map.offset_fold(i, j, k);
          nb::doNotOptimizeAway(tmp);
        }
      }
    }
  });

  nb::Bench b4;
  b4.title("block loop full offset").relative(true);
  b4.run("ltl_offset for direct", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        for (size_t z = 0; z < ext.extent(2); z += TExts...[2]) {
          const size_t blk = map.tile_outer_offset_for_direct(x, y, z);
          for (size_t i = x; i < TExts...[0]; ++i) {
            for (size_t j = y; j < TExts...[1]; ++j) {
              for (size_t k = z; k < TExts...[2]; ++k) {
                tmp = blk + map.tile_inner_offset_for_direct(i, j, k);
                nb::doNotOptimizeAway(tmp);
              }
            }
          }
        }
      }
    }
  });
  b4.run("ltl_offset for direct strides", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        for (size_t z = 0; z < ext.extent(2); z += TExts...[2]) {
          const size_t blk = map.tile_outer_offset_for_direct_strides(x, y, z);
          for (size_t i = x; i < TExts...[0]; ++i) {
            for (size_t j = y; j < TExts...[1]; ++j) {
              for (size_t k = z; k < TExts...[2]; ++k) {
                tmp = blk + map.tile_inner_offset_for_direct_strides(i, j, k);
                nb::doNotOptimizeAway(tmp);
              }
            }
          }
        }
      }
    }
  });
  b4.run("ltl_offset for prep", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        for (size_t z = 0; z < ext.extent(2); z += TExts...[2]) {
          const size_t blk = map.tile_outer_offset_for_prep(x, y, z);
          for (size_t i = x; i < TExts...[0]; ++i) {
            for (size_t j = y; j < TExts...[1]; ++j) {
              for (size_t k = z; k < TExts...[2]; ++k) {
                tmp = blk + map.tile_inner_offset_for_prep(i, j, k);
                nb::doNotOptimizeAway(tmp);
              }
            }
          }
        }
      }
    }
  });
  b4.run("ltl_offset for prep strides", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        for (size_t z = 0; z < ext.extent(2); z += TExts...[2]) {
          const size_t blk = map.tile_outer_offset_for_prep_strides(x, y, z);
          for (size_t i = x; i < TExts...[0]; ++i) {
            for (size_t j = y; j < TExts...[1]; ++j) {
              for (size_t k = z; k < TExts...[2]; ++k) {
                tmp = blk + map.tile_inner_offset_for_prep_strides(i, j, k);
                nb::doNotOptimizeAway(tmp);
              }
            }
          }
        }
      }
    }
  });
  b4.run("ltl_offset fold direct", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        for (size_t z = 0; z < ext.extent(2); z += TExts...[2]) {
          const size_t blk = map.tile_outer_offset_fold_direct(x, y, z);
          for (size_t i = x; i < TExts...[0]; ++i) {
            for (size_t j = y; j < TExts...[1]; ++j) {
              for (size_t k = z; k < TExts...[2]; ++k) {
                tmp = blk + map.tile_inner_offset_fold_direct(i, j, k);
                nb::doNotOptimizeAway(tmp);
              }
            }
          }
        }
      }
    }
  });
  b4.run("ltl_offset fold direct strides", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        for (size_t z = 0; z < ext.extent(2); z += TExts...[2]) {
          const size_t blk = map.tile_outer_offset_fold_direct_strides(x, y, z);
          for (size_t i = x; i < TExts...[0]; ++i) {
            for (size_t j = y; j < TExts...[1]; ++j) {
              for (size_t k = z; k < TExts...[2]; ++k) {
                tmp = blk + map.tile_inner_offset_fold_direct_strides(i, j, k);
                nb::doNotOptimizeAway(tmp);
              }
            }
          }
        }
      }
    }
  });
  b4.run("ltl_offset fold prep", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        for (size_t z = 0; z < ext.extent(2); z += TExts...[2]) {
          const size_t blk = map.tile_outer_offset_fold_prep(x, y, z);
          for (size_t i = x; i < TExts...[0]; ++i) {
            for (size_t j = y; j < TExts...[1]; ++j) {
              for (size_t k = z; k < TExts...[2]; ++k) {
                tmp = blk + map.tile_inner_offset_fold_prep(i, j, k);
                nb::doNotOptimizeAway(tmp);
              }
            }
          }
        }
      }
    }
  });
  b4.run("ltl_offset fold prep strides", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        for (size_t z = 0; z < ext.extent(2); z += TExts...[2]) {
          const size_t blk = map.tile_outer_offset_fold_prep_strides(x, y, z);
          for (size_t i = x; i < TExts...[0]; ++i) {
            for (size_t j = y; j < TExts...[1]; ++j) {
              for (size_t k = z; k < TExts...[2]; ++k) {
                tmp = blk + map.tile_inner_offset_fold_prep_strides(i, j, k);
                nb::doNotOptimizeAway(tmp);
              }
            }
          }
        }
      }
    }
  });
  b4.run("offset for (Kokkos impl)", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        for (size_t z = 0; z < ext.extent(2); z += TExts...[2]) {
          for (size_t i = x; i < TExts...[0]; ++i) {
            for (size_t j = y; j < TExts...[1]; ++j) {
              for (size_t k = z; k < TExts...[2]; ++k) {
                tmp = map.offset_for(i, j, k);
                nb::doNotOptimizeAway(tmp);
              }
            }
          }
        }
      }
    }
  });
  b4.run("offset fold (LLVM impl)", [&]() {
    auto tmp = 0UL;
    for (size_t x = 0; x < ext.extent(0); x += TExts...[0]) {
      for (size_t y = 0; y < ext.extent(1); y += TExts...[1]) {
        for (size_t z = 0; z < ext.extent(2); z += TExts...[2]) {
          for (size_t i = x; i < TExts...[0]; ++i) {
            for (size_t j = y; j < TExts...[1]; ++j) {
              for (size_t k = z; k < TExts...[2]; ++k) {
                tmp = map.offset_fold(i, j, k);
                nb::doNotOptimizeAway(tmp);
              }
            }
          }
        }
      }
    }
  });
}

template <class Exts, size_t... TExts, class... I>
auto eval_4d(I... idxs) -> void
  requires (4 == Exts::rank() and 4 == sizeof...(TExts) and 4 == sizeof...(I))
{
  using layout_ltl = ltxplr::ltl<TExts...>::template mapping<Exts>;
  layout_ltl map;
  auto ext = map.extents();

  // clang-format off
  nb::Bench b0;
  b0.title("tile outer offset").relative(true);
  b0.run("tile_outer_offset for direct", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_direct(idxs...)); });
  b0.run("tile_outer_offset for direct strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_direct_strides(idxs...)); });
  b0.run("tile_outer_offset for prep", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_prep(idxs...)); });
  b0.run("tile_outer_offset for prep strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_prep_strides(idxs...)); });
  b0.run("tile_outer_offset fold direct", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_direct(idxs...)); });
  b0.run("tile_outer_offset fold direct strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_direct_strides(idxs...)); });
  b0.run("tile_outer_offset fold prep", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_prep(idxs...)); });
  b0.run("tile_outer_offset fold prep strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_prep_strides(idxs...)); });

  nb::Bench b1;
  b1.title("tile inner offset").relative(true);
  b1.run("tile_inner_offset for direct", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_for_direct(idxs...)); });
  b1.run("tile_inner_offset for direct strides", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_for_direct_strides(idxs...)); });
  b1.run("tile_inner_offset for prep", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_for_prep(idxs...)); });
  b1.run("tile_inner_offset for prep strides", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_for_prep_strides(idxs...)); });
  b1.run("tile_inner_offset fold direct", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_fold_direct(idxs...)); });
  b1.run("tile_inner_offset fold direct strides", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_fold_direct_strides(idxs...)); });
  b1.run("tile_inner_offset fold prep", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_fold_prep(idxs...)); });
  b1.run("tile_inner_offset fold prep strides", [&]() { nb::doNotOptimizeAway(map.tile_inner_offset_fold_prep_strides(idxs...)); });

  nb::Bench b2;
  b2.title("full offset").relative(true);
  b2.run("ltl_offset for direct", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_direct(idxs...) + map.tile_inner_offset_for_direct(idxs...)); });
  b2.run("ltl_offset for direct strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_direct_strides(idxs...) + map.tile_inner_offset_for_direct_strides(idxs...)); });
  b2.run("ltl_offset for prep", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_prep(idxs...) + map.tile_inner_offset_for_prep(idxs...)); });
  b2.run("ltl_offset for prep strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_for_prep_strides(idxs...) + map.tile_inner_offset_for_prep_strides(idxs...)); });
  b2.run("ltl_offset fold direct", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_direct(idxs...) + map.tile_inner_offset_fold_direct(idxs...)); });
  b2.run("ltl_offset fold direct strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_direct_strides(idxs...) + map.tile_inner_offset_fold_direct_strides(idxs...)); });
  b2.run("ltl_offset fold prep", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_prep(idxs...) + map.tile_inner_offset_fold_prep(idxs...)); });
  b2.run("ltl_offset fold prep strides", [&]() { nb::doNotOptimizeAway(map.tile_outer_offset_fold_prep_strides(idxs...) + map.tile_inner_offset_fold_prep_strides(idxs...)); });
  b2.run("offset for (Kokkos impl)", [&]() { nb::doNotOptimizeAway(map.offset_for(idxs...)); });
  b2.run("offset fold (LLVM impl)", [&]() { nb::doNotOptimizeAway(map.offset_fold(idxs...)); });
  // clang-format on
}

template <class Exts, size_t... TExts, class... I>
auto eval_5d(I... idxs) -> void
  requires (5 == Exts::rank() and 5 == sizeof...(TExts) and 5 == sizeof...(I))
{
  fmt::println("todo!");
}

template <class Exts, size_t... TExts, class... I>
auto eval_6d(I... idxs) -> void
  requires (6 == Exts::rank() and 6 == sizeof...(TExts) and 6 == sizeof...(I))
{
  fmt::println("todo!");
}

} // namespace

auto main(int argc, char* argv[]) -> int {
  if (argc < 2) {
    fmt::println("usage: {} <DIM>", argv[0]);
    return 1;
  }

  int dim = std::atoi(argv[1]);
  switch (dim) {
  case 2:
    using exts2 = std::extents<size_t, 100, 100>;
    check<exts2, 10, 10>(1, 2);
    eval_2d<exts2, 10, 10>(1, 2);
    break;
  case 3:
    using exts3 = std::extents<size_t, 100, 100, 100>;
    check<exts3, 10, 10, 10>(1, 2, 4);
    eval_3d<exts3, 10, 10, 10>(1, 2, 4);
    break;
  case 4:
    using exts4 = std::extents<size_t, 100, 100, 100, 100>;
    check<exts4, 10, 10, 10, 10>(1, 2, 4, 8);
    eval_4d<exts4, 10, 10, 10, 10>(1, 2, 4, 8);
    break;
  case 5:
    using exts5 = std::extents<size_t, 100, 100, 100, 100, 100>;
    check<exts5, 10, 10, 10, 10, 10>(1, 2, 4, 8, 16);
    eval_5d<exts5, 10, 10, 10, 10, 10>(1, 2, 4, 8, 16);
    break;
  case 6:
    using exts6 = std::extents<size_t, 100, 100, 100, 100, 100, 100>;
    check<exts6, 10, 10, 10, 10, 10, 10>(1, 2, 4, 8, 16, 32);
    eval_6d<exts6, 10, 10, 10, 10, 10, 10>(1, 2, 4, 8, 16, 32);
    break;
  default:
    fmt::println("error: unimplemented dimension");
    return 1;
  }
  return 0;
}
