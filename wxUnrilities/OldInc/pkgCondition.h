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
//  pkgCondition.h
// ============================================================================
#ifndef PKGCONDITION_H
#define PKGCONDITION_H

class dtToken;

// ============================================================================
// conditions
// ============================================================================
class pkgTokenCondition
{
public:
    pkgTokenCondition()
    {
    }

    dtToken* GetToken()
    {
        return Token;
    }


    virtual bool Eval()
    {
        return true;
    }

    static pkgTokenCondition* Create()
    {
        return new pkgTokenCondition;
    }

    void Destroy() 
    { 
        delete this; 
    }

protected:
    dtToken* Token;
};





// ============================================================================
// pkgTokenConditions
// ============================================================================
typedef vector<pkgTokenCondition*> pkgTokenConditions;


// ============================================================================
// pkgTokenSelector
// ============================================================================
class pkgTokenSelector
{
public:
    friend class pkgDecompiler;

public:
    pkgTokenSelector();

public:
    void AddToken( dtToken* token );
    dtToken* SelectToken( pkgDecompiler* decompiler );

public:
    pkgTokenConditions Conditions;
    dtToken* Token;
};




#endif PKGCONDITION_H
// ============================================================================