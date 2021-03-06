#ifndef __SKIPLIST__
#define __SKIPLIST__

/*
    This file contains the skiplist implementation.
    To use the skiplist, you must provide a max key,
    all keys which are larger than the maxkey will be ditched.
    If you choose the default maxkey, it probably won't work as you expected.
*/

#include <cmath>

__DST_BEGIN_NAMESPACE

template <class _Key, class _Val>
struct __skiplist_node {
    typedef pair<_Key, _Val> entry;
    typedef __skiplist_node<_Key, _Val> *__link_type;

    entry _entry;
    int _height;
    __link_type *_M_nexts; // 1D array of pointers

    __skiplist_node(entry &e, int __size = 0) : _entry(e) {
        _M_nexts = new __link_type[__size];
        _height = __size;
    }
    ~__skiplist_node() { delete[] _M_nexts; }
};

template <class _Key, class _Val>
struct __skip_list_iterator {
    typedef __skiplist_node<_Key, _Val> __node;
    typedef __node *__link_type;
    typedef pair<_Key, _Val> entry;
    typedef __skip_list_iterator __self;

    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef forward_iterator_tag iterator_category;
    typedef entry value_type;
    typedef value_type *pointer;
    typedef value_type &reference;

    __link_type _M_node;

    __skip_list_iterator() {}
    __skip_list_iterator(__link_type p) : _M_node(p) {}

    reference operator*() { return _M_node->_entry; }
    pointer operator->() { return &(operator*()); }
    __self &operator++() {
        _M_node = _M_node->_M_nexts[0];
        return *this;
    }
    __self operator++(int) {
        __self __tmp = *this;
        ++*this;
        return __tmp;
    }
    bool operator==(__self &__x) const { return _M_node == __x._M_node; }
    bool operator!=(__self &__x) const { return _M_node != __x._M_node; }
};

template <class _Key, class _Val = int>
class skiplist {
  protected:
    typedef __skiplist_node<_Key, _Val> __node;
    typedef __node *__link_type;

  public:
    typedef pair<_Key, _Val> entry;
    typedef entry *enrty_ptr;
    typedef size_t size_type;
    typedef _Val value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef ptrdiff_t difference_type;

    typedef __skip_list_iterator<_Key, _Val> iterator;
    typedef __skip_list_iterator<const _Key, const _Val> const_iterator;

    skiplist();
    skiplist(_Key __max_key = _Key(), size_t __max_size = 10000000);
    ~skiplist();

    size_t level() const { return _level; }
    size_t size() const { return _size; }
    bool empty() const { return _size == 0; }

    void clear();

    entry *get(_Key &);

    void erase(_Key &);

    void insert(entry);

    bool count(_Key &the_key) { return get(the_key) != nullptr; }

    inline void insert(_Key k, _Val v) {
        entry e = entry(k, v);
        insert(e);
    }
    inline void insert(_Key &k, _Val &v) {
        entry e = entry(k, v);
        insert(e);
    }

    reference operator[](_Key key) {
        entry *p = get(key);
        if (p)
            return p->second;
        insert(key, _Val());
        return get(key)->second;
    }

    iterator begin() { return iterator(_M_start->_M_nexts[0]); }
    iterator end() { return iterator(_M_finish); }

    void print();

  protected:
    // generate a level number by 0.5 probability
    size_t __decide_level();

    __link_type __search(_Key &);

    size_t _level;     // max current nonempty chain
    size_t _size;      // number of pairs in dictionary
    size_t _max_level; // max permissible chain level

    _Key __null_key; // a max key

    __link_type _M_start;     // dummy header node pointer
    __link_type _M_finish;    // dummy tail node pointer
    __link_type _M_previous;  // every time operate, remember the pre node.
    __link_type _M_next_node; //  every time operate, remember the next node.
    __link_type *_M_last_checked_arr; // [i] = node seen on level i
};

template <class _Key, class _Val>
skiplist<_Key, _Val>::skiplist(_Key __max_key, size_t __max_size) {
    // Constructor for skip lists with keys smaller than __max_key
    _max_level = (size_t)ceil(log2(__max_size)) - 1;
    _level = _size = 0;
    __null_key = __max_key;

    entry __null_entry;
    __null_entry.first = __null_key;
    _M_start = new __node(__null_entry, _max_level + 1);
    _M_finish = new __node(__null_entry);
    _M_last_checked_arr = new __link_type[_max_level + 1];

    // header points to tail at all _level as lists are empty
    fill(_M_start->_M_nexts, _M_start->_M_nexts + _max_level + 1, _M_finish);
}

template <class _Key, class _Val>
void skiplist<_Key, _Val>::clear() {
    _M_next_node = _M_start->_M_nexts[0];
    __link_type tmp;
    while (_M_next_node != _M_finish) {
        tmp = _M_next_node;
        _M_next_node = _M_next_node->_M_nexts[0];
        delete tmp;
    }
    for (int i = 0; i <= _max_level; i++)
        _M_start->_M_nexts[i] = _M_finish;
    _size = 0;
    _level = 0;
}

template <class _Key, class _Val>
skiplist<_Key, _Val>::~skiplist() {
    // delete all nodes by following level 0 chain
    while (_M_start != _M_finish) {
        _M_next_node = _M_start->_M_nexts[0];
        delete _M_start;
        _M_start = _M_next_node;
    }
    delete _M_finish;
    delete[] _M_last_checked_arr;
}

template <class _Key, class _Val>
typename skiplist<_Key, _Val>::enrty_ptr
skiplist<_Key, _Val>::get(_Key &the_key) {
    if (the_key >= __null_key)
        return nullptr; // no matching pair possible

    _M_previous = _M_start;
    for (int i = _level; i > -1; i--) // go down _level
        while (_M_previous->_M_nexts[i]->_entry.first < the_key)
            _M_previous = _M_previous->_M_nexts[i];

    if (_M_previous->_M_nexts[0]->_entry.first == the_key)
        return &_M_previous->_M_nexts[0]->_entry;

    return nullptr; // no matching pair
}

template <class _Key, class _Val>
void skiplist<_Key, _Val>::erase(_Key &the_key) {
    // Delete the pair, if any, whose key equals the_key.
    if (the_key >= __null_key) // too large
        return;

    __link_type __the_node = __search(the_key);
    if (__the_node->_entry.first != the_key) // not present
        return;

    // delete node from skip list
    for (int i = 0;
         i <= _level && _M_last_checked_arr[i]->_M_nexts[i] == __the_node; i++)
        _M_last_checked_arr[i]->_M_nexts[i] = __the_node->_M_nexts[i];

    // update _level
    while (_level > 0 && _M_start->_M_nexts[_level] == _M_finish)
        _level--;

    delete __the_node;
    _size--;
}

template <class _Key, class _Val>
void skiplist<_Key, _Val>::insert(entry __entry) {
    if (__entry.first >= __null_key) // key too large
    {
        return;
    }

    __link_type __the_node = __search(__entry.first);
    if (__the_node->_entry.first ==
        __entry.first) { // update __the_node->_entry.second
        __the_node->_entry.second = __entry.second;
        return;
    }

    size_t _new_level = __decide_level();
    // fix _new_level to be <= _level + 1
    if (_new_level > _level) {
        _new_level = ++_level;
        _M_last_checked_arr[_new_level] = _M_start;
    }

    __link_type new_node = new __node(__entry, _new_level + 1);
    ++_size;
    for (int i = 0; i <= _new_level; i++) { // insert into level i chain
        new_node->_M_nexts[i] = _M_last_checked_arr[i]->_M_nexts[i];
        _M_last_checked_arr[i]->_M_nexts[i] = new_node;
    }
}

template <class _Key, class _Val>
size_t skiplist<_Key, _Val>::__decide_level() {
    size_t __lev = 0;
    while (rand() & 1)
        ++__lev;
    return (__lev < _max_level) ? __lev : _max_level;
}

template <class _Key, class _Val>
typename skiplist<_Key, _Val>::__link_type
skiplist<_Key, _Val>::__search(_Key &the_key) {
    _M_previous = _M_start;
    for (int i = _level; i > -1; i--) {
        while (_M_previous->_M_nexts[i]->_entry.first < the_key)
            _M_previous = _M_previous->_M_nexts[i];
        _M_last_checked_arr[i] =
            _M_previous; // _M_last_checked_arr level i node seen
    }
    return _M_previous->_M_nexts[0];
}

template <class _Key, class _Val>
void skiplist<_Key, _Val>::print() {
    std::cout << "(begin h:" << _M_start->_height << ")->";
    for (__link_type _walk = _M_start->_M_nexts[0]; _walk != _M_finish;
         _walk = _walk->_M_nexts[0])
        std::cout << "(" << _walk->_entry.first << " h:" << _walk->_height
                  << ")->";
    std::cout << "(end)" << endl;
}

__DST_END_NAMESPACE

#endif