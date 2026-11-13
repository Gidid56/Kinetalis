#pragma once
#include "types.h"
/*
   Represents an individual lattice site within the lattice structure.

   Note:
   - Instances of 'lattice::Site' are constructed and managed within 'Lattice::constructSites'.
   - Stored in the 'Sites = std::vector<Site>;' attribute of the 'lattice::Lattice' class.
   - Indices within this class relate to 'std::vector' indices of sites and paths in 'lattice::Lattice'.

   Public Interface:
   - Constructors: Initialize site position, element, and index based on provided parameters.
   - Methods for setting and accessing site properties: element, neighbors, position, etc.
   - Functionality to manage site state: setting element, index, position, layer, and checking site properties.
   - Methods to handle site interactions: insertion, executing processes, updating, and determining events.
   - Utility functions to query site characteristics: occupancy, surface/bulk status, neighbors, and indices.

   Private Attributes:
   - 'Element element_': Represents the element occupying the lattice site.
   - 'Coordination coordination_, maxNeighbors_': Track neighboring sites and the maximum allowed neighbors.
   - 'bool pristine_': Marks the site as pristine or modified.
   - 'Layer layer_': Indicates the layer of the lattice site within the lattice structure.
   - 'Index siteIndex_, neighborIndex_': Identifies the site and path indices.
   - 'Position position_': Represents the 3D spatial position of the lattice site.
*/

class lattice::Site
{
public:
    enum class Flags : std::uint8_t
    {
        none,
        pristine = 1 << 0,
        dirty = 1 << 1,
    };

    explicit Site(Position const& position, Element element, Index index)
        : position_{ position },
          site_index_{ index },
          element_{ element }
    { }
    explicit Site(Site& basis, Position const& translation)
        : position_{ basis.position_ + translation },
          site_index_{ basis.site_index_ },
          element_{ basis.element_ }
    { }

    void set_element(Lattice&, Element);
    void set_index(Index index) { site_index_ = index; }
    void set_position(Position const& position) { position_ = position; }

    template <Flags flag>
    void set(bool enabled)
    {
        if (enabled)
            flags_ |= flag;
        else
            flags_ &= ~flag;
    }
    void set_pristine(bool enabled) { set<Flags::pristine>(enabled); }
    void set_dirty(bool enabled) { set<Flags::dirty>(enabled); }
    void set_layer(Layer layer) { layer_.set(layer); }
    //void setOriginal(Position const& position) { original_ = position; }
    //void resetOriginal() { original_ = position_; }

    void insert(Site*, Index);

    //bool is_active() const;
    //bool is_allowed() const;
    //kmc::Event event() const;

    void execute_process(Lattice&);
    void update_neighbors(Lattice&, std::int_fast8_t);

    physics::quantity::Displacement layer_position(math::Axis) const;

    bool is_occupied() const { return element_ != Element::none; }
    bool is_vacant() const { return element_ == Element::none; }
    bool is_surface() const { return coordination_ != max_neighbors_; }
    bool is_bulk() const { return coordination_ == max_neighbors_; }

    template <Flags flag>
    bool is() const
    {
        return bool(flags_ & flag);
    }
    bool is_pristine() const { return is<Flags::pristine>(); }
    bool is_dirty() const { return is<Flags::dirty>(); }

    Element element() const { return element_; }
    Coordination coordination(Element) const;
    Coordination coordination() const { return coordination_; }
    auto max_neighbors() const { return max_neighbors_; }
    auto vacant_neighbors() const { return max_neighbors_ - coordination_; }
    //auto diffusionVector() const { return original_ - position_; }

    auto& element_neighbors() const { return *element_neighbors_; }
    auto flags() const { return flags_; }
    auto index() const { return site_index_; }
    auto neighbor_index() const { return neighbor_index_; }
    auto process_index() const { return site_index_ + neighbor_index_; }
    auto& position() const { return position_; }
    Layer layer() const { return layer_.get(); }
    //auto& original() const { return original_; }

private:
    Element element_{};
    Coordination coordination_{}, max_neighbors_{};
    [[no_unique_address]] ElementNeighbors element_neighbors_{};
    Flags flags_{};
    [[no_unique_address]] utility::Storage<Layer, useLayers> layer_{};

    Index site_index_{}, neighbor_index_{};

    Position position_{};
    //Position original_{};
};

namespace lattice::site
{
    // TODO remove
    enum class Type : std::uint32_t
    {
        none,
        isolated,
        kink,
        bulk,

        //_100__ad,
        //_100__surface,

        //_111__ad,
        //_111__dimer_ad,
        //_111__step_A_ad,
        //_111__step_B_ad,
        //_111__step_edge_A,
        //_111__step_edge_B,
        //_111__step_corner,
        //_111__surface,
    };
}
