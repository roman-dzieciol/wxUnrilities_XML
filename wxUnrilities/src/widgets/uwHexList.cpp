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
//	uwHexList.cpp
// ============================================================================
#include "prec.h"
#include "uwHexList.h"
#include "asvNamedSize.h"
#include "asvNamedFormat.h"


// ============================================================================
// uwHexList - Event Table
// ============================================================================
BEGIN_EVENT_TABLE(uwHexList, uwList)
    // TODO :: Disable resizing
    //EVT_LIST_COL_CLICK            ( wxID_ANY, uwHexList::IgnoreEvent )
    //EVT_LIST_COL_RIGHT_CLICK      ( wxID_ANY, uwHexList::IgnoreEvent )
    //EVT_LIST_COL_BEGIN_DRAG       ( wxID_ANY, uwHexList::IgnoreEvent )
    EVT_LEFT_DOWN                     ( uwHexList::IgnoreMouseEvent )
END_EVENT_TABLE()


// ============================================================================
// uwHexList
// ============================================================================
uwHexList::uwHexList
(	wxWindow*			parent
,	wxWindowID			id
,	const wxPoint&		pos
,	const wxSize&		size
,	long				style
,	const wxValidator&	validator
,	const wxString&		name
) 
:	uwList(parent,id,pos,size,style,validator,name)
,   HexWidth(16)
{
}

uwHexList::~uwHexList()
{
}

// ============================================================================
// uwHexList
// ============================================================================
void uwHexList::SetHexWidth( dword w )
{
    HexWidth = w;

    AddColumn( wxT("Abs Offset"), wxLIST_FORMAT_LEFT, GNamedSize(wxT("Hex")) );
    AddColumn( wxT("Rel Offset"), wxLIST_FORMAT_LEFT, GNamedSize(wxT("Hex")) );

    for( dword i=0; i!=HexWidth; ++i )
    {
        AddColumn( wxString::Format( wxT("%X"), i%0x10 ), wxLIST_FORMAT_CENTRE, GNamedSize(wxT("HexByte")) );
    }

    AddColumn( wxT(" "), wxLIST_FORMAT_CENTRE, GNamedSize(wxT("HexByte")) );

    for( dword i=0; i!=HexWidth; ++i )
    {
        AddColumn( wxT(" "), wxLIST_FORMAT_CENTRE, GNamedSize(wxT("HexByte")) );
    }
}

void uwHexList::SetFontSize( int delta )
{
    static const wxFont base = GetFont();
    wxFont newfont = base;
    newfont.SetPointSize( base.GetPointSize() + delta );
    SetFont(newfont);
}


// ============================================================================
// uwHexList - Mutators
// ============================================================================
void uwHexList::SetData( wxFFile* file, wxFileOffset offset, dword size )
{
    File = file;
    DataOffset = offset;
    DataSize = size;

    //DataCache.clear();
    DataCache.reserve(DataSize);
    DataCache.resize(DataSize);

    dword count = static_cast<dword>(ceil(static_cast<double>(DataSize)/static_cast<double>(HexWidth)));

    File->Seek(DataOffset);
    File->Read(&DataCache.at(0),DataSize);

    SetItemCount(count);
}

void uwHexList::SetItemCount( dword count )
{
    wxListView::SetItemCount(count);
}


// ============================================================================
// uwHexList - Virtual mode
// ============================================================================
wxString uwHexList::OnGetItemText( long item, long column ) const
{
    if( column == 0 )
    {
        dword pos = (item * HexWidth) + DataOffset;
        return wxString::Format( GNamedFormat(wxT("Offset")), pos );
    }
    else if ( column == 1 )
    {
        dword pos = (item * HexWidth);
        return wxString::Format( GNamedFormat(wxT("Offset")), pos );
    }
    else if( column < HexWidth + 2 )
    {
        dword pos = (item * HexWidth) + (column - 2);
        if( pos < DataCache.size() )
            return wxString::Format( GNamedFormat(wxT("HexByte")), DataCache.at(pos) );
        else
            return wxEmptyString;
    }
    else if( column > HexWidth + 2 )
    {
        dword col = column - 3 - HexWidth;
        dword pos = (item * HexWidth) + col;
        if( pos < DataCache.size() )
            return wxString::Format( GNamedFormat(wxT("HexChar")), DataCache.at(pos) );
        else
            return wxEmptyString;
    }
    return wxEmptyString;
}

wxListItemAttr* uwHexList::OnGetItemAttr( long item ) const
{
    static wxListItemAttr attr;
    wxLogMessage("%d",item);

    static int i=0;
    i+=40;

    byte c = i % 255;
    wxColour colour;
    colour.Set(c,c,c);
    attr.SetTextColour(colour);

    return &attr;
}



// ============================================================================