Build the glfw 
```bash
cmake -S vendor/glfw \
      -B libs/glfw-build \
      -D GLFW_BUILD_TESTS=OFF \
      -D GLFW_BUILD_EXAMPLES=OFF \
      -D GLFW_BUILD_DOCS=OFF \
      -D GLFW_BUILD_WAYLAND=0

cmake --build libs/glfw-build --parallel <your-cpu-threads>
cmake --install libs/glfw-build --prefix libs/glfw-build/install
```

Build the assimp
```bash
cmake -S vendor/assimp \
      -B libs/assimp-build

cmake --build libs/assimp-build --parallel <your-cpu-threads> 
cmake --install libs/assimp-build --prefix libs/assimp-build/install
```

Build the tinyxml2
```bash
cmake -S vendor/tinyxml2 \
      -B libs/tinyxml2-build

cmake --build libs/tinyxml2-build --parallel <your-cpu-threads> 
cmake --install libs/tinyxml2-build --prefix libs/tinyxml2-build/install
```

Build yaml-cpp
```bash
cmake -S vendor/yaml-cpp \
      -B libs/yaml-cpp-build \
      -D YAML_BUILD_SHARED_LIBS=ON

cmake --build libs/yaml-cpp-build --parallel <your-cpu-threads> 
cmake --install libs/yaml-cpp-build --prefix libs/yaml-cpp-build/install
```