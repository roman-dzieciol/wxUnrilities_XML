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
//	uwHexTree.cpp
// ============================================================================
#include "prec.h"
#include "uwHexTree.h"
#include "uwHexCtrl.h"
#include "uwTreeList.h"
#include "unWindowFactory.h"
#include "asvNamedSize.h"
#include "pkgType.h"
#include "unDataTree.h"


// ============================================================================
// uwTreeBookTree - Events
// ============================================================================
BEGIN_EVENT_TABLE(uwHexTree, wxSplitterWindow)
    EVT_TREE_SEL_CHANGED(wxID_ANY, uwHexTree::OnTreeSelChanged)
END_EVENT_TABLE()


// ============================================================================
// uwHexTree
// ============================================================================
uwHexTree::uwHexTree
(	wxWindow*		parent
,	wxWindowID		id
,	const wxPoint&	pos
,	const wxSize&	size
,	long			style
,	const wxString&	name
) 
:	wxSplitterWindow(parent,id,pos,size,style,name)
,   Tree(NULL)
,   Hex(NULL)
{
    Hex = new uwHexCtrl;
}


// ============================================================================
// uwHexTree
// ============================================================================
void uwHexTree::SetTreeFactory( unWindowFactory* factory ) 
{ 
    if( Tree != NULL )
        return;

    Hex->SetFontSize( GNamedSize(wxT("HexTreeFontDelta")) );
    Hex->SetHexWidth( GNamedSize(wxT("HexTreeColumns")) );
    Hex->Create(this);

    factory->SetParent(this);
    Tree = static_cast<uwTreeList*>(factory->Create()); 

    SplitHorizontally( Tree, Hex, GNamedSize(wxT("HexTreeSplit")) );
    SetSashGravity(static_cast<float>(GNamedSize(wxT("HexTreeSplitScale")))/100);
    m_minimumPaneSize = 40;
}

// ============================================================================
// uwHexTree
// ============================================================================
void uwHexTree::OnDoubleClickSash( int x, int y )
{
}

void uwHexTree::OnTreeSelChanged( wxTreeEvent& event )
{
    if( event.GetEventObject() != Tree )
    {
        //wxLogFunction;
        return;
    }

    unDataTree& tree = Tree->GetDataTree(event.GetItem());
    pkgType* type = static_cast<pkgType*>(tree.GetClientData());
    if( type == NULL || static_cast<void*>(type) == static_cast<void*>(&tree) )
        return;

    Hex->SetHighlight( type->GetOffset(), type->GetSerialSize() );
    Hex->Refresh();

    event.Skip();
}

// ============================================================================