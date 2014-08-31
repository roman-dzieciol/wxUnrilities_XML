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
//	pxpByteCode.cpp
// ============================================================================
#include "prec.h"
#include "pxpByteCode.h"
#include "unXmlParseCmd.h"
#include "pkgDataTypeFactory.h"
#include "pkgDataTypes.h"
#include "pkgDecompiler.h"
#include "pkgCondition.h"

namespace pl = xpParseLimits;


#define txpToken                        pxpCall(this,&pxpByteCode::GetToken)
#define txpTokenTree                    pxpCall(this,&pxpByteCode::GetTokenTree)
#define txpTokenGroup                   pxpCall(this,&pxpByteCode::GetTokenGroup)
#define txpTokenGroupTree               pxpCall(this,&pxpByteCode::GetTokenGroupTree)
#define txpTokenItem                    pxpCall(this,&pxpByteCode::GetTokenItem)
#define txpTokenItemTree                pxpCall(this,&pxpByteCode::GetTokenItemTree)
#define txpTokenGroupCnd                pxpCall(this,&pxpByteCode::GetTokenGroupCnd)
#define txpTokenGroupCndTree            pxpCall(this,&pxpByteCode::GetTokenGroupCndTree)

#define txpTokenObject                  txpTreeObject(dtToken,txpParseTree)
#define txpTokenTreeObject              txpTreeObject(dtToken,txpTokenTree)
#define txpTokenItemObject              txpTreeObject(pkgDataType,txpParseTree)
#define txpTokenItemTreeObject          txpTreeObject(pkgDataType,txpTokenItemTree)
#define txpTokenGroupObject             txpTreeObject(pkgTokenGroup,txpParseTree)
#define txpTokenGroupTreeObject         txpTreeObject(pkgTokenGroup,txpTokenGroupTree)
#define txpTokenGroupCndObject          txpTreeObject(pkgTokenCondition,txpParseTree)
#define txpTokenGroupCndTreeObject      txpTreeObject(pkgTokenCondition,txpTokenGroupCndTree)


// ============================================================================
// xcToken
// ============================================================================
class xcToken : public unPtrTree<xcToken>
{
public:
    // Constructors
    xcToken()
    {
    }

    xcToken( const wxString& name ) : Name(name)
    {
    }

    virtual ~xcToken()
    {
    }

    // Accessors
    wxString GetName() { return Name; }

    // Mutators
    void SetName( const wxString& name  ) { Name = name; }

    // Common
    virtual bool Eval()
    {
        return true;
    }

    virtual bool IsSameAs( xcToken* token )
    {
        return false;
    }

protected:
    wxString Name;
};

// ============================================================================
// xcTokenData
// ============================================================================
template<class T>
class xcTokenData : public xcToken
{
public:
    // Constructors
    xcTokenData()
    {
    }

    xcTokenData( const wxString& name ) : xcToken(name)
    {
    }

    // Common
    virtual bool Eval()
    {
        return true;
    }

protected:
    T Data;
};



// ============================================================================
// pxpByteCode
// ============================================================================
pxpByteCode::pxpByteCode( pkgDecompiler* decompiler )
:   Decompiler(decompiler)
,   CToken(NULL)
,   CTokenTree(NULL)
,   CTokenGroup(NULL)
,   CTokenGroupTree(NULL)
,   CTokenItem(NULL)
,   CTokenItemTree(NULL)
,   CTokenGroupCnd(NULL)
,   CTokenGroupCndTree(NULL)
,   unXmlParser()
{
    // temp tree nodes
    unXmlParseTree bytecode   ( wxT("bytecode") );
    unXmlParseTree bgroup     ( wxT("group") );
    unXmlParseTree gname      ( wxT("name") );
    unXmlParseTree gmemo      ( wxT("memo") );

    unXmlParseTree gtoken     ( wxT("token") );
    unXmlParseTree tcode      ( wxT("code") );
    unXmlParseTree tname      ( wxT("name") );
    unXmlParseTree tdesc      ( wxT("desc") );
    unXmlParseTree tdata      ( wxT("data") );

    unXmlParseTree titem      ( wxT("item") );
    unXmlParseTree itype      ( wxT("type") );
    unXmlParseTree iname      ( wxT("name") );

    unXmlParseTree ttext      ( wxT("text") );

    unXmlParseTree gcond      ( wxT("gcond") );
    unXmlParseTree cif        ( wxT("if") );
    unXmlParseTree ceq        ( wxT("eq") );
    unXmlParseTree cthen      ( wxT("then") );
    unXmlParseTree cleft      ( wxT("left") );
    unXmlParseTree cright     ( wxT("right") );

    unXmlParseTree ctstream   ( wxT("tstream") );
    unXmlParseTree cnum       ( wxT("num") );

    unXmlParseTree nfunc      ( wxT("nativefunctions") );
    unXmlParseTree ffirst     ( wxT("first") );
    unXmlParseTree fextended  ( wxT("extended") );

    // token group - pre
    bgroup.AddCommand( new_xpObjCreate<pkgTokenGroup>(txpParseTree) );
    bgroup.AddCommand( new_xpFunc1( this, &pxpByteCode::SetTokenGroup, txpTokenGroupObject ) );
    bgroup.AddCommand( new_xpFunc1( this, &pxpByteCode::SetTokenGroupTree, txpParseTree ) );
        gname.AddCommand( new_xpFunc1( txpTokenGroup, &pkgTokenGroup::SetName, txpNodeName(txpParseTree) ) );
        gname.AddPostCommand( new_xpFunc1( Decompiler, &pkgDecompiler::AddTokenGroup, txpTokenGroup ) );

    // token group - post
    bgroup.AddPostCommand( new_xpObjClear(txpParseTree) );
    bgroup.AddPostCommand( new_xpFunc0( this, &pxpByteCode::ClearTokenGroup ) );
    bgroup.AddPostCommand( new_xpFunc0( this, &pxpByteCode::ClearTokenGroupTree ) );

    // gcond = pre
    gcond.AddCommand( new_xpObjCreate<pkgTokenCondition>(txpParseTree) );
    gcond.AddCommand( new_xpFunc1( this, &pxpByteCode::SetTokenGroupCnd, txpTokenGroupCndObject ) );
    gcond.AddCommand( new_xpFunc1( this, &pxpByteCode::SetTokenGroupCndTree, txpParseTree ) );

    // gcond = post
    gcond.AddPostCommand( new_xpObjClear(txpParseTree) );
    gcond.AddPostCommand( new_xpFunc0( this, &pxpByteCode::ClearTokenGroupCnd ) );
    gcond.AddPostCommand( new_xpFunc0( this, &pxpByteCode::ClearTokenGroupCndTree ) );

    // token - pre
    gtoken.AddCommand( new_xpObjCreate<dtToken>(txpParseTree) );
    gtoken.AddCommand( new_xpFunc1( this, &pxpByteCode::SetToken, txpTokenObject ) );
    gtoken.AddCommand( new_xpFunc1( this, &pxpByteCode::SetTokenTree, txpParseTree ) );
        tcode.AddCommand( new_xpFunc1( txpTokenTreeObject, &dtToken::SetTokenData, txpNodeData(txpParseTree) ) );
        tname.AddCommand( new_xpFunc1( txpTokenTreeObject, &dtToken::SetTokenName, txpNodeName(txpParseTree) ) );
        tdesc.AddCommand( new_xpFunc1( txpTokenTreeObject, &dtToken::SetDesc, txpNodeName(txpParseTree) ) );
        tdesc.AddCommand( new_xpFunc1( txpTokenGroup, &pkgTokenGroup::AddToken, txpToken ) );
        tdesc.AddCommand( new_xpFunc2( Decompiler, &pkgDecompiler::AddToken, txpToken, txpTokenGroupCnd ) );

    // token - post
    gtoken.AddPostCommand( new_xpFunc0( txpToken, &dtToken::DumpInfo ) );
    gtoken.AddPostCommand( new_xpObjClear(txpParseTree) );
    gtoken.AddPostCommand( new_xpFunc0( this, &pxpByteCode::ClearToken ) );
    gtoken.AddPostCommand( new_xpFunc0( this, &pxpByteCode::ClearTokenTree ) );

    // titem - pre
    titem.AddCommand( new_xpFunc1( this, &pxpByteCode::SetTokenItemTree, txpParseTree ) );
        itype.AddCommand( new_xpObjFactory( txpTokenItemTree, &GDataTypeFactory, &pkgDataTypeFactory::Create, txpNodeName(txpParseTree) ) );
        itype.AddCommand( new_xpFunc1( this, &pxpByteCode::SetTokenItem, txpTokenItemTreeObject ) );
        iname.AddCommand( new_xpFunc1( txpTokenItem, &pkgDataType::SetDesc, txpNodeName(txpParseTree) ) );

    // titem - post
    titem.AddPostCommand( new_xpFunc1( txpToken, &dtToken::AddItem, txpTokenItem ) );
    titem.AddPostCommand( new_xpObjClear(txpParseTree) );
    titem.AddPostCommand( new_xpFunc0( this, &pxpByteCode::ClearTokenItem ) );
    titem.AddPostCommand( new_xpFunc0( this, &pxpByteCode::ClearTokenItemTree ) );
    
    ffirst.AddCommand( new_xpFunc1( Decompiler, &pkgDecompiler::SetFunctionIdFirst, txpNodeData(txpParseTree) ) );
    fextended.AddCommand( new_xpFunc1( Decompiler, &pkgDecompiler::SetFunctionIdExtended, txpNodeData(txpParseTree) ) );


    // construct tree starting from leaves
    // ie: node on right side *cannot* appear anywhere below on left side

    // token
    gtoken.AddChild( tcode, pl::one );
    gtoken.AddChild( tname, pl::one );
    gtoken.AddChild( tdesc, pl::one );
            titem.AddChild( itype, pl::one );
            titem.AddChild( iname, pl::one );
        tdata.AddChild( titem, pl::minone );
        tdata.AddChild( ttext, pl::maxone );
    gtoken.AddChild( tdata, pl::maxone );

    // if
            cleft.AddChild( ctstream, pl::maxone );
            cleft.AddChild( cnum, pl::maxone );
            cright.AddChild( ctstream, pl::maxone );
            cright.AddChild( cnum, pl::maxone );
        ceq.AddChild( cleft, pl::one );
        ceq.AddChild( cright, pl::one );
    cif.AddChild( ceq, pl::one );

    // then
    cthen.AddChild( gtoken, pl::any );

    // group
    bgroup.AddChild( gname, pl::one );
    bgroup.AddChild( gmemo, pl::any );
        gcond.AddChild( cif, pl::one );
        gcond.AddChild( cthen, pl::one );
    bgroup.AddChild( gcond, pl::maxone );
    bgroup.AddChild( gtoken, pl::any );

    // native functions
    nfunc.AddChild( fextended, pl::one );
    nfunc.AddChild( ffirst, pl::one );

    // bytecode
    bytecode.AddChild( bgroup, pl::minone );
    bytecode.AddChild( nfunc, pl::maxone );

    ParseTree = new unXmlParseTree( bytecode );
}


// ============================================================================