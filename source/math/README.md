# Math test architecture

The math tests mirror the physics test organization: each public area owns a named
Boost.UT suite, compile-time contracts use `static_assert`, and runtime behavior
uses `ut::expect`.

Suites:

- `math.functions`: scalar helpers, powers, rounding, axes, signs, ordinals.
- `math.vector`: construction, access, reductions, geometry, arithmetic, exceptions.
- `math.matrix`: construction, reductions, transpose, determinants, inverse,
  structure predicates, products, arithmetic, look-at basis.
- `math.format`: `std::formatter` behavior for vectors and matrices.
- `math.interop.std`: built-in arithmetic, STL ranges/spans and `std::complex`.
- `math.interop.physics`: dimensional compatibility with `physics::Quantity`.

## Matrix range semantics

`Matrix<N, M, T>::begin()` and `end()` iterate over the `N` rows.
`size()` returns `N`, consistent with `std::ranges::size()` and
`std::span{matrix}`; `elements()` reports the total number of scalar
elements, `N * M`. These contracts are covered by the matrix tests.

## Compatibility findings

### Physics quantities

Compatibility is strong because vector/matrix multiplicative operations are
mostly expression-based rather than constrained to `math::Arithmetic`:

- `Vector<Length>::norm_squared()` -> `Area`
- `Vector<Length>::norm()` -> `Length`
- `Vector<Length>::normalized()` -> `Vector<double>`
- dot/cross products propagate dimensions
- matrix determinants and inverses propagate dimensions
- vector/matrix formatters forward quantity-specific format specs

`normalize()` in-place intentionally requires a floating-point element type, so
a dimensional vector uses `normalized()` instead.

### Standard library numeric types

Built-in arithmetic types have full support.

`std::complex<T>` has useful algebraic support in `square`, `cube`,
compile-time-exponent `power<>()`, vectors, matrices, determinants and inverses.
It is *not* an `Arithmetic` type, so APIs explicitly constrained by
`math::Arithmetic` do not accept it.

Also note that the current vector `norm()` is bilinear:

```cpp
sqrt(sum(x_i * x_i))
```

not the conventional Hermitian complex norm:

```cpp
sqrt(sum(norm(x_i)))
```

so the tests deliberately do not treat complex-vector `norm()` as a standard
complex Euclidean norm.
