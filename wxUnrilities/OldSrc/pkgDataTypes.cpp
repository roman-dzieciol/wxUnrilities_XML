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
//	pkgDataTypes.cpp
// ============================================================================
#include "prec.h"
#include "pkgDataTypes.h"
#include "pkgDecompiler.h"


// ============================================================================
// dtToken
// ============================================================================
dtToken::dtToken( const dtToken& token )
:   Data(token.Data)
,   TokenName(token.TokenName)
,   pkgDataType(token)
{
    for( ptrvector<pkgDataType*>::const_iterator it=token.Items.begin(); it!=token.Items.end(); ++it )
    {
        pkgDataType* dt = static_cast<pkgDataType*>(*it);
        Items.push_back( dt->Clone() );
    }
}

void dtToken::Read( pkgSerializer& A )
{
    // Read token id
    A >> Data; 

    // Find token in database
    dtToken* token = A.GetDecompiler()->FindToken(Data);
    if( token == NULL )
        throw exception( wxString::Format( wxT("Unknown token 0x%.2X"), Data ).c_str() );

    // Copy token properties
    TokenName = token->TokenName;
    Desc = token->Desc;
    DT_DUMPINFO;

    // Copy token items
    for( ptrvector<pkgDataType*>::const_iterator it=token->Items.begin(); it!=token->Items.end(); ++it )
    {
        pkgDataType* dt = static_cast<pkgDataType*>(*it)->Clone();
        AddItem(dt);
        dt->Read(A);
    }
}

void dtToken::Write( pkgSerializer& A )
{
}

dword dtToken::GetSerialSize() const 
{ 
    dword size = 1;
    for( ptrvector<pkgDataType*>::const_iterator it=Items.begin(); it!=Items.end(); ++it )
    {
        size += static_cast<pkgDataType*>(*it)->GetSerialSize(); 
    }
    return size;
}

dword dtToken::GetDataSize() const 
{ 
    dword size = 1;
    for( ptrvector<pkgDataType*>::const_iterator it=Items.begin(); it!=Items.end(); ++it )
    {
        size += static_cast<pkgDataType*>(*it)->GetDataSize(); 
    }
    return size;
}

wxString dtToken::GetText() const
{
    return wxString::Format(wxT("0x%.2X"),Data);
}

void dtToken::DumpInfo()
{
    wxLogMessage( wxT("[%s] %s"), GetTokenName().c_str(), GetTextVerbose().c_str() );
    for( ptrvector<pkgDataType*>::const_iterator it=Items.begin(); it!=Items.end(); ++it )
    {
        pkgDataType* dt = static_cast<pkgDataType*>(*it);
        dt->DumpInfo();
    }
    /*GDecompiler.TokenStack.push_back( GetTokenName() );
    wxLogMessage( wxT("[%s] %s"), GDecompiler.GetTokenStack().c_str(), GetTextVerbose().c_str() );
    for( ptrvector<pkgDataType*>::const_iterator it=Items.begin(); it!=Items.end(); ++it )
    {
        pkgDataType* dt = static_cast<pkgDataType*>(*it);
        dt->DumpInfo();
    }
    GDecompiler.TokenStack.resize(GDecompiler.TokenStack.size()-1);*/
}


// ============================================================================
// Static
// ============================================================================
const wxString dtIndex::Name = wxT("INDEX");
const wxString dtByte::Name = wxT("BYTE");
const wxString dtWord::Name = wxT("WORD");
const wxString dtDword::Name = wxT("DWORD");
const wxString dtFloat::Name = wxT("FLOAT");
const wxString dtToken::Name = wxT("TOKEN");
const wxString dtAsciiZ::Name = wxT("ASCIIZ");
const wxString dtUnicodeZ::Name = wxT("UNICODEZ");




// ============================================================================