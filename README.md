# Katakerm
Originally written in Java as the key derivation algorithm of a cipher I work on, katakerm acts as a cryptographic hash function.

A 256-bit output is produced for every input, as every byte of it
is cascaded onto every other, and retrieving the original input is computationally impossible due to the combination of successive xors of the content bytes with each other, and with the results of trigonometric functions and multiplications on intermediate long integers.

#### Assumptions
1. The byte ordering is assumed to be **little endian**.
2. All chars and numbers are assumed to be **signed**.
3. The rounding mode of decimal operations is assumed to be the default (equivalent to Java's HALF_UP)
4. All doubles are rounded to **exactly 12** decimal values.

If any of those is not met, the algorithm can and will yield different keys than meant to.


### The algorithm
A verbal description of the algorithm follows.

1. If the given input is less than 32bytes, it is copied to a 32byte array as is
and padded with zeros. Else, it is broken into 32-byte chunks whose respective bytes are xored with each other, as well as with their chunk number.

2) The original input length L used to deduce an xor-operation value X as follows:
<br>for any input of length L <= 127, X=L.
<br>Otherwise, X = L mod 128. If zero, then X = L mod 127

3. A two-dimensional array 4x8 is created, and populated with the contents of the result of the previous operation xored with the value X, the multiply of the respective position (e.g. 3*2 for the element [3][2], element count starting from 1 instead of 0). All bytes besides the first, are xored with the previous one. Let T the result.

4. The four 8-byte rows of T are treated as four signed long integer numbers, let Q,W,E,R. We deduce two doubles A,B as follows:
&nbsp;&nbsp;&nbsp;&nbsp;<br>&nbsp;&nbsp;&nbsp;&nbsp;A = cos(Q) + sin (W+E)
&nbsp;&nbsp;&nbsp;&nbsp;<br>&nbsp;&nbsp;&nbsp;&nbsp;B = cos(W+E) + sin(R)
They are then treated as simple 8-byte arrays for XOR operations.

5. A one-dimensional byte array K of length four is deduced as follows
<br>for every j ∈ [1,4], K[j] = T[j][1] XOR T[j][2] XOR T[j][7] XOR T[j][8]
	
6. We define the following:
<br>&nbsp;&nbsp;&nbsp;&nbsp;V = K[1] treated as signed long. If zero, V=1.
<br>&nbsp;&nbsp;&nbsp;&nbsp;B = K[2]^2 treated as long. If zero, V=2.
<br>&nbsp;&nbsp;&nbsp;&nbsp;N = K[3]^3 treated as long. If zero, V=4.
<br>&nbsp;&nbsp;&nbsp;&nbsp;M = K[4]^4 treated as long. If zero, V=27.
    <br>Let  V * B * N * M, a 64bit signed long treated as a byte array Z.
    <br>Every byte of Z is XORED with the respective byte of A and B
    and stored in Z.

7. key[i][j] = T[i][j] XOR Z[j] XOR (i * i * i AND 0XFF)
<br>

The result is guaranteed to never be an array of zeros, and
always change drastically for any input modification.

This is a key derivation algorithm with some of the properties of hash functions,
and may be used as such. This is not a mathematically robust hash function. The percentage of existent 256bit combinations that are possible through it is uncertain. Yet no collisions were noted for repeated automated tests of up to 50m randomly generated input strings and numbers.


