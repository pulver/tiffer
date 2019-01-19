#pragma once

#include "tiffio.h"
#include "args.h"

#include <functional>
#include <memory>
#include <string>

namespace tiffer {

struct TiffFree
{
    void operator()(void* data) { _TIFFfree(data); }
    void operator()(TIFF* tiff) { TIFFClose(tiff); }
};

using TiffData = std::unique_ptr<void,TiffFree>;
using TiffFile = std::unique_ptr<TIFF,TiffFree>;

class Tiff
{
    const std::string mFilename;

    TiffFile mOutput;

    int mOutputPage;

    Tiff(const std::string& filename);

    // Return: 0 if file has no directories or does not exist.
    // Otherwise directory count.
    size_t dirCount() const;

    void each_page(const std::function<void(const TiffFile&)>& callback);

    // Condition for a blank page - all 64-bit elements are the same. This only assumes that the number of
    // bytes per sample or pixel is a factor of 64. Thus any uniform color would be considered blank.
    bool is_blank(const TiffFile& input) const;

    bool open_for_writing(const bool clobber);;

    // Ok to ignore less than 8 bytes at end of strip, if any, when testing for overall page being blank.
    bool strip_is_blank(uint64_t blank64, const TiffData& tdata, const tsize_t len) const;

    static int write_output(const Args&);

    // Write current directory in tiffFile to mOutput which is assumed to be open for writing.
    void write_page(const TiffFile& input);

  public:

    static int run(int argc, char** argv);
};

} // namespace tiffer
