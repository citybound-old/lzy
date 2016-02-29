#ifndef LZY_PUSHABLE_H
#define LZY_PUSHABLE_H

namespace lzy {

    template<typename Derived>
    class pushable {
    public:
        template<typename Target>
        using TargetResult = typename std::result_of<Target(Derived)>::type;

        template<typename Target>
        TargetResult<Target> operator>>(Target &&target) {
            return target(std::move(static_cast<Derived &>(*this)));
        }
    };

}

#endif //LZY_PUSHABLE_H
