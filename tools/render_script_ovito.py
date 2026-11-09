from __future__ import annotations

import argparse
import re
from pathlib import Path
from typing import Any

import ovito
from ovito.pipeline import FileSource
from ovito.vis import OSPRayRenderer


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("--state", required=True)
    parser.add_argument("--input-directory", required=True)
    parser.add_argument("--output-directory", required=True)
    parser.add_argument("--prefix", default="surface")
    parser.add_argument("--width", type=int, default=1000)
    parser.add_argument("--height", type=int, default=1000)
    parser.add_argument("--overwrite", action="store_true")
    return parser.parse_args()


def snapshot_index(path: Path) -> tuple[int, int, str]:
    indexed_name = re.match(r"^(\d+)(?:-step_(\d+))?$", path.stem)

    if indexed_name:
        snapshot = int(indexed_name.group(1))
        step = int(indexed_name.group(2) or snapshot)
        return snapshot, step, path.name

    step_name = re.search(r"(?:^|[-_])step[-_](\d+)(?:$|[-_])", path.stem)

    if step_name:
        step = int(step_name.group(1))
        return step, step, path.name

    leading_number = re.match(r"^(\d+)", path.stem)

    if leading_number:
        value = int(leading_number.group(1))
        return value, value, path.name

    return 2**63 - 1, 2**63 - 1, path.name


def scene_object() -> Any:
    scene = getattr(ovito, "scene", None)

    if scene is not None:
        return scene

    dataset = getattr(ovito, "dataset", None)

    if dataset is not None:
        return dataset

    raise RuntimeError("The OVITO session object is unavailable.")


def file_pipeline(scene: Any) -> Any:
    pipelines = list(getattr(scene, "pipelines", []))

    if not pipelines:
        raise RuntimeError("The OVITO session contains no data pipeline.")

    selected = getattr(scene, "selected_pipeline", None)

    if selected is not None and isinstance(getattr(selected, "source", None), FileSource):
        return selected

    for pipeline in pipelines:
        if isinstance(getattr(pipeline, "source", None), FileSource):
            return pipeline

    raise RuntimeError("The OVITO session contains no pipeline with a file source.")


def session_objects(scene: Any) -> list[Any]:
    objects = [scene]
    dataset = getattr(ovito, "dataset", None)

    if dataset is not None and dataset is not scene:
        objects.append(dataset)

    return objects


def active_viewport(scene: Any) -> Any:
    for session in session_objects(scene):
        viewports = getattr(session, "viewports", None)
        viewport = getattr(viewports, "active_vp", None)

        if viewport is not None:
            return viewport

    raise RuntimeError(
        "The active viewport stored in the OVITO session could not be accessed. "
        "Open and resave rendering.ovito with the installed OVITO Pro version."
    )


def session_background(scene: Any) -> tuple[float, float, float]:
    for session in session_objects(scene):
        render_settings = getattr(session, "render_settings", None)
        background = getattr(render_settings, "background_color", None)

        if background is not None:
            return tuple(float(component) for component in background)

    return 1.0, 1.0, 1.0


def main() -> None:
    arguments = parse_arguments()

    if arguments.width <= 0 or arguments.height <= 0:
        raise ValueError("Render dimensions must be positive.")


    state_path = Path(arguments.state)
    input_directory = Path(arguments.input_directory)
    output_directory = Path(arguments.output_directory)

    if not state_path.is_file():
        raise RuntimeError(f"OVITO session state does not exist: {state_path}")

    if not input_directory.is_dir():
        raise RuntimeError(f"Input directory does not exist: {input_directory}")

    frames = sorted(input_directory.glob("*.xyz"), key=snapshot_index)

    if not frames:
        raise RuntimeError(f"No XYZ files found in: {input_directory}")

    scene = scene_object()
    load_state = getattr(scene, "load", None)

    if not callable(load_state):
        raise RuntimeError(
            "This OVITO version does not provide a session-state loading API."
        )

    load_state(str(state_path.resolve()))
    pipeline = file_pipeline(scene)
    pipeline.source.load([str(frame.resolve()) for frame in frames])

    frame_count = int(pipeline.source.num_frames)

    if frame_count != len(frames):
        raise RuntimeError(
            f"OVITO loaded {frame_count} animation frames from {len(frames)} XYZ files."
        )

    viewport = active_viewport(scene)
    background = session_background(scene)
    output_directory.mkdir(parents=True, exist_ok=True)

    renderer = OSPRayRenderer()

    print(f"OVITO version: {getattr(ovito, 'version_string', 'unknown')}")
    print(f"Loaded {frame_count} XYZ frames.")
    print(f"First frame: {frames[0]}")
    print(f"Last frame:  {frames[-1]}")
    print(f"Output:      {output_directory / (arguments.prefix + '0000.png')}")

    for frame in range(frame_count):
        output_path = output_directory / f"{arguments.prefix}{frame:04d}.png"

        if output_path.exists() and not arguments.overwrite:
            print(f"Skipping existing frame {frame + 1}/{frame_count}: {output_path.name}")
            continue

        print(f"Rendering frame {frame + 1}/{frame_count}: {output_path.name}")

        viewport.render_image(
            filename=str(output_path),
            size=(arguments.width, arguments.height),
            frame=frame,
            background=background,
            renderer=renderer,
        )


if __name__ == "__main__":
    main()
