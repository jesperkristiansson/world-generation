# World Generation
A simple project playing around with generating a 2D world(tile grid) using the wave [function collapse algorithm](https://robertheaton.com/2018/12/17/wavefunction-collapse-algorithm/)

Uses [Jansson](https://jansson.readthedocs.io/en/latest/index.html) for parsing JSON config files.

Build with `make` and run with `./world-gen [-i] [width] [height]`. If run with `-i`, each step in the world generation will be printed.

TODO:
- find a better way to sleep between generation steps
- improve printing of world to avoid flickering
