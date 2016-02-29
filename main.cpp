#include <iostream>
#include <list>
#include <vector>
#include <list>

using namespace std;

template<typename Itr>
using IteratorValueType = typename std::iterator_traits<Itr>::value_type;

template<typename Container>
using ContainerIteratorType = decltype(std::begin(std::declval<Container>()));

template<typename Container>
using ContainerItemType = IteratorValueType<ContainerIteratorType<Container>>;

template<typename Sequence>
using SequenceItemType = decltype((std::declval<Sequence>()).current());

template<typename Derived>
class composable;

template<typename FirstComposable, typename SecondComposable>
class composition : public composable<composition<FirstComposable, SecondComposable>> {
public:
    composition (FirstComposable& first, SecondComposable& second) : first(first), second(second) {};

    template<typename Input>
    using FirstResult = typename std::result_of<FirstComposable(Input)>::type;

    template<typename Input>
    using EndResult = typename std::result_of<SecondComposable(FirstResult<Input>)>::type;

    template<typename Input>
    EndResult<Input> operator() (Input&& input) {
        return second(first(std::move(input)));
    }
private:
    FirstComposable& first;
    SecondComposable& second;
};

template<typename Derived>
class composable {
public:
    template<typename OtherComposable>
    composition<Derived, OtherComposable> operator | (OtherComposable&& other) {
        return composition<Derived, OtherComposable>(static_cast<Derived&>(*this), other);
    }
};

template<typename Derived>
class pushable {
public:
    template<typename Target>
    using TargetResult = typename std::result_of<Target(Derived)>::type;

    template<typename Target>
    TargetResult<Target> operator >> (Target&& target) {
        return target(std::move(static_cast<Derived&>(*this)));
    }
};

template<typename Derived>
class sequence : public pushable<Derived> {
    class sequence_iterator {
        using item = SequenceItemType<Derived>;
        friend class sequence;
    public:
        typedef item value_type;
        typedef item* pointer;
        typedef item& reference;
        typedef void difference_type;
        typedef std::input_iterator_tag iterator_category;

        bool operator==(const sequence_iterator& other) const {
            // TODO: check for same sequence?
            return isSentinel == other.isSentinel;
        };
        bool operator!=(const sequence_iterator& other) const {
            return isSentinel != other.isSentinel;
        };
        sequence_iterator& operator++() {
            sequence.advance();
            isSentinel = isSentinel || sequence.done();
            return *this;
        };
        value_type operator*() const {
            return sequence.current();
        };
        pointer operator->() const {
            return &sequence.current();
        };
    private:
        sequence_iterator (Derived& sequence, bool forceSentinel = false) : sequence(sequence) {
            isSentinel = forceSentinel || sequence.done();
        }
        Derived& sequence;
        bool isSentinel;
    };
public:
    sequence_iterator begin () {
        return sequence_iterator(*(static_cast<Derived*>(this)));
    }

    sequence_iterator end () {
        return sequence_iterator(*(static_cast<Derived*>(this)), true);
    }
};

/* FROM CONTAINER */

template <typename Container>
class container_sequence : public sequence<container_sequence<Container>> {
    using iterator = ContainerIteratorType<Container>;
    using item = ContainerItemType<Container>;
public:
    container_sequence(Container& container)
            : curr(std::begin(container)), end(std::end(container)) {}
    container_sequence(container_sequence&& other)
            : curr(std::move(other.curr)), end(std::move(other.end)) {};

    bool done() {
        return curr == end;
    }
    void advance () {
        curr++;
    }
    const item & current () {
        return *curr;
    }

private:
    iterator curr;
    iterator end;
};

template <typename Container>
container_sequence<Container> from (Container& container) {
    return container_sequence<Container>(container);
};


/* MAP */

template<typename Sequence, typename MappingFunction>
class map_sequence : public sequence<map_sequence<Sequence, MappingFunction>> {
    using SourceItem = SequenceItemType<Sequence>;
    using MappedItem = typename std::result_of<MappingFunction(SourceItem)>::type;
public:
    map_sequence(Sequence&& source, MappingFunction mappingFunction)
            : source(std::move(source)), mappingFunction(mappingFunction) {};
    map_sequence(map_sequence&& other)
            : source(std::move(other.source)), mappingFunction(other.mappingFunction) {};
    bool done () {return source.done();};
    void advance () {source.advance();};
    MappedItem current () {
        return mappingFunction(source.current());
    }

private:
    Sequence source;
    MappingFunction mappingFunction;
};

template<typename MappingFunction>
class map_op : public composable<map_op<MappingFunction>> {
public:
    map_op(MappingFunction mappingFunction) : mappingFunction(mappingFunction) {};

    template<typename InputSequence>
    map_sequence<InputSequence, MappingFunction> operator () (InputSequence&& input) {
        return map_sequence<InputSequence, MappingFunction>(std::move(input), mappingFunction);
    };
private:
    MappingFunction mappingFunction;
};

template<typename MappingFunction>
map_op<MappingFunction> map (MappingFunction mappingFunction) {
    return map_op<MappingFunction>(mappingFunction);
}


/* FILTER */

template<typename Sequence, typename Predicate>
class filter_sequence : public sequence<filter_sequence<Sequence, Predicate>> {
    using item = SequenceItemType<Sequence>;
public:
    filter_sequence(Sequence&& source, Predicate predicateFunction)
            : source(std::move(source)), predicateFunction(predicateFunction), started(false), test(43) {};
    filter_sequence(filter_sequence&& other)
            : source(std::move(other.source)), predicateFunction(other.predicateFunction), started(other.started), test(44) {};
    bool done () {
        if (source.done()) return true;
        else if (!started && ! predicateFunction(source.current())) {
            started = true;
            advance();
            return source.done();
        } else return false;
    };
    void advance () {
        while (!source.done()) {
            source.advance();
            if (predicateFunction(source.current())) return;
        }
    }
    item current() {return source.current();}
private:
    Sequence source;
    Predicate predicateFunction;
    bool started;
    int test;
};

template<typename Predicate>
class filter_op : public composable<filter_op<Predicate>> {
public:
    filter_op (Predicate predicateFunction) : predicateFunction(predicateFunction) {};
    template<typename InputSequence>
    filter_sequence<InputSequence, Predicate> operator() (InputSequence&& input) {
        return filter_sequence<InputSequence, Predicate>(std::move(input), predicateFunction);
    }
private:
    Predicate predicateFunction;
};

template<typename Predicate>
filter_op<Predicate> filter (Predicate predicateFunction) {return filter_op<Predicate>(predicateFunction);};

/* TAKE */

template<typename Sequence>
class take_sequence : public sequence<take_sequence<Sequence>> {
    using item = SequenceItemType<Sequence>;
public:
    take_sequence(Sequence&& source, size_t nToTake) : source(std::move(source)), left(nToTake) {};
    take_sequence(take_sequence&& other) : source(std::move(other.source)), left(other.left) {};
    bool done () {
        return source.done() || left == 0;
    };
    void advance () {
        source.advance();
        left--;
    }
    item current() {
        return source.current();
    }
private:
    Sequence source;
    size_t left;
};

class take : public composable<take> {
public:
    take (size_t nToTake) : nToTake(nToTake) {};
    template<typename InputSequence>
    take_sequence<InputSequence> operator() (InputSequence&& input) {
        return take_sequence<InputSequence>(std::move(input), nToTake);
    }
private:
    size_t nToTake;
};

template<typename Container>
struct to {
    template<typename Sequence>
    Container operator () (Sequence sequence) {
        return Container(sequence.begin(), sequence.end());
    };
};

int main() {
    vector<int> shortVector = {1, 2, 3, 4, 5, 6, 7, 8};

    auto iteration = filter([](int i) {return i % 2 == 0;}) | map([](int i) {return i * 1.1;});
    auto l = from(shortVector) >> (iteration | take(3)) >> to<list<double>>();

    for (auto d : l) {
        cout << d << endl;
    }

    return 0;
}