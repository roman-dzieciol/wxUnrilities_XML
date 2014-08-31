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
//	pkgTypeFlow.cpp
// ============================================================================
#include "prec.h"
#include "pkgTypeFlow.h"
#include "pkgPackage.h"
#include "pkgPackageData.h"
#include "pkgExpression.h"


const wxString tfAssert::StaticTypeName     = wxT("assert");
const wxString tfArray::StaticTypeName      = wxT("array");
const wxString tfIf::StaticTypeName         = wxT("if");
const wxString tfOffset::StaticTypeName     = wxT("offset");
const wxString tfSeek::StaticTypeName       = wxT("seek");
const wxString tfArrayItem::StaticTypeName  = wxT(" ");
const wxString tfBreak::StaticTypeName		= wxT("break");


const wxString tfAssert::StaticDesc         = wxT("Throw exception if expression is false.");
const wxString tfArray::StaticDesc          = wxT("Load data specified number of times.");
const wxString tfIf::StaticDesc             = wxT("Load data if expression is true.");
const wxString tfOffset::StaticDesc         = wxT("Jump to absolute address in file.");
const wxString tfSeek::StaticDesc           = wxT("Jump to relative address in file.");
const wxString tfArrayItem::StaticDesc      = wxT(" ");
const wxString tfBreak::StaticDesc			= wxT("Break into debugger.");


// ============================================================================
// pkgTypeFlowRegistry
// ============================================================================
void pkgTypeFlowRegistry::OnReload()
{
    AddChild( new tfAssert, true );
    AddChild( new tfArray, true );
    AddChild( new tfIf, true );
    AddChild( new tfOffset, true );
    AddChild( new tfSeek, true );
    AddChild( new tfBreak, true );
}


// ============================================================================
// pkgTypeFlow
// ============================================================================
pkgTypeFlow::pkgTypeFlow( const pkgTypeFlow& c )
:   pkgType(c)
,   ExpressionText(c.ExpressionText)
,   Expression(NULL)
{
    pkgLogTypeCopy;
    if( c.Expression != NULL )
        Expression = c.Expression->Clone();
}

pkgTypeFlow::~pkgTypeFlow()
{
    delete Expression;
}


// ============================================================================
// tfAssert
// ============================================================================
void tfAssert::OnRead( pkgPackage& package, pkgType& parent )
{
    bTest = Expression->GetBool(package,parent);
    pkgLogTypeOnRead;

    if( !bTest )
        throw exString( wxString::Format( wxT("%s"), ExpressionText.c_str() ));
}


// ============================================================================
// tfArrayItem
// ============================================================================
void tfArrayItem::OnRead( pkgPackage& package, pkgType& parent )
{
    pkgLogTypeOnRead;
}


// ============================================================================
// tfArray
// ============================================================================
void tfArray::OnRead( pkgPackage& package, pkgType& parent )
{
    Count = Expression->GetUnsigned(package,parent);
    pkgLogTypeOnRead;

    // create data type
    tfArray* datatype = Construct();
    datatype->Count = Count;
    datatype->SetPrototype(this);
    datatype->SetParent(&parent);


    // add data type
    parent.AddChild(datatype);

    // if template has children
    if( HasChildren() )
    {
        // create data children
        for( dword i=0; i<Count; ++i )
        {
            tfArrayItem* item = new tfArrayItem();
            item->SetUnsigned(i);
            item->SetName(Name);
            pkgType::OnRead(package,*item);
            datatype->AddChild(item);
        }
    }
}


// ============================================================================
// tfIf
// ============================================================================
void tfIf::OnRead( pkgPackage& package, pkgType& parent )
{
    bTest = Expression->GetBool(package,parent);
    pkgLogTypeOnRead;

    if( bTest && HasChildren() )
    {
        pkgType::OnRead(package,parent);
    }
}


// ============================================================================
// tfOffset
// ============================================================================
void tfOffset::OnRead( pkgPackage& package, pkgType& parent )
{
    OldOffset = package.GetFile().Tell();
    NewOffset = Expression->GetUnsigned(package,parent);
    pkgLogTypeOnRead;

    package.GetFile().Seek( NewOffset, wxFromStart );
    if( HasChildren() )
    {
        pkgType::OnRead(package,parent);
        package.GetFile().Seek( OldOffset, wxFromStart );
    }
}


// ============================================================================
// tfSeek
// ============================================================================
void tfSeek::OnRead( pkgPackage& package, pkgType& parent )
{
    OldOffset = package.GetFile().Tell();
    NewOffset = Expression->GetUnsigned(package,parent);
    pkgLogTypeOnRead;

    package.GetFile().Seek( NewOffset, wxFromCurrent );
    if( HasChildren() )
    {
        pkgType::OnRead(package,parent);
        package.GetFile().Seek( OldOffset, wxFromStart );
    }
}

// ============================================================================
// tfBreak
// ============================================================================
void tfBreak::OnRead( pkgPackage& package, pkgType& parent )
{
    pkgLogTypeOnRead;
	wxTrap();
}



// ============================================================================