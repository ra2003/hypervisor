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

#include <debug.h>
#include <global_descriptor_table_register_t.h>
#include <types.h>

/** @brief defines the first set of bits associated with the base field */
#define BASE_MASK1 ((uint64_t)0x00000000FFFF0000)
/** @brief defines the second set of bits associated with the base field */
#define BASE_MASK2 ((uint64_t)0x000000FF00000000)
/** @brief defines the third set of bits associated with the base field */
#define BASE_MASK3 ((uint64_t)0xFF00000000000000)
/** @brief defines the fourth set of bits associated with the base field */
#define BASE_MASK4 ((uint64_t)0x00000000FFFFFFFF)
/** @brief defines the bit location of the first set of base field */
#define BASE_SHIFT1 ((uint64_t)16)
/** @brief defines the bit location of the second set of base field */
#define BASE_SHIFT2 ((uint64_t)16)
/** @brief defines the bit location of the third set of base field */
#define BASE_SHIFT3 ((uint64_t)32)
/** @brief defines the bit location of the fourth set of base field */
#define BASE_SHIFT4 ((uint64_t)32)

/** @brief defines the bit location of the S bit in the attrib field */
#define SYSTEM_BIT ((uint64_t)0x0000100000000000)

/**
 * <!-- description -->
 *   @brief Sets a GDT descriptor's base given a GDT and a selector into
 *     the provided GDT.
 *
 * <!-- inputs/outputs -->
 *   @param gdtr a pointer to the gdtr that stores the GDT to get from
 *   @param selector the selector of the descriptor in the provided GDT
 *     to get from
 *   @param base a pointer to store the the resulting base to
 *   @return 0 on success, LOADER_FAILURE on failure.
 */
int64_t
get_gdt_descriptor_base(
    struct global_descriptor_table_register_t const *const gdtr,
    uint16_t const selector,
    uint64_t *const base)
{
    uint64_t bytes64_0;
    uint64_t bytes64_1;
    uint64_t idx64_0 = (((uint64_t)selector) >> ((uint64_t)3)) + ((uint64_t)0);
    uint64_t idx64_1 = (((uint64_t)selector) >> ((uint64_t)3)) + ((uint64_t)1);

    if (((void *)0) == gdtr) {
        BFERROR("invalid argument: gdtr == ((void *)0)\n");
        return LOADER_FAILURE;
    }

    bytes64_0 = ((uint64_t)gdtr->limit) + ((uint64_t)1);
    bytes64_1 = ((uint64_t)gdtr->limit) + ((uint64_t)1) - sizeof(uint64_t);

    if (((void *)0) == base) {
        BFERROR("invalid argument: base == ((void *)0)\n");
        return LOADER_FAILURE;
    }

    if (((uint64_t)0) == idx64_0) {
        *base = ((uint64_t)0);
        return LOADER_SUCCESS;
    }

    if (idx64_0 >= (bytes64_0 / sizeof(uint64_t))) {
        BFERROR("invalid argument: index into GDT is out of range\n");
        return LOADER_FAILURE;
    }

    if ((gdtr->base[idx64_0] & SYSTEM_BIT) == 0) {
        if (idx64_1 >= (bytes64_1 / sizeof(uint64_t))) {
            BFERROR("invalid argument: index into GDT is out of range\n");
            return LOADER_FAILURE;
        }

        *base = ((gdtr->base[idx64_0] & BASE_MASK1) >> BASE_SHIFT1) |
                ((gdtr->base[idx64_0] & BASE_MASK2) >> BASE_SHIFT2) |
                ((gdtr->base[idx64_0] & BASE_MASK3) >> BASE_SHIFT3) |
                ((gdtr->base[idx64_1] & BASE_MASK4) << BASE_SHIFT4);
    }
    else {
        *base = ((gdtr->base[idx64_0] & BASE_MASK1) >> BASE_SHIFT1) |
                ((gdtr->base[idx64_0] & BASE_MASK2) >> BASE_SHIFT2) |
                ((gdtr->base[idx64_0] & BASE_MASK3) >> BASE_SHIFT3);
    }

    return LOADER_SUCCESS;
}
