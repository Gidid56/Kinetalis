#pragma once
#include "site.h"

class lattice::Cells
{
public:
    static constexpr auto index_shifts{ Indices{ -1, 0, 1 }.cartesian_product() };

    explicit Cells(Sites const& sites, Distance cell_size) : cell_size_{ cell_size }
    {
        if (sites.empty())
            return;

        if (sites.size() >= std::numeric_limits<Index>::max())
            throw std::overflow_error{ "Increase lattice::Index size." };

        determine_bounds(sites);

        offsets_.assign(cell_count() + 1, Index{});

        count_sites(sites);
        accumulate_offsets();
        assign_sites(sites);
    }

private:
    void determine_bounds(Sites const& sites)
    {
        min_cell_ = indices(sites.front());
        auto max_cell{ min_cell_ };

        for (auto const& site : sites)
        {
            auto const cell{ indices(site) };

            min_cell_ = { std::min(min_cell_.x(), cell.x()),
                std::min(min_cell_.y(), cell.y()),
                std::min(min_cell_.z(), cell.z()) };

            max_cell = { std::max(max_cell.x(), cell.x()),
                std::max(max_cell.y(), cell.y()),
                std::max(max_cell.z(), cell.z()) };
        }

        dimensions_ = max_cell - min_cell_ + 1;
    }

    void count_sites(Sites const& sites)
    {
        for (auto const& site : sites)
            ++offsets_[linear_index(indices(site)) + 1];
    }

    void accumulate_offsets()
    {
        for (std::size_t i{ 1 }; i < offsets_.size(); ++i)
            offsets_[i] += offsets_[i - 1];
    }

    void assign_sites(Sites const& sites)
    {
        sites_.resize(sites.size());

        // Use offsets as insertion cursors to avoid another cell-sized allocation.
        for (std::size_t site_index{}; site_index < sites.size(); ++site_index)
        {
            auto const cell{ linear_index(indices(sites[site_index])) };
            sites_[offsets_[cell]++] = static_cast<Index>(site_index);
        }

        // Restore offsets after using them as cursors.
        for (std::size_t i{ offsets_.size() - 1 }; i > 0; --i)
            offsets_[i] = offsets_[i - 1];

        offsets_.front() = 0;
    }

public:
    Indices indices(Site const& site) const
    {
        return (site.position() / cell_size_).to<int>();
    }

    std::span<Index const> sites(Indices const& cell) const
    {
        auto const relative{ cell - min_cell_ };

        if (relative.x() < 0 or relative.x() >= dimensions_.x() or relative.y() < 0 or
            relative.y() >= dimensions_.y() or relative.z() < 0 or relative.z() >= dimensions_.z())
            return {};

        auto const index{ linear_index(cell) };
        auto const first{ offsets_[index] };
        auto const last{ offsets_[index + 1] };

        return std::span<Index const>{ sites_ }.subspan(first, last - first);
    }

private:
    std::size_t linear_index(Indices const& cell) const
    {
        auto const relative{ cell - min_cell_ };

        return (static_cast<std::size_t>(relative.x()) * static_cast<std::size_t>(dimensions_.y()) +
                   static_cast<std::size_t>(relative.y())) *
                   static_cast<std::size_t>(dimensions_.z()) +
               static_cast<std::size_t>(relative.z());
    }

    std::size_t cell_count() const
    {
        auto const x{ static_cast<std::size_t>(dimensions_.x()) };
        auto const y{ static_cast<std::size_t>(dimensions_.y()) };
        auto const z{ static_cast<std::size_t>(dimensions_.z()) };

        if (x > std::numeric_limits<std::size_t>::max() / y or
            x * y > std::numeric_limits<std::size_t>::max() / z)
            throw std::overflow_error{ "Cell list is too large." };

        return x * y * z;
    }

    Distance cell_size_;
    Indices min_cell_{};
    Indices dimensions_{};

    std::vector<Index> offsets_;
    std::vector<Index> sites_;
};
