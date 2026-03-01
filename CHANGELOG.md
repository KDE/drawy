## Unreleased

### Added
* Support for transforming items with rotation, resizing, and snapping to angles using the Shift key.
* Copy and paste functionality for plain text and application objects.
* Ability to export the canvas to SVG format and drop MIME data directly onto the canvas.
* New tool shapes including a Diamond tool and customizable arrow start and end points.
* Autosave functionality that restores files automatically in case of unexpected closures.
* UI panels for adjusting opacity, stroke styles, background colors, and item alignment.
* Support for hardware stylus erasers.
* A Hamburger menu and a recent files menu for easier navigation.
* A quick action command to clear the entire canvas.
* KCrash integration to handle and report application crashes.
* Extensive internal unit tests and continuous integration checks.
* Add support for plugin.
* Add basic shapes plugin.
* Disable screenlocker when using apps in fullscreen mode.
* Add Image support.

### Changed
* Freehand strokes now use Bezier curves and interpolation to make drawing feel significantly smoother.
* Zooming now follows the mouse cursor and scales exponentially for better control.
* Text items automatically scale their internal font size when the text box is resized.
* The window title now displays the current file name along with an unsaved changes indicator.
* Large freehand strokes are now rendered roughly in real time and smoothed upon completion to improve application performance.
* Canvas rendering and caching logic was heavily optimized to prevent lag during fast movements.
* The application interface was updated to use standard KDE icons and styling.
* The internal codebase was modernized to strict C++ standards and newer Qt 6 requirements.
* Flatpak and Nix build configurations were updated for better app exporting.

### Fixed
* Multiple issues with group items and multi-selections acting incorrectly during rotation or resizing.
* Canvas corruption bugs and visual artifacts that appeared when zooming or resizing the window.
* Undo and redo history breaking after grouping items or clearing the canvas.
* Loss of stylus pressure data when saving and loading files.
* Bounding box and intersection logic errors for the Diamond tool.
* Accidental duplication of items when attempting to translate them across the canvas.
* Saved strokes failing to load correctly upon opening a document.
* Property bar flickering rapidly when selecting new items.
* Background color fills unintentionally applying to grouped polygon strokes.
* Memory leaks and uninitialized variables flagged by internal analysis tools.

## v1.0.0-alpha

### Added
* An infinite canvas to draw without boundary restrictions.
* Core drawing tools including a freeform pen, text box, and eraser.
* A selection tool to freely move items around the canvas.
* The ability to group multiple items together and ungroup them.
* File management to save and load your drawings.
* Undo and redo support for basic canvas operations.
* A property bar to modify item details like stroke width, colors, and font sizes.
* Support for drawing tablets with pressure sensitivity.
* Distribution packages for Linux via Flatpak, AppImage, and Nix.

### Changed
* Rendering logic was rewritten to only process visible items on the screen for better performance.

### Fixed
* Bounding box calculation errors that caused freeform shapes to behave unpredictably.
