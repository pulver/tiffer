# tiffer

A simple tiff concatenation and filter program for C++ demonstration purposes only.

## Requirements

 * libtiff-4.0.3
 * C++11 compiler

## Compile and Run (on Linux)

   g++ -ltiff src/main.cpp src/args.cpp src/tiffer.cpp -o tiffer

   ./tiffer -help
   ./tiffer test/*.tif -output all.tif
   ./tiffer test/*.tif -output noblank.tif -remove-blank


## Limitations

 * Tiled tiff files are not supported in this version.
 * Some older encodings such as ThuderScan, OJPEG, etc. are not supported.

## TODO

 * Handle tiled images.
 * Distinguish between encoded and unencoded files.
 * Support older formats.

Some [problems](Problems.md) have been documented when applying tiffer to sample tif images.
