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
//  uwNotebook.h
// ============================================================================
#ifndef UWNOTEBOOK_H
#define UWNOTEBOOK_H

class pkgPackage;


// ============================================================================
// uwNotebook - Events
// ============================================================================
DECLARE_EVENT_TYPE(unEVT_NOTEPAGE_ADD, -1)
DECLARE_EVENT_TYPE(unEVT_NOTEPAGE_CHANGED, -1)


// ============================================================================
// uwNotebook
// ============================================================================
class uwNotebook: public wxNotebook
{
public:
    // Special
    uwNotebook
    (	wxWindow*		parent
    ,	wxWindowID		id = wxID_ANY
    ,	const wxPoint&	pos = wxDefaultPosition
    ,	const wxSize&	size = wxDefaultSize
    ,	long			style = wxNO_BORDER
    ,	const wxString&	name = wxT("uwNotebook")
    );

    // Interface
    virtual void OnOpenFile( pkgPackage* package );
    virtual void OnCloseFile();
    virtual void OnAddPages();
    bool AddPage( wxNotebookPage* page, const wxString& text, bool bSelect=false, int imageId=-1 );

    // Events
    void OnPageChanged( wxNotebookEvent& event );

protected:
    pkgPackage*	    Package;
    bool	        bClosing;

private:
    DECLARE_EVENT_TABLE()	
};


#endif UWNOTEBOOK_H
// ============================================================================