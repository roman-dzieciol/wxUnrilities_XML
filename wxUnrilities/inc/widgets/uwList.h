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
//  uwList.h
// ============================================================================
#ifndef UWLIST_H
#define UWLIST_H

#include "unTextPrinter.h"

class uwContextMenu;


// ============================================================================
// uwList
// ============================================================================
class uwList : public wxListView
{
public:
    // Special
	uwList
	(	wxWindow*			parent
	,	wxWindowID			id = wxID_ANY
	,	const wxPoint&		pos = wxDefaultPosition
	,	const wxSize&		size = wxDefaultSize
    ,	long				style = wxLC_REPORT | wxLC_VIRTUAL 
	,	const wxValidator&	validator = wxDefaultValidator
	,	const wxString&		name = wxT("uwList")
	);

    virtual ~uwList();

    // Accesors
    unTextPrinterGroup* GetPrinterGroup() { return Printers; }

    // Mutators
    void SetContextMenu( uwContextMenu* menu ) { ContextMenu = menu; }
    void SetPrinterGroup( unTextPrinterGroup* p );
    void SetItemCount( dword count );
    
    // Features
    long AddColumn( const wxString& heading, int format = wxLIST_FORMAT_LEFT, int width = -1 );
    dword AlignToListFormat( dword align );

    // Virtual mode
    virtual wxString OnGetItemText( long item, long column ) const;

    // Events
    void OnColClick( wxListEvent& event );
    void OnItemRightClick( wxListEvent& event );

protected:
    unTextPrinterGroup* Printers;
    uwContextMenu* ContextMenu;

private:
    DECLARE_EVENT_TABLE()
};


#endif UWLIST_H
// ============================================================================