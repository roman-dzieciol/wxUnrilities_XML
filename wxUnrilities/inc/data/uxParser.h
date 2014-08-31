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
//  uxParser.h
// ============================================================================
#ifndef UXPARSER_H
#define UXPARSER_H

#include "unXmlDocument.h"
#include "uxCommand.h"

class uxItem;
class uxElement;


// ============================================================================
// uxItemArray
// ============================================================================
typedef vector<uxItem*> uxItemArray;


// ============================================================================
// uxParser - macros
// ============================================================================
#define UXP_VARIABLE( TYPE, NAME ) \
public: \
    void    Set##NAME   ( TYPE p ) { StateVar_##NAME = p; } \
    TYPE    Get##NAME   ()          { return StateVar_##NAME; } \
protected:  \
    TYPE    StateVar_##NAME;

#define UXP_VARS( Type, Name ) \
    UXP_VARIABLE( uxElement*, Name##Tree ); \
    UXP_VARIABLE( Type*, Name );


// ============================================================================
// uxParser
// ============================================================================
class uxParser
{
public:
    // Special
    uxParser() ;
    virtual ~uxParser();

    // Interface
    uxItem* Register( uxItem* item ) { Items.push_back(item); return item; }
    virtual void ParseFile( wxFileName filename );

    // State variables
    UXP_VARIABLE( unXmlProperty*, Prop );
    UXP_VARIABLE( unXmlNode*, Node );
    UXP_VARIABLE( uxElement*, Tree );

protected:
    uxItem* Root;
    uxItemArray Items;
};


// ============================================================================
// Member function
// ============================================================================

// Object, Calls a member function and returns result.
#define UXC_FUNC_0(OBJ,FUNC)                make_tcTypeCall_0(OBJ,FUNC)
#define UXC_FUNC_1(OBJ,FUNC,A)              make_tcTypeCall_1(OBJ,FUNC,A)
#define UXC_FUNC_2(OBJ,FUNC,A,B)            make_tcTypeCall_2(OBJ,FUNC,A,B)
#define UXC_FUNC_3(OBJ,FUNC,A,B,C)          make_tcTypeCall_3(OBJ,FUNC,A,B,C)

// Command, Calls a member function.
#define UXC_PROC_0(OBJ,FUNC)                new_xcMutator_0(OBJ,FUNC)
#define UXC_PROC_1(OBJ,FUNC,A)              new_xcMutator_1(OBJ,FUNC,A)
#define UXC_PROC_2(OBJ,FUNC,A,B)            new_xcMutator_2(OBJ,FUNC,A,B)
#define UXC_PROC_3(OBJ,FUNC,A,B,C)          new_xcMutator_3(OBJ,FUNC,A,B,C)


// ============================================================================
// XML node
// ============================================================================

// Object, Returns xml node text as string or dword.
#define UXN_NAME(NODE)                      make_tcNodeName(NODE)
#define UXN_DATA(NODE)                      make_tcNodeData(NODE)


// ============================================================================
// XML attribute
// ============================================================================

// Object, Returns xml property text as string or dword.
#define UXP_NAME(PROP)                      make_tcPropName(PROP)
#define UXP_DATA(PROP)                      make_tcPropData(PROP)


// ============================================================================
// State variables
// ============================================================================

// Object, Calls a member function on named state variable object and returns result.
#define UXS_FUNC_0(NAME,FUNC)               UXC_FUNC_0(UXS_GET(NAME),FUNC)
#define UXS_FUNC_1(NAME,FUNC,A)             UXC_FUNC_1(UXS_GET(NAME),FUNC,A)
#define UXS_FUNC_2(NAME,FUNC,A,B)           UXC_FUNC_2(UXS_GET(NAME),FUNC,A,B)
#define UXS_FUNC_3(NAME,FUNC,A,B,C)         UXC_FUNC_3(UXS_GET(NAME),FUNC,A,B,C)

// Command, Calls a member function on named state variable object
#define UXS_PROC_0(NAME,FUNC)               UXC_PROC_0(UXS_GET(NAME),FUNC)
#define UXS_PROC_1(NAME,FUNC,A)             UXC_PROC_1(UXS_GET(NAME),FUNC,A)
#define UXS_PROC_2(NAME,FUNC,A,B)           UXC_PROC_2(UXS_GET(NAME),FUNC,A,B)
#define UXS_PROC_3(NAME,FUNC,A,B,C)         UXC_PROC_3(UXS_GET(NAME),FUNC,A,B,C)

// Command, Sets state variable.
#define UXS_SET(NAME,OBJ)                   UXC_PROC_1(this,&UXP_CLASS::Set##NAME,OBJ)

// Object, Returns state variable.
#define UXS_GET(NAME)                       UXC_FUNC_0(this,&UXP_CLASS::Get##NAME)

// Object, Returns current uxElement.
#define UXS_TREE                            UXS_GET(Tree)   

// Object, Returns current unXmlNode.   
#define UXS_NODE                            UXS_GET(Node)  

// Object, Returns current unXmlProperty.   
#define UXS_PROP                            UXS_GET(Prop)     

// Object, Returns current unXmlNode as wxString or dword
#define UXS_NODENAME                        UXN_NAME(UXS_NODE)  
#define UXS_NODEDATA                        UXN_DATA(UXS_NODE)     

// Object, Returns current unXmlProperty as wxString or dword
#define UXS_PROPNAME                        UXP_NAME(UXS_PROP)  
#define UXS_PROPDATA                        UXP_DATA(UXS_PROP)     


// ============================================================================
// uxElement client data
// ============================================================================

// Object, Calls a member function on typecasted object stored in uxElement and returns result
#define UXO_FUNC_0(TREE,TYPE,FUNC)          UXC_FUNC_0(UXO_GETOBJ(TYPE,TREE),FUNC)
#define UXO_FUNC_1(TREE,TYPE,FUNC,A)        UXC_FUNC_1(UXO_GETOBJ(TYPE,TREE),FUNC,A)
#define UXO_FUNC_2(TREE,TYPE,FUNC,A,B)      UXC_FUNC_2(UXO_GETOBJ(TYPE,TREE),FUNC,A,B)
#define UXO_FUNC_3(TREE,TYPE,FUNC,A,B,C)    UXC_FUNC_3(UXO_GETOBJ(TYPE,TREE),FUNC,A,B,C)

// Command, Calls a member function on typecasted object stored in uxElement
#define UXO_PROC_0(TREE,TYPE,FUNC)          UXC_PROC_0(UXO_GETOBJ(TYPE,TREE),FUNC)
#define UXO_PROC_1(TREE,TYPE,FUNC,A)        UXC_PROC_1(UXO_GETOBJ(TYPE,TREE),FUNC,A)
#define UXO_PROC_2(TREE,TYPE,FUNC,A,B)      UXC_PROC_2(UXO_GETOBJ(TYPE,TREE),FUNC,A,B)
#define UXO_PROC_3(TREE,TYPE,FUNC,A,B,C)    UXC_PROC_3(UXO_GETOBJ(TYPE,TREE),FUNC,A,B,C)

// Command, Stores object in uxElement
#define UXO_SET(TREE,OBJ)                   UXC_PROC_1(TREE,&uxElement::SetObject,OBJ)

// Object, Returns typecasted object stored in uxElement
#define UXO_GETOBJ(TYPE,TREE)               make_tcCastVoid<TYPE>(UXC_FUNC_0(TREE,&uxElement::GetObject))

// Object, Returns typecasted object stored in named uxElement
#define UXO_GET(TYPE,NAME)                  UXO_GETOBJ(TYPE,UXS_GET(NAME))

// Object, Returns typecasted object stored in current uxElement
#define UXO_THIS(TYPE)                      UXO_GETOBJ(TYPE,UXS_TREE)


// ============================================================================
// uxItem declarations
// ============================================================================

#define UXR_ITEM(TYPENAME, NODENAME)        uxItem* TYPENAME = Register(new_uxItem(NODENAME))
#define UXR_PROP(TYPENAME, NODENAME)        uxItem* TYPENAME = Register(new_uxProperty(NODENAME))
#define UXR_CHOICE(TYPENAME, NODENAME)      uxItem* TYPENAME = Register(new_uxChoice(NODENAME))


// ============================================================================
// Debug
// ============================================================================

#define UXD_BREAK                           new_xcBreak()
#define UXD_DEBUG(DESC)                     new_xcDebug( wxT(DESC) )
#define UXD_DEBUGOBJ(DESC,OBJ)              new_xcDebugObj( wxT(DESC), OBJ )



// ============================================================================
// uxExecCommands
// ============================================================================
inline void uxExecCommands( const uxCommands& cmd, unXmlNode* node, uxParser* parser, uxElement* element, unXmlProperty* prop )
{
    if( !cmd.empty() )
    {
        parser->SetProp(prop);
        parser->SetTree(element);
        parser->SetNode(node);
        for( uxCommands::const_iterator it=cmd.begin(); it!=cmd.end(); ++it )
            (*it)->Execute();
    }
}



#endif UXPARSER_H
// ============================================================================