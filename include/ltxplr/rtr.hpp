// include/ltxplr/rtr.hpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDED_LTXPLR_RTR
#define INCLUDED_LTXPLR_RTR

#include <array>
#include <cstddef>
#include <tuple>

namespace ltxplr {

template <size_t... TileExtents>
struct rtr {
  template <class Extents>
  struct mapping {
    using extents_type = Extents;
    using index_type   = typename extents_type::index_type;
    using rank_type    = typename extents_type::rank_type;

    [[nodiscard]] constexpr auto extents() const -> const extents_type& {
      return extents_;
    }

    static constexpr auto textents() noexcept -> std::array<index_type, sizeof...(TileExtents)> {
      return std::array<index_type, sizeof...(TileExtents)>{TileExtents...};
    }

    static constexpr auto textent(index_type i) noexcept -> index_type {
      return textents()[i];
    }

    template <index_type i>
    static constexpr auto textent() noexcept -> index_type {
      return TileExtents...[i];
    }

    static constexpr auto tile_size() noexcept -> index_type {
      return (1 * ... * TileExtents);
    }

    [[nodiscard]] constexpr auto ntiles_in_extent(index_type ext) const noexcept -> index_type {
      return extents_.extent(ext) / textent(ext);
    }

    template <index_type ext>
    [[nodiscard]] constexpr auto ntiles_in_extent() const noexcept -> index_type {
      return extents_.extent(ext) / textent<ext>();
    }

  private:
    template <size_t... I>
    [[nodiscard]] constexpr auto make_strides_outer([[maybe_unused]] std::index_sequence<I...> _) const noexcept
      -> std::array<index_type, sizeof...(I)> {
      constexpr rank_type R = extents_type::rank();
      std::array<index_type, sizeof...(I)> strides{};
      index_type stride = 1;
      ((strides[R - 1 - I] = std::exchange(stride, stride * ntiles_in_extent(R - 1 - I))), ...);
      return strides;
    }

    template <size_t... I>
    [[nodiscard]] static constexpr auto make_strides_inner([[maybe_unused]] std::index_sequence<I...> _) noexcept
      -> std::array<index_type, sizeof...(I)> {
      constexpr rank_type R = extents_type::rank();
      std::array<index_type, sizeof...(I)> strides{};
      index_type stride = 1;
      ((strides[R - 1 - I] = std::exchange(stride, stride * TileExtents...[R - 1 - I])), ...);
      return strides;
    }

  public:
    template <class... Idxs>
    constexpr auto tile_outer_offset_for_direct(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R = extents_type::rank();
      auto idx_arr          = [idxs...]<size_t... I>(std::index_sequence<I...>) {
        return std::array<index_type, R>{static_cast<index_type>(idxs)...};
      }(std::make_index_sequence<R>());

      index_type offset = 0;
      index_type stride = 1;
      for (rank_type i = R; i > 0; --i) {
        offset += idx_arr[i - 1] / textent(i - 1) * stride;
        stride *= ntiles_in_extent(i - 1);
      }
      return offset * tile_size();
    }

    template <class... Idxs>
    constexpr auto tile_outer_offset_for_direct_strides(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R             = extents_type::rank();
      std::array<index_type, R> strides = make_strides_outer(std::make_index_sequence<R>{});
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs)...};

      index_type offset = 0;
      for (rank_type i = R; i > 0; --i) {
        offset += idx_arr[i - 1] / textent(i - 1) * strides[i - 1];
      }
      return offset * tile_size();
    }

    template <class... Idxs>
    constexpr auto tile_outer_offset_for_prep(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R = extents_type::rank();
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs) / static_cast<index_type>(TileExtents)...};

      index_type offset = 0;
      index_type stride = 1;
      for (rank_type i = R; i > 0; --i) {
        offset += idx_arr[i - 1] * stride;
        stride *= ntiles_in_extent(i - 1);
      }
      return offset * tile_size();
    }

    template <class... Idxs>
    constexpr auto tile_outer_offset_for_prep_strides(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R             = extents_type::rank();
      std::array<index_type, R> strides = make_strides_outer(std::make_index_sequence<R>{});
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs) / static_cast<index_type>(TileExtents)...};

      index_type offset = 0;
      for (rank_type i = R; i > 0; --i) {
        offset += idx_arr[i - 1] * strides[i - 1];
      }
      return offset * tile_size();
    }

    template <class... Idxs>
    constexpr auto tile_outer_offset_fold_direct(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R = extents_type::rank();
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs)...};

      return [&]<rank_type... I>(std::index_sequence<I...>) {
        index_type offset = 0;
        ((offset = idx_arr[I] / TileExtents...[I] + ntiles_in_extent(I) * offset), ...);
        return offset;
      }(std::make_index_sequence<R>())
             * tile_size();
    }

    template <class... Idxs>
    constexpr auto tile_outer_offset_fold_direct_strides(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R             = extents_type::rank();
      std::array<index_type, R> strides = make_strides_outer(std::make_index_sequence<R>{});
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs)...};

      return [&]<rank_type... I>(std::index_sequence<I...>) {
        return ((idx_arr[I] / TileExtents...[I] * strides[I]) + ...);
      }(std::make_index_sequence<R>())
             * tile_size();
    }

    template <class... Idxs>
    constexpr auto tile_outer_offset_fold_prep(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R = extents_type::rank();
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs) / static_cast<index_type>(TileExtents)...};

      return [&]<rank_type... I>(std::index_sequence<I...>) {
        index_type offset = 0;
        ((offset = idx_arr[I] + ntiles_in_extent(I) * offset), ...);
        return offset;
      }(std::make_index_sequence<R>())
             * tile_size();
    }

    template <class... Idxs>
    constexpr auto tile_outer_offset_fold_prep_strides(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R             = extents_type::rank();
      std::array<index_type, R> strides = make_strides_outer(std::make_index_sequence<R>{});
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs) / static_cast<index_type>(TileExtents)...};

      return [&]<rank_type... I>(std::index_sequence<I...>) {
        return ((idx_arr[I] * strides[I]) + ...);
      }(std::make_index_sequence<R>())
             * tile_size();
    }

    // -- inner --

    template <class... Idxs>
    constexpr auto tile_inner_offset_for_direct(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R = extents_type::rank();
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs)...};

      index_type offset = 0;
      index_type stride = 1;
      for (rank_type i = R; i > 0; --i) {
        offset += idx_arr[i - 1] % textent(i - 1) * stride;
        stride *= textent(i - 1);
      }
      return offset;
    }

    template <class... Idxs>
    constexpr auto tile_inner_offset_for_direct_strides(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R             = extents_type::rank();
      std::array<index_type, R> strides = make_strides_inner(std::make_index_sequence<R>{});
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs)...};

      index_type offset = 0;
      for (rank_type i = R; i > 0; --i) {
        offset += idx_arr[i - 1] % textent(i - 1) * strides[i - 1];
      }
      return offset;
    }

    template <class... Idxs>
    constexpr auto tile_inner_offset_for_prep(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R = extents_type::rank();
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs) % static_cast<index_type>(TileExtents)...};

      index_type offset = 0;
      index_type stride = 1;
      for (rank_type i = R; i > 0; --i) {
        offset += idx_arr[i - 1] * stride;
        stride *= textent(i - 1);
      }
      return offset;
    }

    template <class... Idxs>
    constexpr auto tile_inner_offset_for_prep_strides(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R             = extents_type::rank();
      std::array<index_type, R> strides = make_strides_inner(std::make_index_sequence<R>{});
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs) % static_cast<index_type>(TileExtents)...};

      index_type offset = 0;
      for (rank_type i = R; i > 0; --i) {
        offset += idx_arr[i - 1] * strides[i - 1];
      }
      return offset;
    }

    template <class... Idxs>
    constexpr auto tile_inner_offset_fold_direct(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R = extents_type::rank();
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs)...};

      return [&]<rank_type... I>(std::index_sequence<I...>) {
        index_type offset = 0;
        ((offset = idx_arr[I] % TileExtents...[I] + TileExtents...[I] * offset), ...);
        return offset;
      }(std::make_index_sequence<R>());
    }

    template <class... Idxs>
    constexpr auto tile_inner_offset_fold_direct_strides(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R             = extents_type::rank();
      std::array<index_type, R> strides = make_strides_inner(std::make_index_sequence<R>{});
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs)...};

      return [&]<rank_type... I>(std::index_sequence<I...>) {
        return ((idx_arr[I] % TileExtents...[I] * strides[I]) + ...);
      }(std::make_index_sequence<R>());
    }

    template <class... Idxs>
    constexpr auto tile_inner_offset_fold_prep(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R = extents_type::rank();
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs) % static_cast<index_type>(TileExtents)...};

      return [&]<rank_type... I>(std::index_sequence<I...>) {
        index_type offset = 0;
        ((offset = idx_arr[I] + TileExtents...[I] * offset), ...);
        return offset;
      }(std::make_index_sequence<R>());
    }

    template <class... Idxs>
    constexpr auto tile_inner_offset_fold_prep_strides(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R                       = extents_type::rank();
      constexpr std::array<index_type, R> strides = make_strides_inner(std::make_index_sequence<R>{});
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs) % static_cast<index_type>(TileExtents)...};

      return [&]<rank_type... I>(std::index_sequence<I...>) {
        return ((idx_arr[I] * strides[I]) + ...);
      }(std::make_index_sequence<R>());
    }

    template <class... Idxs>
    constexpr auto tiled_offset_fold_direct(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R = extents_type::rank();
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs)...};

      const auto [outer, inner] = [&]<rank_type... I>(std::index_sequence<I...>) {
        index_type outer = 0;
        index_type inner = 0;
        ((outer = idx_arr[I] / TileExtents...[I] + ntiles_in_extent<I>() * outer), ...);
        ((inner = idx_arr[I] % TileExtents...[I] + TileExtents...[I] * inner), ...);
        return std::tuple<index_type, index_type>{outer, inner};
      }(std::make_index_sequence<R>());
      return outer * tile_size() + inner;
    }

    template <class... Idxs>
      requires (
        (sizeof...(Idxs) == extents_type::rank()) and (std::is_convertible_v<Idxs, index_type> and ...)
        and (std::is_nothrow_constructible_v<index_type, Idxs> and ...)
      )
    constexpr auto offset_fold(Idxs... idx) const noexcept -> index_type {
      constexpr rank_type R = extents_type::rank();
      std::array<index_type, R> idx_arr{static_cast<index_type>(idx)...};

      return [&]<rank_type... Pos>(std::index_sequence<Pos...>) {
        index_type offset = 0;
        ((offset = idx_arr[Pos] + extents_.extent(Pos) * offset), ...);
        return offset;
      }(std::make_index_sequence<R>());
    }

    template <class... Idxs>
    constexpr auto offset_for(Idxs... idxs) const noexcept -> index_type {
      constexpr rank_type R = extents_type::rank();
      std::array<index_type, R> idx_arr{static_cast<index_type>(idxs)...};

      index_type offset = 0;
      index_type stride = 1;
      for (rank_type i = R; i > 0; --i) {
        offset += idx_arr[i - 1] * stride;
        stride *= extents_.extent(i - 1);
      }
      return offset;
    }

  private:
    extents_type extents_;
  };
};

} // namespace ltxplr

#endif // INCLUDED_LTXPLR_RTR
