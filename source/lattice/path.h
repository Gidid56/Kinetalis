#pragma once
/*
   Represents a unidirectional diffusion path between lattice sites.

   Public Interface:
   - Constructor initializes a path between two lattice sites.
   - Methods query path characteristics: event, activity, allowed state, execute processes.
   - Accessors for front, back, and sites along the diffusion path.

   Private Members:
   - 'std::array<Site*, 2> sites_{}': Pointers to defining lattice sites.

   Note:
   - 'lattice::Path' models unidirectional diffusion paths, simulating one-way movement
	 between lattice sites.
   - Unlike bidirectional bonds, 'lattice::Path' allows traversal in a single direction,
	 specifically representing diffusion behavior.
*/
class lattice::Path
{
public:
    explicit Path(Site* left, Site* right) : sites_{ left, right } { }

    kmc::Event event() const;

    bool is_open() const;
    bool is_active() const;
    bool is_allowed() const;
    void execute_process(Lattice&);

    bool is_same_layer() const;

    math::Vector<3, Displacement> path_vector(Lattice const&) const;
    std::vector<Position> transition_states(Lattice const&) const;

    auto reversed() const { return Path{ sites_[1], sites_[0] }; }

    auto& front() { return *sites_.front(); }
    auto& back() { return *sites_.back(); }
    auto& sites() { return sites_; }

    auto const& front() const { return *sites_.front(); }
    auto const& back() const { return *sites_.back(); }
    auto const& sites() const { return sites_; }

private:
    std::array<Site*, 2> sites_{};
};
