#!/usr/bin/env python3
"""Inject matched low-overhead timers into both versions without changing their branches."""
import pathlib
import sys

# Support the pre-refactor and renamed implementations in the same A/B job.
STAGES = (
    (("::select_random_location(", "::select_event_slot("), "Select"),
    (("::evaluate_event(",), "Evaluate"),
    (("::update_affected_rates(",), "Update"),
    (("::rate_sum(", "::update_site_classes("), "Enumerate"),
    (("::class_key(", "::register_class("), "Classify"),
    (("::block_rate(",), "Block"),
)


def instrument(path: pathlib.Path) -> None:
    text = path.read_text()
    include = '#include "../../tools/performance/perf_probe.h"\n'
    if include not in text:
        text = text.replace("#pragma once\n", "#pragma once\n" + include, 1)
    for signatures, stage in STAGES:
        at = next((position for signature in signatures
                   if (position := text.find(signature)) != -1), -1)
        if at == -1:
            raise RuntimeError(f"{path}: missing function {signatures}")
        brace = text.find("{", at)
        if brace == -1:
            raise RuntimeError(f"{path}: missing function body {signature}")
        text = text[: brace + 1] + f"\n        KMC_PERF_SCOPE({stage});" + text[brace + 1 :]
    path.write_text(text)
    print(f"Instrumented {path} ({len(STAGES)} function stages)")


if __name__ == "__main__":
    for filename in sys.argv[1:]:
        instrument(pathlib.Path(filename))
