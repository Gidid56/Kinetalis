#pragma once

namespace utility::file
{
    enum class Type
    {
        csv, // Comma-separated values
        txt, // Plain-text data
        xyz, // XYZ coordinate format
    };

    template <typename Stream>
    concept FileStream = std::same_as<Stream, std::ifstream> or std::same_as<Stream, std::ofstream>;

    template <FileStream Stream>
    [[nodiscard]]
    Stream wait_for_open(std::filesystem::path const& path, std::ios_base::openmode mode)
    {
        if constexpr (std::same_as<Stream, std::ofstream>)
            if (auto const parent{ path.parent_path() }; not parent.empty())
                std::filesystem::create_directories(parent);

        for (auto retry_delay{ std::chrono::seconds{ 2 } };;
            retry_delay = std::min(2 * retry_delay, std::chrono::seconds{ 30 }))
        {
            Stream file{ path, mode };

            if (file)
                return file;

            if constexpr (std::same_as<Stream, std::ifstream>)
                if (not std::filesystem::exists(path))
                    throw std::runtime_error{
                        std::format("Input file '{}' does not exist.", path.string())
                    };

            std::print("Could not open \"{}\". Retry in {}.\n", path.string(), retry_delay);
            std::this_thread::sleep_for(retry_delay);
        }
    }

    [[nodiscard]] inline auto wait_for_read(std::filesystem::path const& path)
    {
        return wait_for_open<std::ifstream>(path, std::ios_base::in);
    }

    [[nodiscard]] inline auto wait_for_write(std::filesystem::path const& path)
    {
        return wait_for_open<std::ofstream>(path, std::ios_base::out);
    }

    inline bool try_write(std::filesystem::path const& path, std::string_view text)
    {
        std::ofstream stream{ path, std::ios::binary | std::ios::trunc };
        stream.write(text.data(), static_cast<std::streamsize>(text.size()));
        return static_cast<bool>(stream);
    }

    template <Type type>
    void save(std::filesystem::path path, std::ios_base::openmode mode, std::string_view output)
    {
        path.replace_extension(enum_name(type));

        auto file{ wait_for_open<std::ofstream>(path, mode) };
        file << output;

        if (not file)
            throw std::runtime_error{ std::format("Error while writing to '{}'.", path.string()) };
    }

    template <Type type>
    void write(std::filesystem::path const& path, std::string_view output)
    {
        save<type>(path, std::ios_base::out, output);
    }

    template <Type type>
    void append(std::filesystem::path const& path, std::string_view output)
    {
        save<type>(path, std::ios_base::app, output);
    }

    [[nodiscard]] inline std::vector<std::filesystem::path> directories(
        std::filesystem::path const& path)
    {
        std::vector<std::filesystem::path> result;

        for (auto const& entry : std::filesystem::directory_iterator{ path })
            if (entry.is_directory())
                result.push_back(entry.path());

        std::ranges::sort(result);
        return result;
    }

    [[nodiscard]] inline std::vector<std::filesystem::path>
    recursive_files(std::filesystem::path const& root, file::Type type)
    {
        std::vector<std::filesystem::path> result;

        auto const extension{ std::format(".{}", enum_name(type)) };

        for (auto const& entry : std::filesystem::recursive_directory_iterator{ root })
            if (entry.is_regular_file() and entry.path().extension() == extension)
                result.push_back(entry.path());

        std::ranges::sort(result);
        return result;
    }

    [[nodiscard]] inline auto create_unique_directory(
        std::filesystem::path const& parent,
        std::string_view name = "new_directory")
    {
        std::filesystem::create_directories(parent);

        for (std::size_t index{ 1 };; ++index)
        {
            auto const candidate{ index == 1 ? parent / name
                                             : parent / std::format("{} ({})", name, index) };

            if (std::filesystem::create_directory(candidate))
                return candidate;
        }
    }
}
