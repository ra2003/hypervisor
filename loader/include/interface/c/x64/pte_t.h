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

#ifndef PTE_T_H
#define PTE_T_H

#pragma pack(push, 1)

#include <static_assert.h>
#include <types.h>

/**
 * @struct pte_t
 *
 * <!-- description -->
 *   @brief Defines the layout of a page table entry (PTE).
 */
struct pte_t
{
    /** @brief defines the "present" field in the page */
    uint64_t p : ((uint64_t)1);
    /** @brief defines the "read/write" field in the page */
    uint64_t rw : ((uint64_t)1);
    /** @brief defines the "user/supervisor" field in the page */
    uint64_t us : ((uint64_t)1);
    /** @brief defines the "page-level writethrough" field in the page */
    uint64_t pwt : ((uint64_t)1);
    /** @brief defines the "page-level cache disable" field in the page */
    uint64_t pcd : ((uint64_t)1);
    /** @brief defines the "accessed" field in the page */
    uint64_t a : ((uint64_t)1);
    /** @brief defines the "dirty" field in the page */
    uint64_t d : ((uint64_t)1);
    /** @brief defines the "page-attribute table" field in the page */
    uint64_t pat : ((uint64_t)1);
    /** @brief defines the "global page" field in the page */
    uint64_t g : ((uint64_t)1);
    /** @brief defines the "available to software" field in the page */
    uint64_t avl : ((uint64_t)3);
    /** @brief defines the physical address field in the page */
    uint64_t phys : ((uint64_t)40);
    /** @brief defines whether or not the page can be auto released */
    uint64_t auto_release : ((uint64_t)7);
    /** @brief defines the "memory protection key" field in the page */
    uint64_t mpk : ((uint64_t)4);
    /** @brief defines the "no-execute" field in the page */
    uint64_t nx : ((uint64_t)1);
};

/** @brief Check to make sure the pte_t is the right size. */
STATIC_ASSERT(sizeof(struct pte_t) == 8, invalid_size);

#pragma pack(pop)

#endif