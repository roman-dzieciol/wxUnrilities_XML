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
//  pkgDecompiler.h
// ============================================================================
#ifndef PKGDECOMPILER_H
#define PKGDECOMPILER_H

#include "pkgSerializer.h"
#include "unXmlDocument.h"


class dtToken;
class pkgDataType;
class pkgTokenCondition;
class pkgTokenSelector;
class pkgTokenGroup;
class unXmlParseTree;
class pkgNativeFunctions;


// ============================================================================
// pkgTokenGroup
// ============================================================================
class pkgTokenGroup
{
public:
    pkgTokenGroup(){}

public:
    wxString GetName() const { return Name; }
    void SetName( const wxString& name ) { Name=name; }
    dtToken* AddToken( dtToken* token ) { Tokens.push_back(token); return token; }
    static pkgTokenGroup* Create() { return new pkgTokenGroup; }
    void Destroy() { delete this; }

public:
    wxString            Name;
    vector<dtToken*>   Tokens;
};





// ============================================================================
// pkgTokenStream
// ============================================================================
class pkgTokenStream : public vector<dtToken*>
{
public:
    pkgTokenStream(){}

public:
    dtToken* Add( dtToken* token ) { push_back(token); return token; }
};



// ============================================================================
// pkgTokenMap
// ============================================================================
typedef map<dword,pkgTokenSelector*> pkgTokenMap;


// ============================================================================
// pkgDecompiler
// ============================================================================
class pkgDecompiler
{
public:
    // Constructors
    pkgDecompiler();
    ~pkgDecompiler();

    static pkgDecompiler* Create();
    void Destroy();

    // Public interface
    void Decompile( pkgSerializer& A, dword size, vector<dtToken*>& stream );

    // Config file
    void OnReload();
    void LoadConfig( const wxFileName& path );
    void ParseConfig();

    // Token map
    dtToken* AddToken( dtToken* token, pkgTokenCondition* condition );
    pkgTokenGroup* AddTokenGroup( pkgTokenGroup* group );
    dtToken* FindToken( byte tokenid );
    pkgTokenSelector* FindTokenSelector( byte tokenid );

    // native func
    void SetFunctionIdExtended( dword d );
    void SetFunctionIdFirst( dword d );

    // Debug
    void DumpTokens();
    wxString GetTokenStack();

protected:
    unXmlDocument               ScriptFormat;
    wxFileName                  ScriptFile;
    pkgTokenMap                 TokenMap;
    vector<pkgTokenGroup*>      TokenGroups;
    pkgTokenStream              TokenStream;
    dword                       IdExtended;
    dword                       IdFirst;
    pkgNativeFunctions*         NativeFunctions;

public:
    vector<wxString>            TokenStack;
};



#endif PKGDECOMPILER_H
// ============================================================================