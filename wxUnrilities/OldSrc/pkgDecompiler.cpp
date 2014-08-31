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
//	pkgDecompiler.cpp
// ============================================================================
#include "prec.h"
#include "pkgDecompiler.h"
#include "pkgDataTypes.h"
#include "pkgDataTypeFactory.h"
#include "pkgCondition.h"
#include "unXmlParser.h"
#include "unXmlParseCmd.h"
#include "pxpByteCode.h"
#include "pkgNativeFunction.h"


// REGEX: EExprToken in UnStack.h
// ^[\s]+([aA-zZ]+_[aA-zZ]+)[\s]+= (0x[0-9A-F]+),[\s/]*([\s\S]*)
// <token>\n\t<code>\2</code>\n\t<name>\1</name>\n\t<desc>\3</desc>\n</token>


// ============================================================================
// pkgDecompiler :: Constructors
// ============================================================================
pkgDecompiler::pkgDecompiler()
:   IdExtended(-1)
,   IdFirst(-1)
,   NativeFunctions(NULL)
{
}

pkgDecompiler::~pkgDecompiler()
{    
    safe_destroy(NativeFunctions);

    for( pkgTokenMap::iterator it=TokenMap.begin(); it!=TokenMap.end(); ++it )
    {
        delete static_cast<pkgTokenSelector*>((*it).second);
    }
}


pkgDecompiler* pkgDecompiler::Create()
{
    return new pkgDecompiler;
}

void pkgDecompiler::Destroy()
{
    delete this;
}


// ============================================================================
// pkgDecompiler :: Token map
// ============================================================================
dtToken* pkgDecompiler::AddToken( dtToken* token, pkgTokenCondition* condition )
{
    wxLogMessage("%d",token->GetData());
    pkgTokenSelector* ts = FindTokenSelector( token->GetData() );
    if( ts == NULL )
    {
        ts = new pkgTokenSelector();
        TokenMap.insert( make_pair<dword,pkgTokenSelector*>(token->GetData(),ts) );
    }

    ts->AddToken(token);
    return token;
}

pkgTokenGroup* pkgDecompiler::AddTokenGroup( pkgTokenGroup* group )
{
    TokenGroups.push_back(group);
    return group;
}

dtToken* pkgDecompiler::FindToken( byte tokenid )
{
    pkgTokenSelector* ts = FindTokenSelector(tokenid);
    if( ts != NULL )
    {
        dtToken* token = ts->SelectToken(this);
        return token;
    }

    if( tokenid >= IdFirst )
    {
        pkgNativeFunction* func = NativeFunctions->FindByIndex(tokenid);
        if( func != NULL )
        {
            dtToken* nfunc = new dtToken();
            nfunc->SetTokenName(wxT("NATIVE F"));
            return nfunc;
        }
    }

    if( tokenid >= IdExtended )
    {
        dtToken* nfunc = new dtToken();
        nfunc->SetTokenName( wxString::Format(wxT("NATIVE E")) );
        nfunc->AddItem( dtByte::Create() );
        return nfunc;
    }


    return NULL;
}

pkgTokenSelector* pkgDecompiler::FindTokenSelector( byte tokenid )
{
    pkgTokenMap::iterator it = TokenMap.find(tokenid);
    if( it != TokenMap.end() )  
            return static_cast<pkgTokenSelector*>((*it).second);
    else    return NULL;
}


// ============================================================================
// pkgDecompiler :: Config file
// ============================================================================
void pkgDecompiler::OnReload()
{
    safe_destroy(NativeFunctions);
    NativeFunctions = pkgNativeFunctions::Create();
    NativeFunctions->OnReload();

    LoadConfig( wxFileName( wxT(".\\Formats\\ByteCode.xml") ) );
    ParseConfig();
}

void pkgDecompiler::LoadConfig( const wxFileName& path )
{
    SW_DEF(sw);
    
    ScriptFile = path;
    if( !path.FileExists() )
        throw exception( wxString::Format( wxT("Can't open file %s"), path.GetFullPath().c_str() ).c_str() );

    ScriptFormat.SetRoot(NULL);
    ScriptFormat.Load(path.GetFullPath());
    if( !ScriptFormat.IsOk() )
        throw exception( wxString::Format( wxT("File %s contains invalid data."), path.GetFullPath().c_str() ).c_str() );

    SW_LOGF(sw);
}



void pkgDecompiler::ParseConfig()
{
    SW_DEF(sw);

    //unXmlParser
    pxpByteCode test(this);
    test.ParseFile(ScriptFormat);
   
    SW_LOGF(sw);
}



// ============================================================================
// pkgDecompiler :: ByteCode parser
// ============================================================================
void pkgDecompiler::Decompile( pkgSerializer& A, dword size, vector<dtToken*>& stream )
{
    if( !ScriptFormat.IsOk() )
        throw exception( wxString::Format( wxT("File %s contains invalid data."), ScriptFile.GetFullPath().c_str() ).c_str() );

    dword readsize = 0;
    TokenStream.clear();

    while( readsize < size )
    {
        dtToken* token = TokenStream.Add(new dtToken(A));
        readsize += token->GetDataSize();
        
        //token->DumpInfo();
        //wxLogMessage("readsize: %d",readsize);
    }

    stream.clear();
    stream = TokenStream;
    //throw exception( wxT("Can't parse script, decompiler required.") );
}

wxString pkgDecompiler::GetTokenStack()
{
    wxString s;
    for( vector<wxString>::const_iterator it=TokenStack.begin(); it!=TokenStack.end(); ++it )
    {
        s.Append( static_cast<wxString>(*it) );
        if( it+1 != TokenStack.end() )
            s.Append( "::" );
    }
    return s;
}


void pkgDecompiler::DumpTokens()
{
    for( pkgTokenMap::iterator it=TokenMap.begin(); it!=TokenMap.end(); ++it )
    {
        pkgTokenSelector* ts = static_cast<pkgTokenSelector*>((*it).second);

        for( pkgTokenConditions::iterator ci=ts->Conditions.begin(); ci!=ts->Conditions.end(); ++ci )
        {
            pkgTokenCondition* tc = static_cast<pkgTokenCondition*>(*ci);
            dtToken* token = tc->GetToken();
            token->DumpInfo();
        }
        
    }
}


void pkgDecompiler::SetFunctionIdExtended( dword d )
{
    IdExtended = d;
}

void pkgDecompiler::SetFunctionIdFirst( dword d )
{
    IdFirst = d;
}


// ============================================================================