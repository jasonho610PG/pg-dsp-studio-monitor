# JUCE Build Workflow

**CMake and Projucer build process.**

---

## Tools

- **Projucer:** JUCE project manager (GUI)
- **CMake:** Build system (command-line, CI-friendly)

---

## CMakeLists.txt Structure

```cmake
cmake_minimum_required(VERSION 3.15)
project(MyDSPPlugin VERSION 1.0.0)

# Add JUCE
add_subdirectory(JUCE)

# Define plugin
juce_add_plugin(MyDSPPlugin
    PLUGIN_MANUFACTURER_CODE Manu
    PLUGIN_CODE Plg1
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "My DSP Plugin")

# Add source files
target_sources(MyDSPPlugin PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp)

# Link libraries
target_link_libraries(MyDSPPlugin PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp)
```

---

## Build Commands

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Output: build/MyDSPPlugin_artefacts/Release/
```

---

## Projucer Workflow (Alternative)

1. Open `.jucer` file in Projucer
2. Configure settings (plugin formats, modules)
3. Save project → generates IDE projects
4. Build in Xcode (macOS) or Visual Studio (Windows)

---

## Testing

**Standalone:**
```bash
./build/MyDSPPlugin_artefacts/Release/Standalone/MyDSPPlugin
```

**VST3/AU:**
- Copy to plugin folder (`~/Library/Audio/Plug-Ins/VST3/` on macOS)
- Open in DAW (Reaper, Logic, etc.)

---

## Continuous Integration

```yaml
# .github/workflows/build.yml
- name: Build JUCE Plugin
  run: |
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build --config Release
```

---

*Load this file for build issues.*
