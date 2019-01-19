# tiffer

A simple tiff concatenation and filter program for C++ demonstration purposes only.

## Requirements

 * libtiff-4.0.3
 * C++11 compiler

## Compile and Run (on Linux)

```bash
g++ -ltiff src/main.cpp src/args.cpp src/tiffer.cpp -o tiffer
```

## Examples

```bash
./tiffer -help
./tiffer -version
```

### Concatenate tiff files:
```bash
./tiffer test/*.tif -output all.tif
```

### Concatenate tiff files removing blank pages:
```bash
./tiffer test/*.tif -output noblank.tif -remove-blank
```

### Concatenate tiff files removing blank pages, omit pages 2,4,5-9 from output, and overwrite existing file:
```bash
./tiffer test/*.tif -clobber -output noblank.tif -remove-blank -remove-pages 2,4,5-9
```

## Limitations

 * Tiled tiff files are not supported in this version.
 * Some older encodings such as ThuderScan, OJPEG, etc. are not supported.

## TODO

 * Handle tiled images.
 * Distinguish between encoded and unencoded files.
 * Support older formats.

Some [problems](Problems.md) have been documented when applying tiffer to sample tif images.
