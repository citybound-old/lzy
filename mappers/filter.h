#ifndef LZY_FILTER_H
#define LZY_FILTER_H

#include "../common.h"

namespace lzy {

    template<typename Sequence, typename Predicate>
    class filter_sequence : public sequence<filter_sequence<Sequence, Predicate>> {
        using item = SequenceItemType<Sequence>;
    public:
        filter_sequence(Sequence &&source, Predicate predicateFunction)
                : source(std::move(source)), predicateFunction(predicateFunction), started(false) {};

        filter_sequence(filter_sequence &&other)
                : source(std::move(other.source)), predicateFunction(other.predicateFunction), started(other.started) {};

        bool done() {
            if (source.done()) return true;
            else if (!started && !applyPredicate(predicateFunction, source)) {
                started = true;
                advance();
                return source.done();
            } else return false;
        };

        void advance() {
            while (true) {
                source.advance();
                if (source.done()) return;
                if (applyPredicate(predicateFunction, source)) return;
            }
        }

        item current() { return source.current(); }

    private:
        Sequence source;
        Predicate predicateFunction;
        bool started;
    };

    template <typename Predicate, typename Sequence>
    auto applyPredicate(Predicate predicate, Sequence& sequence) -> decltype(predicate(sequence.current())) {
        return predicate(sequence.current());
    };

    template <typename Predicate, typename Sequence>
    auto applyPredicate(Predicate predicate, Sequence& sequence) -> decltype((sequence.current().*predicate)()) {
        return (sequence.current().*predicate)();
    };

    template<typename Predicate>
    class filter_op : public composable<filter_op<Predicate>> {
    public:
        filter_op(Predicate predicateFunction) : predicateFunction(predicateFunction) { };

        template<typename InputSequence>
        filter_sequence<InputSequence, Predicate> operator()(InputSequence &&input) {
            return filter_sequence<InputSequence, Predicate>(std::move(input), predicateFunction);
        }

    private:
        Predicate predicateFunction;
    };

    template<typename Predicate>
    filter_op<Predicate> filter(Predicate predicateFunction) { return filter_op<Predicate>(predicateFunction); };

}

#endif //LZY_FILTER_H
