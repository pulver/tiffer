#include "tiffer.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace tiffer {

#include "version.h" // Set TIFFER_VERSION

// Tags listed in `man TIFFSetField`.
ttag_t TIFFTAGS[] = { TIFFTAG_ARTIST, TIFFTAG_BITSPERSAMPLE, TIFFTAG_COLORMAP, TIFFTAG_COMPRESSION,
    TIFFTAG_CONSECUTIVEBADFAXLINES, TIFFTAG_COPYRIGHT, TIFFTAG_DATETIME, TIFFTAG_DOCUMENTNAME, TIFFTAG_DOTRANGE,
    TIFFTAG_EXTRASAMPLES, TIFFTAG_FAXFILLFUNC, TIFFTAG_FAXMODE, TIFFTAG_FILLORDER, TIFFTAG_GROUP3OPTIONS,
    TIFFTAG_GROUP4OPTIONS, TIFFTAG_HALFTONEHINTS, TIFFTAG_HOSTCOMPUTER, TIFFTAG_ICCPROFILE, TIFFTAG_IMAGEDEPTH,
    TIFFTAG_IMAGEDESCRIPTION, TIFFTAG_IMAGELENGTH, TIFFTAG_IMAGEWIDTH, TIFFTAG_INKNAMES, TIFFTAG_INKSET,
    TIFFTAG_JPEGCOLORMODE, TIFFTAG_JPEGQUALITY, TIFFTAG_JPEGTABLES, TIFFTAG_JPEGTABLESMODE, TIFFTAG_MAKE,
    TIFFTAG_MATTEING, TIFFTAG_MAXSAMPLEVALUE, TIFFTAG_MINSAMPLEVALUE, TIFFTAG_MODEL, TIFFTAG_ORIENTATION,
    TIFFTAG_PAGENAME, TIFFTAG_PAGENUMBER, TIFFTAG_PHOTOMETRIC, TIFFTAG_PHOTOSHOP, TIFFTAG_PLANARCONFIG,
    TIFFTAG_PREDICTOR, TIFFTAG_PRIMARYCHROMATICITIES, TIFFTAG_REFERENCEBLACKWHITE, TIFFTAG_RESOLUTIONUNIT,
    TIFFTAG_RICHTIFFIPTC, TIFFTAG_ROWSPERSTRIP, TIFFTAG_SAMPLEFORMAT, TIFFTAG_SAMPLESPERPIXEL, TIFFTAG_SMAXSAMPLEVALUE,
    TIFFTAG_SMINSAMPLEVALUE, TIFFTAG_SOFTWARE, TIFFTAG_STONITS, TIFFTAG_SUBFILETYPE, TIFFTAG_SUBIFD,
    TIFFTAG_TARGETPRINTER, TIFFTAG_THRESHHOLDING, TIFFTAG_TILEDEPTH, TIFFTAG_TILELENGTH, TIFFTAG_TILEWIDTH,
    TIFFTAG_TRANSFERFUNCTION, TIFFTAG_WHITEPOINT, TIFFTAG_XMLPACKET, TIFFTAG_XPOSITION, TIFFTAG_XRESOLUTION,
    TIFFTAG_YCBCRCOEFFICIENTS, TIFFTAG_YCBCRPOSITIONING, TIFFTAG_YPOSITION, TIFFTAG_YRESOLUTION };

// Removed the following tags due to TIFFSetField errors:
// TIFFTAG_BADFAXLINES  // TIFFSetField: output.tif: Unknown tag 326.
// TIFFTAG_CLEANFAXDATA // TIFFSetField: output.tif: Unknown tag 327.

// TIFFTAG_YCBCRSAMPLING was listed in `man TIFFSetField` but not in any include file.

Tiff::Tiff(const std::string& filename)
:   mFilename(filename)
{
}

// Return: 0 if file has no directories or does not exist.
// Otherwise directory count.
size_t Tiff::dirCount() const
{
    if (TiffFile tiff = TiffFile(TIFFOpen(mFilename.c_str(),"r")))
    {
        size_t count = 1;
        while (TIFFReadDirectory(tiff.get()))
            ++count;
        return count;
    }
    else
        return 0;
}

void Tiff::each_page(const std::function<void(const TiffFile&)>& callback)
{
    if (TiffFile tiff = TiffFile(TIFFOpen(mFilename.c_str(),"r")))
    {
        do
        {
            callback(tiff);
        } while (TIFFReadDirectory(tiff.get()));
    }
    else
    {
        std::cerr << "Error: Unable to TIFFOpen("<<mFilename<<",\"r\") in each_page()." << std::endl;
    }
}

// Condition for a blank page - all 64-bit elements are the same. This only assumes that the number of
// bytes per sample or pixel is a factor of 64. Thus any uniform color would be considered blank.
bool Tiff::is_blank(const TiffFile& input) const
{
    const tsize_t stripSize = TIFFStripSize(input.get());
    const tsize_t numberOfStrips = TIFFNumberOfStrips(input.get());
    if (TiffData tdata = TiffData(_TIFFmalloc(stripSize)))
    {
        TIFFReadEncodedStrip(input.get(), 0, tdata.get(), static_cast<tsize_t>(-1));
        const uint64_t blank64 = *static_cast<uint64_t*>(tdata.get());
        for (tstrip_t s=0 ; s<numberOfStrips ; ++s)
        {
            const tsize_t m = TIFFReadEncodedStrip(input.get(), s, tdata.get(), static_cast<tsize_t>(-1));
            if (stripSize < m)
                std::cerr << "Error: stripSize("<<stripSize<<") < number of bytes read("<<m<<")." << std::endl;
            if (!strip_is_blank(blank64, tdata, m))
                return false;
        }
    }
    else
    {
        std::cerr << "Error: Unable to _TIFFmalloc("<<stripSize<<") in write_page()." << std::endl;
    }
    return true;
}

bool Tiff::open_for_writing(const bool clobber)
{
    if (!clobber && std::ifstream(mFilename))
    {
        std::cerr << "Error: File("<<mFilename<<") exists and will not be overwriten." << std::endl;
        return false;
    }
    mOutput = TiffFile(TIFFOpen(mFilename.c_str(),"w"));
    mOutputPage = 0;
    return static_cast<bool>(mOutput);
}

int Tiff::run(int argc, char** argv)
{
    const Args args(argc, argv);

    if (const char* unknown_option = args.unknown_option())
    {
        std::cout << "Error: Unknown option(" << unknown_option << ')' << std::endl;
        return 1;
    }
    else if (args.show_help())
    {
        std::cout << "Usage: tiffer [OPTIONS] input0 [input1 input2 ...] -output output\n"
            "Options:\n"
            "  -help           Show this message\n"
            "  -version        Show name, version, copyright, author, license\n"
            "  -clobber        Overwrite output if it exists\n"
            "  -remove-blank   Omit blank pages from output\n"
            "  -remove-pages page_list\n"
            "                  Omit pages in page_list. page_list is a comma-delimited list of\n"
            "                  single pages or page ranges. A page range is a pair of page\n"
            "                  numbers separated by a dash. Example: 2,4,6-9,12,14-20\n"
            "\n"
            "Note: Tiled images are not supported in this version and are omitted from the output.\n"
            << std::endl;
        return 0;
    }
    else if (args.show_version())
    {
        std::cout << "tiffer " << TIFFER_VERSION << '\n'
            << "Copyright (c) 2019 Matthew Pulver\n"
            << "MIT License\n" << std::endl;
        return 0;
    }
    else
    {
        try
        {
            return write_output(args);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception caught: " << e.what() << std::endl;
            return 1;
        }
    }
}

// Ok to ignore less than 8 bytes at end of strip, if any, when testing for overall page being blank.
bool Tiff::strip_is_blank(uint64_t blank64, const TiffData& tdata, const tsize_t len) const
{
    const tsize_t N = len / 8;
    for (tsize_t i=0 ; i<N ; ++i)
    {
        if (blank64 != *(static_cast<uint64_t*>(tdata.get())+i))
            return false;
    }
    return true;
}

int Tiff::write_output(const Args& args)
{
    const bool remove_blank = args.remove_blank();
    Tiff output(args.output_file());
    std::unordered_set<size_t> remove_pages = args.remove_pages();
    size_t page_num = 1;
    size_t pages_written = 0;
    uint32_t* tile_length; // Not used - only to test if input is tiled.

    if (output.open_for_writing(args.clobber()))
    {
        const std::list<std::string> input_files = args.input_files();
        for (const std::string& input_file : input_files)
        {
            //std::cerr << "Opening input_file("<<input_file<<')' << std::endl;
            Tiff tiff(input_file);
            tiff.each_page([&](const TiffFile& input)
            {
                //TIFFPrintDirectory(input.get(), stdout); // DEBUG
                const bool is_tiled = TIFFGetField(input.get(), TIFFTAG_TILELENGTH, &tile_length);
                if (is_tiled)
                    std::cerr << "File("<<input_file<<") is tiled. Skipping." << std::endl;
                const bool skip_page = is_tiled || (remove_blank && tiff.is_blank(input))
                        || remove_pages.count(page_num)==1;
                if ( !skip_page )
                {
                    output.write_page(input);
                    ++pages_written;
                }
                ++page_num;
            });
        }
        std::cerr << "Done. Pages written = " << pages_written << std::endl;
        return 0;
    }
    else
    {
        std::cerr << "Error: Unable to write to " << args.output_file() << std::endl;
        return 1;
    }
}

// Write current directory in tiffFile to mOutput which is assumed to be open for writing.
void Tiff::write_page(const TiffFile& input)
{
    if (0 < mOutputPage)
    {
        // Advance to next directory if at least one page was written. Called automatically when closed.
        if (!TIFFWriteDirectory(mOutput.get()))
            std::cerr << "Error calling TIFFWriteDirectory() in write_page()." << std::endl;
    }
    void* fp[3]; // field params
    for (ttag_t tag : TIFFTAGS)
    {
        if (TIFFGetField(input.get(), tag, &fp[0], &fp[1], &fp[2]) == 1)
        {
            if (TIFFSetField(mOutput.get(), tag, fp[0], fp[1], fp[2]) == 0)
                std::cerr << "Error: TIFFSetField() for tag("<<tag<<") returned 0." << std::endl;
        }
    }
    const tsize_t stripSize = TIFFStripSize(input.get());
    const tsize_t numberOfStrips = TIFFNumberOfStrips(input.get());
    if (TiffData tdata = TiffData(_TIFFmalloc(stripSize)))
    {
        for (tstrip_t s=0 ; s<numberOfStrips ; ++s)
        {
            // Strips
            const tsize_t m = TIFFReadEncodedStrip(input.get(), s, tdata.get(), static_cast<tsize_t>(-1));
            if (stripSize < m)
                std::cerr << "Error: stripSize("<<stripSize<<") < number of bytes read("<<m<<")." << std::endl;
            const tsize_t n = TIFFWriteEncodedStrip(mOutput.get(), s, tdata.get(), m);
            if (m != n)
                std::cerr << "Error: number of bytes read("<<m<<") != bytes written("<<n<<")." << std::endl;
        }
    }
    else
    {
        std::cerr << "Error: Unable to _TIFFmalloc("<<stripSize<<") in write_page()." << std::endl;
    }
    ++mOutputPage;
    std::cerr << "Info: mOutputPage = " << mOutputPage << std::endl;
}

} // namespace tiffer
