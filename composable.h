#ifndef LZY_COMPOSABLE_H
#define LZY_COMPOSABLE_H

namespace lzy {

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

    template<typename FirstComposable, typename SecondComposable>
    class owning_composition : public composable<owning_composition<FirstComposable, SecondComposable>> {
    public:
        owning_composition (FirstComposable&& first, SecondComposable&& second)
                : first(std::move(first)), second(std::move(second)) {};

        template<typename Input>
        using FirstResult = typename std::result_of<FirstComposable(Input)>::type;

        template<typename Input>
        using EndResult = typename std::result_of<SecondComposable(FirstResult<Input>)>::type;

        template<typename Input>
        EndResult<Input> operator() (Input&& input) {
            return second(first(std::move(input)));
        }
    private:
        FirstComposable first;
        SecondComposable second;
    };
}


#endif //LZY_COMPOSABLE_H
