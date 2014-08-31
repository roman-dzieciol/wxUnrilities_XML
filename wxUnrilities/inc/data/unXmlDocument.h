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
//  unXmlDocument.h
// ============================================================================
#ifndef UNXMLDOCUMENT_H
#define UNXMLDOCUMENT_H


// ============================================================================
// unXmlNode
// ============================================================================
typedef wxXmlNode unXmlNode;


// ============================================================================
// unXmlProperty
// ============================================================================
typedef wxXmlProperty unXmlProperty;


// ============================================================================
// wxXmlNode
// ============================================================================
namespace xml
{
    inline bool FindNodeNamed( wxXmlNode*& node, const wxString& name )
    {
        for( ; node!=NULL; node=node->GetNext() )
            if( node->GetName().IsSameAs( name ) )
                return true;
        return false;
    }

    inline wxString GetNodeText( wxXmlNode* node )
    {
        node = node->GetChildren();
        if( node )   return node->GetContent();
        else         return wxEmptyString;
    }

    inline wxString GetNodeName( wxXmlNode* node )
    {
        if( node )   return node->GetName();
        else         return wxEmptyString;
    }

    inline bool IsSameText( wxXmlNode* node, const wxString& text )
    {
        if( node )  return text.IsSameAs( GetNodeText(node) );
        else        return false;
    }

    inline bool IsSameName( wxXmlNode* node, const wxString& text )
    {
        if( node )  return text.IsSameAs( node->GetName() );
        else        return false;
    }

    inline wxXmlNode* GetNodeNamed( wxXmlNode* node, const wxString& name )
    {
        if( node != NULL && name.IsSameAs( GetNodeName(node) ) )   
            return node;
        else    return NULL;
    }


    inline bool IsElementNode( unXmlNode* node )
    {
        return node != NULL && node->GetType() == wxXML_ELEMENT_NODE;
    }

    inline dword GetElementCount( unXmlNode* parent,  unXmlNode* marker=NULL )
    {
        dword count = 0;
        for( unXmlNode* it=parent->GetChildren(); it!=NULL; it=it->GetNext() )
        {
            if( it == marker )
                break;

            if( IsElementNode(it) && it->GetName() == marker->GetName() )
                ++count;
        }
        return count;
    }

    inline wxString TraceBack( unXmlNode* node )
    {
        wxString s;
        if( IsElementNode(node) )
        {
            for( unXmlNode* parent=node->GetParent(); parent!=NULL; parent=parent->GetParent() )
            {
                if( IsElementNode(parent) )
                {
                    dword count = GetElementCount(parent,node)+1;
                    s.Prepend( wxString::Format( wxT("<%s>(%d)"), node->GetName().c_str(), count ) );
                    node = parent;
                }
            }
            if( node->GetParent() == NULL )
                s.Prepend( wxString::Format( wxT("<%s>"), node->GetName().c_str() ) );
        }
        return s;
    }



    inline wxString GetPropName( wxXmlProperty* prop )
    {
        if( prop )   return prop->GetName();
        else         return wxEmptyString;
    }

    inline wxString GetPropText( wxXmlProperty* prop )
    {
        if( prop )   return prop->GetValue();
        else         return wxEmptyString;
    }

    inline bool IsSameName( wxXmlProperty* prop, const wxString& text )
    {
        if( prop )  return text.IsSameAs( prop->GetName() );
        else        return false;
    }

    inline wxString TraceBack( unXmlNode* node, wxXmlProperty* pnode )
    {
        wxString s;
        if( pnode )
        {
            s = TraceBack(node);
            s.Append( wxString::Format( wxT("@%s"), pnode->GetName().c_str() ) );
        }
        return s;
    }
}


// ============================================================================
// unXmlDocument
// ============================================================================
class unXmlDocument : public wxXmlDocument
{
public:

};


#endif UNXMLDOCUMENT_H
// ============================================================================