# Supporting resources

This directory contains non-source files used by Kinetalis during development
or by its optional viewer.

| Directory | Contents | Build/runtime use |
| --- | --- | --- |
| `codegen/` | `chemistry.xlsx`, `colors.xlsx`, `physics.xlsx` | Source tables for chemistry, color, and physics definitions; not loaded by the KMC executable at runtime |
| `references/` | Extracts from the SI Brochure, CODATA 2022, and the reference `xoshiro256plus.c` implementation | Scientific and algorithmic reference material; not compiled into Kinetalis |
| `fonts/` | Inter font | Copied to the viewer's runtime resources when the viewer is built |
| `shaders/` | Vertex and fragment shaders | Used by the viewer and copied to its runtime resources |

The reference documents and spreadsheets are tracked for provenance and
maintenance. The ordinary CMake builds do not read the Excel or PDF files.
For builds without the viewer, the font and shader resources are not needed.

See [the project README](../README.md) for build and usage instructions.
