# Physics Library — Functionality and Structure

The library is a header-only, compile-time dimensional-analysis system for modern C++. Its central model is:

- **Dimensions are types.**
- **Quantities store values in a canonical coherent representation.**
- **Units are construction and formatting metadata, not persistent runtime state.**
- **Semantic quantity names are aliases over dimensional types.**
- **A small set of persistent `Property` flags carries information that genuinely changes valid algebra.**
- **Plane angle is modeled as an eighth independent dimension.**
- **Dimensionless numerical results remain `Quantity` objects until they are converted to their underlying floating-point value.**

The archive contains 16 headers, including a generated literal header with **1,039 distinct literal suffixes** (each with integer and floating-literal overloads).

> **External dependency:** several headers include `math/functions.h` and use `math::Arithmetic`, `math::power`, `math::abs`, `math::functions::square`, `cube`, and `power`. That `math` implementation is not part of this archive, so its internals are outside the scope of this document.

---

## 1. Header map

| Header | Role |
|---|---|
| `config.h` | Fundamental configuration: exponent type, default numeric type, angle-decay policy. |
| `dimension.h` | The eight-exponent `Dimensions` type and dimension concepts/compatibility rules. |
| `dimensions.h` | Canonical named derived dimensions built algebraically from the base dimensions. |
| `property.h` | Persistent quantity-property flags and their arithmetic propagation rules. |
| `quantity.h` | The core `Quantity<Dimension, Value, properties>` value type and operators. |
| `quantities.h` | User-facing semantic aliases such as `Length`, `Energy`, `Torque`, and `AbsoluteTemperature`. |
| `units.h` | The `Unit` enum and unit metadata: symbols, prefixes, factors, offsets, default properties. |
| `unit_dimensions.h` | Compile-time and runtime mapping from `Unit` to dimensions; formatter decomposition helpers. |
| `prefix.h` | SI prefixes from quetta to quecto, with factors and symbols. |
| `prefixed_unit.h` | `PrefixedUnit` plus parsing of textual prefixed-unit tokens. |
| `literals.h` | `quantity_cast`, literal-wrapper behavior, and inclusion of generated UDLs. |
| `literals.generated.h` | Generated user-defined literal overloads for units and allowed prefixes. |
| `format.h` | `std::formatter<Quantity<...>>`, unit conversion, decomposition, and custom format syntax. |
| `functions.h` | Quantity-aware roots, exponential, rounding, finiteness, and trigonometry. |
| `constants.h` | SI defining constants, CODATA 2022 values, and derived physical constants. |
| `physics.h` | Umbrella include plus a small set of global convenience `using` declarations. |

A useful dependency view is:

```text
config
  |
dimension -------> dimensions
  |                   |
property               |
  |                   |
quantity <-------------+
  |
  +---- units ---- unit_dimensions
  |        |             |
  |      prefix ------ prefixed_unit
  |        |             |
  +---- literals --------+
  |
  +---- functions
  +---- quantities ---- constants
  +---- format

physics.h includes constants + functions + format + literals
```

---

## 2. Core configuration

`config.h` defines:

```cpp
using Exponent = int;
using DefaultValue = double;

namespace config
{
    inline constexpr bool allow_angle_decay = false;
}
```

Therefore:

- dimensional exponents are integral;
- ordinary `Quantity` aliases and literals use `double`;
- angle compatibility is **strict by default**.

Changing `DefaultValue` changes the default storage type used by `Quantity` aliases and by generated literals. `quantity_cast` can also request a different floating-point `Value` explicitly.

---

## 3. Dimension system

### 3.1 Eight independent exponents

The fundamental type is:

```cpp
template <
    Exponent T = 0, // time
    Exponent L = 0, // length
    Exponent M = 0, // mass
    Exponent I = 0, // electric current
    Exponent Θ = 0, // thermodynamic temperature
    Exponent N = 0, // amount of substance
    Exponent J = 0, // luminous intensity
    Exponent A = 0> // plane angle
class Dimensions;
```

The basis is therefore:

```text
T  time
L  length
M  mass
I  electric current
Θ  thermodynamic temperature
N  amount of substance
J  luminous intensity
A  plane angle
```

`Dimensionless` is exactly:

```cpp
using Dimensionless = Dimensions<>;
```

The eight base aliases live in `physics::dimension::base`, which is an inline namespace:

```cpp
dimension::Time
dimension::Length
dimension::Mass
dimension::ElectricCurrent
dimension::Temperature
dimension::AmountOfSubstance
dimension::LuminousIntensity
dimension::Angle
```

### 3.2 Algebra on dimensions

`Dimensions` deliberately uses type-level arithmetic:

```cpp
D1 + D2   // add exponent vectors: dimension of multiplication
D1 - D2   // subtract exponent vectors: dimension of division
-D        // negate all exponents: reciprocal dimension
D.power<n>()
D.root<n>()
```

For example:

```cpp
using Velocity =
    decltype(dimension::Length{} - dimension::Time{});

using Force =
    decltype(dimension::Mass{} + dimension::Acceleration{});
```

A root is available only when every exponent is divisible by the requested positive divisor. This is exposed through `IsRootable<n, D>`.

Every dimension can also produce an eight-element runtime representation:

```cpp
auto exponents = D::representation();
```

This representation is used by unit formatting/decomposition.

### 3.3 Angle is a genuine dimension

Plane angle is not erased into ordinary dimensionless arithmetic. Examples from `dimensions.h` include:

```text
Angle²                     SolidAngle
Angle / Time               AngularVelocity
Angle / Time²              AngularAcceleration
Angle / Length             Wavevector
Mass·Length² / Angle²      MomentOfInertia
Action / Angle             AngularMomentum
Energy / Angle             Torque
Current·Area / Angle       MagneticMoment
```

This makes angular relationships exact rather than relying on semantic naming alone.

For example, the library distinguishes:

```cpp
dimension::Frequency         // T^-1
dimension::AngularVelocity   // A T^-1
```

and:

```cpp
dimension::Wavenumber        // L^-1
dimension::Wavevector        // A L^-1
```

### 3.4 Optional angle decay

`dimension.h` defines three important relations:

```cpp
dimension::Same<D1, D2>
dimension::Equivalent<D1, D2>
dimension::Convertible<From, To>
```

With the default:

```cpp
config::allow_angle_decay == false
```

all three effectively require exact dimensional agreement where relevant.

If angle decay is enabled, `Equivalent` allows dimensions that agree in the first seven SI exponents while differing in the angle exponent. `Convertible` is directional: decay may remove angular information when the destination has angle exponent zero; it does not invent angular information.

`dimension::SI_type<D>` strips the angle exponent and is used when relaxed addition/subtraction combines angle-distinct but SI-equivalent dimensions.

---

## 4. Named derived dimensions

`dimensions.h` is the canonical dimensional vocabulary. It does **not** create semantic quantity types; it creates unique names for exponent vectors.

The definitions are expressed in terms of the physical algebra rather than by manually writing exponent packs. This makes relationships visible in the code:

```cpp
using Energy = decltype(Force{} + Length{});
using Torque = decltype(Energy{} - Angle{});
using Power = decltype(Energy{} - Time{});

using MagneticMoment =
    decltype(ElectricCurrent{} + Area{} - Angle{});

using LuminousFlux =
    decltype(LuminousIntensity{} + SolidAngle{});
```

A complete derived-dimension reference is included in [Appendix A](#appendix-a-derived-dimension-reference).

---

## 5. The `Quantity` type

The computational value type is:

```cpp
template <
    IsDimension Dimension = Dimensionless,
    std::floating_point Value = DefaultValue,
    quantity::Property properties = quantity::Property::none>
class Quantity;
```

A `Quantity` therefore carries three compile-time facts:

1. **Dimension** — the exponent vector.
2. **Value** — the floating-point storage type.
3. **Properties** — persistent algebraic/reference semantics.

The only runtime data member is the numeric value.

### 5.1 Canonical storage

Unit information is not stored in a `Quantity`. Construction converts input to a coherent canonical value.

Examples:

```text
1 km       -> stored as 1000 m
1 g        -> stored as 0.001 kg-equivalent mass value
20 °C      -> stored as 293.15 K, Property::absolute
20 dB      -> stored as linear ratio 10, Property::logarithmic
25 percent -> stored as 0.25
```

The mass unit metadata uses `gram` with factor `1e-3`, while the formatter's base-unit set pairs it with the kilo prefix. Consequently the coherent base representation is effectively kilograms.

Angles are stored in radians because radian has factor 1.

### 5.2 Construction and assignment between quantities

Cross-quantity construction is permitted when:

- the source dimension is `dimension::Convertible` to the destination;
- the numeric type is convertible;
- **the property set is identical**.

The property equality is encoded directly in the source parameter type:

```cpp
Quantity<OtherDimension, Other, properties>
```

There is no implicit property decay during conversion.

Thus an absolute temperature and a temperature difference do not silently convert into each other even though they have the same dimensional exponents.

### 5.3 Access and numeric-type conversion

Raw access is explicit:

```cpp
auto x = q.value();
```

Numeric storage can be changed without changing dimensions or properties:

```cpp
auto qf = q.to<float>();
```

### 5.4 Naked dimensionless conversion

A completely ordinary dimensionless quantity can convert implicitly to its storage type:

```cpp
constexpr operator Value() const
    requires IsDimensionless<Dimension> &&
             properties == Property::none;
```

This allows:

```cpp
Quantity<> ratio{ 0.5 };
double x = ratio;
```

but does **not** allow the same escape for dimensionless quantities carrying persistent properties, such as logarithmic quantities.

Arithmetic itself therefore remains inside the quantity framework:

```cpp
auto ratio = length1 / length2;
// ratio is Quantity<Dimensionless, ...>, not immediately a double.
```

The conversion to a scalar happens only when a scalar context actually requests it.

### 5.5 Comparison

`==` and `<=>` require:

- equivalent dimensions;
- identical properties.

Different floating-point storage types are supported.

---

## 6. Persistent quantity properties

`property.h` defines a bit flag:

```cpp
enum class Property : unsigned
{
    none        = 0,
    absolute    = 1 << 0,
    logarithmic = 1 << 1,
};
```

The flags are intentionally orthogonal. Current built-in units choose at most one of them, but the representation permits combinations.

These properties are **not unit metadata**. They persist after construction because they change valid arithmetic.

### 6.1 `none`

`Property::none` is the ordinary state. Most quantities use it.

Examples:

```cpp
quantity::Length
quantity::Energy
quantity::TemperatureDifference
Quantity<> // ordinary dimensionless factor
```

### 6.2 `absolute`

`Property::absolute` marks a value with an absolute reference/origin. The built-in use is absolute thermodynamic temperature.

```cpp
using AbsoluteTemperature =
    Quantity<
        dimension::Temperature,
        DefaultValue,
        quantity::Property::absolute>;

using TemperatureDifference =
    Quantity<dimension::Temperature>;
```

Only these two temperature interpretations are provided; there is no separate persistent Celsius-point type.

### 6.3 `logarithmic`

`Property::logarithmic` marks a quantity whose stored value is linear but whose algebra is intentionally non-additive.

Built-in logarithmic literals (`Np`, `B`, prefixed `B` such as `dB`) materialize with this property.

The purpose of the flag is algebraic: ordinary `+` and `-` are rejected for logarithmic quantities even though the stored value is a linear ratio.

### 6.4 Addition rules

`property::can_add` implements:

| Left | Right | Allowed | Result |
|---|---|---:|---|
| ordinary | ordinary | yes | ordinary |
| absolute | ordinary | yes | absolute |
| ordinary | absolute | yes | absolute |
| absolute | absolute | no | — |
| logarithmic | anything | no | — |
| anything | logarithmic | no | — |

The result property is simply `left | right` after validity has been checked.

This gives the affine translation rule:

```text
absolute + difference -> absolute
difference + absolute -> absolute
```

but prevents adding two absolute coordinates.

### 6.5 Subtraction rules

`property::can_subtract` implements:

| Left | Right | Allowed | Result |
|---|---|---:|---|
| ordinary | ordinary | yes | ordinary |
| absolute | ordinary | yes | absolute |
| absolute | absolute | yes | ordinary |
| ordinary | absolute | no | — |
| logarithmic | anything | no | — |
| anything | logarithmic | no | — |

Thus:

```text
T1 - T2   -> temperature difference
T  - dT   -> absolute temperature
dT - T    -> invalid
```

### 6.6 Multiplication and division

Properties survive multiplication/division only when the other operand is a **naked factor**:

```cpp
IsDimensionless<Dimension> &&
properties == Property::none
```

For multiplication:

```text
special quantity * naked factor -> preserve special properties
naked factor * special quantity -> preserve special properties
otherwise                       -> Property::none
```

For division:

```text
special quantity / naked factor -> preserve numerator properties
otherwise                       -> Property::none
```

Ordinary arithmetic scalars (`2`, `0.5`, etc.) are also pure scale factors, so scalar multiplication/division preserves the quantity's properties.

This distinguishes scaling from participation in new dimensional algebra.

### 6.7 Compound assignment

`+=` and `-=` accept an ordinary-property right-hand side and reject logarithmic left-hand sides.

This directly supports operations such as:

```cpp
AbsoluteTemperature T = 300_K;

auto dT =
    quantity_cast<
        Unit::kelvin,
        Prefix::none,
        quantity::Property::none>(10);

T += dT;
T -= dT;
```

without permitting `absolute += absolute` or logarithmic addition.

`*=` and `/=` by arithmetic scalars preserve the property state.

---

## 7. Quantity arithmetic

### 7.1 Multiplication and division

For two quantities:

```cpp
q1 * q2
q1 / q2
```

the result dimension is computed directly from the dimension algebra:

```cpp
decltype(D1{} + D2{})
decltype(D1{} - D2{})
```

The result remains a `Quantity`, including when the resulting dimension is `Dimensionless`.

Example:

```cpp
auto ratio = 10_m / 2_m;
// Quantity<Dimensionless>, value 5
```

That naked result can subsequently convert to its `Value` type if needed.

### 7.2 Addition and subtraction

Binary `+` and `-` require `dimension::Equivalent` dimensions and valid property combinations.

If dimensions are exactly the same, the result keeps the original dimension type. If relaxed angle decay is enabled and the operands are only SI-equivalent, the result uses the angle-stripped `dimension::SI_type`.

### 7.3 Scalar arithmetic

A scalar can:

- multiply or divide any quantity;
- add to/subtract from a dimensionless non-logarithmic quantity;
- be added to a dimensionless non-logarithmic quantity;
- be subtracted by an ordinary naked `Quantity<Dimensionless>`.

Scalar multiplication and quantity/scalar division preserve the quantity's properties because the scalar is only a scale factor.

Unary `-` also preserves dimensions and properties.

---

## 8. Semantic quantity vocabulary

`quantities.h` is intentionally richer than `dimensions.h`.

Multiple physical concepts can be aliases of the same mathematical `Quantity` type:

```cpp
using Energy         = Quantity<dimension::Energy>;
using Work           = Quantity<dimension::Energy>;
using Heat           = Quantity<dimension::Energy>;
using InternalEnergy = Quantity<dimension::Energy>;
```

This makes semantic names useful in APIs and documentation without pretending that equal dimensions are incompatible types.

The important exception is when semantics change valid algebra. Absolute temperature therefore carries `Property::absolute`, while a temperature difference does not.

The archive defines **201 semantic quantity aliases**. They are catalogued in [Appendix C](#appendix-c-semantic-quantity-vocabulary).

---

## 9. Units

### 9.1 `Unit` is metadata, not stored state

`Unit` enumerates supported construction/formatting representations. `Unit::none` is the identity/no-unit representation and maps to `Dimensionless`.

`unit::is_valid` considers the concrete unit range `second ... ppm` valid; `none` is outside that range because it is not an explicit textual unit.

Two dimensionless classifications are exposed:

- `unit::is_dimensionless` identifies the explicit dimensionless representations `Np`, `B`, `%`, and `ppm`;
- `unit::is_si_dimensionless` additionally treats radian, steradian, turn, degree, arcminute, and arcsecond as SI-dimensionless representations, even though the core dimensional model deliberately retains an angle exponent.

`unit::is_si_base` covers the seven conventional SI base units from second through candela; radian is not classified as an SI base unit.

The unit system stores:

- dimension mapping;
- scale factor;
- affine offset;
- printable symbol;
- selected ASCII alternative;
- prefixability;
- default persistent property.

### 9.2 Default unit properties

Current defaults are:

```text
kelvin          absolute
degree_celsius  absolute
neper           logarithmic
bel             logarithmic
everything else none
```

This means a normal Kelvin literal represents an **absolute temperature**, not a temperature difference:

```cpp
auto T = 300_K;
```

A temperature difference must be requested explicitly, for example:

```cpp
auto dT =
    quantity_cast<
        Unit::kelvin,
        Prefix::none,
        quantity::Property::none>(10);
```

For Celsius, requesting `Property::none` also suppresses the affine offset, making the value a temperature interval:

```cpp
auto dT =
    quantity_cast<
        Unit::degree_celsius,
        Prefix::none,
        quantity::Property::none>(10);
// stored difference: 10 K
```

### 9.3 Scale factors and offsets

Unit factors are returned as `long double`. Examples:

```text
gram               1e-3
minute             60
hour               3600
turn               2π
degree             2π / 360
liter              1e-3
bar                1e5
calorie            4.184
electronvolt       1.602176634e-19
astronomical_unit  149597870700
percent            1e-2
ppm                1e-6
```

The only built-in affine offset is:

```text
degree_celsius -> 273.15
```

### 9.4 Logarithmic unit convention

The logarithmic units use the field/amplitude-ratio convention encoded by:

```cpp
factor(neper) = 1
factor(bel)   = ln(10) / 2
```

`quantity_cast` exponentiates the logarithmic coordinate into the stored linear ratio.

Therefore:

```text
0 Np   -> 1
1 Np   -> e
0 B    -> 1
1 B    -> sqrt(10)
20 dB  -> 10
-20 dB -> 0.1
```

This is not a power-ratio `10 log10(P/P0)` interpretation; the built-in bel factor corresponds to `2 log10(F/F0)` for a field/amplitude ratio.

### 9.5 Prefixes

`Prefix` encodes the decimal exponent directly:

```text
quetta +30     ronna +27      ... kilo +3
hecto +2       deca +1        none 0
deci -1        centi -2       milli -3
micro -6       ...            quecto -30
```

The factor is `10^exponent`.

Unicode `µ` is the normal micro symbol; ASCII `u` is accepted as an alternative.

Most valid units are prefixable. Explicitly non-prefixable units include time units (`min`, `h`, `d`), non-radian angle units, are/hectare, astronomical and nautical units, knot, percent, and ppm.

A full unit table is in [Appendix B](#appendix-b-unit-reference).

---

## 10. Mapping units to dimensions

`unit_dimensions.h` provides two levels of mapping.

### 10.1 Compile-time mapping

```cpp
template <Unit unit>
consteval auto unit::dimensions::get();
```

Examples:

```cpp
decltype(unit::dimensions::get<Unit::joule>())
    // dimension::Energy

decltype(unit::dimensions::get<Unit::radian>())
    // dimension::Angle

decltype(unit::dimensions::get<Unit::neper>())
    // Dimensionless
```

This mapping is what `quantity_cast` uses to determine the result type.

### 10.2 Runtime representation

```cpp
unit::dimensions::representation(Unit)
```

returns the eight-exponent runtime array for formatter planning.

`contains(representation, unit)` decides whether a unit can be consumed from the remaining dimensions during formatting. Most units use direct exponent containment; several mechanical and electrical units have special heuristics so compound representations such as `V/m` or `F/m` can be found by the greedy formatter.

Dimensionless units only match an exactly dimensionless representation so their zero exponent vector cannot accidentally match every quantity.

---

## 11. Constructing quantities

### 11.1 `quantity_cast`

The general boundary conversion is:

```cpp
template <
    Unit unit,
    Prefix prefix = Prefix::none,
    quantity::Property properties = unit::default_property(unit),
    std::floating_point Value = DefaultValue>
constexpr auto quantity_cast(math::Arithmetic auto value);
```

The sequence is:

1. determine the unit's dimension;
2. multiply by prefix and unit scale factors;
3. for logarithmic units, exponentiate the scaled coordinate;
4. otherwise, if the requested properties contain `absolute`, add the unit offset;
5. construct `Quantity<Dimension, Value, properties>`.

Examples:

```cpp
auto x = quantity_cast<Unit::meter, Prefix::kilo>(2);
// 2000 m stored canonically

auto T = quantity_cast<Unit::degree_celsius>(20);
// absolute temperature, stored 293.15 K

auto dT =
    quantity_cast<
        Unit::degree_celsius,
        Prefix::none,
        quantity::Property::none>(20);
// temperature difference, stored 20 K

auto gain = quantity_cast<Unit::bel, Prefix::deci>(20);
// logarithmic property, stored linear ratio 10
```

### 11.2 Generated literals

`literals.generated.h` contains **1,039 unique literal suffixes**, each with:

```cpp
unsigned long long
long double
```

overloads.

The generated UDLs delegate to:

```cpp
detail::literal<unit, prefix>(value)
```

Examples include:

```cpp
10_m
5_km
2_ms
3_J
12_mV
90_deg
1_rad
300_K
20_degC
20_dB
25_percent
3_ppm
```

ASCII-safe spellings are used where a Unicode symbol cannot form a C++ identifier:

```text
Ohm
degC
deg
arcmin
arcsec
Angstrom
percent
u  (micro prefix)
```

Use:

```cpp
using namespace physics::unit::literals;
```

to bring the literal operators into scope.

### 11.3 Why affine and logarithmic literals use a wrapper

C++ parses:

```cpp
-20_degC
-20_dB
```

as unary minus applied **after** the literal operator:

```cpp
-(20_degC)
-(20_dB)
```

If the literal were immediately converted, the sign would operate on the already transformed canonical value and produce the wrong meaning.

Therefore affine and logarithmic literals return a tiny `detail::Wrapper` that retains the lexical coordinate until unary `+`, unary `-`, or conversion to the exact-property `Quantity` occurs.

Consequences:

```cpp
quantity::AbsoluteTemperature T = 20_degC; // wrapper converts
auto T2 = +20_degC;                        // materializes a Quantity
auto T3 = -20_degC;                        // materializes -20 °C correctly

auto w = 20_degC; // w itself is the wrapper
```

The same rule applies to logarithmic literals:

```cpp
auto gain = +20_dB; // logarithmic Quantity, stored ratio 10
auto loss = -20_dB; // logarithmic Quantity, stored ratio 0.1

auto w = 20_dB;     // wrapper until materialized
```

The wrapper does **not** decay properties. It converts only to:

```cpp
Quantity<Dimension, To, properties>
```

with the unit's exact default property set.

---

## 12. Formatting

`format.h` specializes:

```cpp
std::formatter<
    physics::Quantity<Dimension, Value, properties>>
```

The formatter can:

- print a value in coherent base units;
- convert to explicitly requested units;
- decompose a dimension into compound units;
- print value only, unit only, or both;
- delegate numeric formatting to `std::formatter<Value>`;
- apply affine offsets for absolute quantities;
- apply logarithmic conversion for logarithmic output units.

### 12.1 Format-spec grammar

Conceptually:

```text
[output-mode ":"] [unit specification] [";" value-format]
```

Output mode is:

```text
v:  value only
u:  unit only
```

No mode means value and unit.

Examples:

```cpp
std::format("{}", q)             // default base-unit representation
std::format("{:J}", energy)      // joules
std::format("{:km/h}", speed)    // km/h
std::format("{:v:km}", length)   // converted numeric value only
std::format("{:u:J}", energy)    // unit text only
std::format("{:J;.3f}", energy)  // J, numeric value with .3f
```

Unit tokens are separated by:

```text
space
*
/
```

The presence of `/` requests fraction-style output. Without `/`, denominator terms are printed with negative superscript exponents.

### 12.2 Default decomposition

The formatter begins with base-unit candidates:

```text
s, m, kg, A, K, mol, cd, rad
```

(`kg` is represented internally as `Prefix::kilo + Unit::gram`.)

Explicit unit tokens are added as preferred candidates, and the formatter greedily consumes the quantity's dimensional exponent representation. Any remaining dimensions are then represented by the base candidates.

The resulting plan records:

- numerator unit terms;
- denominator unit terms;
- total inverse conversion factor;
- total affine offset;
- whether the requested representation is logarithmic.

### 12.3 Affine formatting

If the quantity carries `Property::absolute`, the accumulated offset is applied before the final scale factor.

For Celsius this means:

```text
stored 293.15 K absolute -> 20 °C
```

For an ordinary temperature difference, the offset is not applied:

```text
stored 10 K difference -> 10 °C difference representation
```

The type system still sees the latter as `TemperatureDifference`; the unit symbol itself is simply `°C`.

### 12.4 Logarithmic formatting

If the selected output unit is logarithmic, the formatter first computes:

```cpp
std::log(quantity.value())
```

then applies the inverse unit/prefix scale factor.

Thus a stored ratio of 10 formats as `20 dB`.

The **output unit** controls logarithmic representation. A quantity does not need `Property::logarithmic` merely to be *formatted* in dB; the property exists to constrain algebra, while the unit controls boundary representation.

### 12.5 Dimensionless-unit validation

Explicit `%`, `ppm`, `Np`, or `B` units require the quantity dimension to be exactly `Dimensionless`.

Only one explicit dimensionless unit may appear in a format specification.

Logarithmic units must also be the sole unit term; compound expressions involving a logarithmic unit are rejected.

Invalid or dimensionally incompatible explicit units produce `std::format_error`.

### 12.6 Unit symbols and exponents

Formatter output uses canonical Unicode symbols where defined:

```text
Ω, °C, °, ′, ″, Å, µ
```

`exponent.h` converts signed integer exponents to Unicode superscripts, including superscript minus.

When fraction style is requested and there are multiple denominator terms, the formatter surrounds the denominator with parentheses.

---

## 13. Mathematical functions

`functions.h` provides quantity-aware overloads in namespace `physics`.

### 13.1 Roots

```cpp
physics::sqrt(q)
physics::cbrt(q)
```

are available only if the dimension's exponents are exactly divisible by 2 or 3.

The result dimension is rooted at compile time.

Property behavior is deliberate:

```text
absolute    dropped
logarithmic retained
```

The implementation keeps only the `logarithmic` bit.

Thus a logarithmic dimensionless ratio can be square-rooted while remaining non-additive.

### 13.2 Exponential

```cpp
physics::exp(q)
```

accepts only:

```cpp
Quantity<Dimensionless, Value, Property::none>
```

and returns an ordinary dimensionless quantity.

### 13.3 Rounding

```cpp
round
floor
ceil
```

are available only for `Property::none`. Absolute and logarithmic values are intentionally excluded because rounding the canonical stored coordinate would depend on the hidden representation/reference interpretation.

### 13.4 Finiteness

```cpp
physics::isfinite(q)
```

accepts any dimension and any property and returns `bool`.

### 13.5 Trigonometry

Direct trigonometric functions require an exact plane-angle quantity:

```cpp
physics::sin(angle)
physics::cos(angle)
physics::tan(angle)
```

and return `Quantity<Dimensionless, Result>` rather than immediately leaving the quantity framework.

Inverse functions accept an ordinary dimensionless quantity:

```cpp
physics::asin(x)
physics::acos(x)
physics::atan(x)
```

and return `Quantity<dimension::Angle, Result>`.

`atan2` accepts two ordinary quantities of the same exact dimension:

```cpp
physics::atan2(y, x)
```

and returns an angle.

---

## 14. Physical constants

`constants.h` states that SI defining constants and CODATA 2022 recommended values are used, without representing standard uncertainties.

### 14.1 Stored defining/recommended values

| Name | Symbol alias | Stored value |
|---|---|---:|
| Cesium-133 hyperfine transition frequency | `nu_Cs` | `9'192'631'770` |
| Speed of light | `c` | `299'792'458` |
| Planck constant | `h` | `6.62607015e-34` |
| Elementary charge | `e` | `1.602176634e-19` |
| Boltzmann constant | `k_B` | `1.380649e-23` |
| Avogadro constant | `N_A` | `6.02214076e23` |
| Luminous efficacy | `K_cd` | `683` |
| Newtonian gravitational constant | `G` | `6.67430e-11` |
| Fine-structure constant | `alpha` | `7.2973525643e-3` |
| Electron g-factor | `g_e` | `-2.00231930436092` |
| Electron mass | `m_e` | `9.1093837139e-31` |
| Proton mass | `m_p` | `1.67262192595e-27` |
| Neutron mass | `m_n` | `1.67492750056e-27` |
| Atomic mass constant | `m_u` | `1.66053906892e-27` |

`constant::pi` is a mathematical scalar, while:

```cpp
constant::full_turn
```

is a physical `PlaneAngle` of `2π` radians.

### 14.2 Derived constants

The library derives, dimensionally, among others:

```text
F          Faraday constant
R          molar gas constant
hbar       reduced Planck constant
epsilon_0  vacuum permittivity
mu_0       vacuum permeability
R_inf      Rydberg constant
R_k        von Klitzing constant
Z_0        characteristic impedance of vacuum
G_0        conductance quantum
k_J        Josephson constant
k_e        Coulomb constant
phi_0      magnetic flux quantum
mu_B       Bohr magneton
mu_N       nuclear magneton
r_e        classical electron radius
E_h        Hartree energy
sigma      Stefan-Boltzmann constant
```

For example:

```cpp
constexpr auto hbar{ h / full_turn };
constexpr auto mu_B{ e * hbar / (2 * m_e) };
```

Because angle is a true dimension, `hbar` carries angular-momentum dimensions and the magnetic moments inherit the intended angle-aware dimension automatically.

Descriptive aliases such as `reduced_planck`, `vacuum_permittivity`, `bohr_magneton`, and `stefan_boltzmann` are also provided.

---

## 15. `physics.h` umbrella header

`physics.h` includes:

```cpp
constants.h
functions.h
format.h
literals.h
```

Because `constants.h` includes `quantities.h`, the semantic quantity vocabulary is available transitively.

The header additionally places these selected quantity aliases into the **global namespace** with `using` declarations:

```text
ElectricCurrent
ElectricPotential
Energy
Length
Area
Volume
Fraction
MolarConcentration
Wavevector
```

Code that wants to avoid these global convenience names can include the individual physics headers instead of the umbrella header.

---

## 16. Typical usage

```cpp
#include "physics/physics.h"

#include <format>

using namespace physics;
using namespace physics::unit::literals;

int main()
{
    quantity::Length distance = 1.5_km;
    quantity::Time time = 2_min;

    auto speed = distance / time;

    auto text = std::format("{:km/h;.2f}", speed);

    quantity::AbsoluteTemperature room = 20_degC;

    auto delta =
        quantity_cast<
            Unit::kelvin,
            Prefix::none,
            quantity::Property::none>(5);

    auto warmer = room + delta;

    auto gain = +20_dB;
    auto half_gain = gain / 2;

    auto angle = 30_deg;
    auto cosine = physics::cos(angle);

    double scalar_cosine = cosine;
}
```

The important separation is visible here:

```text
literal / quantity_cast   boundary representation -> canonical Quantity
Quantity operators        dimensional and property-safe computation
formatter                 canonical Quantity -> requested representation
```

---

## 17. Extending the library

### 17.1 Add a semantic quantity name

If the dimension already exists, add an alias to `quantities.h`:

```cpp
using MyPhysicalName = Quantity<dimension::ExistingDimension>;
```

Use a non-`none` property only if the property changes valid computation, not merely to preserve a semantic label.

### 17.2 Add a derived dimension

Add an algebraic alias to `dimensions.h`:

```cpp
using MyDimension =
    decltype(ExistingA{} + ExistingB{} - ExistingC{});
```

Prefer expressing the physical relationship over manually spelling an exponent pack.

### 17.3 Add a unit

A complete unit addition generally requires:

1. add an enumerator to `Unit`;
2. update its factor/offset as needed;
3. add canonical and optional ASCII symbols;
4. decide prefixability;
5. decide its default property;
6. map it in `unit_dimensions.h` at compile time;
7. add it to the runtime representation switch;
8. regenerate `literals.generated.h` if literals are desired.

If the formatter cannot decompose useful compound forms with ordinary exponent containment, `unit::dimensions::contains` is the place for narrowly targeted decomposition behavior.

### 17.4 Add a persistent property

A property should be added only when it changes valid computation after units have disappeared.

The current examples are:

```text
absolute     changes addition/subtraction and reference semantics
logarithmic  forbids ordinary addition/subtraction
```

If a new flag is introduced, review:

```text
can_add
add
can_subtract
subtract
multiply
divide
sqrt/cbrt property propagation
quantity_cast defaulting
formatter interpretation
```

Property loss should occur as an explicit result of an algebraic operation, not silently through `Quantity` conversion.

---

## 18. Design invariants

The current implementation can be summarized by the following invariants:

1. **Dimensional correctness is compile-time structure.**
2. **Angle information is preserved as an eighth exponent unless the optional decay policy is enabled.**
3. **Units do not remain attached to quantities.**
4. **Construction converts to canonical values; formatting converts out.**
5. **Semantic quantity aliases may intentionally share a type when no mathematical distinction is required.**
6. **Persistent properties exist only for distinctions that alter valid algebra.**
7. **Properties do not silently decay during conversion.**
8. **Scaling by a pure factor preserves properties; genuine dimensional multiplication/division drops them.**
9. **Logarithmic quantities store the underlying linear value.**
10. **Dimensionless arithmetic remains in `Quantity` until a naked result is actually requested as a scalar.**

These rules are distributed cleanly across the library:

```text
dimension.h       dimensional compatibility
dimensions.h      dimensional vocabulary
property.h        non-dimensional algebraic state
quantity.h        core value algebra
units.h           representation metadata
literals.h        input boundary
format.h          output boundary
quantities.h      semantic vocabulary
constants.h       physical data and derived identities
```

---

# Appendix A: Derived dimension reference

The following aliases are defined in `dimensions.h`. `+` means multiplication of physical dimensions and `-` means division because these expressions operate on exponent vectors.

| Dimension alias | Definition |
|---|---|
| `SolidAngle` | `decltype(Angle{} + Angle{})` |
| `Frequency` | `decltype(-Time{})` |
| `Wavenumber` | `decltype(-Length{})` |
| `Wavevector` | `decltype(Angle{} - Length{})` |
| `Area` | `decltype(Length{} + Length{})` |
| `Volume` | `decltype(Length{} + Length{} + Length{})` |
| `NumberDensity` | `decltype(-Volume{})` |
| `Absement` | `decltype(Length{} + Time{})` |
| `Velocity` | `decltype(Length{} - Time{})` |
| `Acceleration` | `decltype(Velocity{} - Time{})` |
| `Jerk` | `decltype(Acceleration{} - Time{})` |
| `Jounce` | `decltype(Jerk{} - Time{})` |
| `Crackle` | `decltype(Jounce{} - Time{})` |
| `Pop` | `decltype(Crackle{} - Time{})` |
| `Diffusivity` | `decltype(Area{} - Time{})` |
| `VolumetricFlowRate` | `decltype(Volume{} - Time{})` |
| `LinearDensity` | `decltype(Mass{} - Length{})` |
| `AreaDensity` | `decltype(Mass{} - Area{})` |
| `MassDensity` | `decltype(Mass{} - Volume{})` |
| `MassFlowRate` | `decltype(Mass{} - Time{})` |
| `MassFlux` | `decltype(MassFlowRate{} - Area{})` |
| `MolarMass` | `decltype(Mass{} - AmountOfSubstance{})` |
| `SpecificVolume` | `decltype(Volume{} - Mass{})` |
| `Momentum` | `decltype(Mass{} + Velocity{})` |
| `Force` | `decltype(Mass{} + Acceleration{})` |
| `SurfaceTension` | `decltype(Force{} - Length{})` |
| `Pressure` | `decltype(Force{} - Area{})` |
| `Viscosity` | `decltype(Pressure{} + Time{})` |
| `MomentOfInertia` | `decltype(Mass{} + Area{} - SolidAngle{})` |
| `Action` | `decltype(Momentum{} + Length{})` |
| `AngularMomentum` | `decltype(Action{} - Angle{})` |
| `Energy` | `decltype(Force{} + Length{})` |
| `Torque` | `decltype(Energy{} - Angle{})` |
| `Power` | `decltype(Energy{} - Time{})` |
| `Intensity` | `decltype(Power{} - Area{})` |
| `RadiantIntensity` | `decltype(Power{} - SolidAngle{})` |
| `Radiance` | `decltype(RadiantIntensity{} - Area{})` |
| `SpecificEnergy` | `decltype(Energy{} - Mass{})` |
| `SpecificPower` | `decltype(Power{} - Mass{})` |
| `MolarEnergy` | `decltype(Energy{} - AmountOfSubstance{})` |
| `TemperatureGradient` | `decltype(Temperature{} - Length{})` |
| `ThermalExpansionCoefficient` | `decltype(-Temperature{})` |
| `Entropy` | `decltype(Energy{} - Temperature{})` |
| `SpecificHeatCapacity` | `decltype(SpecificEnergy{} - Temperature{})` |
| `MolarEntropy` | `decltype(MolarEnergy{} - Temperature{})` |
| `VolumetricHeatCapacity` | `decltype(Entropy{} - Volume{})` |
| `ThermalConductivity` | `decltype(Power{} - Length{} - Temperature{})` |
| `HeatTransferCoefficient` | `decltype(Intensity{} - Temperature{})` |
| `ThermalConductance` | `decltype(Power{} - Temperature{})` |
| `ThermalResistance` | `decltype(Temperature{} - Power{})` |
| `ThermalResistivity` | `decltype(ThermalResistance{} + Length{})` |
| `Compressibility` | `decltype(-Pressure{})` |
| `ElectricCharge` | `decltype(ElectricCurrent{} + Time{})` |
| `CurrentDensity` | `decltype(ElectricCurrent{} - Area{})` |
| `LinearChargeDensity` | `decltype(ElectricCharge{} - Length{})` |
| `ElectricDisplacementField` | `decltype(ElectricCharge{} - Area{})` |
| `ElectricChargeDensity` | `decltype(ElectricCharge{} - Volume{})` |
| `ElectricDipoleMoment` | `decltype(ElectricCharge{} + Length{})` |
| `ElectricFieldStrength` | `decltype(Force{} - ElectricCharge{})` |
| `ElectricPotential` | `decltype(Energy{} - ElectricCharge{})` |
| `ElectricalResistance` | `decltype(ElectricPotential{} - ElectricCurrent{})` |
| `ElectricalResistivity` | `decltype(ElectricalResistance{} + Length{})` |
| `ElectricalConductance` | `decltype(-ElectricalResistance{})` |
| `ElectricalConductivity` | `decltype(-ElectricalResistivity{})` |
| `Capacitance` | `decltype(ElectricCharge{} - ElectricPotential{})` |
| `Permittivity` | `decltype(ElectricDisplacementField{} - ElectricFieldStrength{})` |
| `MagneticFieldStrength` | `decltype(ElectricCurrent{} - Length{})` |
| `MagneticMoment` | `decltype(ElectricCurrent{} + Area{} - Angle{})` |
| `Magnetization` | `decltype(MagneticMoment{} - Volume{})` |
| `MagneticFlux` | `decltype(ElectricPotential{} + Time{})` |
| `MagneticFluxDensity` | `decltype(MagneticFlux{} - Area{})` |
| `Inductance` | `decltype(MagneticFlux{} - ElectricCurrent{})` |
| `Permeability` | `decltype(MagneticFluxDensity{} - MagneticFieldStrength{})` |
| `Reluctance` | `decltype(ElectricCurrent{} - MagneticFlux{})` |
| `MolarConcentration` | `decltype(AmountOfSubstance{} - Volume{})` |
| `MolarVolume` | `decltype(Volume{} - AmountOfSubstance{})` |
| `CatalyticActivity` | `decltype(AmountOfSubstance{} - Time{})` |
| `ReactionRate` | `decltype(CatalyticActivity{} - Volume{})` |
| `LuminousFlux` | `decltype(LuminousIntensity{} + SolidAngle{})` |
| `Illuminance` | `decltype(LuminousFlux{} - Area{})` |
| `Luminance` | `decltype(LuminousIntensity{} - Area{})` |
| `LuminousEfficacy` | `decltype(LuminousFlux{} - Power{})` |
| `AngularVelocity` | `decltype(Angle{} - Time{})` |
| `AngularAcceleration` | `decltype(AngularVelocity{} - Time{})` |
| `AngularJerk` | `decltype(AngularAcceleration{} - Time{})` |


---

# Appendix B: Unit reference

`none` is the identity/no-unit state. It maps to `Dimensionless` but is not part of the explicit valid-unit range used for textual unit iteration.

| Unit enum | Symbol | ASCII input | Dimension | Default property | Prefixable | Description |
|---|---:|---|---|---|---:|---|
| `none` | — | — | `Dimensionless` | `none` | no | No unit. |
| `second` | s | — | `Time` | `none` | yes | SI base unit of time. |
| `meter` | m | — | `Length` | `none` | yes | SI base unit of length. |
| `gram` | g | — | `Mass` | `none` | yes | Unit of mass; 1 g = 1e-3 kg. |
| `ampere` | A | — | `ElectricCurrent` | `none` | yes | SI base unit of electric current. |
| `kelvin` | K | — | `Temperature` | `absolute` | yes | SI base unit of thermodynamic temperature. |
| `mole` | mol | — | `AmountOfSubstance` | `none` | yes | SI base unit of amount of substance. |
| `candela` | cd | — | `LuminousIntensity` | `none` | yes | SI base unit of luminous intensity. |
| `radian` | rad | — | `Angle` | `none` | yes | Plane angle; arc length divided by radius. |
| `steradian` | sr | — | `SolidAngle` | `none` | yes | Solid angle; subtended area divided by radius squared. |
| `hertz` | Hz | — | `Frequency` | `none` | yes | Frequency; 1 Hz = 1 s^-1. |
| `newton` | N | — | `Force` | `none` | yes | Force; 1 N = 1 kg*m*s^-2. |
| `pascal` | Pa | — | `Pressure` | `none` | yes | Pressure and stress; 1 Pa = 1 N/m^2. |
| `joule` | J | — | `Energy` | `none` | yes | Energy, work, and heat; 1 J = 1 N*m. |
| `watt` | W | — | `Power` | `none` | yes | Power and radiant flux; 1 W = 1 J/s. |
| `coulomb` | C | — | `ElectricCharge` | `none` | yes | Electric charge; 1 C = 1 A*s. |
| `volt` | V | — | `ElectricPotential` | `none` | yes | Electric potential; 1 V = 1 W/A. |
| `farad` | F | — | `Capacitance` | `none` | yes | Capacitance; 1 F = 1 C/V. |
| `ohm` | Ω | Ohm | `ElectricalResistance` | `none` | yes | Electrical resistance; 1 ohm = 1 V/A. |
| `siemens` | S | — | `ElectricalConductance` | `none` | yes | Electrical conductance; 1 S = 1 A/V. |
| `weber` | Wb | — | `MagneticFlux` | `none` | yes | Magnetic flux; 1 Wb = 1 V*s. |
| `tesla` | T | — | `MagneticFluxDensity` | `none` | yes | Magnetic flux density; 1 T = 1 Wb/m^2. |
| `henry` | H | — | `Inductance` | `none` | yes | Inductance; 1 H = 1 Wb/A. |
| `degree_celsius` | °C | degC | `Temperature` | `absolute` | yes | Celsius temperature; 0 deg C = 273.15 K. |
| `lumen` | lm | — | `LuminousFlux` | `none` | yes | Luminous flux; 1 lm = 1 cd*sr. |
| `lux` | lx | — | `Illuminance` | `none` | yes | Illuminance; 1 lx = 1 lm/m^2. |
| `becquerel` | Bq | — | `Frequency` | `none` | yes | Activity of a radionuclide; 1 Bq = 1 s^-1. |
| `gray` | Gy | — | `SpecificEnergy` | `none` | yes | Absorbed dose; 1 Gy = 1 J/kg. |
| `sievert` | Sv | — | `SpecificEnergy` | `none` | yes | Dose equivalent; 1 Sv = 1 J/kg. |
| `katal` | kat | — | `CatalyticActivity` | `none` | yes | Catalytic activity; 1 kat = 1 mol/s. |
| `minute` | min | — | `Time` | `none` | no | Time; 1 min = 60 s. |
| `hour` | h | — | `Time` | `none` | no | Time; 1 h = 60 min = 3600 s. |
| `day` | d | — | `Time` | `none` | no | Time; 1 d = 24 h = 86400 s. |
| `turn` | tr | — | `Angle` | `none` | no | Plane angle of one complete revolution; 1 turn = 2*pi rad. |
| `degree` | ° | deg | `Angle` | `none` | no | Plane angle; 1 deg = pi/180 rad. |
| `arcminute` | ′ | arcmin | `Angle` | `none` | no | Plane angle; 1 arcmin = 1/60 deg. |
| `arcsecond` | ″ | arcsec | `Angle` | `none` | no | Plane angle; 1 arcsec = 1/60 arcmin. |
| `are` | a | — | `Area` | `none` | no | Area; 1 a = 100 m^2. |
| `hectare` | ha | — | `Area` | `none` | no | Area; 1 ha = 100 a = 1e4 m^2. |
| `liter` | l | — | `Volume` | `none` | yes | Volume; 1 L = 1e-3 m^3. |
| `tonne` | t | — | `Mass` | `none` | yes | Mass; 1 t = 1000 kg. |
| `gal` | Gal | — | `Acceleration` | `none` | yes | Acceleration; 1 Gal = 1e-2 m/s^2. |
| `bar` | bar | — | `Pressure` | `none` | yes | Pressure; 1 bar = 1e5 Pa. |
| `var` | var | — | `Power` | `none` | yes | Reactive power; dimensionally equivalent to watt. |
| `calorie` | cal | — | `Energy` | `none` | yes | Thermochemical energy unit; 1 cal = 4.184 J exactly. |
| `angstrom` | Å | Angstrom | `Length` | `none` | yes | Length; 1 angstrom = 1e-10 m. |
| `barn` | b | — | `Area` | `none` | yes | Area used for cross sections; 1 b = 1e-28 m^2. |
| `dalton` | Da | — | `Mass` | `none` | yes | Mass; 1 Da is 1/12 the mass of a carbon-12 atom. |
| `electronvolt` | eV | — | `Energy` | `none` | yes | Energy; 1 eV = 1.602176634e-19 J exactly. |
| `astronomical_unit` | au | — | `Length` | `none` | no | Length; 1 au = 149597870700 m exactly. |
| `nautical_mile` | nmi | — | `Length` | `none` | no | Length; 1 nautical mile = 1852 m exactly. |
| `knot` | kn | — | `Velocity` | `none` | no | Speed; 1 kn = 1 nautical mile per hour. |
| `neper` | Np | — | `Dimensionless` | `logarithmic` | yes | Logarithmic ratio unit based on the natural logarithm. |
| `bel` | B | — | `Dimensionless` | `logarithmic` | yes | Logarithmic ratio unit based on the base-10 logarithm. |
| `percent` | % | percent | `Dimensionless` | `none` | no | Dimensionless ratio; 1 percent = 1e-2. |
| `ppm` | ppm | — | `Dimensionless` | `none` | no | Dimensionless ratio; 1 ppm = 1e-6. |


---

# Appendix C: Semantic quantity vocabulary

The table below follows the grouping and comments in `quantities.h`. Many aliases intentionally share one underlying dimension.

### Dimensionless quantities and angles

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `Factor` | `Quantity<>` | Multiplicative dimensionless factor. |
| `Fraction` | `Quantity<>` | Dimensionless fraction of a whole. |
| `MassFraction` | `Quantity<>` | Mass of a constituent divided by total mass. |
| `VolumeFraction` | `Quantity<>` | Volume of a constituent divided by total volume. |
| `MoleFraction` | `Quantity<>` | Amount of substance of a constituent divided by total amount of substance. |
| `Porosity` | `Quantity<>` | Fraction of a material volume occupied by voids. |
| `Strain` | `Quantity<>` | Relative deformation of a body. |
| `EngineeringStrain` | `Quantity<>` | Engineering strain relative to the original length. |
| `TrueStrain` | `Quantity<>` | Logarithmic strain based on incremental deformation. |
| `PoissonsRatio` | `Quantity<>` | Ratio of transverse to axial strain. |
| `Mach` | `Quantity<>` | Ratio of the speed of an object to the local speed of sound. |
| `RefractiveIndex` | `Quantity<>` | Ratio of light speed in vacuum to that in a medium. |
| `RelativePermittivity` | `Quantity<>` | Permittivity relative to vacuum permittivity. |
| `RelativePermeability` | `Quantity<>` | Permeability relative to vacuum permeability. |
| `MagneticSusceptibility` | `Quantity<>` | Linear magnetic response of a material. |
| `CoefficientOfFriction` | `Quantity<>` | Ratio between friction force and normal force. |
| `Emissivity` | `Quantity<>` | Fraction of black-body radiation emitted by a surface. |
| `Reflectance` | `Quantity<>` | Fraction of incident radiation reflected by a surface. |
| `Transmittance` | `Quantity<>` | Fraction of incident radiation transmitted through a material. |
| `PlaneAngle` | `dimension::Angle` | Ratio of circular arc length to radius. |
| `SolidAngle` | `dimension::SolidAngle` | Ratio of spherical surface area to radius squared. |

### Time

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `Time` | `dimension::Time` | Duration of an event. |
| `Period` | `dimension::Time` | Duration of one cycle of a periodic process. |
| `HalfLife` | `dimension::Time` | Time required for a quantity to fall to half its initial value. |
| `MeanLifetime` | `dimension::Time` | Mean lifetime of an unstable state or population. |

### Frequency and rates

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `Frequency` | `dimension::Frequency` | Number of periodic occurrences per unit time. |
| `AngularVelocity` | `dimension::AngularVelocity` | Rate of change of angular position. |
| `Radioactivity` | `dimension::Frequency` | Number of nuclear decays per unit time. |
| `DecayConstant` | `dimension::Frequency` | First-order exponential decay coefficient. |
| `StrainRate` | `dimension::Frequency` | Rate of deformation per unit time. |
| `ShearRate` | `dimension::Frequency` | Rate of shear deformation per unit time. |
| `AngularAcceleration` | `dimension::AngularAcceleration` | Rate of change of angular velocity. |

### Length, geometry, kinematics, and transport

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `Length` | `dimension::Length` | One-dimensional spatial extent. |
| `Distance` | `dimension::Length` | Separation between two positions. |
| `Displacement` | `dimension::Length` | Change in position. |
| `Position` | `dimension::Length` | Position coordinate measured from a reference. |
| `Radius` | `dimension::Length` | Radius of a circle or sphere. |
| `Diameter` | `dimension::Length` | Diameter of a circle or sphere. |
| `Thickness` | `dimension::Length` | Thickness of a layer or body. |
| `Wavelength` | `dimension::Length` | Spatial period of a wave. |
| `Wavenumber` | `dimension::Wavenumber` | Number of wave cycles per unit distance. |
| `Wavevector` | `dimension::Wavevector` | Spatial frequency vector magnitude. |
| `OpticalPower` | `dimension::Wavenumber` | Reciprocal focal length of an optical system. |
| `Area` | `dimension::Area` | Extent of a surface. |
| `Volume` | `dimension::Volume` | Three-dimensional extent of a body or region. |
| `NumberDensity` | `dimension::NumberDensity` | Number of dimensionless entities per unit volume. |
| `Absement` | `dimension::Absement` | Time integral of displacement. |
| `Velocity` | `dimension::Velocity` | Rate of change of position. |
| `Speed` | `dimension::Velocity` | Magnitude of velocity. |
| `Acceleration` | `dimension::Acceleration` | Rate of change of velocity. |
| `Jerk` | `dimension::Jerk` | Rate of change of acceleration. |
| `Jounce` | `dimension::Jounce` | Fourth time derivative of position. |
| `Crackle` | `dimension::Crackle` | Fifth time derivative of position. |
| `Pop` | `dimension::Pop` | Sixth time derivative of position. |
| `Diffusivity` | `dimension::Diffusivity` | Generic transport coefficient with dimensions of area per time. |
| `ThermalDiffusivity` | `dimension::Diffusivity` | Thermal diffusivity governing heat spreading in a material. |
| `KinematicViscosity` | `dimension::Diffusivity` | Momentum diffusivity of a fluid. |
| `DiffusionCoefficient` | `dimension::Diffusivity` | Diffusion coefficient in Fickian transport. |
| `VolumetricFlowRate` | `dimension::VolumetricFlowRate` | Volume transported per unit time. |

### Mass and density

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `Mass` | `dimension::Mass` | Resistance of a body to acceleration. |
| `LinearDensity` | `dimension::LinearDensity` | Mass per unit length. |
| `AreaDensity` | `dimension::AreaDensity` | Mass per unit area. |
| `MassDensity` | `dimension::MassDensity` | Mass per unit volume. |
| `MassFlowRate` | `dimension::MassFlowRate` | Mass transported per unit time. |
| `MassFlux` | `dimension::MassFlux` | Mass transported through unit area per unit time. |
| `MolarMass` | `dimension::MolarMass` | Mass per unit amount of substance. |
| `SpecificVolume` | `dimension::SpecificVolume` | Volume per unit mass. |

### Momentum and force

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `Momentum` | `dimension::Momentum` | Linear momentum of a body. |
| `Impulse` | `dimension::Momentum` | Change in momentum accumulated over time. |
| `LinearMomentum` | `dimension::Momentum` | Explicit name for translational momentum. |
| `Force` | `dimension::Force` | Rate of change of momentum. |
| `Weight` | `dimension::Force` | Gravitational force acting on a body. |
| `CentrifugalForce` | `dimension::Force` | Apparent outward force in a rotating reference frame. |
| `Thrust` | `dimension::Force` | Propulsive force produced by an engine or actuator. |
| `Drag` | `dimension::Force` | Resistive force opposing motion through a fluid. |
| `Lift` | `dimension::Force` | Force component perpendicular to a flow direction. |

### Pressure and material properties

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `Pressure` | `dimension::Pressure` | Normal force per unit area in a fluid or solid. |
| `Stress` | `dimension::Pressure` | Internal force per unit area in a material. |
| `NormalStress` | `dimension::Pressure` | Stress component normal to a plane. |
| `ShearStress` | `dimension::Pressure` | Stress component tangential to a plane. |
| `EnergyDensity` | `dimension::Pressure` | Energy stored or carried per unit volume. |
| `YoungsModulus` | `dimension::Pressure` | Ratio of uniaxial stress to elastic strain. |
| `ShearModulus` | `dimension::Pressure` | Ratio of shear stress to shear strain. |
| `BulkModulus` | `dimension::Pressure` | Resistance to uniform compression. |
| `ElasticModulus` | `dimension::Pressure` | Generic elastic stiffness expressed as stress per strain. |
| `Hardness` | `dimension::Pressure` | Resistance to local permanent indentation. |
| `TensileStrength` | `dimension::Pressure` | Maximum tensile stress sustained by a material. |
| `YieldStrength` | `dimension::Pressure` | Stress at the onset of plastic deformation. |
| `CompressiveStrength` | `dimension::Pressure` | Maximum compressive stress sustained by a material. |
| `FractureStrength` | `dimension::Pressure` | Stress associated with fracture under a specified loading mode. |
| `Compressibility` | `dimension::Compressibility` | Reciprocal bulk modulus or pressure sensitivity of volume. |

### Viscosity

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `Viscosity` | `dimension::Viscosity` | Dynamic resistance of a fluid to shear deformation. |
| `DynamicViscosity` | `dimension::Viscosity` | Explicit name for dynamic viscosity. |

### Rotational mechanics

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `MomentOfInertia` | `dimension::MomentOfInertia` | Rotational inertia about an axis. |
| `AngularMomentum` | `dimension::AngularMomentum` | Rotational analogue of linear momentum. |
| `Spin` | `dimension::AngularMomentum` | Intrinsic angular momentum. |
| `Action` | `dimension::Action` | Energy-time or momentum-length action. |

### Energy and thermodynamics

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `Energy` | `dimension::Energy` | Mechanical or thermodynamic energy. |
| `Work` | `dimension::Energy` | Energy transferred by a force through a displacement. |
| `Heat` | `dimension::Energy` | Energy transferred because of a temperature difference. |
| `Torque` | `dimension::Torque` | Moment of force about an axis. |
| `InternalEnergy` | `dimension::Energy` | Microscopic and macroscopic energy stored within a system. |
| `Enthalpy` | `dimension::Energy` | Thermodynamic energy including pressure-volume work. |
| `GibbsFreeEnergy` | `dimension::Energy` | Gibbs free energy of a system. |
| `HelmholtzFreeEnergy` | `dimension::Energy` | Helmholtz free energy of a system. |
| `ActivationEnergy` | `dimension::Energy` | Energy barrier associated with a process or reaction. |

### Specific energy and dose

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `SpecificEnergy` | `dimension::SpecificEnergy` | Energy per unit mass. |
| `SpecificInternalEnergy` | `dimension::SpecificEnergy` | Internal energy per unit mass. |
| `SpecificEnthalpy` | `dimension::SpecificEnergy` | Enthalpy per unit mass. |
| `SpecificGibbsFreeEnergy` | `dimension::SpecificEnergy` | Gibbs free energy per unit mass. |
| `SpecificHelmholtzFreeEnergy` | `dimension::SpecificEnergy` | Helmholtz free energy per unit mass. |
| `AbsorbedDose` | `dimension::SpecificEnergy` | Absorbed ionizing-radiation energy per unit mass. |
| `DoseEquivalent` | `dimension::SpecificEnergy` | Dose weighted for biological effectiveness. |
| `RadioactiveDose` | `dimension::SpecificEnergy` | Generic radioactive dose expressed as energy per mass. |

### Specific power

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `SpecificPower` | `dimension::SpecificPower` | Power per unit mass. |
| `AbsorbedDoseRate` | `dimension::SpecificPower` | Rate of absorbed dose. |

### Molar energy

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `MolarEnergy` | `dimension::MolarEnergy` | Energy per unit amount of substance. |
| `ChemicalPotential` | `dimension::MolarEnergy` | Partial molar Gibbs free energy of a species. |
| `MolarEnthalpy` | `dimension::MolarEnergy` | Enthalpy per unit amount of substance. |
| `MolarGibbsFreeEnergy` | `dimension::MolarEnergy` | Gibbs free energy per unit amount of substance. |
| `MolarHelmholtzFreeEnergy` | `dimension::MolarEnergy` | Helmholtz free energy per unit amount of substance. |

### Power

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `Power` | `dimension::Power` | Rate of energy transfer. |
| `RadiantIntensity` | `dimension::RadiantIntensity` | Electromagnetic power emitted into unit solid angle. |
| `RadiantFlux` | `dimension::Power` | Total radiant power emitted or received. |
| `HeatFlowRate` | `dimension::Power` | Rate of heat transfer. |
| `MechanicalPower` | `dimension::Power` | Mechanical work performed per unit time. |
| `ElectricalPower` | `dimension::Power` | Electrical energy transferred per unit time. |
| `AcousticPower` | `dimension::Power` | Acoustic energy transferred per unit time. |

### Intensity and radiometry

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `Intensity` | `dimension::Intensity` | Power transported through unit area. |
| `Irradiance` | `dimension::Intensity` | Incident radiant power per unit area. |
| `Radiance` | `dimension::Radiance` | Radiant power per projected area and unit solid angle. |
| `RadiantExitance` | `dimension::Intensity` | Radiant power leaving a surface per unit area. |
| `HeatFluxDensity` | `dimension::Intensity` | Heat flow through unit area. |
| `EnergyFluxDensity` | `dimension::Intensity` | Energy transported through unit area per unit time. |

### Surface tension

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `SurfaceTension` | `dimension::SurfaceTension` | Energy change per unit surface area. |
| `SurfaceEnergyDensity` | `dimension::SurfaceTension` | Surface free energy per unit area. |

### Temperature, thermal transport, and entropy

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `AbsoluteTemperature` | `Temperature + absolute` | Absolute thermodynamic temperature. |
| `TemperatureDifference` | `dimension::Temperature` | Difference between two temperatures. |
| `TemperatureGradient` | `dimension::TemperatureGradient` | Spatial rate of change of temperature. |
| `LinearThermalExpansionCoefficient` | `dimension::ThermalExpansionCoefficient` | Relative length change per unit temperature change. |
| `VolumetricThermalExpansionCoefficient` | `dimension::ThermalExpansionCoefficient` | Relative volume change per unit temperature change. |
| `Entropy` | `dimension::Entropy` | Entropy of a system. |
| `HeatCapacity` | `dimension::Entropy` | Heat required per unit temperature change. |
| `SpecificHeatCapacity` | `dimension::SpecificHeatCapacity` | Heat capacity per unit mass. |
| `SpecificEntropy` | `dimension::SpecificHeatCapacity` | Entropy per unit mass. |
| `MolarEntropy` | `dimension::MolarEntropy` | Entropy per unit amount of substance. |
| `MolarHeatCapacity` | `dimension::MolarEntropy` | Heat capacity per unit amount of substance. |
| `VolumetricHeatCapacity` | `dimension::VolumetricHeatCapacity` | Heat capacity per unit volume. |
| `ThermalConductivity` | `dimension::ThermalConductivity` | Conductive heat-flow coefficient of a material. |
| `HeatTransferCoefficient` | `dimension::HeatTransferCoefficient` | Heat-transfer coefficient across a surface. |
| `ThermalConductance` | `dimension::ThermalConductance` | Thermal conductance of a body or interface. |
| `ThermalResistance` | `dimension::ThermalResistance` | Resistance of a body or interface to heat flow. |
| `ThermalResistivity` | `dimension::ThermalResistivity` | Material property corresponding to thermal resistance per geometry. |

### Electricity

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `ElectricCurrent` | `dimension::ElectricCurrent` | Rate of flow of electric charge. |
| `MagnetomotiveForce` | `dimension::ElectricCurrent` | Magnetomotive force around a magnetic circuit. |
| `ElectricCharge` | `dimension::ElectricCharge` | Electric charge. |
| `CurrentDensity` | `dimension::CurrentDensity` | Electric current per unit cross-sectional area. |
| `LinearChargeDensity` | `dimension::LinearChargeDensity` | Electric charge per unit length. |
| `SurfaceChargeDensity` | `dimension::ElectricDisplacementField` | Electric charge per unit area. |
| `ElectricDisplacementField` | `dimension::ElectricDisplacementField` | Electric displacement field. |
| `Polarization` | `dimension::ElectricDisplacementField` | Electric polarization density. |
| `ElectricChargeDensity` | `dimension::ElectricChargeDensity` | Electric charge per unit volume. |
| `ElectricDipoleMoment` | `dimension::ElectricDipoleMoment` | Electric charge multiplied by separation distance. |
| `ElectricFieldStrength` | `dimension::ElectricFieldStrength` | Force per unit electric charge. |
| `ElectricPotential` | `dimension::ElectricPotential` | Energy per unit electric charge. |
| `Voltage` | `dimension::ElectricPotential` | Electric potential difference between two points. |
| `ElectromotiveForce` | `dimension::ElectricPotential` | Source-generated electric potential difference. |
| `ElectricalResistance` | `dimension::ElectricalResistance` | Opposition to direct electric current. |
| `Impedance` | `dimension::ElectricalResistance` | Opposition to alternating electric current. |
| `Reactance` | `dimension::ElectricalResistance` | Reactive component of impedance. |
| `ElectricalResistivity` | `dimension::ElectricalResistivity` | Resistance normalized by conductor geometry. |
| `ElectricalConductance` | `dimension::ElectricalConductance` | Reciprocal of electrical resistance. |
| `Admittance` | `dimension::ElectricalConductance` | Reciprocal of impedance. |
| `ElectricalConductivity` | `dimension::ElectricalConductivity` | Reciprocal of electrical resistivity. |
| `Capacitance` | `dimension::Capacitance` | Charge stored per unit electric potential. |
| `Permittivity` | `dimension::Permittivity` | Electric displacement response to electric field. |

### Magnetism

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `Magnetization` | `dimension::Magnetization` | Magnetic dipole moment per unit volume. |
| `MagneticFieldStrength` | `dimension::MagneticFieldStrength` | Magnetic field strength H. |
| `MagneticMoment` | `dimension::MagneticMoment` | Magnetic dipole moment. |
| `MagneticFlux` | `dimension::MagneticFlux` | Surface integral of magnetic flux density. |
| `MagneticFluxDensity` | `dimension::MagneticFluxDensity` | Magnetic flux per unit area. |
| `Inductance` | `dimension::Inductance` | Magnetic flux linkage per unit current. |
| `Permeability` | `dimension::Permeability` | Magnetic response coefficient relating B and H. |
| `Reluctance` | `dimension::Reluctance` | Opposition of a magnetic circuit to magnetic flux. |

### Amount of substance and chemistry

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `AmountOfSubstance` | `dimension::AmountOfSubstance` | Amount of substance measured in moles. |
| `MolarConcentration` | `dimension::MolarConcentration` | Amount of substance per unit volume. |
| `MolarVolume` | `dimension::MolarVolume` | Volume per unit amount of substance. |
| `ReactionRate` | `dimension::ReactionRate` | Reaction extent change per unit volume and time. |
| `CatalyticActivityConcentration` | `dimension::ReactionRate` | Catalytic activity per unit volume. |
| `CatalyticActivity` | `dimension::CatalyticActivity` | Amount of substance transformed per unit time by a catalyst. |

### Photometry

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `LuminousIntensity` | `dimension::LuminousIntensity` | Photometric luminous intensity of a source. |
| `LuminousFlux` | `dimension::LuminousFlux` | Photometric power weighted by human visual response. |
| `Illuminance` | `dimension::Illuminance` | Luminous flux incident per unit area. |
| `LuminousEfficacy` | `dimension::LuminousEfficacy` | Luminous flux produced per unit radiant power. |
| `Luminance` | `dimension::Luminance` | Luminous intensity per projected unit area. |

### Additional angular vocabulary

| Alias | Underlying dimension/state | Meaning |
|---|---|---|
| `AngularDisplacement` | `dimension::Angle` | Change in angular position. |
| `PhaseAngle` | `dimension::Angle` | Phase of a periodic quantity. |
| `AngularFrequency` | `dimension::AngularVelocity` | Angular rate of a periodic process. |
| `AngularWavenumber` | `dimension::Wavevector` | Angular spatial frequency. |
| `AngularImpulse` | `dimension::AngularMomentum` | Time integral of torque. |
| `AngularJerk` | `dimension::AngularJerk` | Rate of change of angular acceleration. |



---

## Appendix D: Literal-generation notes

The supplied generated header contains 1,039 distinct suffixes. Each suffix has both an integer and a `long double` literal overload, and both ultimately construct the library's configured `DefaultValue` unless `quantity_cast` is called directly with another `Value` type.

Prefixable units receive the supported SI prefix suffixes. Examples:

```text
_s, _ms, _us, _ns
_m, _km, _cm
_g, _kg, _mg
_Hz, _kHz, _MHz
_V, _mV, _kV
_Ohm, _kOhm
_rad, _mrad
_Np, _dB
_degC, _mdegC
```

Non-prefixable units receive only their base literal, for example:

```text
_min
_h
_d
_deg
_arcmin
_arcsec
_ha
_au
_nmi
_kn
_percent
_ppm
```

The generated file is consumed directly by `literals.h`; the generator source itself is not included in the supplied archive.
