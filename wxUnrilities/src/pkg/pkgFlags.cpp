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
//	pkgFlags.cpp
// ============================================================================
#include "prec.h"
#include "pkgFlags.h"


// ============================================================================
// pkgFlags
// ============================================================================
pkgFlags::pkgFlags()
{
}

pkgFlags::~pkgFlags()
{
    DestroyFlagGroups();
}

void pkgFlags::DestroyFlagGroups()
{
    for( TFlagGroupsList::iterator it=Groups.begin(); it!=Groups.end(); ++it )
    {
        alloc_destroy(static_cast<pkgFlagGroup*>(*it));
    }
    Groups.clear();
    ByName.clear();
}

void pkgFlags::AddFlagGroup( pkgFlagGroup* group )
{
    Groups.push_back(group);
    ByName[group->GetName()] = group;
}

pkgFlagData* pkgFlags::FindFlag( const wxString& group, const wxString& flag )
{
    return pkgFlags::FindGroup(group)->FindFlag(flag);
}

pkgFlagData* pkgFlags::FindFlag( const wxString& group, dword flag )
{
    return FindGroup(group)->FindFlag(flag);
}

pkgFlagGroup* pkgFlags::FindGroup( const wxString& group )
{
    TFlagGroupsByName::iterator it = ByName.find(group);
    if( it == ByName.end() )
        throw exception( wxString::Format( wxT("Flag group not found: %s"), group.c_str() ) );

    return static_cast<pkgFlagGroup*>((*it).second);
}


// ============================================================================
// pkgFlagGroup
// ============================================================================
pkgFlagGroup::pkgFlagGroup()
{
}

pkgFlagGroup::~pkgFlagGroup()
{
    DestroyFlags();
}

void pkgFlagGroup::DestroyFlags()
{
    for( TFlagsList::iterator it=Flags.begin(); it!=Flags.end(); ++it )
    {
        alloc_destroy(static_cast<pkgFlagData*>(*it));
    }
    ByData.clear();
    ByName.clear();
    Flags.clear();
}

pkgFlagData* pkgFlagGroup::FindFlag( const wxString& flag )
{
    TFlagsByName::iterator it = ByName.find(flag);
    if( it == ByName.end() )
        throw exception( wxString::Format( wxT("Flag not found: %s"), flag.c_str() ) );

    return static_cast<pkgFlagData*>((*it).second);
}

pkgFlagData* pkgFlagGroup::FindFlag( dword flag )
{
    TFlagsByData::iterator it = ByData.find(flag);
    if( it == ByData.end() )
        throw exception( wxString::Format( wxT("Flag not found: %d"), flag ) );

    return static_cast<pkgFlagData*>((*it).second);
}

void pkgFlagGroup::AddUnknownFlags( dword maxbits )
{
    for( dword i=0; i<maxbits; ++i )
    {
        dword bit = 1 << i;
        TFlagsByData::iterator it = ByData.find(bit);
        if( it == ByData.end() )
        {
            pkgFlagData* fdata = alloc_create<pkgFlagData>();
            fdata->SetData(bit);
            fdata->SetName(wxString::Format( wxT("0x%.8X"), bit ));
            fdata->SetDesc(wxString::Format( wxT("?") ));
            AddFlagData(fdata);
        }
    }
}

void pkgFlagGroup::AddFlagData( pkgFlagData* flag )
{
    Flags.push_back( flag );
    ByData.insert( make_pair(flag->GetData(),flag) );
    ByName[flag->GetName()] = flag;
}

wxString pkgFlagGroup::PrintFlagNames( dword flags )
{
    wxString text;

    if( flags > 0 )
    {
        dword lastbit = 0;
        for( TFlagsByData::iterator it=ByData.begin(); it!=ByData.end(); ++it )
        {
            dword bit = static_cast<dword>((*it).first);
            if((flags & bit) == bit )
            {
                if( lastbit != 0 )
                    text << ((bit == lastbit) ? wxT("/") : wxT(", "));
                text << static_cast<pkgFlagData*>((*it).second)->GetName();
                lastbit = bit;
            }
        }
    }

    return text;
}


// ============================================================================