// include/dorm_energy/application/cli/command_type.hpp
#pragma once

namespace dorm_energy::cli
{

    enum class CommandType
    {
        Unknown,
        Help,
        Simulate,
        Daemon
    };

} // namespace dorm_energy::cli