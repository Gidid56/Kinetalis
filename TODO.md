# Roadmap / TODO

Long-term ideas, not part of the current observer or benchmark refactoring.

## Molecular chemistry and more general reaction models

- [ ] **Represent molecular species.** Extend `chemistry` beyond single `Element`/`Atom` identities to include molecules and adsorbates (e.g. CO, O₂, O). Specify their composition, identity, and any necessary physical properties without breaking atomic Ag–Au models.
- [ ] **Define site occupancy for molecules.** Decide how adsorbed species occupy lattice sites, including single-site adsorbates, multi-site footprints, and potentially site-specific adsorption states. Avoid assuming that a chemical species is always a single atom.
- [ ] **Generalize elementary reactions as needed.** Support adsorption/desorption, dissociative adsorption, surface reactions, and molecular diffusion in `kmc::Process` and the event/kinetics representation. Review the two-site `kmc::event::Location` limit and event-based neighbor invalidation before making a design choice. Preserve current single-atom dealloying behavior and performance.

## Cross-framework validation and benchmarking

- [ ] **Implement one published kmos model in Kinetalis.** Prefer the ZGB CO-oxidation model first: a relatively simple, well-documented baseline that demonstrates applications beyond dealloying. Consider RuO₂(110) CO oxidation as a more substantial second example; PdO with lateral interactions can come later.
- [ ] **Validate physical results.** Reproduce published observables and statistical behavior using matched rate constants, lattice topology, boundary conditions, and simulation protocols. Exact random trajectories need not match between independent KMC engines.
- [ ] **Benchmark matched workloads against kmos.** Run both implementations on the same hardware in optimized single-thread builds, with equivalent physical models and workloads. Record accepted events, attempted selections/rejection fractions, lattice size, and ns/accepted step; characterize local reclassification/update work to make comparisons interpretable.
- [ ] **Turn the example into a reusable showcase.** Include a documented non-dealloying study, correctness tests, and representative cases in the current `benchmarks/` framework.

These are deferred projects. The existing Ag–Au dealloying benchmark remains the current performance baseline.
