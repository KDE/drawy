# Drawy
Your handy, infinite brainstorming tool!

<img src="./assets/screenshot.png" style="width: 100%" />

Drawy is a work-in-progress infinite whiteboard tool written in Qt/C++, which aims to be a native-desktop alternative to the amazing web-based Excalidraw.

### Discuss
If you wish to discuss anything regarding Drawy, you are welcome to join its Matrix room here: https://go.kde.org/matrix/#/#drawy:kde.org

### Compiling from Source
#### Using kde-builder
Drawy can be built using [`kde-builder`](https://develop.kde.org/docs/getting-started/building/kde-builder-compile/)
```
kde-builder drawy
```

#### Using cmake
If you have all dependencies installed, you can use cmake to build it:
```
git clone https://invent.kde.org/graphics/drawy && cd drawy
cmake --preset release
cmake --build build-release
./build-release/bin/drawy
```

### Using ASAN

We can use sanitizers.supp file
example: LSAN_OPTIONS=suppressions=$PWD/sanitizers.supp drawy


### Keyboard Shortcuts
Default keyboard shortcuts can be viewed here: [KEYBINDINGS.md](docs/keybindings.md)

### Contributing
Contributions are welcome. Please read the [contributing guide](CONTRIBUTING.md) before opening pull requests.

### License
This project uses the GNU General Public License V3.
