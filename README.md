# World Generation
A simple project playing around with generating a 2D world(tile grid) using the wave [function collapse algorithm](https://robertheaton.com/2018/12/17/wavefunction-collapse-algorithm/)

Uses [Jansson](https://jansson.readthedocs.io/en/latest/index.html) for parsing JSON config files.

Build with `make` and run with `./world-gen [-i] [width] [height]`. If run with `-i`, each step in the world generation will be printed.

TODO:
- in interactive mode, the last snapshot of the world should be erased before print next step
