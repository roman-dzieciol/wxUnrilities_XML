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
//	uwTreeBook.cpp
// ============================================================================
#include "prec.h"
#include "uwTreeBook.h"
#include "uwTreeList.h"
#include "unDataTree.h"
#include "unWindowFactory.h"


// ============================================================================
// uwTreeBook - Events
// ============================================================================
DEFINE_EVENT_TYPE(unEVT_TREEPAGE_ADD)
DEFINE_EVENT_TYPE(unEVT_TREEPAGE_CHANGED)


// ============================================================================
// uwTreeBookTree - Events
// ============================================================================
BEGIN_EVENT_TABLE(uwTreeBook, wxSplitterWindow)
    EVT_TREE_SEL_CHANGED(wxID_ANY, uwTreeBook::OnTreeChanged)
END_EVENT_TABLE()


// ============================================================================
// uwTreeBook
// ============================================================================
uwTreeBook::uwTreeBook
(	wxWindow*		parent
,	wxWindowID		id
,	const wxPoint&	pos
,	const wxSize&	size
,	long			style
,	const wxString&	name
) 
:	wxSplitterWindow(parent,id,pos,size,style,name)
,   TreeFactory(NULL)
{
    Blank = new wxPanel(this);
}

uwTreeBook::~uwTreeBook()
{
    delete TreeFactory;
    for( unWindowFactoryArray::iterator it=Factories.begin(); it!=Factories.end(); ++it )
        delete (*it);
}

bool uwTreeBook::SplitVertically( wxWindow* window2, int sashPosition )
{
    return wxSplitterWindow::SplitVertically( Tree, window2, sashPosition );
}

bool uwTreeBook::SplitHorizontally( wxWindow* window2, int sashPosition )
{
    return wxSplitterWindow::SplitHorizontally( Tree, window2, sashPosition );
}

unWindowFactory* uwTreeBook::AddFactory( unWindowFactory* factory )
{
    Factories.push_back(factory);
    return factory;
}

void uwTreeBook::OnTreeChanged( wxTreeEvent& event )
{
    if( event.GetEventObject() != Tree )
    {
        //wxLogFunction;
        return;
    }

    unDataTree& tree = Tree->GetDataTree(event.GetItem());

    wxCommandEvent newevent( unEVT_TREEPAGE_CHANGED );
    newevent.SetClientData( tree.GetClientData() );
    GetEventHandler()->ProcessEvent( newevent );

    event.Skip();
}


// ============================================================================