// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2024, Advanced Micro Devices, Inc. All rights reserved.

// auto generated by generate.py
#include "fmha_bwd.hpp"

using fmha_dtype_0 = ck_tile::bf16_t;

using fmha_block_tile_0 = ck_tile::sequence<128, 128, 32, 32, 32, 32, 32, 32, 32>;
using fmha_block_warps0_0 = ck_tile::sequence<1, 4, 1>;
using fmha_block_warps1_0 = ck_tile::sequence<4, 1, 1>;
using fmha_block_warps2_0 = ck_tile::sequence<4, 1, 1>;
using fmha_warp_tile_0 = ck_tile::sequence<32, 32, 16>;

// TODO: simplify Gemm0~4BlockWarps in TileFmhaBwdShape
//       G0&G2 -> GSdP
//       G1&G3 -> GdKV
//       G4    -> GdQ
using fmha_bwd_shape_0 = ck_tile::TileFmhaBwdShape<fmha_block_tile_0,
                                      fmha_block_warps0_0,
                                      fmha_warp_tile_0,
                                      fmha_block_warps1_0,
                                      fmha_warp_tile_0,
                                      fmha_block_warps0_0,
                                      fmha_warp_tile_0,
                                      fmha_block_warps1_0,
                                      fmha_warp_tile_0,
                                      fmha_block_warps2_0,
                                      fmha_warp_tile_0>;

using fmha_bwd_trait_0 = ck_tile::TileFmhaTraits<true,
                                                    true,
                                                    false,
                                                    false,
                                                    ck_tile::BlockAttentionBiasEnum::NO_BIAS,
                                                    false,
                                                    false,
                                                    false,
                                                    false,
                                                    1>;
using fmha_mask_0 = ck_tile::SimplifiedGenericAttentionMask<false>;

using fmha_bwd_pipeline_problem_0 = ck_tile::BlockFmhaBwdPipelineProblem<
    typename FmhaBwdTypeConfig<fmha_dtype_0>::QDataType,
    typename FmhaBwdTypeConfig<fmha_dtype_0>::KDataType,
    typename FmhaBwdTypeConfig<fmha_dtype_0>::VDataType,
    typename FmhaBwdTypeConfig<fmha_dtype_0>::GemmDataType,
    typename FmhaBwdTypeConfig<fmha_dtype_0>::LSEDataType,
    typename FmhaBwdTypeConfig<fmha_dtype_0>::AccDataType,
    typename FmhaBwdTypeConfig<fmha_dtype_0>::DDataType,
    typename FmhaBwdTypeConfig<fmha_dtype_0>::BiasDataType,
    typename FmhaBwdTypeConfig<fmha_dtype_0>::RandValOutputDataType,
    typename FmhaBwdTypeConfig<fmha_dtype_0>::ODataType,
    typename FmhaBwdTypeConfig<fmha_dtype_0>::OGradDataType,
    typename FmhaBwdTypeConfig<fmha_dtype_0>::QGradDataType,
    typename FmhaBwdTypeConfig<fmha_dtype_0>::KGradDataType,
    typename FmhaBwdTypeConfig<fmha_dtype_0>::VGradDataType,
    typename FmhaBwdTypeConfig<fmha_dtype_0>::BiasGradDataType,
    fmha_bwd_shape_0,
    true,
    fmha_mask_0,
    fmha_bwd_trait_0>;

using fmha_bwd_pipeline_0 = ck_tile::BlockFmhaBwdDQDKDVPipelineQSKSVROGradS<
    fmha_bwd_pipeline_problem_0>;

using fmha_bwd_dk_epilogue_0 =
    ck_tile::Default2DEpilogue<ck_tile::Default2DEpilogueProblem<typename FmhaBwdTypeConfig<ck_tile::bf16_t>::AccDataType,
                               typename FmhaBwdTypeConfig<ck_tile::bf16_t>::KGradDataType,
                               false, false>>;

using fmha_bwd_dv_epilogue_0 =
    ck_tile::Default2DEpilogue<ck_tile::Default2DEpilogueProblem<typename FmhaBwdTypeConfig<ck_tile::bf16_t>::AccDataType,
                               typename FmhaBwdTypeConfig<ck_tile::bf16_t>::VGradDataType,
                               false, false>>;

using fmha_bwd_dq_dk_dv_kernel_0 =
    ck_tile::FmhaBwdDQDKDVKernel<ck_tile::FmhaBwdTilePartitioner<fmha_bwd_shape_0>,
                        fmha_bwd_pipeline_0,
                        fmha_bwd_dk_epilogue_0,
                        fmha_bwd_dv_epilogue_0>;

using dq_dk_dv_trait_0 = fmha_bwd_dq_dk_dv_traits_<32, ck_tile::bf16_t, true, ck_tile::BlockFmhaBwdPipelineEnum::QSKSVROGradS, fmha_mask_0, ck_tile::BlockAttentionBiasEnum::NO_BIAS, false, false, true, true, false, false>;

#include <iostream>

template<>
float fmha_bwd_dq_dk_dv_<dq_dk_dv_trait_0>(const ck_tile::stream_config& s, fmha_bwd_args a)
{
    using k_ = fmha_bwd_dq_dk_dv_kernel_0;
    if(s.log_level_ > 0)
        std::cout << ", " << k_::GetName() << std::flush;
    auto [kargs, grids] = fmha_bwd_dq_dk_dv_create_kargs_and_grids<k_>(a);
    constexpr dim3 blocks             = k_::BlockSize();
    constexpr ck_tile::index_t kBlockPerCu = k_::kBlockPerCu;
    return ck_tile::launch_kernel(s, ck_tile::make_kernel<blocks.x, kBlockPerCu>(k_{}, grids, blocks, 0, kargs));
}

template<>
void fmha_bwd_dq_dk_dv_oneshot_<dq_dk_dv_trait_0>(const ck_tile::stream_config& s, fmha_bwd_args a)
{
    using k_ = fmha_bwd_dq_dk_dv_kernel_0;
    auto [kargs, grids] = fmha_bwd_dq_dk_dv_create_kargs_and_grids<k_>(a);
    constexpr dim3 blocks             = k_::BlockSize();
    constexpr ck_tile::index_t kBlockPerCu = k_::kBlockPerCu;
    ck_tile::make_kernel<blocks.x, kBlockPerCu>(k_{}, grids, blocks, 0, kargs)(ck_tile::stream_config{s.stream_id_});
}

template<>
std::string fmha_bwd_dq_dk_dv_get_name_<dq_dk_dv_trait_0>()
{
    using k_ = fmha_bwd_dq_dk_dv_kernel_0;
    return k_::GetName();
}
