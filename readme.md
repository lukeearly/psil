# `psil` - the language

[![Run on Repl.it](https://repl.it/badge/github/lukeearly/psil)](https://repl.it/github/lukeearly/psil)

`psil` is a modern functional language, taking inspiration from the lambda calculus of Alonzo Church and from the LISP family of programming languages. 

## Left-recursivity and Currying

`psil` code looks a lot like code in LISP, in that functions always come first in a list. For example, the expression `(+ 1 2)` is correct both `psil` and most dialects of LISP, evaluating to `3`. Also similar to LISP is the line comment, denoted by `;`. Everything following the semicolon is disregarded by the reader.

One of the most prominent features that sets `psil` apart from LISP languages like Scheme is that lists are left recursive to allow for an elegant representation of curried functions in the code, and a more intuitive direction of expansion for algorithms on lists.

For example, from _Common Lisp the Language, 2nd Edition_:
> A list is recursively defined to be either the empty list or a cons whose _cdr_ component is a list. A list is therefore a chain of conses linked by their _cdr_ components and terminated by `nil`, the empty list. The _car_ components of the conses are called the _elements_ of the list. For each element of the list there is a cons. The empty list has no elements at all. 

`psil` replaces the outdated implementation-based _car_ and _cdr_ methods with _first_ and _second_ for clarity and ease of use. However, more importantly the direction of recursion in lists is reversed. While in LISP `(a b c d)` would be interpreted akin to `(a . (b . (c . (d . nil))))`, in `psil` it would be `((((nil . a) . b ) . c) . d)` (I assume at this point the origin of the name `psil` is clear). To evaluate this expression, the flow of execution starts at the innermost pair, yielding `a`, and each additional cons provides another argument.

Since each argument is applied individually, there must be intermediate states for the function. For this, we utilize the functional idea of currying (named after mathematician Haskell Curry). In `psil`, everything is curried, meaning every function only takes one parameter. To define such a function in user code, you use the `lambda` function:

```lisp
(lambda :a {+ 1 a})
```

Don't worry about the `{+ 1 a}` quite yet, the different types of brackets will be discussed in the next section. To define a function that takes multiple arguments, currying entails returning a lambda that takes the next argument.

```lisp
(def :myfunc (lambda :a (lambda :b {+ a b})))
```

This form, though syntactically correct and important to understand, is often unwieldy. A shortcut is the `lambdas` command (note the `s`):

```lisp
(def :myfunc (lambdas [a b] {+ a b}))
```

The `(def :myfunc ...)` part defines the symbol `myfunc` to point to that lambda for future use. Now, we can apply the function using that symbol, notating the result of each expression in a comment:

```lisp
(myfunc 10) ; => (lambda :b {+ 10 b})
(myfunc 10 13) ; => 23
(def :mypartialfunc (myfunc 10))
(mypartialfunc 13) ; => 23
```

As you can see, the partial application returns the lambda with some variables already "filled in", so it can be used without knowing the previous arguments. This sequential application fits well with the left recursive syntax.

## Variables, Scope, and Type
There are two ways to define a variable in `psil`. You've already seen `def`, which is used often in the REPL:
```lisp
(def :a 2)
(+ 4 a) ; => 6
```
`def` should rarely be used in production code, as it can lead to ambiguity regarding scope. On the other hand, `with` is the standard functional way to define variables, like LISP's `let`:
```lisp
(with <:a 42> {+ a 13}) ; => {+ 42 13}
```
With is similar to `lambda`, in that you can define multiple variables recursively or by using `withs`.
```lisp
(with <:a 42> (with <:b 12> {+ a b})) ; => {+ 42 12}
(withs [<:a 42> <:b 12>] {+ a b}) ; => {+ 42 12}
```
Now the third type of bracket is introduced; `{a b c}`. Braces are syntactic sugar for `(closure [a b c])`, returning a closure. Notice `with` doesn't evaluate the clojure itself, it just defines variables inside it. 
