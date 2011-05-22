/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef CILABNAMESPACEMACROS_H
#define CILABNAMESPACEMACROS_H

//! This macro opens namespace X. It is used to prevent indenting the code living inside namespace brackets.
#define CILAB_BEGIN_NAMESPACE(X) namespace X {
//! This macro end namespace X. It is used to prevent indenting the code living inside namespace brackets.
#define CILAB_END_NAMESPACE(X) } // namespace X
//! This macro opens namespace X::Y. It is used to prevent indenting the code living inside namespace brackets.
#define CILAB_BEGIN_NAMESPACES(X, Y) namespace X { namespace Y {
//! This macro end namespace X::Y. It is used to prevent indenting the code living inside namespace brackets.
#define CILAB_END_NAMESPACES(X, Y) } } // namespace X, namespace Y
//! This macro opens namespace X::Y::Z. It is used to prevent indenting the code living inside namespace brackets.
#define CILAB_BEGIN_NAMESPACE3(X, Y, Z) namespace X { namespace Y { namespace Z {
//! This macro end namespace X::Y::Z. It is used to prevent indenting the code living inside namespace brackets.
#define CILAB_END_NAMESPACE3(X, Y, Z) } } } // namespace X, namespace Y , namespace Z


#endif //CILABNAMESPACEMACROS_H
