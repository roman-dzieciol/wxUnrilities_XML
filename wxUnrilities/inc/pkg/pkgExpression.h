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
//  pkgExpression.h
// ============================================================================
#ifndef PKGEXPRESSION_H
#define PKGEXPRESSION_H


class exToken;
class wxStringTokenizer;
class pkgType;
class pkgTypeFlow;
class pkgTypeBase;
class pkgPackage;
class tbASCIIZ;



// ============================================================================
// exToken - Containers
// ============================================================================
WX_DECLARE_STRING_HASH_MAP( exToken*, exHashMap );        // exHashMap
typedef vector<exToken*> exArray;                         // exArray


// ============================================================================
// exToken
// ============================================================================
class exToken
{
public:
    // Special
    exToken( const wxString& name=wxEmptyString )
    :   Name(name)
    ,   Parent(NULL)
    {
    }

    exToken( const exToken& c )
    :   Name(c.Name)
    ,   Parent(NULL)
    {
    }

    virtual ~exToken() 
    {
    }

    virtual exToken* Clone() const { return new exToken(*this); }

    // Accessors
    wxString GetName() const { return Name; };
    exToken* GetParent() const { return Parent; };

    // Mutators
    void SetName( const wxString& p ) { Name = p; };
    void SetParent( exToken* p ) { Parent = p; };

    // Debug
    virtual wxString GetInfo() { exNO_IMPL; }

    // Comparison
    virtual bool Compare( pkgPackage& package, pkgType& parent, exToken& token ) { exNO_IMPL; }

    // Data
    virtual bool GetBool( pkgPackage& package, pkgType& parent ) { exNO_IMPL; }
    virtual dword GetUnsigned( pkgPackage& package, pkgType& parent ) { exNO_IMPL; }
    virtual pkgType* GetType( pkgPackage& package, pkgType& parent ) { exNO_IMPL; }

protected:
    wxString    Name;
    exToken*    Parent;
};



// ============================================================================
// exUnary
// ============================================================================
class exUnary : public exToken
{
public:
    exUnary( exToken* a, const wxString& name=wxEmptyString  )
    : exToken(name) , A(a)
    {
    }

    exUnary( const exUnary& token )
    :  exToken(token)
    {
        A = token.A->Clone();
    }

    virtual ~exUnary() 
    {
        delete A;
    }

    virtual exUnary* Clone() const { return new exUnary(*this); }
    virtual wxString GetInfo();

protected:
    exToken* A;
};


// ============================================================================
// exBinary
// ============================================================================
class exBinary : public exUnary
{
public:
    exBinary( exToken* a, exToken* b, const wxString& name=wxEmptyString  )
    : exUnary(a,name), B(b)
    {
    }

    exBinary( const exBinary& token )
    :   exUnary(token)
    {
        B = token.B->Clone();
    }

    virtual ~exBinary() 
    {
        delete B;
    }

    virtual exBinary* Clone() const { return new exBinary(*this); }
    virtual wxString GetInfo();

protected:
    exToken* B;
};


// ============================================================================
// exTernary
// ============================================================================
class exTernary : public exBinary
{
public:
    exTernary( exToken* a, exToken* b, exToken* c, const wxString& name=wxEmptyString  )
    : exBinary(a,b,name), C(c)
    {
    }

    exTernary( const exTernary& token )
    :   exBinary(token)
    {
        C = token.C->Clone();
    }

    virtual ~exTernary() 
    {
        delete C;
    }

    virtual exTernary* Clone() const { return new exTernary(*this); }
    virtual wxString GetInfo();

protected:
    exToken* C;
};


// ============================================================================
// exConst
// ============================================================================
class exConst : public exToken
{
public:
    exConst( const wxString& name=wxEmptyString );
    exConst( const exConst& c );
    ~exConst();

    virtual exConst* Clone() const { return new exConst(*this); }
    virtual wxString GetInfo();

    // Comparison
    virtual bool Compare( pkgPackage& package, pkgType& parent, exToken& token ) { exNO_IMPL; }

    // Data
    virtual bool GetBool( pkgPackage& package, pkgType& parent ) { exNO_IMPL; }
    virtual dword GetUnsigned( pkgPackage& package, pkgType& parent );
    virtual pkgType* GetType( pkgPackage& package, pkgType& parent );

protected:
    tbASCIIZ* StringType;
};


// ============================================================================
// exVar
// ============================================================================
class exVar : public exUnary
{
public:
    exVar( exToken* a, const wxString& name=wxT("Var") ) : exUnary(a,name) 
    {
    }

    virtual exVar* Clone() const { return new exVar(*this); }

    // Comparison
    virtual bool Compare( pkgPackage& package, pkgType& parent, exToken& token );

    // Data
    virtual bool GetBool( pkgPackage& package, pkgType& parent ) { exNO_IMPL; }
    virtual dword GetUnsigned( pkgPackage& package, pkgType& parent );
    virtual pkgType* GetType( pkgPackage& package, pkgType& parent );
};


// ============================================================================
// exExport
// ============================================================================
class exExport : public exUnary
{
public:
    exExport( exToken* a, const wxString& name=wxT("Export") ) : exUnary(a,name) 
    {
    }

    virtual exExport* Clone() const { return new exExport(*this); }

    // Comparison
    virtual bool Compare( pkgPackage& package, pkgType& parent, exToken& token ) { exNO_IMPL; }

    // Data
    virtual bool GetBool( pkgPackage& package, pkgType& parent ) { exNO_IMPL; }
    virtual dword GetUnsigned( pkgPackage& package, pkgType& parent );
    virtual pkgType* GetType( pkgPackage& package, pkgType& parent );
};


// ============================================================================
// exEqual
// ============================================================================
class exEqual : public exBinary
{
public:
    exEqual( exToken* a, exToken* b, const wxString& name=wxT("Eq")  ) :  exBinary(a,b,name)
    {
    }

    virtual exEqual* Clone() const { return new exEqual(*this); }

    // Comparison
    virtual bool Compare( pkgPackage& package, pkgType& parent, exToken& token ) { exNO_IMPL; }

    // Data
    virtual bool GetBool( pkgPackage& package, pkgType& parent );
    virtual dword GetUnsigned( pkgPackage& package, pkgType& parent ) { exNO_IMPL; }
    virtual pkgType* GetType( pkgPackage& package, pkgType& parent ) { exNO_IMPL; }
};


// ============================================================================
// exNotEqual
// ============================================================================
class exNotEqual : public exBinary
{
public:
    exNotEqual( exToken* a, exToken* b, const wxString& name=wxT("Neq")  ) :  exBinary(a,b,name)
    {
    }

    virtual exNotEqual* Clone() const { return new exNotEqual(*this); }

    // Comparison
    virtual bool Compare( pkgPackage& package, pkgType& parent, exToken& token ) { exNO_IMPL; }

    // Data
    virtual bool GetBool( pkgPackage& package, pkgType& parent );
    virtual dword GetUnsigned( pkgPackage& package, pkgType& parent ) { exNO_IMPL; }
    virtual pkgType* GetType( pkgPackage& package, pkgType& parent ) { exNO_IMPL; }
};


// ============================================================================
// exHasFlag
// ============================================================================
class exHasFlag : public exTernary
{
public:
    exHasFlag( exToken* a, exToken* b, exToken* c, const wxString& name=wxT("HasFlag")  ) 
    :  exTernary(a,b,c,name)
    {
    }

    virtual exHasFlag* Clone() const { return new exHasFlag(*this); }

    // Comparison
    virtual bool Compare( pkgPackage& package, pkgType& parent, exToken& token ) { exNO_IMPL; }

    // Data
    virtual bool GetBool( pkgPackage& package, pkgType& parent );
    virtual dword GetUnsigned( pkgPackage& package, pkgType& parent ) { exNO_IMPL; }
    virtual pkgType* GetType( pkgPackage& package, pkgType& parent ) { exNO_IMPL; }
};



// ============================================================================
// exParser
// ============================================================================
class exParser
{
public:
    exToken* GetTokens( const wxString& text );
    exToken* ParseToken( const wxString& text, wxStringTokenizer& parser );

protected:
    exHashMap   Functions;
};




#endif PKGEXPRESSION_H
// ============================================================================