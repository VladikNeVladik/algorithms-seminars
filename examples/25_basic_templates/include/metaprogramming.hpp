// Copyright 2024 Vladislav Aleinik
//===================================================================
// Самописная библиотека мета-функций
// Большая её часть уже реализована в заголовочных файлах:
// - <type_traits>
// - <experimental/type_traits>
//===================================================================

#include <type_traits>

// NOTE:
//  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄         ▄       ▄▄▄▄▄▄▄▄▄▄▄  ▄         ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄         ▄  ▄
// ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌     ▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░▌
// ▐░█▀▀▀▀▀▀▀▀▀  ▀▀▀▀█░█▀▀▀▀ ▐░█▀▀▀▀▀▀▀█░▌▐░▌       ▐░▌     ▐░█▀▀▀▀▀▀▀█░▌▐░▌       ▐░▌▐░█▀▀▀▀▀▀▀█░▌▐░▌       ▐░▌▐░▌
// ▐░▌               ▐░▌     ▐░▌       ▐░▌▐░▌       ▐░▌     ▐░▌       ▐░▌▐░▌       ▐░▌▐░▌       ▐░▌▐░▌       ▐░▌▐░▌
// ▐░█▄▄▄▄▄▄▄▄▄      ▐░▌     ▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌     ▐░█▄▄▄▄▄▄▄█░▌▐░▌   ▄   ▐░▌▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌▐░▌
// ▐░░░░░░░░░░░▌     ▐░▌     ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌     ▐░░░░░░░░░░░▌▐░▌  ▐░▌  ▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌
//  ▀▀▀▀▀▀▀▀▀█░▌     ▐░▌     ▐░█▀▀▀▀▀▀▀█░▌ ▀▀▀▀█░█▀▀▀▀      ▐░█▀▀▀▀▀▀▀█░▌▐░▌ ▐░▌░▌ ▐░▌▐░█▀▀▀▀▀▀▀█░▌ ▀▀▀▀█░█▀▀▀▀ ▐░▌
//           ▐░▌     ▐░▌     ▐░▌       ▐░▌     ▐░▌          ▐░▌       ▐░▌▐░▌▐░▌ ▐░▌▐░▌▐░▌       ▐░▌     ▐░▌      ▀
//  ▄▄▄▄▄▄▄▄▄█░▌     ▐░▌     ▐░▌       ▐░▌     ▐░▌          ▐░▌       ▐░▌▐░▌░▌   ▐░▐░▌▐░▌       ▐░▌     ▐░▌      ▄
// ▐░░░░░░░░░░░▌     ▐░▌     ▐░▌       ▐░▌     ▐░▌          ▐░▌       ▐░▌▐░░▌     ▐░░▌▐░▌       ▐░▌     ▐░▌     ▐░▌
//  ▀▀▀▀▀▀▀▀▀▀▀       ▀       ▀         ▀       ▀            ▀         ▀  ▀▀       ▀▀  ▀         ▀       ▀       ▀
//
// NOTE:
//  ▄         ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄   ▄
// ▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░▌ ▐░▌
// ▐░▌       ▐░▌▐░█▀▀▀▀▀▀▀█░▌ ▀▀▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀█░▌▐░▌
// ▐░▌       ▐░▌▐░▌       ▐░▌          ▐░▌▐░▌       ▐░▌▐░▌       ▐░▌▐░▌       ▐░▌▐░▌
// ▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌ ▄▄▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌▐░▌       ▐░▌▐░▌
// ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░▌
// ▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀▀▀ ▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀█░█▀▀ ▐░▌       ▐░▌▐░▌
// ▐░▌       ▐░▌▐░▌       ▐░▌▐░▌          ▐░▌       ▐░▌▐░▌     ▐░▌  ▐░▌       ▐░▌ ▀
// ▐░▌       ▐░▌▐░▌       ▐░▌▐░█▄▄▄▄▄▄▄▄▄ ▐░▌       ▐░▌▐░▌      ▐░▌ ▐░█▄▄▄▄▄▄▄█░▌ ▄
// ▐░▌       ▐░▌▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░▌       ▐░▌▐░░░░░░░░░░▌ ▐░▌
//  ▀         ▀  ▀         ▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀         ▀  ▀         ▀  ▀▀▀▀▀▀▀▀▀▀   ▀
// ПРИМЕЧАНИЕ: эта библиотека написана автором исключительно в целях самообразования.
// СОВЕТ: Используйте аналоги этих функций из стандартной библиотеки.

// Use namespace to prevent namespace collision with namespace std:
namespace Meta
{
    //---------------------------------
    // Basic metaprogramming utilities
    //---------------------------------

    // The equivalent of declaring a typed constant:
    template <typename Type, Type Value>
    struct integral_constant
    {
        static constexpr Type value = Value;
    };

    // Special case of bool constant:
    template <bool Value>
    using bool_constant = integral_constant<bool, Value>;

    // The metaprogramming equivalent of true:
    using true_type = bool_constant<true>;

    // The metaprogramming equivalent of false:
    using false_type = bool_constant<false>;

    // Declare a constant type:
    template <typename Type>
    struct type_is
    {
        using type = Type;
    };

    // The equivalent of operator== on types:
    template <typename Type1, typename Type2>
    struct is_same : false_type
    { };

    template <typename Type>
    struct is_same<Type, Type> : true_type
    { };

    //-----------------------------
    // Implementation of enable_if
    //-----------------------------

    // Description of enable_if:
    // - If the first argument is true, define const static member variable "type".
    // - If the first argument is false, do not define "type".
    //
    // This metafunction uses SFINAE principle:
    // - Substitution Failure Is Not An Error.

    namespace __detail
    {
        // Default specialization of template:
        template<bool, typename Type = void>
        struct enable_if
        { };

        // In case of "true" define "type" as Type:
        template<typename Type>
        struct enable_if<true, Type> : type_is<Type>
        { };
    }; // namespace __detail

    // Define a shorthand for "enable_if<..., ...>::type":
    template <bool Cond, typename Type = void>
    using enable_if_t = typename __detail::enable_if<Cond, Type>::type;

    //---------------------------------
    // Basic if-then-else metafunction
    //---------------------------------

    namespace __detail
    {
        // Helper for if-then-else metafunction:
        template <bool Cond, typename Then, typename Else>
        struct conditional
        {
            using type = Then;
        };

        template <typename Then, typename Else>
        struct conditional<false, Then, Else>
        {
            using type = Else;
        };
    }; // namespace __detail

    // Basic if-then-else metafunction:
    // - Defines "type" template;
    // - if (Cond) type = Then;
    // - else      type = Else;
    template <bool Cond, typename Then, typename Else>
    using conditional_t = typename __detail::conditional<Cond, Then, Else>;

    //----------------------------
    // Boolean logic (AND/OR/NOT)
    //----------------------------

    namespace __detail
    {
        // A variadic alias template that resolves to its first argument:
        template<typename First, typename... Others>
        using first_t = First;

        // Logical OR of all it's arguments:
        // NOTE: Evaluates to true in case of non-bool type.
        // NOTE: Use declarations only to save compile-time memory usage (adequate only before C++17).
        // NOTE: Use return type deduction syntax.
        // NOTE: Use SFINAE to make occurence of true_type lead to ill-formed program.
        template <typename... Inputs>
        auto or_func(int) ->
            first_t<false_type, enable_if_t<!bool(Inputs::value)>...>;

        // NOTE: Use ellipsis (...) as "the worst matching candidate".
        //       This ensures that second case is matched strictly after the first one.
        template <typename... Inputs>
        auto or_func(...) -> true_type;

        // Logical AND of all it's arguments:
        template <typename... Inputs>
        auto and_func(int) ->
            first_t<true_type, enable_if_t<bool(Inputs::value)>...>;

        template <typename... Inputs>
        auto and_func(...) -> false_type;
    }; // namespace __detail

    // Logical OR of all it's arguments:
    // NOTE: decltype() requires only the template declaration.
    template <typename... Inputs>
    struct disjunction : decltype(__detail::or_func<Inputs...>(0))
    { };

    // Logical AND of all it's arguments:
    template <typename... Inputs>
    struct conjunction : decltype(__detail::and_func<Inputs...>(0))
    { };

    // Logical NOT:
    // NOTE: NOT lacks default value used in case of substitution failure.
    template <typename Input>
    struct negation : bool_constant<!bool(Input::value)>
    { };

    //-------------------------
    // Implemenation of void_t
    //-------------------------

    // NOTE: it isn't simple at all.
    // NOTE: this implementation is known to not work in some versions of GCC.
    template<typename...>
    using void_t = void;

    //----------------------------------------
    // Implemenation of is_copy_assignable
    //----------------------------------------

    // Helper alias for the result type of a valid copy assignment:
    template <typename T>
    using copy_assignment_t = decltype(std::declval<T&>() = std::declval<const T&>());

    // Primary template handles all non-copy-assignable types:
    // NOTE: default argument is essential.
    template <typename T, typename = void>
    struct is_copy_assignable : false_type
    { };

    // Specialization recognizes and validates only copy-assignable types:
    template <typename T>
    struct is_copy_assignable<T, void_t<copy_assignment_t<T>>> :
        is_same<copy_assignment_t<T>, T&>
    { };

    //-----------------------------------------
    // Implemenation of is_addition_assignable
    //-----------------------------------------

    template <typename T>
    using addition_assignment_t = decltype(std::declval<T&>() += std::declval<const T&>());

    // Primary template:
    template <typename T, typename = void>
    struct is_addition_assignable : false_type
    { };

    // Template specialization:
    template <typename T>
    struct is_addition_assignable<T, void_t<addition_assignment_t<T>>> :
        is_same<addition_assignment_t<T>, T&>
    { };

    //-----------------------------------------------
    // Implemenation of is_multiplication_assignable
    //-----------------------------------------------

    template <typename T>
    using multiplication_assignment_t = decltype(std::declval<T&>() *= std::declval<const T&>());

    // Primary template:
    template <typename T, typename = void>
    struct is_multiplication_assignable : false_type
    { };

    // Template specialization:
    template <typename T>
    struct is_multiplication_assignable<T, void_t<multiplication_assignment_t<T>>>:
        is_same<multiplication_assignment_t<T>, T&>
    { };

    //--------------------------------
    // Implemenation of is_comparable
    //--------------------------------

    template <typename T>
    using equality_comparison_t = decltype(std::declval<const T&>() == std::declval<const T&>());

    template <typename T>
    using inequality_comparison_t = decltype(std::declval<const T&>() != std::declval<const T&>());

    // Primary template:
    template <typename T, typename = void>
    struct is_comparable : false_type
    { };

    // Template specialization:
    template <typename T>
    struct is_comparable<T, void_t<  equality_comparison_t<T>,
                                   inequality_comparison_t<T>>> :
        conjunction<is_same<  equality_comparison_t<T>, bool>,
                    is_same<inequality_comparison_t<T>, bool>>
    { };

}; // namespace Meta
