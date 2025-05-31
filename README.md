# REAL

Please pull with:

```sh
git clone --recurse-submodules --depth 1 "git@github.com:TeomanDeniz/REAL.git"
```

## **ToC** (Table of Contents):
* [What is REAL?](#What-is-REAL)
* [Where to use?](#Where-to-use)
* [How does it work?](#How-does-it-work)
* [Functions Table (C Language)](#Functions-Table-C)
* [Class Table (C++)](#Class-Table-CPP)

## What is REAL
**REAL** is a novel memory-based numeric format designed to overcome the limitations of traditional floating-point and integer representations. It enables exact arithmetic with extremely large or precise numbers—without any value loss. Whether you're working with massive integers like `1294182491824912471293172391247129658612.2328731` or tiny fractions, REAL guarantees full accuracy and fidelity.

REAL encodes numbers in a custom binary format using nibbles (4-bit segments), supporting special values like `INF`, `-INF`, `NAN`, and `-NAN`, as well as efficient digit compression. Repeating digits are packed using variable-size encoding blocks (12 to 24 bits), reducing memory usage significantly for repetitive sequences.

For example, these 6 bytes `10000001 11000000 11111101 00111010 00000101 01101111` are equals to:

`810000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003.056`

And if you collect this number with `1.1`, it is going to collect it!

## Where to use
This format is ideal for applications requiring extremely large or precise numeric values with minimal storage overhead. Suitable use cases include:

- Scientific computing with ultra-high precision or extremely large numbers.
- Games where point systems or stats need to scale massively or maintain exact precision (e.g., incremental or idle games).
- Blockchain or financial systems handling vast or sensitive numeric transactions.
- Data compression scenarios where repeated numeric patterns occur frequently.

## How does it work
REAL encodes numbers using nibbles (4 bits), optimizing both performance and size.

It includes:
* Native support for signed values
* Supports fraction numbers
* Built-in constants (INF, -INF, NAN)
* Decimal precision
* Compact repeating-digit encoding

### First Byte (Header)

| Value         | Meaning                                                                      |
| ------------- | ---------------------------------------------------------------------------- |
| `00000000`    | `INF`                                                                        |
| `11110000`    | `-INF`                                                                       |
| `11111111`    | `NAN`                                                                        |
| `1111----`    | (prefix) Indicates **negative** numbers (except for special constants above) |
| Else          | Starts normal number encoding                                                |

### Normal Nibbles

| Binary | Meaning                                     |
| ------ | ------------------------------------------- |
| `0000` | 0                                           |
| `0001` | 1                                           |
| `0010` | 2                                           |
| `0011` | 3                                           |
| `0100` | 4                                           |
| `0101` | 5                                           |
| `0110` | 6                                           |
| `0111` | 7                                           |
| `1000` | 8                                           |
| `1001` | 9                                           |
| `1010` | `.` (Decimal Point)                         |
| `1011` | Repetition Pack (4-bit size, 12-bit total)  |
| `1100` | Repetition Pack (8-bit size, 16-bit total)  |
| `1101` | Repetition Pack (12-bit size, 20-bit total) |
| `1110` | Repetition Pack (16-bit size, 24-bit total) |
| `1111` | EOR (End of REAL)                           |

### Repetition Packing

Compression triggers if a digit repeats more than 4 times:

| Code   | Replication Size       | Max Repeat   |
| ------ | ---------------------- | ------------ |
| `1011` | 4 bits  (12-bit total) | 15 digits    |
| `1100` | 8 bits  (16-bit total) | 256 digits   |
| `1101` | 12 bits (20-bit total) | 4095 digits  |
| `1110` | 16 bits (24-bit total) | 65535 digits |

```css
[PACK_CODE][REPEATED_DIGIT][REPEAT_COUNT]
```

Example:

```yaml
1011 0000 0100
^    ^    ^
|    |    Repeats 4 times
|    Repeated digit (0)
PACK_N (4-bit version)
```

### End of Number

REAL numbers are terminated with `1111` (EOR) to mark the end of a number.

### Example Encoding

```yaml
Encoded:
1000 0001 1011 0000 0100 0010 0011 1010 0101 0101 0001 1111
^^^^^^^^^ ^^^^^^^^^ ^^^^^^^^^ ^^^^^^^^^ ^^^^^^^^^ ^^^^^^^^^

Breakdown:
[8][1][PACK_4x0][2][3][.][5][5][1][EOR]

Output:
81000023.551
```

```yaml
Encoded:
0000 1010 1100 0000 0011 1110 0011 0010 0001 0000 0001 1111
^^^^^^^^^ ^^^^^^^^^ ^^^^^^^^^ ^^^^^^^^^ ^^^^^^^^^ ^^^^^^^^^

Breakdown:
[0][.][PACK_62x0][2][1][0][1][EOR]

Output:
0.00000000000000000000000000000000000000000000000000000000000002101
```

```yaml
Encoded:
0011 1011 1001 1110 0011 0010 1111 0000
^^^^^^^^^ ^^^^^^^^^ ^^^^^^^^^ ^^^^^^^^^

Breakdown:
[3][PACK_14x9][3][2][EOR]

Output:
3999999999999932
```

```yaml
Encoded:
0111 1101 0001 0001 1111 1010 1011 0001 1110 0010 1111 0000
^^^^^^^^^ ^^^^^^^^^ ^^^^^^^^^ ^^^^^^^^^ ^^^^^^^^^ ^^^^^^^^^

Breakdown:
[7][PACK_507x1][.][PACK_14x1][2][EOR]

Output:
711111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111.111111111111112
```

## Functions Table C

## Class Table CPP
