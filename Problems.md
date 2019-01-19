# Problems

There were some problems that were encountered when applying tiffer to the below sample tif files,
located in the problems/ subdirectory.

### cramps-tile.tif, quad-tile.tif, zackthecat.tif - This version does not handle tile images.

### caspian.tif - scrambled image output, but document viewer was showing a blank page on original.

    TIFFReadDirectory: Warning, Unknown field with tag 42112 (0xa480) encountered.
    TIFF Directory at offset 0x483b4 (295860)
      Image Width: 279 Image Length: 220
      Bits/Sample: 64
      Sample Format: IEEE floating point
      Compression Scheme: Deflate
      Photometric Interpretation: RGB color
      Samples/Pixel: 3
      Rows/Strip: 3
      Planar Configuration: separate image planes
      Tag 42112: <GDALMetadata>
      <Item name="SATELLITE">NOAA-9(F)</Item>
      <Item name="DATA_TYPE">AVHRR GAC</Item>
      <Item name="REVOLUTION">17492</Item>
      <Item name="SOURCE">Fairbanks, Alaska, USA (formerly Gilmore Creek)</Item>
      <Item name="PROCESSING_CENTER">NOAA/NESDIS - Suitland, Maryland, USA</Item>
      <Item name="START">year: 1988, day: 126, millisecond: 41569115</Item>
      <Item name="STOP">year: 1988, day: 126, millisecond: 41679615</Item>
      <Item name="LOCATION">Ascending</Item>
    </GDALMetadata>

### dscf0013.tif - distorted output image. No errors or warnings.

    TIFF Directory at offset 0x8 (8)
      Image Width: 640 Image Length: 480
      Resolution: 72, 72 pixels/inch
      Bits/Sample: 8
      Compression Scheme: None
      Photometric Interpretation: YCbCr
      YCbCr Subsampling: 2, 1
      YCbCr Positioning: cosited
      Orientation: row 0 top, col 0 lhs
      Samples/Pixel: 3
      Rows/Strip: 15
      Planar Configuration: single image plane
      Make: FUJIFILM
      Model: MX-2900ZOOM
      Software: Digital Camera MX-2900ZOOM Ver1.00
      DateTime: 2004:11:10 00:00:31
      Copyright:           
      EXIFIFDOffset: 0x280
    TIFF Directory at offset 0x3fe (1022)
      Image Width: 160 Image Length: 120
      Resolution: 72, 72 pixels/inch
      Bits/Sample: 8
      Compression Scheme: None
      Photometric Interpretation: YCbCr
      YCbCr Subsampling: 2, 1
      YCbCr Positioning: cosited
      Orientation: row 0 top, col 0 lhs
      Samples/Pixel: 3
      Rows/Strip: 25
      Planar Configuration: single image plane

### off_luv24.tif - Input image is of a rendered office space. Output is a couple yellow rectangles corresponding to panels in original. Rest of image is white.

    TIFF Directory at offset 0x36e10 (224784)
      Image Width: 333 Image Length: 225
      Resolution: 72, 72 pixels/inch
      Bits/Sample: 16
      Sample Format: signed integer
      Compression Scheme: SGILog24
      Photometric Interpretation: CIE Log2(L) (u',v')
      Orientation: row 0 top, col 0 lhs
      Samples/Pixel: 3
      Rows/Strip: 8
      Planar Configuration: single image plane
      Sample to Nits conversion factor: 1.7900e+02

### quad-jpeg.tif - Transcribed image shows half of original repeated, with horizontal green bars added.

    TIFF Directory at offset 0x5b6e (23406)
      Image Width: 512 Image Length: 384
      Position: 0, 0
      Bits/Sample: 8
      Compression Scheme: JPEG
      Photometric Interpretation: YCbCr
      Samples/Pixel: 3
      Rows/Strip: 16
      Planar Configuration: single image plane
      Reference Black/White:
         0:     0   255
         1:   128   255
         2:   128   255
      JPEG Tables: (574 bytes)

### smallliz.tif - Original is photo of lizard. Output is blank green image.

    TIFF Directory at offset 0x1208 (4616)
      Subfile Type: (0 = 0x0)
      Image Width: 160 Image Length: 160
      Resolution: 100, 100 pixels/inch
      Bits/Sample: 8
      Compression Scheme: Old-style JPEG
      Photometric Interpretation: YCbCr
      YCbCr Subsampling: 2, 2
      YCbCr Positioning: centered
      Samples/Pixel: 3
      Rows/Strip: 160
      Planar Configuration: single image plane
      Reference Black/White:
         0:     0   255
         1:   128   255
         2:   128   255
      Software: HP IL v1.1
      YCbCrCoefficients: 0.299000,0.587000,0.114000
      JpegInterchangeFormat: 8
      JpegInterchangeFormatLength: 4608
      JpegQTables: 34 103 103
      JpegDcTables: 172 205 205
      JpegAcTables: 238 421 421
      JpegProc: 1
      JpegRestartInterval: 0
    OJPEGSetupDecode: Warning, Depreciated and troublesome old-style JPEG compression mode, please convert to new-style JPEG compression and notify vendor of writing software.
    OJPEGSetupEncode: OJPEG encoding not supported; use new-style JPEG compression instead.
    Error: number of bytes read(38400) != bytes written(-1).

### text.tif - Contains 2 directories. First image outputs to blank white. Second image transcribes fine.

    TIFF Directory at offset 0x8 (8)
      Image Width: 1512 Image Length: 359
      Resolution: 296.64, 296.64
      Bits/Sample: 4
      Compression Scheme: ThunderScan
      Photometric Interpretation: min-is-white
      FillOrder: msb-to-lsb
      Orientation: row 0 top, col 0 lhs
      Samples/Pixel: 1
      Rows/Strip: 64
      Planar Configuration: single image plane
    ThunderScan strip encoding is not implemented.
    ThunderScan strip encoding is not implemented.
    ThunderScan strip encoding is not implemented.
    ThunderScan strip encoding is not implemented.
    ThunderScan strip encoding is not implemented.
    ThunderDecode: Not enough data at scanline 320 (0 != 1512).
    ThunderScan strip encoding is not implemented.
