
/* NOTE: This is an internal header file, included by other DST headers.
 *   You should not attempt to use it directly.
 */

#ifndef ___DST_INTERNAL_UNINITIALIZED_H
#define ___DST_INTERNAL_UNINITIALIZED_H

__DST_BEGIN_NAMESPACE

// uninitialized_copy

// Valid if copy construction is equivalent to assignment, and if the
//  destructor is trivial.
template <class _InputIter, class _ForwardIter>
inline _ForwardIter
__uninitialized_copy_aux(_InputIter _first, _InputIter _last,
                         _ForwardIter __result, __true_type) {
    return copy(_first, _last, __result);
}

template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy_aux(_InputIter _first, _InputIter _last,
                                      _ForwardIter __result, __false_type) {
    _ForwardIter __cur = __result;
    __DST_TRY {
        for (; _first != _last; ++_first, ++__cur)
            _Construct(&*__cur, *_first);
        return __cur;
    }
    __DST_UNWIND(_Destroy(__result, __cur));
}

template <class _InputIter, class _ForwardIter, class _Tp>
inline _ForwardIter __uninitialized_copy(_InputIter _first, _InputIter _last,
                                         _ForwardIter __result, _Tp *) {
    typedef typename __type_traits<_Tp>::is_POD_type _Is_POD;
    return __uninitialized_copy_aux(_first, _last, __result, _Is_POD());
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter uninitialized_copy(_InputIter _first, _InputIter _last,
                                       _ForwardIter __result) {
    return __uninitialized_copy(_first, _last, __result,
                                __VALUE_TYPE(__result));
}

inline char *uninitialized_copy(const char *_first, const char *_last,
                                char *__result) {
    memmove(__result, _first, _last - _first);
    return __result + (_last - _first);
}

inline wchar_t *uninitialized_copy(const wchar_t *_first, const wchar_t *_last,
                                   wchar_t *__result) {
    memmove(__result, _first, sizeof(wchar_t) * (_last - _first));
    return __result + (_last - _first);
}

// uninitialized_copy_n (not part of the DST standard)

template <class _InputIter, class _Size, class _ForwardIter>
pair<_InputIter, _ForwardIter>
__uninitialized_copy_n(_InputIter _first, _Size __count, _ForwardIter __result,
                       input_iterator_tag) {
    _ForwardIter __cur = __result;
    __DST_TRY {
        for (; __count > 0; --__count, ++_first, ++__cur)
            _Construct(&*__cur, *_first);
        return pair<_InputIter, _ForwardIter>(_first, __cur);
    }
    __DST_UNWIND(_Destroy(__result, __cur));
}

template <class _RandomAccessIter, class _Size, class _ForwardIter>
inline pair<_RandomAccessIter, _ForwardIter>
__uninitialized_copy_n(_RandomAccessIter _first, _Size __count,
                       _ForwardIter __result, random_access_iterator_tag) {
    _RandomAccessIter _last = _first + __count;
    return pair<_RandomAccessIter, _ForwardIter>(
        _last, uninitialized_copy(_first, _last, __result));
}

template <class _InputIter, class _Size, class _ForwardIter>
inline pair<_InputIter, _ForwardIter>
__uninitialized_copy_n(_InputIter _first, _Size __count,
                       _ForwardIter __result) {
    return __uninitialized_copy_n(_first, __count, __result,
                                  __ITERATOR_CATEGORY(_first));
}

template <class _InputIter, class _Size, class _ForwardIter>
inline pair<_InputIter, _ForwardIter>
uninitialized_copy_n(_InputIter _first, _Size __count, _ForwardIter __result) {
    return __uninitialized_copy_n(_first, __count, __result,
                                  __ITERATOR_CATEGORY(_first));
}

// Valid if copy construction is equivalent to assignment, and if the
// destructor is trivial.
template <class _ForwardIter, class _Tp>
inline void __uninitialized_fill_aux(_ForwardIter _first, _ForwardIter _last,
                                     const _Tp &__x, __true_type) {
    fill(_first, _last, __x);
}

template <class _ForwardIter, class _Tp>
void __uninitialized_fill_aux(_ForwardIter _first, _ForwardIter _last,
                              const _Tp &__x, __false_type) {
    _ForwardIter __cur = _first;
    __DST_TRY {
        for (; __cur != _last; ++__cur)
            _Construct(&*__cur, __x);
    }
    __DST_UNWIND(_Destroy(_first, __cur));
}

template <class _ForwardIter, class _Tp, class _Tp1>
inline void __uninitialized_fill(_ForwardIter _first, _ForwardIter _last,
                                 const _Tp &__x, _Tp1 *) {
    typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
    __uninitialized_fill_aux(_first, _last, __x, _Is_POD());
}

template <class _ForwardIter, class _Tp>
inline void uninitialized_fill(_ForwardIter _first, _ForwardIter _last,
                               const _Tp &__x) {
    __uninitialized_fill(_first, _last, __x, __VALUE_TYPE(_first));
}

// Valid if copy construction is equivalent to assignment, and if the
//  destructor is trivial.
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n_aux(_ForwardIter _first, _Size __n,
                                               const _Tp &__x, __true_type) {
    return fill_n(_first, __n, __x);
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n_aux(_ForwardIter _first, _Size __n,
                                        const _Tp &__x, __false_type) {
    _ForwardIter __cur = _first;
    __DST_TRY {
        for (; __n > 0; --__n, ++__cur)
            _Construct(&*__cur, __x);
        return __cur;
    }
    __DST_UNWIND(_Destroy(_first, __cur));
}

template <class _ForwardIter, class _Size, class _Tp, class _Tp1>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter _first, _Size __n,
                                           const _Tp &__x, _Tp1 *) {
    typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
    return __uninitialized_fill_n_aux(_first, __n, __x, _Is_POD());
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter _first, _Size __n,
                                         const _Tp &__x) {
    return __uninitialized_fill_n(_first, __n, __x, __VALUE_TYPE(_first));
}

// Extensions: __uninitialized_copy_copy, __uninitialized_copy_fill,
// __uninitialized_fill_copy.

// __uninitialized_copy_copy
// Copies [first1, last1) into [result, result + (last1 - first1)), and
//  copies [first2, last2) into
//  [result, result + (last1 - first1) + (last2 - first2)).

template <class _InputIter1, class _InputIter2, class _ForwardIter>
inline _ForwardIter
__uninitialized_copy_copy(_InputIter1 __first1, _InputIter1 __last1,
                          _InputIter2 __first2, _InputIter2 __last2,
                          _ForwardIter __result) {
    _ForwardIter __mid = uninitialized_copy(__first1, __last1, __result);
    __DST_TRY { return uninitialized_copy(__first2, __last2, __mid); }
    __DST_UNWIND(_Destroy(__result, __mid));
}

// __uninitialized_fill_copy
// Fills [result, mid) with x, and copies [first, last) into
//  [mid, mid + (last - first)).
template <class _ForwardIter, class _Tp, class _InputIter>
inline _ForwardIter
__uninitialized_fill_copy(_ForwardIter __result, _ForwardIter __mid,
                          const _Tp &__x, _InputIter _first, _InputIter _last) {
    uninitialized_fill(__result, __mid, __x);
    __DST_TRY { return uninitialized_copy(_first, _last, __mid); }
    __DST_UNWIND(_Destroy(__result, __mid));
}

// __uninitialized_copy_fill
// Copies [first1, last1) into [first2, first2 + (last1 - first1)), and
//  fills [first2 + (last1 - first1), last2) with x.
template <class _InputIter, class _ForwardIter, class _Tp>
inline void __uninitialized_copy_fill(_InputIter __first1, _InputIter __last1,
                                      _ForwardIter __first2,
                                      _ForwardIter __last2, const _Tp &__x) {
    _ForwardIter __mid2 = uninitialized_copy(__first1, __last1, __first2);
    __DST_TRY { uninitialized_fill(__mid2, __last2, __x); }
    __DST_UNWIND(_Destroy(__first2, __mid2));
}

__DST_END_NAMESPACE

#endif /* ___DST_INTERNAL_UNINITIALIZED_H */