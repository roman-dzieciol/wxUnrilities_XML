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
//  unXmlParseCmd.h
// ============================================================================
#ifndef UNXMLPARSECMD_H
#define UNXMLPARSECMD_H



#include "unXmlParser.h"

class unXmlParseTree;


// ============================================================================
// xpParams
// ============================================================================
template<class TR> 
struct xpParam0
{
    typedef TR R;
};

template<class TR, class TA> 
struct xpParam1
{ 
    typedef TR R;
    typedef TA A;
    TA a;
    xpParam1( TA ta ) : a(ta) {}
};

template<class TR, class TA, class TB> 
struct xpParam2
{ 
    typedef TR R;
    typedef TA A;
    typedef TB B;
    TA a;
    TB b;
    xpParam2( TA ta, TB tb ) : a(ta), b(tb) {}
};


// ============================================================================
// xpObjCreate
// ============================================================================
template<class TO, class OP>
class xpObjCreate : public xpCommand
{
public:
    xpObjCreate( OP objectparent ) : ObjectParent(objectparent), ParentLevel(0)
    {
    }
    xpObjCreate( int parentlevel ) : ObjectParent(NULL), ParentLevel(parentlevel)
    {
    }

    virtual void OnParseNode( unXmlParseTree* pt, unXmlNode* node )
    {
        GetObjectParent(pt)->SetObject( static_cast<TO*>( CreateObject() ) );
    }

    void* CreateObject()
    {
        return alloc_create<TO>();
    }

    unXmlParseTree* GetObjectParent( unXmlParseTree* pt )
    {
        if( ObjectParent )
            return static_cast<unXmlParseTree*>(ObjectParent);

        if( ParentLevel == 0 )
        {
            return pt;
        }
        else
        {
            unXmlParseTree* parent = pt;
            for( int i=0; i<ParentLevel; ++i )
            {
                parent = parent->GetParent();
                if( parent == NULL )
                    throw exFName( wxT("invalid parent") );
            }
            return parent;
        }
    }

protected:
    OP ObjectParent;
    int ParentLevel;
};

template<class TO>
inline xpObjCreate<TO,unXmlParseTree*>* new_xpObjCreate( int parentlevel )
{
    return new xpObjCreate<TO,unXmlParseTree*>(parentlevel);
}

// Creator
template<class TO, class OP>
inline xpObjCreate<TO,OP>* new_xpObjCreate( OP objectparent )
{
    return new xpObjCreate<TO,OP>(objectparent);
}


// ============================================================================
// xpObjClear
// ============================================================================
template<class OP>
class xpObjClear : public xpCommand
{
public:
    xpObjClear( OP objectparent ) : ObjectParent(objectparent), ParentLevel(0)
    {
    }

    xpObjClear( int parentlevel ) : ObjectParent(NULL), ParentLevel(parentlevel)
    {
    }

    virtual void OnReset( unXmlParseTree* pt )
    {
        ClearObject(pt);
    }

    virtual void OnParseNode( unXmlParseTree* pt, unXmlNode* node )
    {
        ClearObject(pt);
    }

    virtual void Destroy( unXmlParseTree* pt )
    {
        ClearObject(pt);
        xpCommand::Destroy(pt);
    }

    void ClearObject( unXmlParseTree* pt )
    {
        GetObjectParent(pt)->SetObject( NULL );
    }

    unXmlParseTree* GetObjectParent( unXmlParseTree* pt )
    {
        if( ObjectParent != NULL )
            return static_cast<unXmlParseTree*>(ObjectParent);

        if( ParentLevel == 0 )
        {
            return pt;
        }
        else
        {
            unXmlParseTree* parent = pt;
            for( int i=0; i<ParentLevel; ++i )
            {
                parent = parent->GetParent();
                if( parent == NULL )
                    throw exFName( wxT("invalid parent") );
            }
            return parent;
        }
    }

protected:
    OP ObjectParent;
    int ParentLevel;
};

// Creator
inline xpObjClear<unXmlParseTree*>* new_xpObjClear( int parentlevel )
{
    return new xpObjClear<unXmlParseTree*>(parentlevel);
}

template<class OP>
inline xpObjClear<OP>* new_xpObjClear( OP objectparent )
{
    return new xpObjClear<OP>(objectparent);
}


// ============================================================================
// xpObjFactory
// ============================================================================
template<class OP, class TO, class TH, class TF, class TP>
class xpObjFactory : public xpCommand
{
public:
    xpObjFactory( OP objectparent, TH host, TF function, TP param ) 
        : ObjectParent(objectparent), Host(host), Function(function), Param(param)
    {
    }

    virtual void OnParseNode( unXmlParseTree* pt, unXmlNode* node )
    {
        GetObjectParent()->SetObject( static_cast<TO>( CreateObject(Param) ) );
    }

    unXmlParseTree* GetObjectParent()
    {
        return static_cast<unXmlParseTree*>(ObjectParent);
    }

    template<class R>
        void* CreateObject( xpParam0<R> param ) { return ((*Host).*Function)(); }

        template<class R, class A>
            void* CreateObject( xpParam1<R,A> param ) { return ((*Host).*Function)(param.a); }

            template<class R, class A, class B>
                void* CreateObject( xpParam2<R,A,B> param ) { return ((*Host).*Function)(param.a,param.b); }

protected:
    OP ObjectParent;
    TH Host;
    TF Function;
    TP Param;
};

// Creator
template<class OP, class TH, class TA, class FR, class FH, class A>
inline xpCommand* new_xpObjFactory( OP objectparent, TH host, FR(FH::*function)(A), TA a )
{
    return new xpObjFactory< OP,FR,TH,FR(FH::*)(A),xpParam1<FR,TA> >
        (objectparent, host, function, xpParam1<FR,TA>(a) );
}


// ============================================================================
// xpObjDestroy
// ============================================================================
template<class TO, class OP>
class xpObjDestroy : public xpCommand
{
public:
    xpObjDestroy( OP objectparent ) : ObjectParent(objectparent)
    {
    }

    virtual void OnReset( unXmlParseTree* pt )
    {
        DestroyObject();
    }

    virtual void OnParseNode( unXmlParseTree* pt, unXmlNode* node )
    {
        DestroyObject();
    }

    virtual void Destroy( unXmlParseTree* pt )
    {
        DestroyObject();
        xpCommand::Destroy(pt);
    }

    void DestroyObject()
    {
        TO* object = static_cast<TO*>( GetObjectParent()->GetObject() );
        if( object != NULL )
            object->Destroy();
        GetObjectParent()->SetObject( NULL );
    }

    unXmlParseTree* GetObjectParent()
    {
        return static_cast<unXmlParseTree*>(ObjectParent);
    }

protected:
    OP ObjectParent;
};

// Creator
template<class TO, class OP>
inline xpObjDestroy<TO,OP>* new_xpObjDestroy( OP objectparent )
{
    return new xpObjDestroy<TO,OP>(objectparent);
}


// ============================================================================
// xpFunc
// ============================================================================
template<class TH, class TF, class TP>
class xpFunc : public xpCommand
{
public:
    xpFunc( TH host, TF function, TP param ) : Host(host), Function(function), Param(param)
    {
    }

    virtual void OnParseNode( unXmlParseTree* pt, unXmlNode* node )
    {
        Call(Param);
    }

    template<class R>
    void Call( xpParam0<R> param ) { ((*Host).*Function)(); }

    template<class R, class A>
    void Call( xpParam1<R,A> param ) { ((*Host).*Function)(param.a); }

    template<class R, class A, class B>
    void Call( xpParam2<R,A,B> param ) { ((*Host).*Function)(param.a,param.b); }

protected:
    TH Host;
    TF Function;
    TP Param;
};

// Creators
template<class TH, class FR, class FH>
inline xpCommand* new_xpFunc0( TH host, FR(FH::*function)() )
{
    return new xpFunc< TH,FR(FH::*)(),xpParam0<FR> >
        (host, function, xpParam0<FR>() );
}

template<class TH, class TA, class FR, class FH, class A>
inline xpCommand* new_xpFunc1( TH host, FR(FH::*function)(A), TA a )
{
    return new xpFunc< TH,FR(FH::*)(A),xpParam1<FR,TA> >
        (host, function, xpParam1<FR,TA>(a) );
}

template<class TH, class TA, class TB, class FR, class FH, class A, class B>
inline xpCommand* new_xpFunc2( TH host, FR(FH::*function)(A,B), TA a, TB b )
{
    return new xpFunc< TH,FR(FH::*)(A,B),xpParam2<FR,TA,TB> >
        (host, function, xpParam2<FR,TA,TB>(a,b) );
}



// ============================================================================
// xpObj
// ============================================================================
template<class T>
class xpObj
{
public:
    xpObj( unXmlParseTree* pt ) : ParseTree(pt)
    {
    }

    operator T*() 
    { 
        return static_cast<T*>(ParseTree->GetObject());  
    }

protected:
    unXmlParseTree* ParseTree;
};




// ============================================================================
// pxpParser
// ============================================================================
template<class TP>
class pxpParser
{
public:
    pxpParser( TP* xp ) : Parser(xp){}

protected:
    TP* Parser;
};


// ============================================================================
// pxpTreeParser
// ============================================================================
template<class TP, class TR>
struct pxpTreeParser : public pxpParser<TP>
{
    pxpTreeParser( TP* xp, TR tree ) : Tree(tree), pxpParser<TP>(xp){}
    unXmlParseTree* GetParseTree() { return static_cast<unXmlParseTree*>(Tree); }
    TR Tree;
};


// ============================================================================
// pxpParserCall
// ============================================================================
template<class TP, class TR, class TF>
class pxpParserCall : public pxpParser<TP>
{
public:
    pxpParserCall( TP* xp, TF function ) : Function(function), pxpParser<TP>(xp){}
    operator TR() { return (Parser->*Function)(); }

protected:
    TF Function;
};

// Creators
template<class TD, class TP, class TR>
inline pxpParserCall<TP,TR,TR(TP::*)()> pxpCall( TD* xp, TR(TP::*function)() )
{
    return pxpParserCall<TP,TR,TR(TP::*)()> (xp,function);
}

#define txpNode             pxpCall(this,&unXmlParser::GetNode)
#define txpThis         pxpCall(this,&unXmlParser::GetParseTree)


// ============================================================================
// pxpNodeName
// ============================================================================
template<class TP, class TR>
struct pxpNodeName : public pxpTreeParser<TP,TR>
{
    pxpNodeName( TP* xp, TR tree ) : pxpTreeParser<TP,TR>(xp,tree){}
    operator wxString() { return xml::GetNodeText(GetParseTree()->GetNode()); }
};

template<class TP, class TR>
inline pxpNodeName<TP,TR> make_pxpNodeName( TP* xp, TR tree )
{
    return pxpNodeName<TP,TR> (xp,tree);
}

#define txpNodeName(Tree) make_pxpNodeName(this,Tree)


// ============================================================================
// pxpNodeData
// ============================================================================
template<class TP, class TR>
struct pxpNodeData : public pxpNodeName<TP,TR>
{
    pxpNodeData( TP* xp, TR tree ) : pxpNodeName<TP,TR>(xp,tree){}
    operator dword() { return unStringToDword(static_cast<wxString>(*this)); }
};

template<class TP, class TR>
inline pxpNodeData<TP,TR> make_pxpNodeData( TP* xp, TR tree )
{
    return pxpNodeData<TP,TR> (xp,tree);
}

#define txpNodeData(Tree) make_pxpNodeData(this,Tree)


// ============================================================================
// pxpTreeObject
// ============================================================================
template<class TO, class TR, class TP>
struct pxpTreeObject : public pxpTreeParser<TP,TR>
{
    pxpTreeObject( TP* xp, TR tree ) : pxpTreeParser<TP,TR>(xp,tree){}
    operator TO*() { return static_cast<TO*>( GetParseTree()->GetObject() ); }
};

template<class TR, class TP>
inline pxpTreeObject<void*,TR,TP> make_pxpTreeObject( TP* xp, TR tree )
{
    return pxpTreeObject<void*,TR,TP> (xp,tree);
}

template<class TO, class TR, class TP>
inline pxpTreeObject<TO,TR,TP> make_pxpTreeObject( TP* xp, TR tree, TO* ptrtype )
{
    return pxpTreeObject<TO,TR,TP> (xp,tree);
}

#define txpObject(TO)           make_pxpTreeObject<TO>(this,txpParseTree)
#define txpTreeObject(TO,TR)    make_pxpTreeObject<TO>(this,TR)



#endif UNXMLPARSECMD_H
// ============================================================================