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
//  uwTreeList.h
// ============================================================================
#ifndef UWTREELIST_H
#define UWTREELIST_H


class unDataTreeList;
class unDataTree;
class unTable;


// ============================================================================
// uwTreeList
// ============================================================================
class uwTreeList : public wxTreeListCtrl
{
public:
    // Special
	uwTreeList
	(	wxWindow*			parent
	,	wxWindowID			id = wxID_ANY
	,	const wxPoint&		pos = wxDefaultPosition
	,	const wxSize&		size = wxDefaultSize
	,	long				style = wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT | wxTR_FULL_ROW_HIGHLIGHT | wxTR_NO_LINES | wxTR_VIRTUAL
	,	const wxValidator&	validator = wxDefaultValidator
	,	const wxString&		name = wxT("uwTreeList")
	);

    virtual ~uwTreeList();

    // Accessors
    unDataTree* GetDataTree() { return DataTree; }

    // Mutators
    void SetDataTree( unDataTree& tree );

    // Tree
    unDataTree& GetDataTree( wxTreeItemData* item ) const;
    unDataTree& GetDataTree( const wxTreeItemId& item ) const;

    void AddPlaceholder( unDataTree& parent );
    void AddChildren( unDataTree& parent );
    void AddChild( unDataTree& parent, unDataTree& child );
    void SortChildren( const wxTreeItemId& item );

    void ExpandTree( unDataTree& tree );
    void ExpandRoot();

    void ExpandRange( const wxTreeItemId& item, int sublevels );
	void ExpandPlaceholder( const wxTreeItemId& parent );

	// Fake
	void SetVerticalAlign( bool b ){}

protected:
    // Events
    void OnColClick( wxListEvent& event );
    void OnItemExpanding( wxTreeEvent& event );
    virtual int OnCompareItems( const wxTreeItemId& item1, const wxTreeItemId& item2 );
    virtual wxString OnGetItemText( wxTreeItemData* item, long column ) const;

protected:
    unTable*	Table;
    unDataTree* DataTree;

private:
    DECLARE_EVENT_TABLE()
};


// ============================================================================
// uwTreeListColumnInfo
// ============================================================================
class uwTreeListColumnInfo : public wxTreeListColumnInfo 
{
public:
    uwTreeListColumnInfo 
    (   const wxString& text = wxEmptyString
    ,   int width = DEFAULT_COL_WIDTH
    ,   int flag = wxALIGN_LEFT
    ,   int image = -1
    ,   int selectedimage = -1
    ,   bool shown = true
    ,   bool edit = false
    )   
    :   wxTreeListColumnInfo( text, width, flag, image, shown, edit )
    {
        SetSelectedImage(selectedimage);
    }

    uwTreeListColumnInfo( const wxString& text, const wxTreeListColumnInfo& other )
    :   wxTreeListColumnInfo(other)
    {
        SetText(text);
    }
};


// ============================================================================
// uwTreeListColumnInfo - Presets
// ============================================================================
namespace uwTreeListColumnInfos
{
    static uwTreeListColumnInfo Offset          ( wxEmptyString, 75,    wxALIGN_LEFT );
    static uwTreeListColumnInfo FlagBits        ( wxEmptyString, 75,    wxALIGN_LEFT );
    static uwTreeListColumnInfo ShortName       ( wxEmptyString, 100,   wxALIGN_LEFT );
    static uwTreeListColumnInfo Name            ( wxEmptyString, 200,   wxALIGN_LEFT );
    static uwTreeListColumnInfo LongName        ( wxEmptyString, 350,   wxALIGN_LEFT );
    static uwTreeListColumnInfo Desc            ( wxEmptyString, 500,   wxALIGN_LEFT );
    static uwTreeListColumnInfo HiddenLeft      ( wxEmptyString, 50,    wxALIGN_LEFT, -1, -1, false );
    static uwTreeListColumnInfo HiddenRight     ( wxEmptyString, 50,    wxALIGN_RIGHT, -1, -1, false );
    static uwTreeListColumnInfo Hex             ( wxEmptyString, 75,    wxALIGN_LEFT );
    static uwTreeListColumnInfo TypeName        ( wxEmptyString, 100,   wxALIGN_RIGHT );

    static uwTreeListColumnInfo Index           ( wxEmptyString, 45,   wxALIGN_LEFT );
    static uwTreeListColumnInfo IndexOf         ( wxEmptyString, 45,   wxALIGN_RIGHT );
    static uwTreeListColumnInfo Size            ( wxEmptyString, 75,   wxALIGN_RIGHT );

    static uwTreeListColumnInfo LDecimal        ( wxEmptyString, 75,   wxALIGN_LEFT );
    static uwTreeListColumnInfo RDecimal        ( wxEmptyString, 75,   wxALIGN_RIGHT );
}




#endif UWTREELIST_H
// ============================================================================