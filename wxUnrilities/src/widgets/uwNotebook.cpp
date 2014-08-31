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
//	uwNotebook.cpp
// ============================================================================
#include "prec.h"
#include "uwNotebook.h"


DEFINE_EVENT_TYPE(unEVT_NOTEPAGE_ADD)
DEFINE_EVENT_TYPE(unEVT_NOTEPAGE_CHANGED)


BEGIN_EVENT_TABLE(uwNotebook, wxNotebook)
    EVT_NOTEBOOK_PAGE_CHANGED	( wxID_ANY, uwNotebook::OnPageChanged )
END_EVENT_TABLE()


// ============================================================================
// uwNotebook
// ============================================================================
uwNotebook::uwNotebook
(	wxWindow*		parent
,	wxWindowID		id
,	const wxPoint&	pos
,	const wxSize&	size
,	long			style
,	const wxString&	name
) 
:	Package(NULL)
,	bClosing(false)
,	wxNotebook(parent,id,pos,size,style,name)
{
}

void uwNotebook::OnOpenFile( pkgPackage* package )
{
    Package = package;
    Show(false);

    OnAddPages();

    Show(true);
}


void uwNotebook::OnAddPages()
{
}

bool uwNotebook::AddPage( wxNotebookPage* page, const wxString& text, bool bSelect, int imageId )
{
    //Pages.push_back(page);
    page->Show(false);

    wxCommandEvent event( unEVT_NOTEPAGE_ADD );
    event.SetClientData(Package);
    page->GetEventHandler()->ProcessEvent( event );

    return wxNotebook::AddPage(page,text,bSelect,imageId);
}

void uwNotebook::OnCloseFile()
{
    Show(false);
    bClosing = true;

    while( GetPageCount() > 0 )
    {
        wxNotebookPage* page = GetPage(0);
        RemovePage(0);
        page->Show(false);
        delete page;
        //page->Destroy();
    }

    Package = NULL;
    bClosing = false;
    Show(true);
}

void uwNotebook::OnPageChanged( wxNotebookEvent& event )
{
    if( !bClosing )
    {
        //wxLogMessage( "OnPageChanged %d", event.GetSelection() );


        //uwNotebookPage::PageChanged( GetPage(event.GetSelection()) );
    }
    event.Skip();
}

// ============================================================================