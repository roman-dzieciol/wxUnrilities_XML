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
//  uwHexList.h
// ============================================================================
#ifndef UWHEXLIST_H
#define UWHEXLIST_H

#include "uwList.h"

class uwHexListCacheRow
{
public:
};


// ============================================================================
// uwHexList
// ============================================================================
class uwHexList : public uwList
{
public:
    // Special
	uwHexList
	(	wxWindow*			parent
	,	wxWindowID			id = wxID_ANY
	,	const wxPoint&		pos = wxDefaultPosition
	,	const wxSize&		size = wxDefaultSize
    ,	long				style = wxLC_REPORT | wxLC_VIRTUAL 
	,	const wxValidator&	validator = wxDefaultValidator
	,	const wxString&		name = wxT("uwHexList")
	);

    virtual ~uwHexList();

    // Mutators
    void SetItemCount( dword count );
    void SetData( wxFFile* file, wxFileOffset offset, dword size );
    void SetHexWidth( dword w );
    void SetFontSize( int delta );

    // Virtual mode
    virtual wxString OnGetItemText( long item, long column ) const;
    virtual wxListItemAttr* OnGetItemAttr( long item ) const;

    // Events
    void IgnoreEvent( wxListEvent& event ) { event.Veto(); event.StopPropagation(); }
    void IgnoreMouseEvent( wxMouseEvent& event ) {  }

protected:
    dword HexWidth;
    wxFFile* File;
    wxFileOffset DataOffset;
    dword DataSize;
    vector<byte> DataCache;

private:
    DECLARE_EVENT_TABLE()
};


#endif UWHEXLIST_H
// ============================================================================