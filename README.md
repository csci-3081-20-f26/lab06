## Linux

### Build
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

```bash
brew install pkg-config
brew install glfw3
brew install glm
```

Now you can build the same way as the linux machine:

### Build


On some macs if you get a seg fault when you run the code there is one change you need to make. In main.cpp you will need to change the glfw minor version to 1, and it macs can work with `glfw` upto version 4.1. 
```cpp
.
.
.
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);   // was 3; macOS max is 4.1
```

No you can build it

```bash
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
