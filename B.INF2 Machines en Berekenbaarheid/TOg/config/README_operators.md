# Operator Configuration (`operators.json`)

This project supports **fully custom unary and binary operators** through `config/operators.json`.

Operators are **not hardcoded**:
their **syntax, precedence, associativity, notation, and semantics** are all defined in JSON.

Each operator’s behavior is expressed as a small expression tree (`OpExpr`), which is parsed into an AST and evaluated at runtime using `std::cmath`.

This enables:

* custom symbols (`+`, `*`, `$`, etc.)
* custom precedence rules
* custom function-style operators (`max(a,b)`, `$(a,b)`)
* composed operator logic (`max(a,b) - b`, `sqrt(a*a + b*b)`, …)

---

## 1) File structure

`operators.json` contains two sections:

* `unary_operators` - operators/functions with **one argument**
* `binary_operators` - operators/functions with **two arguments**

Each operator entry includes:

| Field      | Meaning                                         |
| ---------- | ----------------------------------------------- |
| `symbol`   | The token text (e.g. `"+"`, `"sqrt"`, `"$"`)    |
| `notation` | `"prefix"`, `"postfix"`, `"infix"`, or `"call"` |
| `expr`     | Expression tree defining semantics              |

Additional fields:

| Field    | Applies to   | Meaning                                |
| -------- | ------------ | -------------------------------------- |
| `arity`  | `call`       | Number of arguments (`1` or `2`)       |
| `weight` | infix binary | Precedence (higher = stronger binding) |
| `assoc`  | infix binary | `"left"` or `"right"` associativity    |

---

## 2) Supported notation types

### A) Unary prefix operators

Examples: `-x`, `+x`

```json
{
  "symbol": "-",
  "notation": "prefix",
  "expr": {
    "type": "unary",
    "op": "neg",
    "arg": { "type": "var", "name": "x" }
  }
}
```

Notes:

* Unary operators **must use variable `x`**
* Unary operators bind tighter than binary infix by design

---

### B) Unary postfix operators

Example: `50%`

```json
{
  "symbol": "%",
  "notation": "postfix",
  "expr": {
    "type": "binary",
    "op": "div",
    "left": { "type": "var", "name": "x" },
    "right": { "type": "number", "value": 100 }
  }
}
```

---

### C) Binary infix operators

Examples: `a + b`, `a * b`, `a ^ b`

```json
{
  "symbol": "*",
  "notation": "infix",
  "weight": 20,
  "assoc": "left",
  "expr": {
    "type": "binary",
    "op": "mul",
    "left":  { "type": "var", "name": "a" },
    "right": { "type": "var", "name": "b" }
  }
}
```

Rules:

* Must use variables **`a`** and **`b`**
* `weight` controls precedence
* `assoc` controls grouping when weights match

#### Associativity behavior

| assoc | Expression  | Parsed as     |
| ----- | ----------- | ------------- |
| left  | `a - b - c` | `(a - b) - c` |
| right | `a ^ b ^ c` | `a ^ (b ^ c)` |

---

### D) Function-call operators

Examples: `sin(x)`, `max(a,b)`, `$(a,b)`

Unary call:

```json
{
  "symbol": "sin",
  "notation": "call",
  "arity": 1,
  "expr": {
    "type": "unary",
    "op": "sin",
    "arg": { "type": "var", "name": "x" }
  }
}
```

Binary call:

```json
{
  "symbol": "max",
  "notation": "call",
  "arity": 2,
  "expr": {
    "type": "binary",
    "op": "max",
    "left": { "type": "var", "name": "a" },
    "right": { "type": "var", "name": "b" }
  }
}
```

Notes:

* Calls always evaluate arguments first
* Function precedence is naturally **higher than infix**

---

## 3) Custom operator example: `$`

This project supports defining **non-standard operator symbols**, including **custom semantics**.

### `$` as a binary operator

Semantic definition:

```
a $ b = max(a, b) - b
```

### Infix form: `a $ b`

```json
{
  "symbol": "$",
  "notation": "infix",
  "weight": 5,
  "assoc": "left",
  "expr": {
    "type": "binary",
    "op": "sub",
    "left": {
      "type": "binary",
      "op": "max",
      "left": { "type": "var", "name": "a" },
      "right": { "type": "var", "name": "b" }
    },
    "right": { "type": "var", "name": "b" }
  }
}
```

### Call form: `$(a, b)`

```json
{
  "symbol": "$",
  "notation": "call",
  "arity": 2,
  "expr": {
    "type": "binary",
    "op": "sub",
    "left": {
      "type": "binary",
      "op": "max",
      "left": { "type": "var", "name": "a" },
      "right": { "type": "var", "name": "b" }
    },
    "right": { "type": "var", "name": "b" }
  }
}
```

This demonstrates:

* Custom characters as first-class operators
* Identical semantics across infix + call syntax
* Parser extensibility beyond traditional math operators

---

## 4) Precedence system (weight + assoc)

Binary infix precedence is fully configurable.

### Recommended defaults

| Operator | Weight | Assoc |
| -------- | ------ | ----- |
| `+` `-`  | 10     | left  |
| `*` `/`  | 20     | left  |
| `^`      | 30     | right |

### Example

Expression:

```
5 + 10 * 3
```

* If `*` weight > `+` → `5 + (10 * 3)`
* If `+` weight > `*` → `(5 + 10) * 3`

> Unary and call operators already bind tighter by grammar design.

---

## 5) Expression tree format (`OpExpr`)

Operators compile to AST nodes.

### Node types

| Type     | Meaning                     |
| -------- | --------------------------- |
| `number` | Constant literal            |
| `var`    | Placeholder (`x`, `a`, `b`) |
| `unary`  | Unary operation             |
| `binary` | Binary operation            |

### Example: `hypot(a,b) = sqrt(a*a + b*b)`

```json
{
  "type": "unary",
  "op": "sqrt",
  "arg": {
    "type": "binary",
    "op": "add",
    "left": {
      "type": "binary",
      "op": "mul",
      "left": { "type": "var", "name": "a" },
      "right": { "type": "var", "name": "a" }
    },
    "right": {
      "type": "binary",
      "op": "mul",
      "left": { "type": "var", "name": "b" },
      "right": { "type": "var", "name": "b" }
    }
  }
}
```

---

## 6) Supported operation identifiers (`"op"`)

These are **hard-coded in C++** (`applyUnaryOp` / `applyBinaryOp`).

### Unary ops

* abs / fabs
* sin, cos, tan
* asin, acos, atan
* sinh, cosh, tanh
* exp
* log, log10
* sqrt
* ceil, floor, trunc, round
* neg, pos

### Binary ops

* add, sub, mul, div
* pow
* fmod
* atan2
* min, max

Unknown ops throw runtime errors.

---

## 7) Adding new custom operators

### A) Binary infix operator (modulo)

```json
{
  "symbol": "|",
  "notation": "infix",
  "weight": 20,
  "assoc": "left",
  "expr": {
    "type": "binary",
    "op": "fmod",
    "left": { "type": "var", "name": "a" },
    "right": { "type": "var", "name": "b" }
  }
}
```

### B) Unary function (`ceil(x)`)

```json
{
  "symbol": "ceil",
  "notation": "call",
  "arity": 1,
  "expr": {
    "type": "unary",
    "op": "ceil",
    "arg": { "type": "var", "name": "x" }
  }
}
```

### C) Binary function (`atan2(a,b)`)

```json
{
  "symbol": "atan2",
  "notation": "call",
  "arity": 2,
  "expr": {
    "type": "binary",
    "op": "atan2",
    "left": { "type": "var", "name": "a" },
    "right": { "type": "var", "name": "b" }
  }
}
```

---

## 8) Parser behavior notes (important)

* Call-operators may be identifiers **or symbols** (e.g. `max(...)`, `$(...)`)
* Token classification depends on **parser configuration**
* Custom symbols must be supported by the lexer to avoid “unknown symbol” errors
* Precedence rules only apply to **binary infix operators**
* Validation strictly follows the loaded operator set - no fallback operators exist