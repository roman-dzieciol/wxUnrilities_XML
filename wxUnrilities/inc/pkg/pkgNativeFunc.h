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
//  pkgNativeFunc.h :: ACHTUNG! LAVA!
// ============================================================================
#ifndef PKGNATIVEFUNC_H
#define PKGNATIVEFUNC_H

class pkgNativeFuncClass;
class pkgNativeFuncPackage;


// ============================================================================
// pkgNativeFunc
// ============================================================================
class pkgNativeFunc
{
public:
    // Special
    pkgNativeFunc();

    // Typedefs
    typedef vector<pkgNativeFunc*> Array;

    // Accessors
    dword GetIndex() const { return Index; }
    dword GetPriority() const { return Priority; }
    dword GetFlags() const { return Flags; }
    wxString GetName() const { return Name; }
    wxString GetFriendlyName() const { return FriendlyName; }
    pkgNativeFuncClass* GetClass() const { return Class; }
    pkgNativeFuncPackage* GetPackage() const { return Package; }

    // Mutators
    void SetIndex( dword d ) { Index = d; }
    void SetPriority( dword d ) { Priority = d; }
    void SetFlags( dword d ) { Flags = d; }
    void SetName( const wxString& d ) { Name = d; }
    void SetFriendlyName( const wxString& d ) { FriendlyName = d; }
    void SetClass( pkgNativeFuncClass* p ) { Class = p; }
    void SetPackage( pkgNativeFuncPackage* p ) { Package = p; }

protected:
    dword Index;
    dword Priority;
    dword Flags;
    wxString Name;
    wxString FriendlyName;
    pkgNativeFuncClass* Class;
    pkgNativeFuncPackage* Package;
};


// ============================================================================
// pkgNativeFuncClass
// ============================================================================
class pkgNativeFuncClass
{
private:
    WX_DECLARE_STRING_HASH_MAP( pkgNativeFunc*, hmFuncByName );

public:
    // Special
    ~pkgNativeFuncClass();

    // Typedefs
    typedef vector<pkgNativeFuncClass*> Array;

    // Accessors
    wxString GetName() const { return Name; }

    // Mutators
    void SetName( const wxString& d ) { Name = d; }

    // Map
    pkgNativeFunc::Array& GetFunctions();
    void AddFunction( pkgNativeFunc* p );

protected:
    wxString Name;
    pkgNativeFunc::Array Functions;
    hmFuncByName FuncByName;
};


// ============================================================================
// pkgNativeFuncPackage
// ============================================================================
class pkgNativeFuncPackage
{
private:
    WX_DECLARE_STRING_HASH_MAP( pkgNativeFuncClass*, hmClassByName );

public:
    // Special
    ~pkgNativeFuncPackage();

    // Typedefs
    typedef vector<pkgNativeFuncPackage*> Array;

    // Accessors
    wxString GetName() const { return Name; }

    // Mutators
    void SetName( const wxString& d ) { Name = d; }

    // Map
    pkgNativeFuncClass::Array& GetClasses();
    void AddClass( pkgNativeFuncClass* p );

protected:
    wxString Name;
    pkgNativeFuncClass::Array Classes;
    hmClassByName ClassByName;
};


// ============================================================================
// pkgNativeFuncMap
// ============================================================================
class pkgNativeFuncMap
{
private:
    WX_DECLARE_STRING_HASH_MAP( pkgNativeFuncPackage*, hmPackageByName );
    WX_DECLARE_HASH_MAP( int, pkgNativeFunc*, wxIntegerHash, wxIntegerEqual, hmFuncByIndex );

public:
    // Special
    ~pkgNativeFuncMap();

    // Map
    pkgNativeFuncPackage::Array& GetPackages();
    void AddPackage( pkgNativeFuncPackage* p );

protected:
    pkgNativeFuncPackage::Array Packages;
    hmPackageByName PackageByName;
    pkgNativeFunc::Array Functions;
    hmFuncByIndex FuncByIndex;
};


#endif PKGNATIVEFUNC_H
// ============================================================================