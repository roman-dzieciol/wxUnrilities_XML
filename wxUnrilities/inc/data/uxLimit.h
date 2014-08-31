// ============================================================================
//  Unrilities :: Cross-platform toolkit for Unreal Engine packages
// ============================================================================
//  Copyright (C) 2005  Roman Dzieciol <neai at o2.pl>
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// ============================================================================
//  uxLimit.h
// ============================================================================
#ifndef UXLIMIT_H
#define UXLIMIT_H


// ============================================================================
// uxLimit
// ============================================================================
class uxLimit
{
public:
    // Special
    uxLimit( dword mincount=0, dword maxcount=-1, const wxString& name=wxEmptyString ) 
    :   ParseCount(0)
    ,   MinCount(mincount)
    ,   MaxCount(maxcount)
    ,   Name(name)
    {
    }

    // Interface
    void Reset() { ParseCount = 0; }
    void Increase() { ++ParseCount; }
    bool IsValid() const { return ParseCount >= MinCount && ParseCount <= MaxCount; }
    bool IsNextValid() const { return ParseCount < MaxCount; }
    dword GetCount() const { return ParseCount; }
    dword GetMinCount() const { return MinCount; }
    dword GetMaxCount() const { return MaxCount; }
    wxString GetName() const { return Name; }

protected:
    dword ParseCount;
    dword MinCount;
    dword MaxCount;
    wxString Name;
};


// ============================================================================
// uxLimits
// ============================================================================
namespace uxLimits
{
    static uxLimit one      ( 1,  1, wxT("1") );       // 1
    static uxLimit minone   ( 1, -1, wxT("1+") );      // 1-inf
    static uxLimit maxone   ( 0,  1, wxT("0~1") );     // 0-1
    static uxLimit any      ( 0, -1, wxT("0+") );      // 0-inf
}


#endif UXLIMIT_H
// ============================================================================