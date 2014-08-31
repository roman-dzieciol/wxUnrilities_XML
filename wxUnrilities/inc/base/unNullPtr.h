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
//	unNullPtr.h - real null pointer of type T*, required for some templates
// ============================================================================
#ifndef UNNULLPTR_H
#define UNNULLPTR_H

//
// Borrowed from Matthew Wilson's book "Imperfect C++"
//

// ============================================================================
// unNullPtr
// ============================================================================
struct unNullPtr
{
public:
    // Construction
    unNullPtr(){}

    // Conversion
    template <typename T>
    operator T*() const
    {
        return 0;
    }

    template <typename T2, typename C>
    operator T2 C::*() const
    {
        return 0;
    }

    template <typename T>
    bool equals(T const &rhs) const
    {
        return rhs == 0;
    }

private:
    // Not to be implemented
    void operator &() const;
    unNullPtr(unNullPtr const &);
    unNullPtr &operator =(unNullPtr const &);
};

template <typename T>
inline bool operator ==(unNullPtr const &lhs, T const &rhs)
{
    return lhs.equals(rhs);
}

template <typename T>
inline bool operator ==(T const &lhs, unNullPtr const &rhs)
{
    return rhs.equals(lhs);
}

template <typename T>
inline bool operator !=(unNullPtr const &lhs, T const &rhs)
{
    return !lhs.equals(rhs);
}

template <typename T>
inline bool operator !=(T const &lhs, unNullPtr const &rhs)
{
    return !rhs.equals(lhs);
}

#define NULLT   unNullPtr()


#endif UNNULLPTR_H
// ============================================================================