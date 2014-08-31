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
//	uwList.cpp
// ============================================================================
#include "prec.h"
#include "uwList.h"
#include "uwContextMenu.h"


// ============================================================================
// uwTreeList - Event Table
// ============================================================================
BEGIN_EVENT_TABLE(uwList, wxListView)
    EVT_LIST_COL_CLICK	        ( wxID_ANY, uwList::OnColClick )
    EVT_LIST_ITEM_RIGHT_CLICK   ( wxID_ANY, uwList::OnItemRightClick ) 
END_EVENT_TABLE()


// ============================================================================
// uwList
// ============================================================================
uwList::uwList
(	wxWindow*			parent
,	wxWindowID			id
,	const wxPoint&		pos
,	const wxSize&		size
,	long				style
,	const wxValidator&	validator
,	const wxString&		name
) 
:	wxListView(parent,id,pos,size,style,validator,name)
,   Printers(NULL)
,   ContextMenu(NULL)
{
}

uwList::~uwList()
{
    delete ContextMenu;
}


// ============================================================================
// uwList - Features
// ============================================================================
long uwList::AddColumn( const wxString& heading, int format, int width )
{
    return InsertColumn( GetColumnCount(), heading, format, width );
}

dword uwList::AlignToListFormat( dword align )
{
    switch(align)
    {
    case wxALIGN_LEFT:      return wxLIST_FORMAT_LEFT;
    case wxALIGN_RIGHT:     return wxLIST_FORMAT_RIGHT;
    case wxALIGN_CENTER:    return wxLIST_FORMAT_CENTER;
    default:                return wxLIST_FORMAT_LEFT;
    }
}


// ============================================================================
// uwList - Virtual mode
// ============================================================================
wxString uwList::OnGetItemText( long item, long column ) const
{
    return Printers->PrintText(item,column);
}

void uwList::SetPrinterGroup( unTextPrinterGroup* p  )
{
    Printers = p;

    if( GetColumnCount() == 0 )
    {
        vector<unTextColumn*>& columns = Printers->GetColumns();
        for( vector<unTextColumn*>::iterator it=columns.begin(); it!=columns.end(); ++it )
        { 
            unTextColumn* column = (*it);
            AddColumn( column->GetName(), AlignToListFormat(column->GetAlign()), column->GetWidth() );
        }
    }
}

void uwList::SetItemCount( dword count )
{
    Printers->SetItemCount(count);
    Printers->EnableCache();
    wxListView::SetItemCount(count);
}


// ============================================================================
// uwList - Events
// ============================================================================
void uwList::OnColClick( wxListEvent& event )
{
    //wxLogMessage("OnColClick");
    if( Printers != NULL )
    {
        Printers->Sort( event.GetColumn() );
        Refresh();
    }
    event.Skip();
}

void uwList::OnItemRightClick( wxListEvent& event )
{
    //wxLogMessage("OnItemRightClick");
    if( ContextMenu != NULL )
    {
        ContextMenu->PopupMenu(this,event);
    }

    //event.Skip();
}


// ============================================================================