#pragma once

// must be constructable, destructable, copy constructable, copy assignable, move constructable, move assignable
#define SemiRegular typename
// SemiRegular, and must also be comparable for equality and inequality
#define Regular typename
// SemiRegular, and must be comparable using <, >
// Based on equivalence, not equality.
// And must have following properties.
// irreflexitivity: x<x must be false
// asymytry: if x<y then y<x must be false
// transitivity: if x<y and y<z, then x<z must be true
#define StrictWeakOrdering typename
// Is a StrictWeakOrdering, but uses equality instead of equivalence. Is also Regular. In addition is comparable using
// <= and >=. And holds following additional properties antitymytry: if x<=y and y<=x then x=y totality: One of x<=y or
// y<=x must be true transitivity: if x<=y and y<=z then x<=z must be true
#define TotallyOrdered typename
// TotallyOrdered and also supports standard arithmetic operators. +,+=,-,-=,*,*=,/,/=
#define Arithmetic typename
// Arithmetic and also supports operators %,%=,&,&=,|,|=,^,^=,<<,<<=,>>,>>=
#define IntegerArithmetic typename

// Can only be read. supports increment only.
// Can only be passed through once(i.e. a getting a new iterator from begin would not allow you to re-read previous
// values).
#define InputIterator typename
// Can only be written too. supports increment only.
// Can only be passed through once(i.e. a getting a new iterator from begin would not allow you to re-read previous
// values).
#define OutputIterator typename
// Suports read/write. supports increment only.
// Can be passed through multiple times.
#define ForwardIterator typename
// ForwardIterator plus it support decrement
#define BidirectionalIterator typename
// BidirectionalIterator plus supports random access.
#define RandomAccessIterator typename

// An object that is a collection of values. can use nonmenber begin/end to get iterators to walk through the values
#define Container typename
// A Container that stores its values in a sequence.
#define SequenceContainer typename
// A Container that associates its values with a key.
#define AssociativeContainer typename
// Something that can be called like a function, functor, lambda, std::function, ect.
#define Callable typename
// A Callable that returns true/false
#define Predicate typename
// A Predicate that takes 2 instances of a type and compares them.
#define Comparator typename