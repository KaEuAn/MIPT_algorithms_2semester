#include <type_traits>

template <size_t m, size_t i, size_t mask>
struct A;

template <size_t i, size_t m, size_t mask>
struct conditionLoop {
    static constexpr bool value_func() {
        return std::conditional_t<(mask & (1 << i)), conditionLoop<i + 1, m, mask>,
                std::conditional_t<(i + 1 == m || mask & 1 << (i + 1)), std::false_type,
                        conditionLoop<i + 1 + 1, m, mask>>>::value;
    }
    static constexpr bool value = value_func();
};

template<size_t m, size_t mask>
struct conditionLoop<m, m, mask> {
    static constexpr bool value = true;
};


template <int m, size_t mask, size_t newMask>
struct IsRight{
    static constexpr bool condition() {
        return std::conditional_t<mask & newMask, std::false_type,
                conditionLoop<0, m, mask + newMask>>::value;
    }
    static constexpr int value = condition();
};

template<size_t m, size_t i, size_t newMask, size_t mask>
struct thirdLoop {
    static constexpr long long element() {
        return (thirdLoop<m, i, newMask, mask - 1>::value +
                        A<m, i - 1, (1 << m) - mask>::value * IsRight<m, (1 << m) - mask, newMask>::value)
               % MODULUS;
    }
    static constexpr long long value = element();
};

template<size_t m, size_t i, size_t newMask>
struct thirdLoop<m, i, newMask, 0> {
    static constexpr long long value = 0;
};


template <size_t m, size_t i, size_t mask>
struct A{
    static constexpr long long value = thirdLoop<m, i, mask, (1 << m)>::value;
};

template <size_t m, size_t mask>
struct A<m, 0, mask> {
    static constexpr long long value = 0;
};

template <size_t m>
struct A<m, 0, 0> {
    static constexpr long long value = 1;
};

template <size_t m, size_t n>
struct AhalaiMahalai {
    static constexpr long long value = std::conditional_t<(m < n),A<m, n, 0>, A<n, m, 0>>::value;
};