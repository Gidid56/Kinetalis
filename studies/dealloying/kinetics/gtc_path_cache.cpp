#include "pch.h"
#include "gtc_path_cache.h"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <compare>
#include <cstdint>
#include <limits>
#include <mutex>
#include <numbers>
#include <stdexcept>
#include <vector>

namespace studies::dealloying::kinetics::gtc
{
    namespace
    {
        struct Point
        {
            double x{}, y{}, z{};
            auto operator<=>(Point const&) const = default;
        };

        Point operator+(Point a, Point b) { return {a.x+b.x, a.y+b.y, a.z+b.z}; }
        Point operator-(Point a, Point b) { return {a.x-b.x, a.y-b.y, a.z-b.z}; }
        Point operator*(Point a, double k) { return {a.x*k, a.y*k, a.z*k}; }
        Point operator/(Point a, double k) { return a*(1.0/k); }
        double dot(Point a, Point b) { return a.x*b.x+a.y*b.y+a.z*b.z; }
        Point cross(Point a, Point b) { return {
            a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x}; }
        double norm(Point a) { return std::sqrt(dot(a,a)); }

        constexpr Point initial{};
        constexpr Point final{1,1,0};
        constexpr double d_nn{std::numbers::sqrt2};
        constexpr double tolerance{1e-7};
        constexpr double width{1.0/6.0};
        constexpr double cutoff{1.5*d_nn};
        constexpr unsigned samples_per_arc{64};
        constexpr std::array<Point,12> directions{{
            {-1,-1,0},{-1,0,-1},{-1,0,1},{-1,1,0},
            {0,-1,-1},{0,-1,1},{0,1,-1},{0,1,1},
            {1,-1,0},{1,0,-1},{1,0,1},{1,1,0}
        }};

        using Mask = std::uint32_t;
        constexpr Mask full_mask{(Mask{1}<<18)-1};
        using Weights = std::array<float,18>;

        std::array<Point,18> make_spectators()
        {
            std::vector<Point> positions;
            for (auto d: directions)
                for (auto position : {d, final+d})
                {
                    if (position == initial or position == final)
                        continue;
                    if (std::ranges::find(positions, position)==positions.end())
                        positions.push_back(position);
                }
            if (positions.size()!=18)
                throw std::logic_error{"Expected 18 FCC spectator positions."};
            std::ranges::sort(positions);
            std::array<Point,18> out{};
            std::ranges::copy(positions,out.begin());
            return out;
        }

        struct Arc { std::vector<Point> samples; };

        // Reproduce the current GTC geometric restrictions, but independently
        // of occupancy. Occupancy is reduced to required and blocking bitmasks.
        std::vector<Arc> arcs(Point begin,Point end,Point a,Point b)
        {
            std::vector<Arc> result;
            if (norm(end-final)+tolerance >= norm(begin-final))
                return result;

            for (auto const point : {begin,end})
                if (std::abs(norm(point-a)-d_nn)>tolerance or
                    std::abs(norm(point-b)-d_nn)>tolerance)
                    return result;

            Point const axis{b-a};
            double const length{norm(axis)};
            if (length<=tolerance or length+tolerance >= 2*d_nn)
                return result;

            Point const center{(a+b)/2.0};
            Point const radial{begin-center};
            Point const target{end-center};
            double const radius{norm(radial)};
            if (radius<=tolerance or std::abs(norm(target)-radius)>tolerance)
                return result;

            Point const tangent{cross(axis,radial)/length};
            double cosine{std::clamp(dot(radial,target)/(radius*radius),-1.0,1.0)};
            double sine{dot(axis,cross(radial,target))/(length*radius*radius)};
            std::array<double,2> angles{};
            if (std::abs(sine)<=tolerance and cosine<0.0)
                angles={std::numbers::pi,-std::numbers::pi};
            else
            {
                const double short_angle{std::atan2(sine,cosine)};
                if (std::abs(short_angle)<=tolerance)
                    return result;
                angles={short_angle,
                    short_angle>0.0?short_angle-2*std::numbers::pi:
                                     short_angle+2*std::numbers::pi};
            }

            for (double angle: angles)
            {
                Arc arc;
                arc.samples.reserve(samples_per_arc+1);
                bool admissible{true};
                double previous{dot(begin,final)/dot(final,final)};
                arc.samples.push_back(begin);
                for (unsigned j{1};j<=samples_per_arc;++j)
                {
                    double const theta{angle*static_cast<double>(j)/samples_per_arc};
                    Point const point{center+radial*std::cos(theta)+tangent*std::sin(theta)};
                    double const coordinate{dot(point,final)/dot(final,final)};
                    if (coordinate<=previous+tolerance)
                    {
                        admissible=false;
                        break;
                    }
                    previous=coordinate;
                    arc.samples.push_back(point);
                }
                if (admissible)
                    result.push_back(std::move(arc));
            }
            return result;
        }

        std::vector<Point> three_sphere_intersections(Point a,Point b,Point c)
        {
            std::vector<Point> result;
            Point const ab{b-a};
            double const ab_length{norm(ab)};
            if (ab_length<=tolerance)
                return result;
            Point const axis_x{ab/ab_length};
            Point const ac{c-a};
            double const projection{dot(axis_x,ac)};
            Point const normal{ac-axis_x*projection};
            double const normal_length{norm(normal)};
            if (normal_length<=tolerance)
                return result;
            Point const axis_y{normal/normal_length};
            Point const axis_z{cross(axis_x,axis_y)};
            double const x{ab_length/2.0};
            double const y{(projection*projection+normal_length*normal_length-
                            2*projection*x)/(2*normal_length)};
            double const z_sq{d_nn*d_nn-x*x-y*y};
            if (z_sq < -tolerance)
                return result;
            Point const base{a+axis_x*x+axis_y*y};
            if (z_sq<=tolerance)
                result.push_back(base);
            else
            {
                double const z{std::sqrt(z_sq)};
                result.push_back(base+axis_z*z);
                result.push_back(base-axis_z*z);
            }
            return result;
        }

        struct Path
        {
            Mask required{};
            Mask blocked{};
            std::vector<Weights> samples;
        };

        struct Templates
        {
            std::vector<Path> paths;
            Weights direct_midpoint_weights{};
            Mask final_neighbor_mask{};

            Templates()
            {
                auto const spectator{make_spectators()};

                // Fallback transition state: midpoint of the direct hop.
                // The target-site neighbors exclude the now-vacant source.
                const Point midpoint{(initial+final)/2.0};
                for (unsigned i{}; i<spectator.size(); ++i)
                {
                    if (std::ranges::find(directions, spectator[i]-final) != directions.end())
                        final_neighbor_mask |= Mask{1}<<i;

                    const double separation{norm(midpoint-spectator[i])};
                    if (separation<=cutoff)
                    {
                        const double t{(separation/d_nn-1.0)/width};
                        direct_midpoint_weights[i]=static_cast<float>(std::exp(-t*t));
                    }
                }
                std::vector<unsigned> common, source, target;
                for (unsigned i{};i<spectator.size();++i)
                {
                    bool const from_source{
                        std::ranges::find(directions,spectator[i])!=directions.end()};
                    bool const from_target{
                        std::ranges::find(directions,spectator[i]-final)!=directions.end()};
                    if (from_source) source.push_back(i);
                    if (from_target) target.push_back(i);
                    if (from_source and from_target) common.push_back(i);
                }

                auto append = [&](Mask required, std::vector<Point> const& positions) {
                    if (positions.empty())
                        return;
                    Path path;
                    path.required=required;
                    path.samples.reserve(positions.size());
                    for (auto const& point:positions)
                    {
                        Weights weights{};
                        for (unsigned i{};i<spectator.size();++i)
                        {
                            double const separation{norm(point-spectator[i])};
                            if (separation+tolerance<d_nn)
                                path.blocked |= Mask{1}<<i;
                            if (separation<=cutoff)
                            {
                                double const t{(separation/d_nn-1.0)/width};
                                weights[i]=static_cast<float>(std::exp(-t*t));
                            }
                        }
                        path.samples.push_back(weights);
                    }
                    if ((path.required & path.blocked)==0)
                        paths.push_back(std::move(path));
                };

                // Direct path: two occupied common support sites.
                for (std::size_t i{};i<common.size();++i)
                    for (std::size_t j{i+1};j<common.size();++j)
                    {
                        unsigned const a{common[i]},b{common[j]};
                        for (auto const& arc:arcs(initial,final,spectator[a],spectator[b]))
                            append((Mask{1}<<a)|(Mask{1}<<b),arc.samples);
                    }

                // Two segments: a persistent common support, plus distinct
                // source and target supports joined at a three-sphere hollow.
                for (auto persistent:common)
                    for (auto first:source)
                    {
                        if (first==persistent)
                            continue;
                        for (auto second:target)
                        {
                            if (second==persistent or second==first)
                                continue;
                            for (auto const junction:three_sphere_intersections(
                                spectator[persistent],spectator[first],spectator[second]))
                            {
                                if (norm(junction-initial)<=tolerance or
                                    norm(junction-final)<=tolerance)
                                    continue;

                                auto first_arcs{arcs(initial,junction,
                                    spectator[persistent],spectator[first])};
                                if (first_arcs.empty())
                                    continue;
                                auto second_arcs{arcs(junction,final,
                                    spectator[persistent],spectator[second])};
                                if (second_arcs.empty())
                                    continue;

                                Mask const required{
                                    (Mask{1}<<persistent)|(Mask{1}<<first)|
                                    (Mask{1}<<second)};
                                for (auto const& a:first_arcs)
                                    for (auto const& b:second_arcs)
                                    {
                                        auto samples{a.samples};
                                        samples.insert(samples.end(),
                                            b.samples.begin()+1,b.samples.end());
                                        append(required,samples);
                                    }
                            }
                        }
                    }
            }
        };

        // One lazy result per 18-bit occupancy pattern, not per physical hop.
        struct Cache
        {
            Templates geometry;
            // Explicit count/value construction prevents the brace-init
            // initializer_list overload (which would create only two entries).
            std::vector<double> values = std::vector<double>(
                std::size_t{1}<<18, std::numeric_limits<double>::quiet_NaN());
            std::mutex mutex;

            static double coordination(Weights const& weights, Mask mask)
            {
                double value{};
                for (Mask bits{mask}; bits; bits &= bits-1)
                    value += weights[std::countr_zero(bits)];
                return value;
            }

            double lookup(Mask mask)
            {
                if (mask & ~full_mask)
                    throw std::invalid_argument{"GTC occupancy mask exceeds 18 bits."};

                std::scoped_lock lock{mutex};
                double& result{values[mask]};
                if (std::isnan(result))
                {
                    double best{-1.0}; // No admissible circular path yet.
                    for (auto const& path:geometry.paths)
                    {
                        if ((mask & path.required)!=path.required or
                            (mask & path.blocked)!=0)
                            continue;

                        double bottleneck{std::numeric_limits<double>::infinity()};
                        for (auto const& weights:path.samples)
                        {
                            bottleneck=std::min(bottleneck,coordination(weights,mask));
                            if (bottleneck<=best)
                                break;
                        }
                        best=std::max(best,bottleneck);
                    }

                    // If no supported arc exists, approximate the straight
                    // trajectory by the midpoint coordination.
                    if (best<0.0)
                        best=coordination(geometry.direct_midpoint_weights,mask);

                    // The final lattice site's binding energy can exceed the
                    // entire path's maximum. Since epsilon=-C*n^a, the
                    // highest energy corresponds to the LOWEST coordination.
                    const double n_final{static_cast<double>(
                        std::popcount(mask & geometry.final_neighbor_mask))};
                    result=std::min(best,n_final);
                }
                return result;
            }
        };
    }

    double saddle_coordination(std::uint32_t occupancy)
    {
        static Cache cache;
        return cache.lookup(occupancy);
    }

    unsigned final_coordination(std::uint32_t occupancy)
    {
        if (occupancy & ~full_mask)
            throw std::invalid_argument{"GTC occupancy mask exceeds 18 bits."};

        static const Mask neighbors{[] {
            Mask mask{};
            const auto positions{make_spectators()};
            for (unsigned i{}; i<positions.size(); ++i)
                if (std::ranges::find(directions, positions[i]-final) != directions.end())
                    mask |= Mask{1}<<i;
            return mask;
        }()};
        return std::popcount(occupancy & neighbors);
    }
}
