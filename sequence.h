#ifndef LZY_SEQUENCE_H
#define LZY_SEQUENCE_H

#include "pushable.h"

namespace lzy {

    template<typename Derived>
    class sequence : public pushable<Derived> {
        class sequence_iterator {
            using item = SequenceItemType<Derived>;
            typedef typename std::remove_reference<item>::type itemWithoutReference;
            friend class sequence;

        public:
            typedef item value_type;
            typedef itemWithoutReference *pointer;
            typedef item &reference;
            typedef void difference_type;
            typedef std::input_iterator_tag iterator_category;

            bool operator==(const sequence_iterator &other) const {
                // TODO: check for same sequence?
                return isSentinel == other.isSentinel;
            };

            bool operator!=(const sequence_iterator &other) const {
                return isSentinel != other.isSentinel;
            };

            sequence_iterator &operator++() {
                sequence.advance();
                isSentinel = isSentinel || sequence.done();
                return *this;
            };

            value_type operator*() const {
                return sequence.current();
            };

            pointer operator->() const {
                return std::addressof(sequence.current());
            };
        private:
            sequence_iterator(Derived &sequence, bool forceSentinel = false) : sequence(sequence) {
                isSentinel = forceSentinel || sequence.done();
            }

            Derived &sequence;
            bool isSentinel;
        };

    public:
        sequence_iterator begin() {
            return sequence_iterator(*(static_cast<Derived *>(this)));
        }

        sequence_iterator end() {
            return sequence_iterator(*(static_cast<Derived *>(this)), true);
        }
    };

}

#endif //LZY_SEQUENCE_H
