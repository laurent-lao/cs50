# Questions

## What's `stdint.h`?

Header introducing the types below

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

Allows to specify a more "abstract" type than those from primary types `int`, `long`, etc

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

A `BYTE` is 1 byte, `DWORD` is 4 bytes, `LONG` is 8 bytes, `WORD` is 2 bytes (half of double word);

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

`bfType` (first two bytes) should be: `0x4d42` or `BM`

## What's the difference between `bfSize` and `biSize`?

According to the documentation linked on the `bmp.h` file, `bfSize` is the size in bytes of the bitmap file while `biSize` is the number of bytes (in hexadecimal) required by the `BITMAPINFOHEADER` structure 

## What does it mean if `biHeight` is negative?

When it is negative, the bitmap needs to be interpreted from the top to bottom and left to right (as opposed to bottom up).

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

`BITMAPINFOHEADER`'s `biBitCount` specifies the colour depth.

## Why might `fopen` return `NULL` in `copy.c`?

It might not have been able to open the file (maybe it does not exist, or permissions issues).

## Why is the third argument to `fread` always `1` in our code?

The size of the corresponding struct equals to the number of bytes we would like to read (and to assign to the corresponding struct's buffer).

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

`int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;`
With `sizeof(RGBTRIPLE)` = 3
`int padding = (4 - (3 * 3) % 4) % 4`
`int padding = (4 - 9 % 4) % 4`
`int padding = (4 - 1) % 4`
`int padding = 3`

## What does `fseek` do?

Move the file pointer to a certain location in relation to a certain position (`SEEK_SET`, `SEEK_END`, `SEEK_CUR`).

## What is `SEEK_CUR`?

`SEEK_CUR` is the current position of the file pointer. It is used as an argument in fseek()'s function.
