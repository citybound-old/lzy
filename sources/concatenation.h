#ifndef LZY_CONCATENATION_H
#define LZY_CONCATENATION_H

#include "../common.h"

namespace lzy {

    template<typename FirstSequence, typename SecondSequence>
    class concatenation_sequence : public sequence<concatenation_sequence<FirstSequence, SecondSequence>> {
        using item = SequenceItemType<FirstSequence>;
    public:
        concatenation_sequence(FirstSequence&& first, SecondSequence&& second)
                : firstDone(first.done()), first(std::move(first)), second(std::move(second)) { }

        concatenation_sequence(concatenation_sequence &&other)
                : firstDone(other.firstDone), first(std::move(other.first)), second(std::move(other.second)) { };

        bool done() {
            return firstDone && second.done();
        }

        void advance() {
            if (firstDone) second.advance();
            else {
                first.advance();
                firstDone = first.done();
            }
        }

        const item& current() {
            return firstDone ? second.current() : first.current();
        }

    private:
        bool firstDone;
        FirstSequence first;
        SecondSequence second;
    };

    template<typename FirstSequence, typename SecondSequence>
    typename std::enable_if<
            FirstSequence::isLazySequence::value && SecondSequence::isLazySequence::value,
            concatenation_sequence<FirstSequence, SecondSequence>
    >::type
    operator + (FirstSequence&& first, SecondSequence&& second) {
        return concatenation_sequence<FirstSequence, SecondSequence>(std::move(first), std::move(second));
    };

}

#endif //LZY_CONCATENATION_H
