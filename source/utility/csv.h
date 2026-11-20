#pragma once
#include <charconv>
#include <concepts>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace utility::csv
{
    template <typename Value>
    requires std::integral<Value> or std::floating_point<Value>
    std::optional<Value> parse(std::string_view field)
    {
        Value value{};
        auto const begin{ field.data() };
        auto const end{ begin + field.size() };
        auto const result{ [&] {
            if constexpr (std::floating_point<Value>)
                return std::from_chars(begin, end, value, std::chars_format::general);
            else
                return std::from_chars(begin, end, value);
        }() };

        if (result.ec != std::errc{} or result.ptr != end)
            return std::nullopt;

        return value;
    }


    enum class Padding
    {
        empty,
        zero,
    };

    // Streaming, append-only wide CSV with an on-disk column-growth history.
    // Column identities and their order are supplied by the owner; new columns
    // may only be appended. Formatter callbacks produce single-line CSV records.
    class Writer
    {
    public:
        using Formatter = std::function<std::string()>;

        explicit Writer(Padding padding = Padding::empty) : padding_{ padding } { }

        void initialize(std::filesystem::path base_path, Formatter header, Formatter row);
        void append(std::size_t data_columns);
        void finalize();

    private:
        std::filesystem::path path(std::string_view suffix = {}) const;

        std::filesystem::path base_path_;
        Formatter header_, row_;
        Padding padding_;
        std::size_t rows_{};
        std::optional<std::size_t> previous_columns_;
    };

    class Table
    {
    public:
        using Row = std::vector<std::string>;
        using Data = std::vector<Row>;

        Table() = default;
        explicit Table(Data data) : data_{ std::move(data) } { }

        Table& read(std::filesystem::path);
        Table& write(std::filesystem::path);

        std::string text() const;
        static std::string encode_field(std::string_view);

        bool rows_match_header() const;

        auto const& data() const { return data_; }
        auto& mutable_data() { return data_; }

        bool empty() const { return data_.empty(); }
        std::size_t size() const { return data_.size(); }
        auto const& front() const { return data_.front(); }
        auto const& operator[](std::size_t index) const { return data_[index]; }
        auto begin() const { return data_.begin(); }
        auto end() const { return data_.end(); }

    private:
        Data data_;
    };

    class Reader
    {
    public:
        explicit Reader(std::filesystem::path);

        Table read();
        [[noreturn]] void fail(std::string) const;

    private:
        enum class State
        {
            awaiting_field,
            reading_unquoted,
            reading_quoted,
            after_quote,
            field_complete,
        };

        std::optional<Table::Row> read_record();

        void parse_character(char, std::string&);
        void parse_awaiting_field(char, std::string&);
        void parse_unquoted(char, std::string&);
        void parse_quoted(char, std::string&);
        void parse_after_quote(char, std::string&);

        std::filesystem::path path_;
        std::ifstream file_;
        State state_{ State::awaiting_field };
    };
}
