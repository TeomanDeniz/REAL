# REAL
**REAL** is a novel memory-based numeric format designed to overcome the limitations of traditional floating-point and integer representations. It enables exact arithmetic with extremely large or precise numbers—without any value loss. Whether you're working with massive integers like `1294182491824912471293172391247129658612.2328731` or tiny fractions, REAL guarantees full accuracy and fidelity.

REAL encodes numbers in a custom binary format using nibbles (4-bit segments), supporting special values like `INF`, `-INF`, `NAN`, and `-NAN`, as well as efficient digit compression. Repeating digits are packed using variable-size encoding blocks (12 to 24 bits), reducing memory usage significantly for repetitive sequences.

For example, these 6 bytes `10000001 11000000 11111101 00111010 00000101 01101111` are equals to:

`810000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003.056`

And if you collect this number with `1.1`, it is going to collect it!

