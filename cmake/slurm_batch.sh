#!/bin/bash
#SBATCH --job-name=kinetalis
#SBATCH --ntasks=1
#SBATCH --mem=128G
#SBATCH --cpus-per-task=64
#SBATCH --time=21-00:00:00
#SBATCH --output=slurm/%x-%j.out
#SBATCH --error=slurm/%x-%j.err
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=gideon.henkelmann@tuhh.de
#SBATCH --constraint=OS9

set -euo pipefail

module load gcc/14.2.0

export LD_LIBRARY_PATH="$(dirname "$(g++ -print-file-name=libstdc++.so.6)"):${LD_LIBRARY_PATH:-}"

srun "$1" --concurrency "$SLURM_CPUS_PER_TASK" --results-dir "$WORK/kinetalis/"
