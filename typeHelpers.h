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

}

#endif //LZY_TYPEHELPERS_H
