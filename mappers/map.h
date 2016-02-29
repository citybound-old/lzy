#ifndef LZY_MAP_H
#define LZY_MAP_H

#include "../common.h"

namespace lzy {

    template<typename Sequence, typename MappingFunction>
    class map_sequence : public sequence<map_sequence<Sequence, MappingFunction>> {
        using SourceItem = SequenceItemType<Sequence>;
        using MappedItem = typename std::result_of<MappingFunction(SourceItem)>::type;
    public:
        map_sequence(Sequence &&source, MappingFunction mappingFunction)
                : source(std::move(source)), mappingFunction(mappingFunction) { };

        map_sequence(map_sequence &&other)
                : source(std::move(other.source)), mappingFunction(other.mappingFunction) { };

        bool done() { return source.done(); };

        void advance() { source.advance(); };

        MappedItem current() {
            return mappingFunction(source.current());
        }

    private:
        Sequence source;
        MappingFunction mappingFunction;
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
