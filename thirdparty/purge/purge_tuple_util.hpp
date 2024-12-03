//
// Created by Michael Burzan on 07.07.20.
//

#ifndef PTEST_PURGE_TUPLE_UTIL_HPP
#define PTEST_PURGE_TUPLE_UTIL_HPP

#define MAKE_TUPE(x) std::make_tuple x

#define _GET_NTH_ARG(_1, _2, _3, _4, _5, N, ...) N

#define _fe_0(_call, ...)
#define _fe_1(_call, x) _call( x )
#define _fe_2(_call, x, ...) _call( x ), _fe_1(_call, __VA_ARGS__)
#define _fe_3(_call, x, ...) _call( x ), _fe_2(_call, __VA_ARGS__)
#define _fe_4(_call, x, ...) _call( x ), _fe_3(_call, __VA_ARGS__)

#define CALL_MACRO_X_FOR_EACH(x, ...) \
    _GET_NTH_ARG("ignored", ##__VA_ARGS__, \
    _fe_4, _fe_3, _fe_2, _fe_1, _fe_0)(x, ##__VA_ARGS__)



namespace purge {

    template<typename T, typename... Ts>
    auto tuple_head(std::tuple<T, Ts...> t) {
        return std::get<0>(t);
    }

    template<std::size_t... Ns, typename... Ts>
    auto tuple_tail_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t) {
        return std::make_tuple(std::get<Ns + 1u>(t)...);
    }

    template<typename... Ts>
    auto tuple_tail(std::tuple<Ts...> t) {
        return tuple_tail_impl(std::make_index_sequence<sizeof...(Ts) - 1u>(), t);
    }

    template<class TUPLE_T>
    auto get_parameter_from_tuple(const TUPLE_T &para_result) {
        return tuple_tail(para_result);
    }
}

#endif //PTEST_PURGE_TUPLE_UTIL_HPP
