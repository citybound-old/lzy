#ifndef LZY_REDUCE_H
#define LZY_REDUCE_H

#include "../common.h"

namespace lzy {

    template<typename ReduceFunction>
    using ReducedValueType = typename std::remove_const<
            typename std::remove_reference<
                    typename function_traits<ReduceFunction>::template arg<1>::type
            >::type
        >::type;

    template<typename ReduceFunction>
    class reduce_op {
    public:
        reduce_op(ReduceFunction reduceFunction, ReducedValueType<ReduceFunction> initialValue)
                : reduceFunction(reduceFunction), initialValue(initialValue) { };

        reduce_op(reduce_op &&other)
                : reduceFunction(std::move(other.reduceFunction)), initialValue(std::move(other.initialValue)) { };

        template<typename InputSequence>
        ReducedValueType<ReduceFunction> operator()(InputSequence&& inputSequence) {
            ReducedValueType<ReduceFunction> currentValue = initialValue;

            for (auto item : inputSequence) {
                currentValue = reduceFunction(item, currentValue);
            }

            return currentValue;
        }

    private:
        ReduceFunction reduceFunction;
        ReducedValueType<ReduceFunction> initialValue;
    };

    template<typename ReduceFunction>
    reduce_op<ReduceFunction> reduce (ReduceFunction reduceFunction, ReducedValueType<ReduceFunction> initialValue) {
        return reduce_op<ReduceFunction>(reduceFunction, initialValue);
    }
}

#endif //LZY_REDUCE_H
