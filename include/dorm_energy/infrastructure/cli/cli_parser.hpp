// include/dorm_energy/infrastructure/cli/cli_parser.hpp
#pragma once

#include "dorm_energy/application/cli/command_options.hpp"

#include <CLI/CLI.hpp>

#include <memory>

namespace dorm_energy::cli
{

    enum class ParseResult
    {
        Continue,
        ExitSuccess,
        ExitError
    };

    /**
     * @brief Отвечает только за настройку и парсинг командной строки
     */
    class CliParser
    {
    public:
        CliParser();

        /**
         * @brief Парсит аргументы командной строки
         * @param Options для заполнения результатами парсинга
         * @return Что нужно делать дальше
         */
        ParseResult parse(int argc, char **argv, CommandOptions &options);

    private:
        void setupCommands();

        std::unique_ptr<CLI::App> app_;
    };

} // namespace dorm_energy::application::cli