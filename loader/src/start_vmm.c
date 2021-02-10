/**
 * @copyright
 * Copyright (C) 2020 Assured Information Security, Inc.
 *
 * @copyright
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * @copyright
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * @copyright
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define DEBUG_LOADER

#include <alloc_and_copy_ext_elf_files_from_user.h>
#include <alloc_and_copy_mk_elf_file_from_user.h>
#include <alloc_and_copy_mk_elf_segments.h>
#include <alloc_mk_huge_pool.h>
#include <alloc_mk_page_pool.h>
#include <alloc_mk_root_page_table.h>
#include <constants.h>
#include <debug.h>
#include <dump_ext_elf_files.h>
#include <dump_mk_elf_file.h>
#include <dump_mk_elf_segments.h>
#include <dump_mk_huge_pool.h>
#include <dump_mk_page_pool.h>
#include <dump_mk_root_page_table.h>
#include <free_ext_elf_files.h>
#include <free_mk_elf_file.h>
#include <free_mk_elf_segments.h>
#include <free_mk_huge_pool.h>
#include <free_mk_page_pool.h>
#include <free_mk_root_page_table.h>
#include <g_ext_elf_files.h>
#include <g_mk_code_aliases.h>
#include <g_mk_debug_ring.h>
#include <g_mk_elf_file.h>
#include <g_mk_elf_segments.h>
#include <g_mk_huge_pool.h>
#include <g_mk_page_pool.h>
#include <g_mk_root_page_table.h>
#include <map_ext_elf_files.h>
#include <map_mk_code_aliases.h>
#include <map_mk_debug_ring.h>
#include <map_mk_elf_file.h>
#include <map_mk_elf_segments.h>
#include <map_mk_huge_pool.h>
#include <map_mk_page_pool.h>
#include <platform.h>
#include <start_vmm_args_t.h>
#include <start_vmm_per_cpu.h>
#include <stop_and_free_the_vmm.h>
#include <stop_vmm_per_cpu.h>
#include <types.h>
#include <vmm_status.h>

static int64_t
alloc_and_start_the_vmm(struct start_vmm_args_t const *const args)
{
    if (VMM_STATUS_RUNNING == g_vmm_status) {
        stop_and_free_the_vmm();
    }

    if (VMM_STATUS_CORRUPT == g_vmm_status) {
        BFERROR("Unable to start, previous VMM failed to properly stop\n");
        return LOADER_FAILURE;
    }

    g_mk_debug_ring->epos = ((uint64_t)0);
    g_mk_debug_ring->spos = ((uint64_t)0);

    if (alloc_mk_root_page_table(&g_mk_root_page_table)) {
        BFERROR("alloc_and_copy_mk_root_page_table failed\n");
        goto alloc_and_copy_mk_root_page_table_failed;
    }

    if (alloc_and_copy_mk_elf_file_from_user(
            &args->mk_elf_file, &g_mk_elf_file)) {
        BFERROR("alloc_and_copy_mk_elf_file_from_user failed\n");
        goto alloc_and_copy_mk_elf_file_from_user_failed;
    }

    if (alloc_and_copy_ext_elf_files_from_user(
            args->ext_elf_files, g_ext_elf_files)) {
        BFERROR("alloc_and_copy_ext_elf_files_from_user failed\n");
        goto alloc_and_copy_ext_elf_files_from_user_failed;
    }

    if (alloc_and_copy_mk_elf_segments(&g_mk_elf_file, g_mk_elf_segments)) {
        BFERROR("alloc_and_copy_mk_elf_segments failed\n");
        goto alloc_and_copy_mk_elf_segments_failed;
    }

    if (alloc_mk_page_pool(args->page_pool_size, &g_mk_page_pool)) {
        BFERROR("alloc_mk_page_pool failed\n");
        goto alloc_mk_page_pool_failed;
    }

    if (alloc_mk_huge_pool(0U, &g_mk_huge_pool)) {
        BFERROR("alloc_mk_huge_pool failed\n");
        goto alloc_mk_huge_pool_failed;
    }

    if (map_mk_debug_ring(g_mk_debug_ring, g_mk_root_page_table)) {
        BFERROR("map_mk_debug_ring failed\n");
        goto map_mk_debug_ring_failed;
    }

    if (map_mk_code_aliases(&g_mk_code_aliases, g_mk_root_page_table)) {
        BFERROR("map_mk_code_aliases failed\n");
        goto map_mk_code_aliases_failed;
    }

    if (map_mk_elf_file(&g_mk_elf_file, g_mk_root_page_table)) {
        BFERROR("map_mk_elf_file failed\n");
        goto map_mk_elf_file_failed;
    }

    if (map_ext_elf_files(g_ext_elf_files, g_mk_root_page_table)) {
        BFERROR("map_ext_elf_files failed\n");
        goto map_ext_elf_files_failed;
    }

    if (map_mk_elf_segments(g_mk_elf_segments, g_mk_root_page_table)) {
        BFERROR("map_mk_elf_segments failed\n");
        goto map_mk_elf_segments_failed;
    }

    if (map_mk_page_pool(
            &g_mk_page_pool, g_mk_page_pool_base_virt, g_mk_root_page_table)) {
        BFERROR("map_mk_page_pool failed\n");
        goto map_mk_page_pool_failed;
    }

    if (map_mk_huge_pool(
            &g_mk_huge_pool, g_mk_huge_pool_base_virt, g_mk_root_page_table)) {
        BFERROR("map_mk_huge_pool failed\n");
        goto map_mk_huge_pool_failed;
    }

#ifdef DEBUG_LOADER
    dump_mk_root_page_table(g_mk_root_page_table);
    dump_mk_elf_file(&g_mk_elf_file);
    dump_ext_elf_files(g_ext_elf_files);
    dump_mk_elf_segments(g_mk_elf_segments);
    dump_mk_page_pool(&g_mk_page_pool);
    dump_mk_huge_pool(&g_mk_huge_pool);
#endif

    if (platform_on_each_cpu(start_vmm_per_cpu, PLATFORM_FORWARD)) {
        BFERROR("start_vmm_per_cpu failed\n");
        goto start_vmm_per_cpu_failed;
    }

    g_vmm_status = VMM_STATUS_RUNNING;

    return LOADER_SUCCESS;

start_vmm_per_cpu_failed:
    if (platform_on_each_cpu(stop_vmm_per_cpu, PLATFORM_REVERSE)) {
        BFERROR("stop_vmm_per_cpu failed\n");
    }

map_mk_huge_pool_failed:
map_mk_page_pool_failed:
map_mk_elf_segments_failed:
map_ext_elf_files_failed:
map_mk_elf_file_failed:
map_mk_code_aliases_failed:
map_mk_debug_ring_failed:

    free_mk_huge_pool(&g_mk_huge_pool);
alloc_mk_huge_pool_failed:
    free_mk_page_pool(&g_mk_page_pool);
alloc_mk_page_pool_failed:
    free_mk_elf_segments(g_mk_elf_segments);
alloc_and_copy_mk_elf_segments_failed:
    free_ext_elf_files(g_ext_elf_files);
alloc_and_copy_ext_elf_files_from_user_failed:
    free_mk_elf_file(&g_mk_elf_file);
alloc_and_copy_mk_elf_file_from_user_failed:
    free_mk_root_page_table(&g_mk_root_page_table);
alloc_and_copy_mk_root_page_table_failed:

    return LOADER_FAILURE;
}

/**
 * <!-- description -->
 *   @brief Verifies that the arguments from the IOCTL are valid.
 *
 * <!-- inputs/outputs -->
 *   @param args the arguments to verify
 *   @return 0 on success, LOADER_FAILURE on failure.
 */
static int64_t
verify_start_vmm_args(struct start_vmm_args_t const *const args)
{
    uint64_t idx;

    if (((uint64_t)1) != args->ver) {
        BFERROR("IOCTL ABI version not supported\n");
        return LOADER_FAILURE;
    }

    if (((void *)0) == args->mk_elf_file.addr) {
        BFERROR("the microkernel is required\n");
        return LOADER_FAILURE;
    }

    if (((uint64_t)0) == args->mk_elf_file.size) {
        BFERROR("mk_elf_file.size is invalid\n");
        return LOADER_FAILURE;
    }

    if (HYPERVISOR_MAX_ELF_FILE_SIZE <= args->mk_elf_file.size) {
        BFERROR("mk_elf_file.size is invalid\n");
        return LOADER_FAILURE;
    }

    if (((void *)0) == args->ext_elf_files[((uint64_t)0)].addr) {
        BFERROR("at least one extension is required\n");
        return LOADER_FAILURE;
    }

    for (idx = ((uint64_t)0); idx < HYPERVISOR_MAX_EXTENSIONS; ++idx) {
        if (((void *)0) == args->ext_elf_files[idx].addr) {
            if (((uint64_t)0) != args->ext_elf_files[idx].size) {
                BFERROR("invalid extension address/size combination\n");
                return LOADER_FAILURE;
            }
        }

        if (((uint64_t)0) == args->ext_elf_files[idx].size) {
            if (((void *)0) != args->ext_elf_files[idx].addr) {
                BFERROR("invalid extension address/size combination\n");
                return LOADER_FAILURE;
            }
        }

        if (HYPERVISOR_MAX_ELF_FILE_SIZE <= args->ext_elf_files[idx].size) {
            BFERROR("ext_elf_files[%u].size is invalid\n", (uint32_t)idx);
            return LOADER_FAILURE;
        }
    }

    return LOADER_SUCCESS;
}

/**
 * <!-- description -->
 *   @brief This function contains all of the code that is common between
 *     all archiectures and all platforms for starting the VMM. This function
 *     will call platform and architecture specific functions as needed.
 *
 * <!-- inputs/outputs -->
 *   @param ioctl_args arguments from the ioctl
 *   @return 0 on success, LOADER_FAILURE on failure.
 */
int64_t
start_vmm(struct start_vmm_args_t const *const ioctl_args)
{
    int64_t ret;
    struct start_vmm_args_t args;

    if (((void *)0) == ioctl_args) {
        BFERROR("ioctl_args was ((void *)0)\n");
        return LOADER_FAILURE;
    }

    ret = platform_copy_from_user(
        &args, ioctl_args, sizeof(struct start_vmm_args_t));
    if (ret) {
        BFERROR("platform_copy_from_user failed\n");
        return LOADER_FAILURE;
    }

    if (verify_start_vmm_args(&args)) {
        BFERROR("verify_start_vmm_args failed\n");
        return LOADER_FAILURE;
    }

    if (alloc_and_start_the_vmm(&args)) {
        BFERROR("alloc_and_start_the_vmm failed\n");
        return LOADER_FAILURE;
    }

    return LOADER_SUCCESS;
}
