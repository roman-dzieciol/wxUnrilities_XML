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
//	unXmlParser.cpp
// ============================================================================
#include "prec.h"
#include "unXmlParser.h"



unXmlParseTree::unXmlParseTree( const wxString& name ) 
: Name(name)
, Object(NULL)
{
}

unXmlParseTree::~unXmlParseTree()
{
    //xpCommandEvent( Commands, &xpCommand::Destroy, this );
    //xpCommandEvent( PostCommands, &xpCommand::Destroy, this );

    for( const_iterator it=begin(); it!=end(); ++it )
        delete static_cast<unXmlParseTree*>(*it);
}

void* unXmlParseTree::GetObject() const
{
    return Object;
}

unXmlNode* unXmlParseTree::GetNode() const
{
    return Node;
}

void* unXmlParseTree::GetFirstObject() const
{
    if( Object == NULL )
    {
        if( GetParent() != NULL )
            return GetParent()->GetFirstObject();
    }
    return Object;
}

void* unXmlParseTree::GetParentObject() const
{
    const unXmlParseTree* pt = NULL;
    if( Object == NULL )
    {
        for( pt=GetParent(); pt!=NULL; pt=pt->GetParent() )
        {
            if( pt->Object != NULL )
                break;
        }
    }

    if( pt == NULL )
        pt = this;

    if( pt->GetParent() != NULL )
        return pt->GetParent()->GetFirstObject();

    return NULL;
}

wxString unXmlParseTree::TraceBack()
{
    wxString s;
    for( unXmlParseTree* parent=GetParent(); parent!=NULL; parent=parent->GetParent() )
    {
        s.Prepend( wxString::Format( wxT("<%s>"), parent->GetName().c_str() ) );
    }
    return s;
}


void unXmlParseTree::Reset( unXmlNode* node )
{
    xpCommandEvent( Commands, &xpCommand::OnReset, this );
    xpCommandEvent( PostCommands, &xpCommand::OnReset, this );

    ParseCount.Reset();
}

bool unXmlParseTree::ParseTree( unXmlNode*& node, unXmlParser* parser )
{
    Reset(node);

    while( true )
    {
        if( node == NULL )
        {
            if( !ParseCount.IsValid() )
                throw exception( wxString::Format( wxT("Expected: %s<%s>, got: nothing"), TraceBack().c_str(), Name.c_str() ).c_str() );
            //wxLogMessage( wxT("[?][%s] %d"), Name.c_str(), ParseCount );
            return true;
        }
        else if( !xml::IsSameName(node,Name) )
        {
            if( !ParseCount.IsValid() )
                throw exception( wxString::Format( wxT("Expected: %s<%s>, got: %s"), TraceBack().c_str(), Name.c_str(), xml::TraceBack(node).c_str() ).c_str() );

            //wxLogMessage( wxT("[!%s][%s] %d"), node->GetName().c_str(), Name.c_str(), ParseCount );
            return true;
        }
        else
        {
            //wxLogMessage( wxT("[%s][%s] %d"), node->GetName().c_str(), Name.c_str(), ParseCount );

            // parse element
            ParseCount.Increase();
            Node = node;
            parser->SetNode(node);
            parser->SetParseTree(this);
            xpCommandEvent( Commands, &xpCommand::OnParseNode, this, node );
            //parser->ClearParseTree();
            //parser->ClearNode();
            //Node = NULL;

            // Parse children
            unXmlNode* next = node->GetChildren();
            for( iterator it=begin(); it!=end(); ++it )
            {
                // parse only elements
                while( next!=NULL && next->GetType()!=wxXML_ELEMENT_NODE )
                    next = next->GetNext();

                unXmlParseTree* pt = static_cast<unXmlParseTree*>(*it);
                pt->ParseTree(next,parser);
            }

            // Unknown element!
            if( next != NULL && next->GetType() == wxXML_ELEMENT_NODE )
                throw exception( wxString::Format( wxT("Unknown element: %s"), xml::TraceBack(node).c_str() ).c_str() );

            //Node = node;
            parser->SetNode(node);
            parser->SetParseTree(this);
            xpCommandEvent( PostCommands, &xpCommand::OnParseNode, this, node );
            //parser->ClearParseTree();
            //parser->ClearNode();
            //Node = NULL;

            node = node->GetNext();
            if( !ParseCount.IsNextValid() )
                return true;
        }
    }

    return true;
}




void unXmlParseTree::AddTo( unXmlParseTree* parent, const xpParseCount& parsecount )
{
    parent->AddChild(this);
    SetParseCount( parsecount );
}

void unXmlParseTree::AddCommand( xpCommand* command ) 
{ 
    Commands.push_back(command);
}

void unXmlParseTree::AddPostCommand( xpCommand* command ) 
{ 
    PostCommands.push_back(command);
}


void unXmlParseTree::SetObject( void* object ) 
{ 
    Object = object;
}


unXmlParseTree::unXmlParseTree( const unXmlParseTree& pt )
: Name(pt.Name)
, ParseCount(pt.ParseCount)
, Object(NULL)
, Commands(pt.Commands)
, PostCommands(pt.PostCommands)
, Node(NULL)
{
    for( const_iterator it=pt.begin(); it!=pt.end(); ++it )
    {
        unXmlParseTree* tree = static_cast<unXmlParseTree*>(*it);
        AddChild( new unXmlParseTree(*tree) );
    }
}

void unXmlParseTree::AddClonedChild( unXmlParseTree* child, const xpParseCount& parsecount )
{
    unXmlParseTree* tree = AddChild( new unXmlParseTree(*child) );
    tree->SetParseCount( parsecount );
}


unXmlParseTree* unXmlParseTree::AddChild( unXmlParseTree* child ) 
{ 
    return unPtrTree<unXmlParseTree>::AddChild(child);
}

unXmlParseTree* unXmlParseTree::AddChild( const unXmlParseTree& child, const xpParseCount& parsecount )
{
    unXmlParseTree* tree = AddChild( new unXmlParseTree(child) );
    tree->SetParseCount( parsecount );
    return tree;
}

// ============================================================================