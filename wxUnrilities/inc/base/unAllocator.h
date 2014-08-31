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
//  unAllocator.h
// ============================================================================
#ifndef UNALLOCATOR_H
#define UNALLOCATOR_H



// ============================================================================
// delete_safe
// ============================================================================
template<class T>
inline void safe_delete( T*& ptr )
{
    delete ptr;
    ptr = NULL;
}


// ============================================================================
// unAllocator
// ============================================================================
template<class T>
class unAllocator
{
public:
    typedef unAllocator<T> TAlloc;

    static T* Create()
    {
        return new T;
    }

    void Destroy( T* ptr )
    {
        delete ptr;
    }
};


// ============================================================================
// unAllocVirtual
// ============================================================================
template<class T>
class unAllocVirtual
{
public:
    // Typedefs
    typedef unAllocVirtual<T> TAlloc;

    // Special
    virtual ~unAllocVirtual()
    {
    }

    // Allocator
    static T* Create()
    {
        return new T;
    }

    virtual void Destroy()
    {
        delete this;
    }
};


// ============================================================================
// unAllocStatic
// ============================================================================
template<class T>
class unAllocStatic
{
public:

    // Allocator
    static T* Create()
    {
        return new T;
    }

    static T* Copy( const T& ref )
    {
        return new T(ref);
    }

    static void Destroy( T*& ptr )
    {
        delete ptr;
    }
};


template<class T>
inline T* alloc_create()
{
    return T::TAlloc::Create();
}

template<class T>
inline void alloc_create( T*& ptr )
{
    ptr = T::TAlloc::Create();
}

template<class T>
inline T* alloc_copy( const T& ref )
{
    return T::TAlloc::Copy(ref);
}

template<class T>
inline void alloc_destroy( T*& ptr )
{
    T::TAlloc::Destroy(ptr);
    ptr = NULL;
}

template<class T>
inline void alloc_vdestroy( T*& ptr )
{
    ptr->Destroy();
    ptr = NULL;
}

#define STATIC_ALLOCATOR(Class) typedef unAllocStatic<Class> TAlloc


//

#define STATIC_CREATOR(NAME,BASE) static BASE* Create() { return new NAME(); }


#endif UNALLOCATOR_H
// ============================================================================