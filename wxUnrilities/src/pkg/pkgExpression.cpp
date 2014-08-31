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
//	pkgExpression.cpp
// ============================================================================
#include "prec.h"
#include "pkgExpression.h"
#include "pkgType.h"
#include "pkgTypeBase.h"
#include "pkgFlags.h"
#include "pkgPackage.h"
#include "pkgPackageFormat.h"


// 
// new token ideas:
// - Lte, Lt, Gt, Gte
// 


// ============================================================================
// exParser
// ============================================================================
exToken* exParser::GetTokens( const wxString& text )
{
    exASSERT( !text.empty(), wxT("no text") );

    // DEBUG: dump expression text
    //wxLogMessage( wxT("EXPR: %s"), text.c_str() );

    // load tokens
    wxStringTokenizer parser( text, wxT("(),"), wxTOKEN_RET_EMPTY );
    exASSERT( parser.HasMoreTokens(), wxT("no tokens found") );
    exToken* root = ParseToken( text, parser );

    // DEBUG: dump expression tokens
    //wxLogMessage( wxT("EXPR: %s"), root->GetInfo().c_str() );
    return root;
}

exToken* exParser::ParseToken( const wxString& text, wxStringTokenizer& parser )
{
    //
    //  Lte(Var(Signature),0)
    //

    // get token name & delimiter
    const wxString name = parser.GetNextToken();
    const wxChar delim = text.GetChar(parser.GetPosition()-1);

    // DEBUG: dump delimiter name
    //wxLogMessage( wxT("DELIM: %c"), delim );

    // DELIM: begin function
    if( delim == wxT('(') )
    {
        exASSERT( !name.empty(), wxT("empty name") );

        if( name.IsSameAs(wxT("Var")) )
        {
            return new exVar(ParseToken(text,parser));
        }
        else if( name.IsSameAs(wxT("Eq")) )
        {
            exToken* left = ParseToken(text,parser);
            exToken* right = ParseToken(text,parser);
            return new exEqual(left,right);
        }
        else if( name.IsSameAs(wxT("Neq")) )
        {
            exToken* left = ParseToken(text,parser);
            exToken* right = ParseToken(text,parser);
            return new exNotEqual(left,right);
        }
        else if( name.IsSameAs(wxT("HasFlag")) )
        {
            exToken* a = ParseToken(text,parser);
            exToken* b = ParseToken(text,parser);
            exToken* c = ParseToken(text,parser);
            return new exHasFlag(a,b,c);
        }
        else if( name.IsSameAs(wxT("Export")) )
        {
            return new exVar(ParseToken(text,parser));
        }
        else
        {
            throw exception( wxString::Format(wxT("Unhandled function: NAME=[%s]"),name).c_str() );
        }
    }

    // DELIM: end function
    else if( delim == wxT(')') )
    {
        if( name.empty() )
        {
            throw exception( wxString::Format(wxT("Unhandled bracket")).c_str() );
        }
        else
        {
            return new exConst(name);
        }
    }

    // DELIM: param
    else if( delim == wxT(',') )
    {
        if( name.empty() )
        {
            return ParseToken(text,parser);
        }
        else
        {
            return new exConst(name);
        }
    }
    else
    {
        throw exception( wxString::Format(wxT("Unhandled delimiter: DELIM=[%c]"),delim).c_str() );
    }
}


// ============================================================================
// exEqual
// ============================================================================
bool exEqual::GetBool( pkgPackage& package, pkgType& parent )
{
    return A->Compare(package,parent,*B);
}


// ============================================================================
// exHasFlag
// ============================================================================
bool exHasFlag::GetBool( pkgPackage& package, pkgType& parent )
{
    wxString flaggroup = B->GetName();
    wxString flagname = C->GetName();
    dword number = A->GetUnsigned(package,parent);
    dword flag = package.GetFormat().GetFlags().FindFlag(flaggroup,flagname)->GetData();
    return (number & flag) == flag;
}


// ============================================================================
// exNotEqual
// ============================================================================
bool exNotEqual::GetBool( pkgPackage& package, pkgType& parent )
{
    return !A->Compare(package,parent,*B);
}


// ============================================================================
// exVar
// ============================================================================
bool exVar::Compare( pkgPackage& package, pkgType& parent, exToken& token )
{
    pkgType* left = GetType(package,parent);
    pkgType* right = token.GetType(package,parent);
    return left->Compare(right);
}

pkgType* exVar::GetType( pkgPackage& package, pkgType& parent )
{
    return parent.FindTypeBySuperName(A->GetName());
}

dword exVar::GetUnsigned( pkgPackage& package, pkgType& parent )
{
    return GetType(package,parent)->GetUnsigned();
}


// ============================================================================
// exExport
// ============================================================================
pkgType* exExport::GetType( pkgPackage& package, pkgType& parent )
{
    return parent.FindTypeBySuperName(A->GetName());
}

dword exExport::GetUnsigned( pkgPackage& package, pkgType& parent )
{
    return GetType(package,parent)->GetUnsigned();
}




// ============================================================================
// exConst
// ============================================================================
exConst::exConst( const wxString& name )
:   exToken(name)
{
    StringType = new tbASCIIZ;
}

exConst::exConst( const exConst& c )
:   exToken(c.Name)
{
    StringType = new tbASCIIZ;
}

exConst::~exConst()
{
    delete StringType;
}

wxString exConst::GetInfo()
{
    return Name;
}

pkgType* exConst::GetType( pkgPackage& package, pkgType& parent )
{
    StringType->SetString(Name);
    return StringType;
}

dword exConst::GetUnsigned( pkgPackage& package, pkgType& parent )
{
    StringType->SetString(Name);
    return StringType->GetUnsigned();
}


// ============================================================================
// exUnary
// ============================================================================
wxString exUnary::GetInfo()
{
    return wxString::Format( wxT("%s(%s)"), Name.c_str(), A->GetInfo().c_str() );
}


// ============================================================================
// exBinary
// ============================================================================
wxString exBinary::GetInfo()
{
    return wxString::Format( wxT("%s(%s,%s)"), Name.c_str(), A->GetInfo().c_str(), B->GetInfo().c_str() );
}


// ============================================================================
// exTernary
// ============================================================================
wxString exTernary::GetInfo()
{
    return wxString::Format( wxT("%s(%s,%s,%s)"), Name.c_str(), A->GetInfo().c_str(), B->GetInfo().c_str(), C->GetInfo().c_str() );
}




// ============================================================================