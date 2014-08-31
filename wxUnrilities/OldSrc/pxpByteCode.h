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
//  pxpByteCode.h
// ============================================================================
#ifndef PXPBYTECODE_H
#define PXPBYTECODE_H

#include "unXmlParser.h"

class pkgTokenCondition;
class pkgDecompiler;
class dtToken;
class pkgTokenGroup;
class pkgDataType;


// ============================================================================
// pxpByteCode
// ============================================================================
class pxpByteCode : public unXmlParser
{
public:
    pxpByteCode( pkgDecompiler* decompiler );

    // state variables mutators
    void SetToken                   ( dtToken* p )                  { CToken = p; }
    void SetTokenTree               ( unXmlParseTree* p )             { CTokenTree = p; }
    void SetTokenGroup              ( pkgTokenGroup* p )            { CTokenGroup = p; }
    void SetTokenGroupTree          ( unXmlParseTree* p )             { CTokenGroupTree = p; }
    void SetTokenItem               ( pkgDataType* p )              { CTokenItem = p; }
    void SetTokenItemTree           ( unXmlParseTree* p )             { CTokenItemTree = p; }
    void SetTokenGroupCnd           ( pkgTokenCondition* p )        { CTokenGroupCnd = p; }
    void SetTokenGroupCndTree       ( unXmlParseTree* p )             { CTokenGroupCndTree = p; }

    // state variables mutators
    void ClearToken                   (){ CToken = NULL; }
    void ClearTokenTree               (){ CTokenTree = NULL; }
    void ClearTokenGroup              (){ CTokenGroup = NULL; }
    void ClearTokenGroupTree          (){ CTokenGroupTree = NULL; }
    void ClearTokenItem               (){ CTokenItem = NULL; }
    void ClearTokenItemTree           (){ CTokenItemTree = NULL; }
    void ClearTokenGroupCnd           (){ CTokenGroupCnd = NULL; }
    void ClearTokenGroupCndTree       (){ CTokenGroupCndTree = NULL; }

    // state variables accessors
    dtToken*                GetToken()                  { return CToken; }
    unXmlParseTree*           GetTokenTree()              { return CTokenTree; }
    pkgTokenGroup*          GetTokenGroup()             { return CTokenGroup; }
    unXmlParseTree*           GetTokenGroupTree()         { return CTokenGroupTree; }
    pkgDataType*            GetTokenItem()              { return CTokenItem; }
    unXmlParseTree*           GetTokenItemTree()          { return CTokenItemTree; }
    pkgTokenCondition*      GetTokenGroupCnd()          { return CTokenGroupCnd; }
    unXmlParseTree*           GetTokenGroupCndTree()      { return CTokenGroupCndTree; }

protected:
    pkgDecompiler*          Decompiler;
    dtToken*                CToken;
    unXmlParseTree*           CTokenTree;
    pkgTokenGroup*          CTokenGroup;
    unXmlParseTree*           CTokenGroupTree;
    pkgDataType*            CTokenItem;
    unXmlParseTree*           CTokenItemTree;
    pkgTokenCondition*      CTokenGroupCnd;
    unXmlParseTree*           CTokenGroupCndTree;
};


#endif PXPBYTECODE_H
// ============================================================================