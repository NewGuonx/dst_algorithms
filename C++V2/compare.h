
template <class T>
static bool lt(T *a, T *b) { return lt(*a, *b); } //less than
template <class T>
static bool lt(T &a, T &b) { return a < b; } //less than
template <class T>
static bool eq(T *a, T *b) { return eq(*a, *b); } //equal
template <class T>
static bool eq(T &a, T &b) { return a == b; } //equal