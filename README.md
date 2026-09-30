## Linux

```
mkdir build
cd build
cmake ..
make
```

### Run
```
cd ..
./build/GLProject
```

## macOS (Apple Silicon)
```bash
xcode-select --install
brew install cmake
```
> Note: Apple deprecated OpenGL, but it still works up to version 4.1 (Core profile).
> Your context must request a 4.1 core profile with forward compatibility enabled.
