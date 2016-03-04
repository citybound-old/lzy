#ifndef LZY_ZIP_H
#define LZY_ZIP_H

#include "../common.h"

namespace lzy {

    template<typename FirstSequence, typename SecondSequence>
    class zip_sequence : public sequence<zip_sequence<FirstSequence, SecondSequence>> {
        using SourceItem = SequenceItemType<FirstSequence>;
        using SecondSequenceItem = SequenceItemType<SecondSequence>;
        using ZippedItem = std::pair<SourceItem, SecondSequenceItem>;
    public:
        zip_sequence(FirstSequence &&firstSource, SecondSequence&& secondSource)
                : firstSource(std::move(firstSource)), secondSource(std::move(secondSource)) { };

        zip_sequence(zip_sequence &&other)
                : firstSource(std::move(other.firstSource)), secondSource(std::move(other.secondSource)) { };

        bool done() { return firstSource.done() || secondSource.done(); };

        void advance() { firstSource.advance(); secondSource.advance();};

        ZippedItem current() {
            return std::pair<SourceItem, SecondSequenceItem>(firstSource.current(), secondSource.current());
        }

    private:
        FirstSequence firstSource;
        SecondSequence secondSource;
    };

    template<typename FirstSequence, typename SecondSequence>
    zip_sequence<FirstSequence, SecondSequence> zip (FirstSequence&& firstSource, SecondSequence&& secondSource) {
        return zip_sequence<FirstSequence, SecondSequence>(std::move(firstSource), std::move(secondSource));
    };
}

#endif //LZY_ZIP_H
