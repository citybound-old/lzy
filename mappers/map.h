#ifndef LZY_MAP_H
#define LZY_MAP_H

#include "../common.h"

namespace lzy {

    template<typename Sequence, typename MappingFunction>
    class map_sequence : public sequence<map_sequence<Sequence, MappingFunction>> {
        using SourceItem = SequenceItemType<Sequence>;
    public:
        map_sequence(Sequence &&source, MappingFunction mappingFunction)
                : source(std::move(source)), mappingFunction(mappingFunction) { };

        map_sequence(map_sequence &&other)
                : source(std::move(other.source)), mappingFunction(other.mappingFunction) { };

        bool done() { return source.done(); };

        void advance() { source.advance(); };

        auto current() -> decltype(applyMapping(std::declval<MappingFunction>(), std::declval<Sequence&>())){
            return applyMapping(mappingFunction, source);
        }

    private:
        Sequence source;
        MappingFunction mappingFunction;
    };

    template<typename MappingFunction, typename Sequence>
    auto applyMapping(MappingFunction mappingFunction, Sequence& source) -> decltype(mappingFunction(source.current())) {
        return mappingFunction(source.current());
    };

    template<typename MappingFunction, typename Sequence>
    auto applyMapping(MappingFunction mappingFunction, Sequence& source) -> decltype((source.current().*mappingFunction)()) {
        return (source.current().*mappingFunction)();
    };

    template<typename MappingFunction>
    class map_op : public composable<map_op<MappingFunction>> {
    public:
        map_op(MappingFunction mappingFunction) : mappingFunction(mappingFunction) { };

        template<typename InputSequence>
        map_sequence<InputSequence, MappingFunction> operator()(InputSequence &&input) {
            return map_sequence<InputSequence, MappingFunction>(std::move(input), mappingFunction);
        };
    private:
        MappingFunction mappingFunction;
    };

    template<typename MappingFunction>
    map_op<MappingFunction> map(MappingFunction mappingFunction) {
        return map_op<MappingFunction>(mappingFunction);
    }

}

#endif //LZY_MAP_H
