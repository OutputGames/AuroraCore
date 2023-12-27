# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog],
and this project adheres to [Semantic Versioning].

## 0.0.2 - 2024-1-1 (The SDL Update)

### Added

* Support for procedural texture creation with shaders

* Input module in AuroraUtils using [SDL](https://www.libsdl.org)
    * Support for controller rumble
    * Support for joystick movement and button pressing
    * Theoretical support for gyro tracking

* Physics module in `AuroraCore` using [JoltPhysics](https://github.com/jrouwe/JoltPhysics)
    * OnCollisionEnter, OnCollisionExit, OnCollisionStay functions added to components

* Lighting module in AuroraCore
    * Support for PBR

* `AuroraCore.Math` now has random number support (basic, only 0 to 1)

* Compute Shader Support

* More customization for render passes (image layout)

* Texture Mipmapping

* Scene Control (Serialization, Deserialization)

* Network Module in `AuroraCore` using [Enet](https://github.com/lsalzman/enet)



### Changed

* Window creation backend changed to SDL from GLFW

* Added Plane shape to agl_ext_primitives

* Original agl_ext removed

* AuroraCore now builds to a library and not an executable file
    * #### Things the DLL Update changed
        * Uniform buffers no longer use a template to prevent explicit instantiation


### Deprecated

### Removed

* OpenGL Support
    * I'm working on experimental features currently, and I don't want to have to change structures between libraries. Deko3D, Citro3D, and Metal are somewhat related to the structure of Vulkan, meaning there won't be much difference between the two.
    * **OpenGL support will come back at a later date** when the essential features of the graphics library are completed.

### Fixed

* Window unable to close with X button using GLFW

### Security

### Planned

* [Deko3d]() Support (Nintendo Switch)
* [Metal]() Support (iOS/macOS)
* [Citro3d]() Support (3DS)
* x86 support

## 0.0.1 - 2023-12-1

- initial release

<!-- Links -->
[keep a changelog]: https://keepachangelog.com/en/1.0.0/
[semantic versioning]: https://semver.org/spec/v2.0.0.html
[Deko3d]: https://github.com/devkitPro/deko3d
[Metal]: https://developer.apple.com/metal/
[Citro3d]: https://github.com/devkitPro/citro3d/tree/master

<!-- Versions -->
[unreleased]: https://github.com/Author/Repository/compare/v0.0.2...HEAD
