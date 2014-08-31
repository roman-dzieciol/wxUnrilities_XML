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
//  uxContent.h
// ============================================================================
#ifndef UXCONTENT_H
#define UXCONTENT_H

#include "unXmlDocument.h"
#include "uxLimit.h"

class uxItem;
class uxElement;


// ============================================================================
// uxItemStack
// ============================================================================
typedef stack<uxElement*> uxElementStack;


// ============================================================================
// uxElementArray
// ============================================================================
typedef vector<uxElement*> uxElementArray;


// ============================================================================
// uxElementCreator
// ============================================================================
typedef uxElement*(*uxElementCreator)(uxItem&,const uxLimit&);


// ============================================================================
// uxContent
// ============================================================================
class uxContent
{
public:
    // Special
    uxContent( uxItem* item, const uxLimit& limit, uxElementCreator creator )
    :   Item(item)
    ,   Limit(limit)
    ,   ElementCreator(creator)
    {
        //wxLogMessage(" %s",__FUNCSIG__);
        //Elements.push_back(CreateElement());
    }

    ~uxContent();

    // Elements
    uxElement* CreateElement();
    uxElement* GetElement();
    void PutElement( uxElement* element );

    // Accessors
    uxItem* GetItem() const { return Item; }
    uxLimit GetLimit() const { return Limit; }

protected:
    uxItem* Item;
    uxLimit Limit;
    uxElementArray Elements;
    uxElementCreator ElementCreator;
};


// ============================================================================
// uxContents - Inline
// ============================================================================
inline uxElement* uxContent::GetElement()
{
    if( Elements.empty() )
    {
        Elements.push_back(CreateElement());
    }

    uxElement* element = Elements.back();
    Elements.pop_back();
    return element;
}

inline void uxContent::PutElement( uxElement* element )
{
    Elements.push_back(element);
}

inline uxElement* uxContent::CreateElement()
{
    return (*ElementCreator)(*Item,Limit);
}


// ============================================================================
// uxContents
// ============================================================================
typedef vector<uxContent*> uxContents;


// ============================================================================
// uxContentsMap
// ============================================================================
WX_DECLARE_STRING_HASH_MAP( uxContent*, uxContentsMap );




#endif UXCONTENT_H
// ============================================================================