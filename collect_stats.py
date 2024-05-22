from pathlib import Path
#
import argparse
import subprocess

def ext_check(filename, ext_name) -> bool:
    return filename.suffix == ext_name


def run_sim(binpath: Path, trace: Path, output: Path, warmup: int, instrs: int):
    exec_cmd = [binpath.as_posix(), trace.as_posix(),
                                    "-w", str(warmup),
                                    "-i", str(instrs),
                                    "--json", output.as_posix()]

    subprocess.run(exec_cmd, check=True)
    #
#


def collect_stats(binpath: Path, inp: Path, output: Path, warmup: int, instrs: int):
#
    if not inp.is_dir():
        return False

    output.mkdir(exist_ok = True)

    for filename in inp.iterdir():
        if ext_check(filename, ".xz"):
            run_sim(binpath=binpath,
                    trace=filename,
                    output=(output / f"{filename.stem}.json"),
                    warmup=warmup,
                    instrs=instrs)
    #
    return True
    #
#
def main():
    parser = argparse.ArgumentParser(
        description="Tool run champsim simulator & collect statistics"
    )
    #
    parser.add_argument("-d", "--input", type=str,
                        required=True, help="Path to input directory w/ traces")

    parser.add_argument("-b", "--binary", type=str,
                        required=True, help="Path to champsim binary")

    parser.add_argument("-w", "--warmup_instructions", type=int, default=5000000, help="")

    parser.add_argument("-i", "--simulation_instructions", type=int, default=20000000, help="")

    parser.add_argument("-o", "--output", type=str,
                        required=True, help="Path to output directory statisics logs")
    #

    args = parser.parse_args()

    collect_stats(binpath=Path(args.binary),
                  inp=Path(args.input),
                  output=Path(args.output),
                  warmup=args.warmup_instructions,
                  instrs=args.simulation_instructions)
    #
#
if __name__  == "__main__":
    main()
