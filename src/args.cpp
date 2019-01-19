#include "args.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace tiffer {

// Params that require one value.
const std::unordered_set<std::string> Args::mOneParam { "-output", "-remove-pages" };

// Params that require zero values.
const std::unordered_set<std::string> Args::mZeroParams { "-clobber", "-help", "-remove-blank", "-version" };

Args::Args(int argc, const char* const* argv)
:   mArgv(argv, argv+argc)
{
}

bool Args::clobber() const
{
    return std::find(mArgv.cbegin(), mArgv.cend(), "-clobber") != mArgv.cend();
}

// All parameters after first that do not start with a dash, and are not after a mOneParam string.
std::list<std::string> Args::input_files() const
{
    std::list<std::string> retval;
    for (Argv::const_iterator itr=mArgv.cbegin()+1 ; itr!=mArgv.cend() ; ++itr)
    {
        if (itr->at(0) != '-' && mOneParam.count(*std::prev(itr)) == 0)
        {
            retval.push_back(*itr);
        }
    }
    return retval;
}

const std::string& Args::output_file() const
{
    Argv::const_iterator itr = std::find(mArgv.cbegin(), mArgv.cend(), "-output");
    if (itr == mArgv.cend() || std::next(itr) == mArgv.cend())
        throw std::invalid_argument("-output is a required parameter to name the output TIFF file.");
    return *std::next(itr);
}

bool Args::remove_blank() const
{
    return std::find(mArgv.cbegin(), mArgv.cend(), "-remove-blank") != mArgv.cend();
}

// Remove individual pages (3) or ranges (5-8) separated by commas (3,5-8,10,11).
// Does not do thorough error checking on valid syntax.
std::unordered_set<size_t> Args::remove_pages() const
{
    std::unordered_set<size_t> pages;
    Argv::const_iterator itr = std::find(mArgv.cbegin(), mArgv.cend(), "-remove-pages");
    if (itr == mArgv.cend() || std::next(itr) == mArgv.cend())
        return pages;
    // Parse pages to remove
    std::istringstream ss(*std::next(itr));
    std::string range;
    while (std::getline(ss, range, ','))
    {
        size_t dash_pos = range.find('-');
        size_t page0, page1;
        if (dash_pos == std::string::npos)
        {
            std::istringstream(range) >> page0;
            pages.insert(page0);
        }
        else
        {
            std::istringstream(range.substr(0,dash_pos)) >> page0;
            std::istringstream(range.substr(dash_pos+1)) >> page1;
            for (size_t page=page0 ; page<=page1 ; ++page)
                pages.insert(page);
        }
    }
    return pages;
}

bool Args::show_help() const
{
    return mArgv.size() < 2 || std::find(mArgv.cbegin(), mArgv.cend(), "-help") != mArgv.cend();
}

bool Args::show_version() const
{
    return std::find(mArgv.cbegin(), mArgv.cend(), "-version") != mArgv.cend();
}

const char* Args::unknown_option() const
{
    for (const std::string& arg : mArgv)
    {
        if (!arg.empty() && arg.front() == '-')
        {
            if (mZeroParams.count(arg) == 0 && mOneParam.count(arg) == 0)
                return arg.c_str();
        }
    }
    return nullptr;
}

} // namespace tiffer
