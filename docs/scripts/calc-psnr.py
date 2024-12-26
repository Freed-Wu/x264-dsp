#!/usr/bin/env python
""".

usage: calc-psnr.py [-hV]

options:
    -h, --help                  Show this screen.
    -V, --version               Show version.
"""

if __name__ == "__main__" and __doc__:
    from docopt import docopt

    Arg = bool | str
    args: dict[str, Arg] = docopt(__doc__, version="v0.0.1")
    import os
    from glob import glob

    import numpy as np

    for fname in glob("tmp/logs/*/*/*.log"):
        print(
            os.path.join(*fname.split("/")[2:]).split("_")[0],
            " ".join(
                map(
                    str,
                    map(lambda x: round(x, 2), np.loadtxt(fname).mean(0)[1:]),
                )
            ),
        )
