//
// Bareflank Hypervisor
//
// Copyright (C) 2015 Assured Information Security, Inc.
// Author: Rian Quinn        <quinnr@ainfosec.com>
// Author: Brendan Kerrigan  <kerriganb@ainfosec.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#include <bitmanip.h>
#include <maskmanip.h>
#include <memory_manager/page_table_entry_x64.h>

#include <intrinsics/x64.h>
using namespace x64;

page_table_entry_x64::page_table_entry_x64(const gsl::not_null<pointer> &pte) noexcept :
    m_pte(pte.get())
{ }

bool
page_table_entry_x64::present() const noexcept
{ return is_bit_enabled(*m_pte, 0); }

void
page_table_entry_x64::set_present(bool enabled) noexcept
{ *m_pte = enabled ? set_bit(*m_pte, 0) : clear_bit(*m_pte, 0); }

bool
page_table_entry_x64::rw() const noexcept
{ return is_bit_enabled(*m_pte, 1); }

void
page_table_entry_x64::set_rw(bool enabled) noexcept
{ *m_pte = enabled ? set_bit(*m_pte, 1) : clear_bit(*m_pte, 1); }

bool
page_table_entry_x64::us() const noexcept
{ return is_bit_enabled(*m_pte, 2); }

void
page_table_entry_x64::set_us(bool enabled) noexcept
{ *m_pte = enabled ? set_bit(*m_pte, 2) : clear_bit(*m_pte, 2); }

bool
page_table_entry_x64::pwt() const noexcept
{ return is_bit_enabled(*m_pte, 3); }

void
page_table_entry_x64::set_pwt(bool enabled) noexcept
{ *m_pte = enabled ? set_bit(*m_pte, 3) : clear_bit(*m_pte, 3); }

bool
page_table_entry_x64::pcd() const noexcept
{ return is_bit_enabled(*m_pte, 4); }

void
page_table_entry_x64::set_pcd(bool enabled) noexcept
{ *m_pte = enabled ? set_bit(*m_pte, 4) : clear_bit(*m_pte, 4); }

bool
page_table_entry_x64::accessed() const noexcept
{ return is_bit_enabled(*m_pte, 5); }

void
page_table_entry_x64::set_accessed(bool enabled) noexcept
{ *m_pte = enabled ? set_bit(*m_pte, 5) : clear_bit(*m_pte, 5); }

bool
page_table_entry_x64::dirty() const noexcept
{ return is_bit_enabled(*m_pte, 6); }

void
page_table_entry_x64::set_dirty(bool enabled) noexcept
{ *m_pte = enabled ? set_bit(*m_pte, 6) : clear_bit(*m_pte, 6); }

bool
page_table_entry_x64::ps() const noexcept
{ return is_bit_enabled(*m_pte, 7); }

void
page_table_entry_x64::set_ps(bool enabled) noexcept
{ *m_pte = enabled ? set_bit(*m_pte, 7) : clear_bit(*m_pte, 7); }

bool
page_table_entry_x64::pat() const noexcept
{ return is_bit_enabled(*m_pte, 7); }

void
page_table_entry_x64::set_pat(bool enabled) noexcept
{ *m_pte = enabled ? set_bit(*m_pte, 7) : clear_bit(*m_pte, 7); }

bool
page_table_entry_x64::global() const noexcept
{ return is_bit_enabled(*m_pte, 8); }

void
page_table_entry_x64::set_global(bool enabled) noexcept
{ *m_pte = enabled ? set_bit(*m_pte, 8) : clear_bit(*m_pte, 8); }

page_table_entry_x64::integer_pointer
page_table_entry_x64::phys_addr() const noexcept
{ return get_mask(*m_pte, 0x000FFFFFFFFFF000UL); }

void
page_table_entry_x64::set_phys_addr(integer_pointer addr) noexcept
{ *m_pte = set_mask(*m_pte, 0x000FFFFFFFFFF000UL, addr); }

bool
page_table_entry_x64::nx() const noexcept
{ return is_bit_enabled(*m_pte, 63); }

void
page_table_entry_x64::set_nx(bool enabled) noexcept
{ *m_pte = enabled ? set_bit(*m_pte, 63) : clear_bit(*m_pte, 63); }
