#include <algorithm>
#include <iostream>
#include <type_traits>
#include <typeinfo>


template<size_t i, typename TupleType>
struct TupleElementType;

template<typename ...Types>
class Tuple;

template<size_t i, typename TupleType>
using TupleElementType_t = typename TupleElementType<i, TupleType>::type ;

template<size_t i, typename ...Types>
std::add_lvalue_reference_t<TupleElementType_t<i, Tuple<Types...>>> get(Tuple<Types...>& tuple);

template<typename FirstType, typename ...OtherTypes>
class Tuple<FirstType, OtherTypes...>{
public:
    FirstType value;
    Tuple<OtherTypes...> recentTuple;

public:
    explicit Tuple() : value(FirstType()), recentTuple(Tuple<OtherTypes...>()) {}
    explicit Tuple(const FirstType& firstArg, const OtherTypes&... args) :
            value(firstArg), recentTuple(args...) {}
    explicit Tuple(FirstType&& firstArg, OtherTypes&&... args) :
            value(std::move(firstArg)), recentTuple(std::move(args)...) {}
    Tuple(const Tuple& other) : value(other.value), recentTuple(other.recentTuple) {}
    Tuple(Tuple&& other) : value(std::move(other.value)), recentTuple(std::move(other.recentTuple)) {}
    Tuple(const FirstType& v, const Tuple<OtherTypes...>& tuple) : value(v), recentTuple(tuple) {}

    Tuple& operator= (const Tuple& other) {
        value = other.value;
        recentTuple = other.recentTuple;
        return *this;
    }
    Tuple& operator= (Tuple&& other) {
        value = std::move(other.value);
        recentTuple = std::move(other.recentTuple);
        return *this;
    }

    void swap(Tuple& other) {
        std::swap(value, other.value);
        recentTuple.swap(other.recentTuple);
    }

    template <size_t i, typename ...Types>
    friend struct GetTuple;

    template<typename T, typename ...Types>
    friend struct GetTuple2;

    template<typename ...FirstTypes, typename ..._OtherTypes>
    friend bool operator<= (const Tuple<FirstTypes...>& firstTuple, const Tuple<_OtherTypes...>& secondTuple);

    template<typename ...FirstTypes, typename ..._OtherTypes>
    friend bool operator== (const Tuple<FirstTypes...>&, const Tuple<_OtherTypes...>&);
};

template<>
class Tuple<> {
public:
    void swap(Tuple<>&) const{}
};

template<typename ...Types>
Tuple<typename std::decay_t<Types>...> makeTuple(Types&&... args) {
    return Tuple<typename std::decay_t<Types>...>(std::forward<Types>(args)...);
}

template<typename FirstType, typename ...OtherTypes>
struct TupleElementType<0, Tuple<FirstType, OtherTypes...>> {
    typedef FirstType type;
};

template<size_t i, typename FirstType, typename ...OtherTypes>
struct TupleElementType<i, Tuple<FirstType, OtherTypes...>> {
    typedef typename TupleElementType<i-1, Tuple<OtherTypes...>>::type type;
};


template <size_t i, typename ...Types>
struct GetTuple;

template <size_t i, typename FirstType, typename ...OtherTypes>
struct GetTuple<i, FirstType, OtherTypes...> {
    static std::add_lvalue_reference_t<TupleElementType_t<i, Tuple<FirstType, OtherTypes...>>>
    getLValue(Tuple<FirstType, OtherTypes...>& tuple) {
        return GetTuple<i-1, OtherTypes...>::getLValue(tuple.recentTuple);
    }
    static const std::add_lvalue_reference_t<TupleElementType_t<i, Tuple<FirstType, OtherTypes...>>>
    getConstLValue(Tuple<FirstType, OtherTypes...>& tuple) {
        return GetTuple<i-1, OtherTypes...>::getConstLValue(tuple.recentTuple);
    }
    static std::add_rvalue_reference_t<TupleElementType_t<i, Tuple<FirstType, OtherTypes...>>>
    getRValue(Tuple<FirstType, OtherTypes...>&& tuple) {
        return std::move(GetTuple<i-1, OtherTypes...>::getRValue(std::move(tuple.recentTuple)));
    }
};
template <typename FirstType, typename ...OtherTypes>
struct GetTuple<0, FirstType, OtherTypes...> {
    static std::add_lvalue_reference_t<TupleElementType_t<0, Tuple<FirstType, OtherTypes...>>>
    getLValue(Tuple<FirstType, OtherTypes...>& tuple) {
        return tuple.value;
    }
    static const std::add_lvalue_reference_t<TupleElementType_t<0, Tuple<FirstType, OtherTypes...>>>
    getConstLValue(const Tuple<FirstType, OtherTypes...>& tuple) {
        return tuple.value;
    }
    static std::add_rvalue_reference_t<TupleElementType_t<0, Tuple<FirstType, OtherTypes...>>>
    getRValue(Tuple<FirstType, OtherTypes...>&& tuple) {
        return std::move(tuple.value);
    }
};

template<size_t i, typename ...Types>
std::add_lvalue_reference_t<TupleElementType_t<i, Tuple<Types...>>>
get(Tuple<Types...>& tuple) {
    return GetTuple<i, Types...>::getLValue(tuple);
};

template<size_t i, typename ...Types>
const std::add_lvalue_reference_t<TupleElementType_t<i, Tuple<Types...>>>
get(const Tuple<Types...>& tuple) {
    return GetTuple<i, Types...>::getConstLValue(tuple);
};

template<size_t i, typename ...Types>
std::add_rvalue_reference_t<TupleElementType_t<i, Tuple<Types...>>>
get(Tuple<Types...>&& tuple) {
    return GetTuple<i, Types...>::getRValue(std::move(tuple));
};

template<typename T, typename ...Types>
struct GetTuple2;

template<typename T, typename FirstType, typename ...Types>
struct GetTuple2<T, FirstType, Types...> {
    static T& getLValue(Tuple<FirstType, Types...> &tuple) {
        return GetTuple2<T, Types...>::getLValue(tuple.recentTuple);
    }
    static const T& getConstLValue(const Tuple<FirstType, Types...> &tuple) {
        return GetTuple2<T, Types...>::getLValue(tuple.recentTuple);
    }
    static T&& getRValue(Tuple<FirstType, Types...>&& tuple) {
        return std::move(GetTuple2<T, Types...>::getLValue(tuple.recentTuple));
    }
};

template<typename T, typename ...Types>
struct GetTuple2<T, T, Types...> {
    static T& getLValue(Tuple<T, Types...> &tuple) {
        return tuple.value;
    }
    static const T& getConstLValue(const Tuple<T, Types...> &tuple) {
        return tuple.value;
    }
    static T&& getRValue(Tuple<T, Types...>&& tuple) {
        return std::move(tuple.value);
    }
};

template<typename T, typename ...Types>
T& get(Tuple<Types...>& tuple) {
    return GetTuple2<T, Types...>::getLValue(tuple);
};

template<typename T, typename ...Types>
const T& get(const Tuple<Types...>& tuple) {
    return GetTuple2<T, Types...>::getConstLValue(tuple);
};

template<typename T, typename ...Types>
T&& get(Tuple<Types...>&& tuple) {
    return std::move(GetTuple2<T, Types...>::getRValue(std::move(tuple)));
};

template<typename ...FirstTypes, typename ..._OtherTypes>
bool operator== (const Tuple<FirstTypes...>& firstTuple, const Tuple<_OtherTypes...>& secondTuple);

template<typename FirstType, typename ...FirstTypes, typename SecondType, typename ...OtherTypes>
bool operator== (const Tuple<FirstType, FirstTypes...>& firstTuple, const Tuple<SecondType, OtherTypes...>& secondTuple) {
    return (firstTuple.value == secondTuple.value) && (firstTuple.recentTuple == secondTuple.recentTuple);
}

template<typename Type, typename ...FirstType>
bool operator== (const Tuple<Type, FirstType...>&, const Tuple<>&) {
    return false;
}
template<typename Type, typename ...SecondType>
bool operator== (const Tuple<>&, const Tuple<Type, SecondType...>&) {
    return false;
}
template<>
bool operator== (const Tuple<>&, const Tuple<>&){
    return true;
}


template<typename ...FirstTypes, typename ...OtherTypes>
bool operator!= (const Tuple<FirstTypes...>& firstTuple, const Tuple<OtherTypes...>& secondTuple) {
    return !(firstTuple == secondTuple);
};


template<typename FirstType, typename ...FirstTypes, typename SecondType, typename ...OtherTypes>
bool operator<= (const Tuple<FirstType, FirstTypes...>& firstTuple, const Tuple<SecondType, OtherTypes...>& secondTuple) {
    if (firstTuple.value < secondTuple.value)
        return true;
    if (firstTuple.value == secondTuple.value)
        return operator<=(firstTuple.recentTuple, secondTuple.recentTuple);
    return false;
};

template<typename ...FirstTypes>
bool operator<= (const Tuple<FirstTypes...>&, const Tuple<>&) {
    return false;
}
template<typename ...SecondTypes>
bool operator<= (const Tuple<>&, const Tuple<SecondTypes...>&) {
    return true;
}
template<typename ...FirstTypes, typename ...OtherTypes>
bool operator<=(const Tuple<>&, const Tuple<>&) {
    return true;
}

template<typename ...FirstTypes, typename ...OtherTypes>
bool operator>= (const Tuple<FirstTypes...>& firstTuple, const Tuple<OtherTypes...>& secondTuple) {
    return (secondTuple <= firstTuple);
};

template<typename ...FirstTypes, typename ...OtherTypes>
bool operator< (const Tuple<FirstTypes...>& firstTuple, const Tuple<OtherTypes...>& secondTuple) {
    return !(secondTuple <= firstTuple);
};

template<typename ...FirstTypes, typename ...OtherTypes>
bool operator> (const Tuple<FirstTypes...>& firstTuple, const Tuple<OtherTypes...>& secondTuple) {
    return !(firstTuple <= secondTuple);
};



template <typename ...Types>
struct tupleCatHelper;

template <typename ...FirstTypes, typename ...OtherTypes>
struct tupleCatHelper<Tuple<FirstTypes...>, Tuple<OtherTypes...>> {
    typedef Tuple<FirstTypes..., OtherTypes...> catType;
};

template <typename ...FirstTypes, typename ...OtherTypes, typename ...OtherTupleTypes>
struct tupleCatHelper<Tuple<FirstTypes...>, Tuple<OtherTypes...>, OtherTupleTypes...> {
    typedef typename tupleCatHelper<Tuple<FirstTypes..., OtherTypes...>, OtherTupleTypes...>::catType catType;
};

template <typename ...Types>
struct tupleCatHelper<Tuple<Types...>> {
    typedef Tuple<Types...> catType;
};


template<typename ...TupleTypes>
struct TupleCat;


template<typename FirstType, typename ...FirstTypes>
struct TupleCat<Tuple<FirstType, FirstTypes...>> {
    template<typename ...OtherTypes>
    static Tuple<FirstType, FirstTypes..., OtherTypes...> value(const Tuple<FirstType, FirstTypes...>& firstTuple,
                                                                const Tuple<OtherTypes...>& secondTuple) {
        return Tuple<FirstType, FirstTypes..., OtherTypes...>
                (firstTuple.value, TupleCat<Tuple<FirstTypes...>>::value(firstTuple.recentTuple, secondTuple));
    }
    template<typename ...SecondTypes, typename ...OtherTupleTypes>
    static typename tupleCatHelper<Tuple<FirstType, FirstTypes...>, Tuple<SecondTypes...>, OtherTupleTypes...>::catType
    value(const Tuple<FirstType, FirstTypes...>& firstTuple, const Tuple<SecondTypes...>& secondTuple, const OtherTupleTypes&... other) {
        return TupleCat<Tuple<FirstType, FirstTypes..., SecondTypes...>>::value(
                TupleCat<Tuple<FirstType, FirstTypes...>>::value(firstTuple, secondTuple), other...);
    };
};

template<>
struct TupleCat<Tuple<>> {
    template<typename ...OtherTypes>
    static Tuple<OtherTypes...> value(const Tuple<>&, const Tuple<OtherTypes...>& secondTuple) {
        return secondTuple;
    }
};



template <typename FirstTupleType, typename ...OtherTupleTypes>
typename tupleCatHelper<FirstTupleType, OtherTupleTypes...>::catType tupleCat
        (const FirstTupleType& firstTuple, const OtherTupleTypes&... otherTuples) {
    return TupleCat<FirstTupleType>::value(firstTuple, otherTuples...);
}


