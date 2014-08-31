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
//  uaFileFormat.h
// ============================================================================
#ifndef UAFILEFORMAT_H
#define UAFILEFORMAT_H

#include "uwTreeBook.h"



// ============================================================================
// uaFileFormat
// ============================================================================
class uaFileFormat : public uwTreeBook
{
public:
	uaFileFormat
	(	wxWindow*		parent
	,	wxWindowID		id = wxID_ANY
	,	const wxPoint&	pos = wxDefaultPosition
	,	const wxSize&	size = wxDefaultSize
	,	long			style = 0
	,	const wxString&	name = wxT("uaFileFormat")
    );



    // Events
    void OnNotePageChanged( wxCommandEvent& event );
    void OnNotePageAdd( wxCommandEvent& event );
    void OnTreePageChanged( wxCommandEvent& event );

private:
    DECLARE_EVENT_TABLE()	
};


#endif UAFILEFORMAT_H
// ============================================================================