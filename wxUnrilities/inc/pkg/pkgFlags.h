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
//  pkgFlags.h
// ============================================================================
#ifndef PKGFLAGS_H
#define PKGFLAGS_H


// ============================================================================
// pkgFlagData
// ============================================================================
class pkgFlagData
{
public:
    STATIC_ALLOCATOR(pkgFlagData);

    dword GetData() const { return Data; }
    wxString GetName() const { return Name; }
    wxString GetDesc() const { return Desc; }

    void SetData( dword d ) { Data = d; }
    void SetName( const wxString& d ) { Name = d; }
    void SetDesc( const wxString& d ) { Desc = d; }

protected:
    dword Data;
    wxString Name;
    wxString Desc;
};


// ============================================================================
// pkgFlagData - maps
// ============================================================================
typedef multimap<dword,pkgFlagData*>      TFlagsByData;
WX_DECLARE_STRING_HASH_MAP( pkgFlagData*, TFlagsByName );
typedef vector<pkgFlagData*>              TFlagsList;


// ============================================================================
// pkgFlagGroup
// ============================================================================
class pkgFlagGroup
{
public:
    STATIC_ALLOCATOR(pkgFlagGroup);

    pkgFlagGroup();
    ~pkgFlagGroup();

    void DestroyFlags();

    dword GetBits() const { return Bits; }
    wxString GetName() const { return Name; }
    wxString GetDesc() const { return Desc; }

    void SetBits( dword d ) { Bits = d; }
    void SetName( const wxString& d ) { Name = d; }
    void SetDesc( const wxString& d ) { Desc = d; }

    pkgFlagData* FindFlag( const wxString& flag );
    pkgFlagData* FindFlag( dword flag );

    void AddFlagData( pkgFlagData* flag );
    void AddUnknownFlags( dword maxbits );

    wxString PrintFlagNames( dword flags );

    TFlagsList& GetFlagsList() { return Flags; }
    TFlagsByData& GetFlagsByData() { return ByData; }
    TFlagsByName& GetFlagsByName() { return ByName; }

protected:
    dword Bits;
    wxString Name;
    wxString Desc;
    TFlagsList Flags;
    TFlagsByData ByData;
    TFlagsByName ByName;
};


// ============================================================================
// pkgFlagGroup - maps
// ============================================================================
WX_DECLARE_STRING_HASH_MAP( pkgFlagGroup*, TFlagGroupsByName );
typedef vector<pkgFlagGroup*>              TFlagGroupsList;


// ============================================================================
// pkgFlags
// ============================================================================
class pkgFlags
{
public:
    STATIC_ALLOCATOR(pkgFlags);

    pkgFlags();
    ~pkgFlags();

    void DestroyFlagGroups();
    void AddFlagGroup( pkgFlagGroup* group );

    pkgFlagGroup* FindGroup( const wxString& group );
    pkgFlagData* FindFlag( const wxString& group, const wxString& flag );
    pkgFlagData* FindFlag( const wxString& group, dword flag );

    TFlagGroupsList& GetFlagGroupsList() { return Groups; }
    TFlagGroupsByName& GetFlagGroupsByName() { return ByName; }

protected:
    TFlagGroupsList Groups;
    TFlagGroupsByName ByName;
};


#endif PKGFLAGS_H
// ============================================================================