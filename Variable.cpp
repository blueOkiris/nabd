/*
 * Author: Dylan Turner
 * Description:
 *   Define the built-in types used in the language
 *   and whatever conversion funcs are necessary
 */

#include <string>
#include <utility>
#include <vector>

using namespace nabd;

/*
 * For now, we assume that
 *  - nabd string -> std::string
 *  - nabd number -> double
 *  - nabd list<T> -> std::vector<T>
 *  - nabd pair<T1, T2> -> std::pair<T1, T2>
 * So we just need conversions
 * With these conversions, all variables should be able to be passed into any function
 */

/* Covert to numbers */

inline double toNum(const double num) {
    return num;
}

inline double toNum(const std::string &str) {
    return str.length() > 0 ? static_cast<double>(str[0]) : 0;
}

template<class T1, class T2>
inline double toNum(const std::pair<T1, T2> &tup) {
    return toNum(tup.first);
}

template<class T>
inline double toNum(const std::vector<T> &ls) {
    return ls.size() > 0 ? toNum(ls[0]) : 0;
}

/* Convert to strings */

inline std::string toStr(const std::string &str) {
    return str;
}

inline std::string toStr(const double num) {
    return "" + num;
}

template<class T1, class T2>
inline std::string toStr(const std::pair<T1, T2> &tup) {
    return "(" + toStr(tup.first) + ", " + toStr(tup.second) + ")";
}

/* Convert to list */

template<class T>
inline std::vector<T> toLs(const std::vector<T> &ls) {
    return ls;
}

template<class T1, class T2>
inline std::vector<T1, T2> toLs(const std::pair<T1, T2> &tup) {
    return toLs(tup.first);
}

template<class T>
inline std::vector<T> toLs(const T &item) {
    return std::vector<T>({ item });
}

/* Convert to tuple */

template<class T1, class T2>
inline std::pair<T1, T2> toTup(const std::pair<T1, T2> &tup) {
    return tup;
}

template<class T>
inline std::pair<T, T> toTup(const T &item) {
    return std::make_pair<T>(item, item);
}

