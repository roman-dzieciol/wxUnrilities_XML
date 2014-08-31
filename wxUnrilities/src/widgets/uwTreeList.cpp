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
//	uwTreeList.cpp
// ============================================================================
#include "prec.h"
#include "uwTreeList.h"
#include "unDataTree.h"
//#include "unTable.h"

// TODO: Fix buggy wxTreeListCtrl HitTest()
// TODO: Fix buggy wxTreeListCtrl scrolling


// ============================================================================
// uwTreeList - Event Table
// ============================================================================
BEGIN_EVENT_TABLE(uwTreeList, wxTreeListCtrl)
	EVT_TREE_ITEM_EXPANDING	    ( wxID_ANY, uwTreeList::OnItemExpanding )
	EVT_LIST_COL_CLICK	        ( wxID_ANY, uwTreeList::OnColClick )
END_EVENT_TABLE()


// ============================================================================
// uwTreeList - Special
// ============================================================================
uwTreeList::uwTreeList
(	wxWindow*			parent
,	wxWindowID			id
,	const wxPoint&		pos
,	const wxSize&		size
,	long				style
,	const wxValidator&	validator
,	const wxString&		name
) 
:	wxTreeListCtrl(parent,id,pos,size,style,validator,name)
{
}

uwTreeList::~uwTreeList()
{
	//delete Table;
}



// ============================================================================
// uwTreeList - DataTree
// ============================================================================
void uwTreeList::SetDataTree( unDataTree& tree )
{
	wxLogMessage("SetDataTree");
    DataTree = &tree;
	tree.SetTreeID( wxTreeListCtrl::AddRoot( tree.GetCaption(), -1, -1, tree.NewTreeData() ) );	
	AddPlaceholder( tree );
}

unDataTree& uwTreeList::GetDataTree( wxTreeItemData* item ) const
{
	return static_cast<unDataTree::unDataTreeData*>(SafePtr(item))->GetTree();
}

unDataTree& uwTreeList::GetDataTree( const wxTreeItemId& item ) const
{
	return GetDataTree(GetItemData(SafeTreeID(item)));
}


// ============================================================================
// uwTreeList - Tree
// ============================================================================
void uwTreeList::AddPlaceholder( unDataTree& parent )
{
	if( parent.ShouldExpand() )
		SetItemHasChildren( parent.GetTreeID(), true );
}

void uwTreeList::AddChildren( unDataTree& parent )
{
	//SW_DEF( sw );
	for( unDataTree::iterator it=parent.begin(); it!=parent.end(); ++it )
	{
		unDataTree& tree = *(*it);
        //if( !tree.GetTreeID().IsOk() )
	    tree.SetTreeID( AppendItem( parent.GetTreeID(), tree.GetCaption(), -1, -1, tree.NewTreeData() ) );
		AddPlaceholder( tree );
	}
	//SW_LOGF( sw );
}

void uwTreeList::AddChild( unDataTree& parent, unDataTree& child )
{
    child.SetTreeID( AppendItem( parent.GetTreeID(), child.GetCaption(), -1, -1, child.NewTreeData() ) );
    AddPlaceholder( child );
}

void uwTreeList::SortChildren( const wxTreeItemId& item )
{
	//SW_DEF( sw );
	wxTreeListCtrl::SortChildren( item );
	//SW_LOGF( sw );
}


void uwTreeList::ExpandTree( unDataTree& tree )
{
	stack<unDataTree*> parents;
	unDataTree* t;
	unDataTree* collapse = NULL;

	for( t = tree.GetParent(); t != NULL; t = t->GetParent() ) 
	{
		parents.push(t);
		if( collapse == NULL && t->GetSerialID() != -1 )
			collapse = t;
	} 

	while( !parents.empty() )
	{
		t = parents.top();
		if( t->ShouldExpand() )
		{
			EnsureVisible( t->GetTreeID() );
			Expand( t->GetTreeID() );
		}
		parents.pop();
	}

	EnsureVisible( tree.GetTreeID() );
	SelectItem( tree.GetTreeID() );
}

void uwTreeList::ExpandRoot()
{		
	wxTreeItemId root = GetRootItem();
	ExpandPlaceholder(root);
	//if( root.IsOk() && HasChildren(root) /*&& !IsExpanded(root)*/ )
	//{
	//	Expand(root);
	//}
}

void uwTreeList::ExpandRange( const wxTreeItemId& parent, int sublevels )
{
    if( parent.IsOk() )
    {
        Expand(parent);
        if( sublevels > 0 )
        {
            wxTreeItemIdValue cookie = 0;
            wxTreeItemId child = GetFirstChild(parent, cookie);
            while( child.IsOk() ) 
            {
                ExpandRange(child,sublevels-1);
                child = GetNextChild(parent, cookie);
            }
        }
    }
}

void uwTreeList::ExpandPlaceholder( const wxTreeItemId& parent )
{
	unDataTree& tree = GetDataTree( parent );

	if( tree.Expand() )
	{
		AddChildren( tree );	
		if( tree.ShouldSort() )				
			SortChildren( tree.GetTreeID() );	

        wxTreeItemIdValue cookie = 0;
        wxTreeItemId child = GetFirstChild(parent, cookie);
        while( child.IsOk() ) 
        {
            unDataTree& tree_child = GetDataTree( child );
            if( tree_child.GetAutoExpand() )
            {
                Expand(child);
            }
            child = GetNextChild(parent, cookie);
        }
	}
}

// ============================================================================
// uwTreeList - Events
// ============================================================================
void uwTreeList::OnColClick( wxListEvent& event )
{
    wxLogMessage("OnColClick");
}

int uwTreeList::OnCompareItems( const wxTreeItemId& item1, const wxTreeItemId& item2 )
{
	return stricmp( GetItemText(item1).c_str(), GetItemText(item2).c_str() );
}

void uwTreeList::OnItemExpanding( wxTreeEvent& event )
{
	ExpandPlaceholder( event.GetItem() );
}

wxString uwTreeList::OnGetItemText( wxTreeItemData* item, long column ) const
{
    return GetDataTree(item).GetText(column);
    //dword index = tree.GetSerialID();
    //if		( index != ID_IGNORE )			return Table->GetText( index, column );	
    //else if	( column == GetMainColumn() )	return GetDataTree(item)->GetCaption();		
    //else									    return wxEmptyString;	
}

// ============================================================================