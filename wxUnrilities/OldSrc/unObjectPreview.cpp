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
//	unObjectPreview.cpp
// ============================================================================
#include "prec.h"
#include "unObjectPreview.h"
#include "pkgSerializer.h"
#include "unTreeList.h"


// ============================================================================
// unObjectPreview
// ============================================================================
unObjectPreview::unObjectPreview
(	wxWindow*		parent
,	wxWindowID		id
,	const wxPoint&	pos
,	const wxSize&	size
,	long			style
,	const wxString&	name
) 
:	unNotebookPage(parent,id,pos,size,style,name)
{
}

unObjectPreview::~unObjectPreview()
{
}

unNotebookPage* unObjectPreview::Initialize( pkgSerializer* serializer )
{
    Serializer = serializer;

    Splitter = new wxSplitterWindow(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxSP_NOBORDER);
    ObjTree = new unTreeList(Splitter);
    Viewer = new wxPanel(Splitter);

    Splitter->SplitVertically(ObjTree,Viewer,300);


    wxBoxSizer* vs = new wxBoxSizer( wxVERTICAL );
    vs->Add( Splitter, 1, wxEXPAND, 0 );	
    vs->SetSizeHints( this );
    SetSizerAndFit( vs ); 

    return this;
}





// ============================================================================