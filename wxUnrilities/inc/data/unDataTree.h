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
//  unDataTree.h
// ============================================================================
#ifndef UNDATATREE_H
#define UNDATATREE_H

#include "unTree.h"
#include "unTextPrinter.h"

#define ID_IGNORE	dword(-1)

class unDataTree;




// ============================================================================
// unDataTree - Data storage for TreeCtrl widgets
// ============================================================================
class unDataTree : public unPtrTree<unDataTree>
{
public:

	// ========================================================================
	// unDataTreeData
	// ========================================================================
	class unDataTreeData : public wxTreeItemData
	{
	public:		unDataTreeData( unDataTree& tree ) : Tree(tree) {}
				unDataTree&	GetTree() const { return Tree; }
	private:	unDataTree&	Tree;
	};


	// ========================================================================
	// unDataTreeSorter
	// ========================================================================
	struct unDataTreeSorter
	{
		bool operator() ( const unDataTree* a, const unDataTree* b )
		{
			return stricmp( a->GetCaption().c_str(), b->GetCaption().c_str() ) < 0;
		}
	};


    // Special
	unDataTree( const wxString& caption, dword serialid=ID_IGNORE, bool bsort=false, void* clientdata=NULL, unTextPrinterGroup* printers=NULL ) 
	:	Caption(caption)
    ,   Image(-1)
    ,   SelectedImage(-1)
	,	TreeData(NULL)
	,	SerialID(serialid)
	,	bSort(bsort)
	,	bExpanded(false)
    ,   ClientData(clientdata)
    ,   Printers(printers)
    ,   bAutoExpand(false)
	{
	}

	~unDataTree()
	{
		for( const_iterator it=begin(); it!=end(); ++it )
			delete static_cast<unDataTree*>(*it);
	}

public:
    void Sort()
    {
        sort( begin(), end(), unDataTreeSorter() );
    }

    bool Expand()								
    { 
        if( ShouldExpand() )
        {
            bExpanded = true;
            return true;
        }
        return false;
    }

    unDataTreeData*	NewTreeData()								
    { 
        wxASSERT_MSG( !TreeData, wxT("TreeData already created") );
        TreeData = new unDataTreeData(*this);
        return TreeData;
    }

    unDataTree* FindSerialID( dword serial ) const;

    template<class A, class B>
    unDataTree* Find( A (unDataTree::*func)()const, B value ) const
    {
        for( const_iterator it=begin(); it!=end(); ++it )
        {
            unDataTree* tree = (*it);
            
            if( value == ((*tree).*func)() )
                return tree;
        }

        for( const_iterator it=begin(); it!=end(); ++it )
        {
            unDataTree* tree = (*it);
            if( !tree->empty() )
            {
                unDataTree* sub = tree->Find( func, value );
                if( sub != NULL )
                    return sub;
            }
        }

        return NULL;
    }

    wxString GetText( dword column )
    {
        if( Printers != NULL )
            return Printers->PrintText(ClientData,column);
        return Caption;
    }

public:
	const wxString&	GetCaption		    () const						{ return Caption; }
	void			SetCaption		    ( const wxString& caption )		{ Caption = caption; }
	void			SetTreeID		    ( const wxTreeItemId& treeid )	{ TreeID = treeid; }
	wxTreeItemId	GetTreeID		    () const						{ return TreeID; }
	void			SetSerialID		    ( dword serial )				{ SerialID = serial; }
	const dword		GetSerialID		    () const						{ return SerialID; }
	bool			ShouldSort		    () const						{ return bSort; }
    bool			ShouldExpand	    () const						{ return !empty() && !bExpanded; }
    void			SetSelectedImage    ( dword d )				        { SelectedImage = d; }
    const dword		GetSelectedImage    () const						{ return SelectedImage; }
    void			SetImage		    ( dword d )				        { Image = d; }
    const dword		GetImage		    () const						{ return Image; }
    void			SetClientData		( void* ptr )				    { ClientData = ptr; }
    void*   		GetClientData		() const						{ return ClientData; }
    void            SetExpanded         ( bool b )                      { bExpanded = b; }

    void			    SetPrinters		    ( unTextPrinterGroup* ptr )	    { Printers = ptr; }
    unTextPrinterGroup* GetPrinters		    () const						{ return Printers; }


    bool GetAutoExpand() const { return bAutoExpand; }
    void SetAutoExpand( bool b ) { bAutoExpand = b; }

protected:
    wxString			Caption;
    int                 Image;
    int                 SelectedImage;
	wxTreeItemId		TreeID;
	unDataTreeData*		TreeData;
	dword				SerialID;
	bool				bSort;
	bool				bExpanded;
    void*               ClientData;
    unTextPrinterGroup* Printers;
    bool                bAutoExpand;
};


// ============================================================================
// unDataTreeGroup
// ============================================================================
struct unDataTreeGroup : public vector<unDataTree*>
{
	unDataTree* Add( unDataTree* tree )
	{
		push_back(tree);
		return back();
	}

	wxString GetCaption( dword index ) const
	{
		return at(index)->GetCaption();
	}

	const wxArrayString GetCaptions() const
	{
		wxArrayString strings;
		for( const_iterator it=begin(); it!=end(); ++it)
			strings.Add( (*it)->GetCaption() );
		return strings;
	}
};



#endif UNDATATREE_H
// ============================================================================