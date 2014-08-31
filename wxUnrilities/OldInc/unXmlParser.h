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
//  unXmlParser.h
// ============================================================================
#ifndef UNXMLPARSER_H
#define UNXMLPARSER_H



#include "unXmlDocument.h"
#include "unTree.h"

class unXmlParseTree;
class unXmlParser;


// ============================================================================
// xpParseCount
// ============================================================================
class xpParseCount
{
public:
    xpParseCount( const dword mincount=0, const dword maxcount=-1 ) 
    :   ParseCount(0)
    ,   MinCount(mincount)
    ,   MaxCount(maxcount)
    {
    }

    xpParseCount operator=( const xpParseCount& right ) 
    {
        return xpParseCount(right);
    }

public:
    void Reset() { ParseCount = 0; }
    void Increase() { ++ParseCount; }
    bool IsValid() { return ParseCount >= MinCount && ParseCount <= MaxCount; }
    bool IsNextValid() { return ParseCount < MaxCount; }

protected:
    dword ParseCount;
    const dword MinCount;
    const dword MaxCount;
};


// ============================================================================
// xpParseLimits
// ============================================================================
namespace xpParseLimits
{
    static xpParseCount one(1,1);       // 1
    static xpParseCount minone(1,-1);   // 1-inf
    static xpParseCount maxone(0,1);    // 0-1
    static xpParseCount any(0,-1);      // 0-inf
}


// ============================================================================
// xpCommand
// ============================================================================
class xpCommand
{
public:
    virtual ~xpCommand()
    {
    }

    virtual void OnReset( unXmlParseTree* pt )
    {
    }

    virtual void OnParseNode( unXmlParseTree* pt, unXmlNode* node )
    {
    }

    virtual void Destroy( unXmlParseTree* pt )
    {
        //delete this;
    }

    virtual xpCommand* Clone()
    {
        return NULL;
    }
};


// ============================================================================
// xpCommands
// ============================================================================
typedef vector<xpCommand*> xpCommands;

template<class A, class B>
inline void xpCommandEvent( xpCommands& cmd, void(xpCommand::*fn)(A,B), A a, B b )
{
    for( xpCommands::const_iterator it=cmd.begin(); it!=cmd.end(); ++it )
        MEMBER_PTR(static_cast<xpCommand*>(*it),fn)( a, b );
}

template<class A>
inline void xpCommandEvent( xpCommands& cmd, void(xpCommand::*fn)(A), A a )
{
    for( xpCommands::const_iterator it=cmd.begin(); it!=cmd.end(); ++it )
        MEMBER_PTR(static_cast<xpCommand*>(*it),fn)( a );
}


// ============================================================================
// unXmlParseTree
// ============================================================================
class unXmlParseTree : public unPtrTree<unXmlParseTree>
{
public:
    // Constructors
    unXmlParseTree( const wxString& name );
    unXmlParseTree( const unXmlParseTree& pt );
    virtual ~unXmlParseTree();

    // Accessors
    wxString GetName() const { return Name; }
    void* GetObject() const;
    void* GetFirstObject() const;
    void* GetParentObject() const;
    unXmlNode* GetNode() const;

    // Mutators
    void SetParseCount( const xpParseCount& parsecount ) { ParseCount = parsecount; }
    void SetObject( void* object );
    void AddCommand( xpCommand* command );
    void AddPostCommand( xpCommand* command );
    void AddTo( unXmlParseTree* parent, const xpParseCount& parsecount );
    void AddClonedChild( unXmlParseTree* child, const xpParseCount& parsecount );
    unXmlParseTree* AddChild( unXmlParseTree* child );
    unXmlParseTree* AddChild( const unXmlParseTree& child, const xpParseCount& parsecount );

    // XmlDocument parser
    virtual bool ParseTree( unXmlNode*& node, unXmlParser* parser );
    virtual void Reset( unXmlNode* node );

    // Debug
    wxString TraceBack();

protected:
    wxString Name;
    xpParseCount ParseCount;
    void* Object;
    xpCommands Commands;
    xpCommands PostCommands;
    unXmlNode* Node;
};


// ============================================================================
// unXmlParser - macros
// ============================================================================
#define UXP_VARIABLE( Type, Name ) \
public: \
    void    Set##Name   ( Type* p ) { C##Name = p; } \
    void    Clear##Name ()          { C##Name = NULL; } \
    Type*   Get##Name   ()          { return C##Name; } \
protected:  \
    Type*   C##Name; \

#define UXP_VARIABLE_POSTFIX( Type, Name, Postfix ) \
public: \
    void    Set##Name##Postfix      ( Type* p ) { C##Name##Postfix = p; } \
    void    Clear##Name##Postfix    ()          { C##Name##Postfix = NULL; } \
    Type*   Get##Name##Postfix      ()          { return C##Name##Postfix; } \
protected:  \
    Type*   C##Name##Postfix; \

#define UXP_VPAIR( Type, Name ) \
    UXP_VARIABLE_POSTFIX( unXmlParseTree, Name, Tree ); \
    UXP_VARIABLE( Type, Name );


// ============================================================================
// unXmlParser - common client code macros
// ============================================================================

// Sets state variable.
#define UXS_SET(Name,Object)        new_xpFunc1( this, &UXP_CLASS::Set##Name, Object )

// Returns state variable.
#define UXS_STATE(Name)             pxpCall( this, &UXP_CLASS::Get##Name )

// Clears state variable.
#define UXS_CLEAR(Name)             new_xpFunc0( this, &UXP_CLASS::Clear##Name )

// Returns object from current pkParseTree.
#define UXO_THIS(Name)              make_pxpTreeObject( this, txpThis, C##Name )    

// Creates object of given type in given pkParseTree.
#define UXO_CREATE(Type,Tree)       new_xpObjCreate<Type>(Tree)

// Clears object pointer in given pkgParseTree.
#define UXO_CLEAR(Tree)             new_xpObjClear(Tree)


// ============================================================================
// unXmlParser - holds parse tree and parse state variables
// ============================================================================
class unXmlParser
{
public:
    // Constructors
    unXmlParser() 
        : ParseTree(NULL)
        , CNode(NULL)
        , CParseTree(NULL)
    {
    }

    virtual ~unXmlParser()
    {
        delete ParseTree;
    }

    virtual void ParseFile( wxFileName filename )
    {
        if( !filename.FileExists() )
            throw exception( wxString::Format( wxT("Can't open file %s"), filename.GetFullPath().c_str() ).c_str() );

        unXmlDocument doc;
        if( !doc.Load( filename.GetFullPath() ) )
            throw exception( wxString::Format( wxT("File %s contains invalid data."), filename.GetFullPath().c_str() ).c_str() );
        
        ParseDocument(doc);
    }

    virtual void ParseDocument( const unXmlDocument& doc )
    {
        unXmlNode* node = doc.GetRoot();
        if( ParseTree && node )
            ParseTree->ParseTree( node, this );
    }

    // state variables
    UXP_VARIABLE( unXmlNode*, Node );
    UXP_VARIABLE( unXmlParseTree*, ParseTree );

protected:
    unXmlParseTree*     ParseTree;
};




#endif UNXMLPARSER_H
// ============================================================================