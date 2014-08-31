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
//  uxElement.h
// ============================================================================
#ifndef UXELEMENT_H
#define UXELEMENT_H

#include "unXmlDocument.h"
#include "uxLimit.h"
#include "uxItem.h"
#include "uxDebug.h"

class uxParser;
class uxItem;


// ============================================================================
// uxElement
// ============================================================================
class uxElement
{
public:
    // Allocator
    STATIC_ALLOCATOR(uxElement);

    // Special
    uxElement( uxItem& item, const uxLimit& limit );
    virtual ~uxElement();
    virtual uxElement* Clone() const { return alloc_copy(*this); }

    // Accessors
    uxElement* GetParent() const;
    void* GetObject() const;
    uxItem& GetItem() const;

    // Mutators
    void SetParent( uxElement* ptr );
    void SetObject( void* ptr );

    // Info
    virtual wxString TraceBack( bool bparentonly=false );
    virtual wxString GetLimitInfo();
    virtual wxString GetItemInfo();
    virtual wxString GetItemName();

    // Parsing
    void CheckLimit();
    virtual unXmlNode* Parse( unXmlNode* node, uxParser* parser, uxElement* parent );

    // Creator
    static uxElement* Create( uxItem& item, const uxLimit& limit )
    {
        return new uxElement(item,limit);
    }

protected:
    uxItem& Item;           // Master item
    uxLimit Limit;          // Parse count & range

    uxElement* Parent;      // Parent element
    void* Object;           // Client data
};

// ============================================================================
// uxElement
// ============================================================================
inline uxElement::uxElement( uxItem& item, const uxLimit& limit ) 
:   Item(item)
,   Limit(limit)
,   Object(NULL)
{
}

inline uxElement::~uxElement()
{
}


// ============================================================================
// uxElement - inline
// ============================================================================
inline uxElement* uxElement::GetParent() const 
{ 
    return Parent; 
}

inline void* uxElement::GetObject() const 
{ 
    //uxDebugAccessor(Object); 
    return Object; 
}

inline uxItem& uxElement::GetItem() const 
{ 
    return Item; 
}

inline void uxElement::SetParent( uxElement* ptr ) 
{ 
    //uxDebugMutator(Parent,ptr); 
    Parent = ptr; 
}

inline void uxElement::SetObject( void* ptr ) 
{ 
    //uxDebugMutator(Object,ptr); 
    Object = ptr; 
}

inline void uxElement::CheckLimit()
{
    if( !Limit.IsValid() )
        throw exString( wxString::Format( wxT("Invalid element count: %s"), TraceBack().c_str() ) );
}


// ============================================================================
// uxElement - Creator
// ============================================================================
inline uxItem* new_uxItem( const wxString& name )
{
    return new uxItem(name, &uxElement::Create);
}


#endif UXELEMENT_H
// ============================================================================