#pragma once
#include "kmc/observer.h"
#include "types.h"

class output::Writer : public kmc::Observer
{
public:
    explicit Writer(std::filesystem::path const& root = {}) : root_{ root } { }

    auto& set_root_path(this auto& self, std::filesystem::path const& root)
    {
        self.root_ = root;
        return self;
    }

    static constexpr auto default_header{ "key:step,time/s" };

    [[nodiscard]] auto& root_path() const { return root_; }
    [[nodiscard]] auto writer_path() const { return root_ / name(); }
    [[nodiscard]] auto& last_snapshot() const { return last_snapshot_; }

protected:
    virtual bool uses_snapshot_resolution() const { return true; }

    virtual void capture_init(kmc::Simulation const&) { }
    virtual void capture_snapshot(kmc::Simulation const&) { }
    virtual void capture_final(kmc::Simulation const&) { }

    virtual void write_init(kmc::Simulation const&) { }
    virtual void write_snapshot(kmc::Simulation const&) { }
    virtual void write_final(kmc::Simulation const&) { }

    auto snapshot_index() const { return snapshot_index_; }
    std::string snapshot_name(kmc::Step) const;

private:
    std::filesystem::path root_{};

    std::size_t snapshot_index_{};
    std::optional<kmc::Observer::Snapshot> last_snapshot_;

    void observe_started(kmc::Simulation const&) override final;
    void take_snapshot(kmc::Simulation const&) override final;
    void observe_completed(kmc::Simulation const&) override final;

    void register_resolution() const;
};
