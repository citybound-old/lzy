#ifndef LZY_WINDOWS_H
#define LZY_WINDOWS_H

#include "../common.h"
#include "../deps/static-queue/StaticQueue.h"

namespace lzy {

    template<typename Sequence, int windowSize>
    class window_sequence : public sequence<window_sequence<Sequence, windowSize>> {
        using itemReference = SequenceItemType<Sequence>;
        typedef typename std::remove_reference<itemReference>::type item;
        using StaticQueueType = squeue::StaticQueue<std::reference_wrapper<item>, windowSize>;
    public:
        window_sequence(Sequence &&source)
                : source(std::move(source)) { };

        window_sequence(window_sequence &&other)
                : source(std::move(other.source)), windowValues(std::move(other.windowValues)) { };

        bool done() {
            return windowUnfillable() || source.done();
        };

        void advance() {
            source.advance();
            windowValues.pop_front();
            windowValues.push_back(std::reference_wrapper<item>(source.current()));
        }

        StaticQueueType& current() {
            return windowValues;
        }

    private:
        bool windowUnfillable() {
            if (windowFull()) return false;

            while (!source.done()) {
                windowValues.push_back(std::reference_wrapper<item>(source.current()));
                if (windowFull()) return false;
                source.advance();
            }

            return true;
        }

        bool windowFull () {
            return windowValues.size() == windowSize;
        };

        Sequence source;
        StaticQueueType windowValues;
    };

    template<int windowSize>
    class windows : public composable<windows<windowSize>> {
    public:
        template<typename InputSequence>
        window_sequence<InputSequence, windowSize> operator()(InputSequence &&input) {
            return window_sequence<InputSequence, windowSize>(std::move(input));
        }
    };

}

#endif //LZY_WINDOWS_H
