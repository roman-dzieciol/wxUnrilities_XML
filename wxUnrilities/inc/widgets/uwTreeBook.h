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
//  uwTreeBook.h
// ============================================================================
#ifndef UWTREEBOOK_H
#define UWTREEBOOK_H

class unWindowFactory;
class uwTreeList;


// ============================================================================
// unWindowFactory
// ============================================================================
typedef vector<unWindowFactory*> unWindowFactoryArray;


// ============================================================================
// uwTreeBook - Events
// ============================================================================
DECLARE_EVENT_TYPE(unEVT_TREEPAGE_ADD, -1)
DECLARE_EVENT_TYPE(unEVT_TREEPAGE_CHANGED, -1)


// ============================================================================
// uwTreeBook
// ============================================================================
class uwTreeBook : public wxSplitterWindow
{
public:
    // Special
	uwTreeBook
	(	wxWindow*		parent
	,	wxWindowID		id = wxID_ANY
	,	const wxPoint&	pos = wxDefaultPosition
	,	const wxSize&	size = wxDefaultSize
	,	long			style = 0
	,	const wxString&	name = wxT("uwTreeBook")
    );

    ~uwTreeBook();

    // Accessors
    uwTreeList* GetTree() { return Tree; }
    unWindowFactoryArray& GetFactories() { return Factories; }

    bool SplitVertically( wxWindow* window2, int sashPosition=0 );
    bool SplitHorizontally( wxWindow* window2, int sashPosition=0 );
    unWindowFactory* AddFactory( unWindowFactory* factory );

private:
    void OnTreeChanged( wxTreeEvent& event );

protected:
    uwTreeList* Tree;
    wxPanel* Blank;
    unWindowFactoryArray Factories;
    unWindowFactory* TreeFactory;

private:
    DECLARE_EVENT_TABLE()
};




#endif UWTREEBOOK_H
// ============================================================================