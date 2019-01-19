#pragma once

#include <list>
#include <string>
#include <unordered_set>
#include <vector>

namespace tiffer {

// Simple cross-platform command-line parser just for this project.
// Assumes filenames do not start with a dash '-'.
class Args
{
    using Argv = std::vector<std::string>;

    const Argv mArgv;

    // Params that require one value.
    static const std::unordered_set<std::string> mOneParam;

    // Params that require zero values.
    static const std::unordered_set<std::string> mZeroParams;

  public:

    Args(int argc, const char* const* argv);

    bool clobber() const;

    // All parameters after first that do not start with a dash, and are not after a mOneParam string.
    std::list<std::string> input_files() const;

    const std::string& output_file() const;

    bool remove_blank() const;

    // Remove individual pages (3) or ranges (5-8) separated by commas (3,5-8,10,11).
    // Does not do thorough error checking on valid syntax.
    std::unordered_set<size_t> remove_pages() const;

    bool show_help() const;

    bool show_version() const;

    const char* unknown_option() const;
};

} // namespace tiffer
