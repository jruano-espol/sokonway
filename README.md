# Sokonway

## Build

`cmake` is required.

### Local

```bash
cmake --build build
```

The output binary is in the `build` folder.

### Web

The [emscripten](https://emscripten.org/) sdk is requireed.

```bash
source deploy_web.sh
```

To run the game on localhost do the following:

```bash
emrun web_build/sokonway.html
```