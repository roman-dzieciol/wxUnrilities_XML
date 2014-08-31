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
//	pvNames.cpp
// ============================================================================
#include "prec.h"
#include "pvNames.h"

#include "unTable.h"
#include "pkgSerializer.h"


pvNames::pvNames
(	wxWindow*		parent
 ,	wxWindowID		id
 ,	const wxPoint&	pos
 ,	const wxSize&	size
 ,	long			style
 ,	const wxString&	name
 ) 
 :	unPackageView(parent,id,pos,size,style,name)
{
}


void pvNames::InitData( unTable& vtable )
{
    unFlagGroup* group = Serializer->GetFlags()->FindGroup(wxT("Object"));

	vtable << thColumn( wxT("#"),		Fmt::Index )	<< new_printer( tf_Stream(), ds_NameTable(ItemIndex), ic_Data() ) << new unMathEx;
	vtable << thColumn( wxT("Name"),	Fmt::Name )		<< new_printer( tf_Stream(), ds_NameTable(Name), ic_TextNoCase() ) << new unRegEx;
	vtable << thColumn( wxT("Flags"),	Fmt::Flag )		<< new_printer( tf_Flags(group), ds_NameTable(Flags), ic_Data() ) << new unRegEx;

	vtable.Init( Serializer->GetNameTable().size() );
}

// ============================================================================