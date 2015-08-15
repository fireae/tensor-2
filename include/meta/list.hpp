// ----------------------------------------------------------------------------------------------------------
/// @file   list.hpp
/// @brief  Header file for the list metaclass to provide compile time lists
// ----------------------------------------------------------------------------------------------------------

/*
 * ----------------------------------------------------------------------------------------------------------
 * list header file for ftl library.
 * Copyright (C) 2015 Rob Clucas
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * ----------------------------------------------------------------------------------------------------------
 */

#ifndef FTL_LIST
#define FTL_LIST

#include "eval.hpp"

#include <cassert>

namespace ftl {
 
// ----------------------------------------------------------------------------------------------------------
/// @struct     list
/// @brief      Meta class that holds types, and allows functions to be applied to the elements of the list 
///             using the internal apply struct.                                                             \n
///                                                                                                          \n
///             Usage:                                                                                       \n
///                                                                                                          \n
///             using ftl::dim;                         \\ To get dimension types                            \n
///             using test_list = list<i, j, k, l>;     \\ A list of dimensions                              \n
///                                                                                                          \n
///             using shifted_list = list::apply<shift>;                                                     \n
///                                                                                                          \n
///             Where shift is some function which operates on a type in the list 
/// @tparam     Ts      The tyes of the elements in the list
// ----------------------------------------------------------------------------------------------------------
template <typename... Ts>
struct list
{
    using type = list<Ts...>;
    
    // ------------------------------------------------------------------------------------------------------
    /// @struct     apply
    /// @brief      Applies the function to each element in the list
    /// @tparam     Function    The function to apply the the list elements
    // ------------------------------------------------------------------------------------------------------
    template <template <typename...> class Function>
    struct apply
    {
        using type = list<typename eval<Function<Ts>, no_args>::result...>;
    };
};

// Define a list with no arguments
using empty_list = list<>;

// ---------------------------------------- Operations on a list(s) -----------------------------------------

namespace detail {
    
// ----------------------------------------------------------------------------------------------------------
/// @struct     get
/// @brief      Meta function to get an element from a list 
/// @tparam     Index       The index of the element in the list to get
/// @tparam     List        The list to get the element from
// ----------------------------------------------------------------------------------------------------------
template <typename Index, typename List>
struct get;

// Recursive case to look through the list
template <std::size_t Index, typename Head, typename... Tail>
struct get<ftl::size_t<Index>, list<Head, Tail...>> : public get<ftl::size_t<Index - 1>, list<Tail...>> {};

// Base case (when we reach the 0 element)
template <typename Head, typename... Tail>
struct get<ftl::size_t<0>, list<Head, Tail...>> : public identify<Head> {};

}

// Wrapper for getting
template <std::size_t Index, typename List>
using get = typename detail::get<ftl::size_t<Index>, List>::result;

// ----------------------------------------------------------------------------------------------------------
/// @struct     find_type
/// @brief      Find the index of a specific type in the list (the index of the first occurrence).           \n 
///             If the type is not found then the value 'parameter' will be -1.
/// @tparam     Type    The type to find
/// @tparam     List    The list to find the type in
// ----------------------------------------------------------------------------------------------------------
template <typename Type, typename List>
struct find_type;
    
// Case to recurse
template <typename Type, typename Head, typename... Tail>
struct find_type<Type, list<Head, Tail...>>
{
    static constexpr int next_result = find_type<Type, list<Tail...>>::result;
    
    // 'Move through list'
    static constexpr int result      = next_result >= 0 ? next_result + 1 : -1;
};

// Case for when the type is found
template <typename Type, typename... Tail>
struct find_type<Type, list<Type, Tail...>>
{
    static constexpr int result = 0;
};

// Case for not found (Tail will be empty)
template <typename Type>
struct find_type<Type, list<>>
{
    static constexpr int result = -1;
};

// ----------------------------------------------------------------------------------------------------------
/// @struct     join
/// @brief      Joins two lists
/// @tparam     List1       The first list to join
/// @tparam     List2       The second list to join
// ----------------------------------------------------------------------------------------------------------
template <typename List1, typename List2>
struct join;

// Specialization for using list types
template <typename... Ts, typename... Us>
struct join<list<Ts...>, list<Us...>> : public identify<list<Ts..., Us...>> {};

// ----------------------------------------------------------------------------------------------------------
/// @struct     zip 
/// @brief      Takes two lists, and zips the corresponding elements into a list of 2 elements if the function
///             to determine if the elements should be zips succeeds, otherwise the elements are not zipped.
/// @tparam     Evaluator       A function which operates on corresponding elements from the 2 lists to
///             determine if the elements should be zipped.
/// @tparam     List1           First list for zipping.
/// @tparam     List2           Second list for zipping.
/// @tparam     Passed          The elements which have 'passed' the functions test and have been added to the
///             zipped list.
// ----------------------------------------------------------------------------------------------------------
template <template <typename...> class  Evaluator   , 
          typename                      List1       , 
          typename                      List2       , 
          typename...                   Passed      >
struct zip;

// Recursize case - when the whole list has not been traversed
template <template <typename...> class  Evaluator   ,
          typename                      Head1       , 
          typename...                   Tail1       ,
          typename                      Head2       ,
          typename...                   Tail2       ,
          typename...                   Passed      >
struct zip<Evaluator, list<Head1, Tail1...>, list<Head2, Tail2...>, list<Passed...>>
{
    using passed = typename std::conditional<
                                Evaluator<Head1, Head2>::result,                // Check if we must zip
                                list<Passed..., list<Head1, Head2>>,            // Zip head elements if true
                                list<Passed...>                                 // Don't zip if false
                                    >::type;
    
    // Recurse until we reach the base case (pattern)
    using result = typename zip<Evaluator, list<Tail1...>, list<Tail2...>, passed>::result;
};

// Base case - not more elements to check in List1 or List 2
template <template <typename...> class Evaluator, typename... Passed>
struct zip<Evaluator, empty_list, empty_list, list<Passed...>>
{
    using result = list<Passed...>;
};

}   // End namespace ftl

#endif