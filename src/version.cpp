#include <string>
#include "time/version.hpp"

using namespace Time;

int Version::getMajor() noexcept
{
    return TIME_MAJOR;
}

int Version::getMinor() noexcept
{
    return TIME_MINOR;
}

int Version::getPatch() noexcept
{
    return TIME_PATCH;
}

bool Version::isAtLeast(const int major, const int minor,
                        const int patch) noexcept
{
    if (TIME_MAJOR < major){return false;}
    if (TIME_MAJOR > major){return true;}
    if (TIME_MINOR < minor){return false;}
    if (TIME_MINOR > minor){return true;}
    if (TIME_PATCH < patch){return false;}
    return true;
}

std::string Version::getVersion() noexcept
{
    std::string version(TIME_VERSION);
    return version;
}
