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
//  pkgTypeFlow.h
// ============================================================================
#ifndef PKGTYPEFLOW_H
#define PKGTYPEFLOW_H

#include "pkgType.h"
#include "pkgTypeRegistry.h"

class exToken;


// ============================================================================
// pkgTypeFlowRegistry
// ============================================================================
class pkgTypeFlowRegistry : public pkgTypeRegistry
{
public:
    // Initialization
    virtual void OnReload();
};



// ============================================================================
// pkgTypeFlow - common interface
// ============================================================================
#define TYPE_FLOW_COMMON(NAME,PARENT) \
private: \
    static const wxString StaticTypeName; \
    static const wxString StaticDesc; \
    \
public: \
    virtual NAME* Clone() { return new NAME(*this); } \
    virtual NAME* Create() { return new NAME(); } \
    virtual NAME* Construct() { return new NAME(TypeName,Name,Desc); } \
    virtual void OnRead( pkgPackage& package, pkgType& parent ); \
    NAME( const wxString& tname=StaticTypeName, const wxString& name=StaticTypeName, const wxString& desc=StaticDesc ) :   PARENT(tname,name,desc) \



// ============================================================================
// pkgTypeFlow
// ============================================================================
class pkgTypeFlow : public pkgType
{
public:
    // Special
    pkgTypeFlow( const wxString& tname=wxEmptyString, const wxString& name=wxEmptyString, const wxString& desc=wxEmptyString )
    :   pkgType(tname,name,desc)
    ,   Expression(NULL)
    {
    }

    pkgTypeFlow( const pkgTypeFlow& c );
    virtual ~pkgTypeFlow();

    // Virtual ctors
    virtual pkgTypeFlow* Clone() { exNO_IMPL; }
    virtual pkgTypeFlow* Create() { exNO_IMPL; }
    virtual pkgTypeFlow* Construct() { exNO_IMPL; }

    // Accessors
    virtual bool ShouldMap() const { return false; };
    virtual exToken* GetExpression() const { return Expression; };

    // mutators
    virtual void SetExpression( exToken* p ) 
    { 
        Expression = p; 
    }

    virtual void SetName( const wxString& p ) 
    { 
        ExpressionText = p;
        Name = wxString::Format(wxT("<%s %s>"), TypeName.Upper().c_str(), p.c_str()); 
    };

    // Ident
    virtual bool IsFlow() { return true; }

protected:
    exToken* Expression;
    wxString ExpressionText;
};


// ============================================================================
// tfAssert
// ============================================================================
class tfAssert : public pkgTypeFlow
{
public:
    TYPE_FLOW_COMMON( tfAssert, pkgTypeFlow )
    {
    }

    // Debug
    virtual wxString GetInfo() { return wxString::Format(wxT("%s = %s"),Name,bTest?wxT("True"):wxT("False")); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { exNO_IMPL; }
    virtual int32 GetSigned() { exNO_IMPL; }
    virtual wxString GetString() { exNO_IMPL; }

    // Data mutators
    virtual void SetUnsigned( dword p ) { exNO_IMPL; }
    virtual void SetSigned( int32 p ) { exNO_IMPL; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

protected:
    bool bTest;
};


// ============================================================================
// tfArrayItem
// ============================================================================
class tfArrayItem : public pkgTypeFlow
{
public:
    TYPE_FLOW_COMMON( tfArrayItem, pkgTypeFlow )
    {
    }

    // Accessors
    virtual wxString GetName() const { return wxString::Format(wxT("%s[%d]"), Name, Index); }

    // Mutators
    virtual void SetName( const wxString& p ) { Name = p; }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { return Index; }
    virtual int32 GetSigned() { return Index; }
    virtual wxString GetString() { return wxString::Format(wxT("%d"), Index); }

    // Data mutators
    virtual void SetUnsigned( dword p ) { Index = p; }
    virtual void SetSigned( int32 p ) { Index = p; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

protected:
    dword Index;
};


// ============================================================================
// tfArray
// ============================================================================
class tfArray : public pkgTypeFlow
{
public:
    TYPE_FLOW_COMMON( tfArray, pkgTypeFlow )
    {
    }

    // Accessors
    virtual bool ShouldMap() const { return pkgType::ShouldMap(); };

    // Mutators
    virtual void SetName( const wxString& p ) { Name = p; }

    // Debug
    virtual wxString GetInfo() { return wxString::Format(wxT("%s = %d"),Name,Count); }

    // Ident
    virtual bool IsArray() { return true; }
    virtual bool IsData() { return true; }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { return Count; }
    virtual int32 GetSigned() { return Count; }
    virtual wxString GetString() { return wxString::Format(wxT("%u"),Count); }

    // Data mutators
    virtual void SetUnsigned( dword p ) { exNO_IMPL; }
    virtual void SetSigned( int32 p ) { exNO_IMPL; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

protected:
    dword Count;
};

// ============================================================================
// tfIf
// ============================================================================
class tfIf : public pkgTypeFlow
{
public:
    TYPE_FLOW_COMMON( tfIf, pkgTypeFlow )
    {
    }

    // Debug
    virtual wxString GetInfo() { return wxString::Format(wxT("%s = %s"),Name,bTest?wxT("True"):wxT("False")); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { exNO_IMPL; }
    virtual int32 GetSigned() { exNO_IMPL; }
    virtual wxString GetString() { exNO_IMPL; }

    // Data mutators
    virtual void SetUnsigned( dword p ) { exNO_IMPL; }
    virtual void SetSigned( int32 p ) { exNO_IMPL; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }

protected:
    bool bTest;
};

// ============================================================================
// tfOffset
// ============================================================================
class tfOffset : public pkgTypeFlow
{
public:
    TYPE_FLOW_COMMON( tfOffset, pkgTypeFlow )
    {
    }

    // Debug
    virtual wxString GetInfo() { return wxString::Format(wxT("%s = 0x%.8x"),Name,static_cast<dword>(NewOffset)); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { exNO_IMPL; }
    virtual int32 GetSigned() { exNO_IMPL; }
    virtual wxString GetString() { exNO_IMPL; }

    // Data mutators
    virtual void SetUnsigned( dword p ) { exNO_IMPL; }
    virtual void SetSigned( int32 p ) { exNO_IMPL; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }


protected:
    wxFileOffset NewOffset;
    wxFileOffset OldOffset;
};

// ============================================================================
// tfSeek
// ============================================================================
class tfSeek : public pkgTypeFlow
{
public:
    TYPE_FLOW_COMMON( tfSeek, pkgTypeFlow )
    {
    }

    // Debug
    virtual wxString GetInfo() { return wxString::Format(wxT("%s = 0x%.8x"),Name,static_cast<dword>(NewOffset)); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { exNO_IMPL; }
    virtual int32 GetSigned() { exNO_IMPL; }
    virtual wxString GetString() { exNO_IMPL; }

    // Data mutators
    virtual void SetUnsigned( dword p ) { exNO_IMPL; }
    virtual void SetSigned( int32 p ) { exNO_IMPL; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }


protected:
    wxFileOffset NewOffset;
    wxFileOffset OldOffset;
};

// ============================================================================
// tfBreak
// ============================================================================
class tfBreak : public pkgTypeFlow
{
public:
    TYPE_FLOW_COMMON( tfBreak, pkgTypeFlow )
    {
    }

    // Debug
    virtual wxString GetInfo() { return wxString::Format(wxT("%s"),Name); }

    // Comparison 
    virtual bool Compare( pkgType* type ) { exNO_IMPL; }

    // Data accessors
    virtual dword GetUnsigned() { exNO_IMPL; }
    virtual int32 GetSigned() { exNO_IMPL; }
    virtual wxString GetString() { exNO_IMPL; }

    // Data mutators
    virtual void SetUnsigned( dword p ) { exNO_IMPL; }
    virtual void SetSigned( int32 p ) { exNO_IMPL; }
    virtual void SetString( const wxString& p ) { exNO_IMPL; }


};



#endif PKGTYPEFLOW_H
// ============================================================================