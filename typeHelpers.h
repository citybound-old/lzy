#ifndef LZY_TYPEHELPERS_H
#define LZY_TYPEHELPERS_H

namespace lzy {

    template<typename Iterator>
    using IteratorValueType = typename std::iterator_traits<Iterator>::value_type;

    template<typename Container>
    using ContainerIteratorType = decltype(std::begin(std::declval<Container>()));

    template<typename Container>
    using ContainerItemType = IteratorValueType<ContainerIteratorType<Container>>;

    template<typename Sequence>
    using SequenceItemType = decltype((std::declval<Sequence>()).current());

    // http://stackoverflow.com/questions/7943525/is-it-possible-to-figure-out-the-parameter-type-and-return-type-of-a-lambda

    template <typename T>
    struct function_traits
            : public function_traits<decltype(&T::operator())>
    {};
    // For generic types, directly use the result of the signature of its 'operator()'

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType(ClassType::*)(Args...) const>
    // we specialize for pointers to member function
    {
        enum { arity = sizeof...(Args) };
        // arity is the number of arguments.

        typedef ReturnType result_type;

        template <size_t i>
        struct arg
        {
            typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
            // the i-th argument is equivalent to the i-th tuple element of a tuple
            // composed of those arguments.
        };
    };
}

#endif //LZY_TYPEHELPERS_H
