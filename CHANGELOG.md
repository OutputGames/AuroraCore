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
    * `OnCollisionEnter`, `OnCollisionExit`, `OnCollisionStay` functions added to components

    * Added `CapsuleCollider`, `MeshCollider`, `CharacterController` to `aclPhysics` (**12/28/23**)

* Lighting module in AuroraCore
    * Support for PBR

* `AuroraCore.Math` now has random number support (basic, only 0 to 1)

* Compute Shader Support

* More customization for render passes (image layout)

* Texture Mipmapping

* Scene Control (Serialization, Deserialization)
    * Texture, Shader, Entity, and other serializaton/deserialization methods added using `nlohmann-json`

* Network Module in `AuroraCore` using [Enet](https://github.com/lsalzman/enet)

* New executable, `AuroraEditor` with tools for editing and creating scenes, making shaders, etc.

    * Added image support to `agl_ext_imgui`

* New utility added to `AuroraUtils`:  `uvector` for finding if an element is in a vector, and finding the index of an element in a vector

* `AuroraFilesystem` now supports writing raw data to header files using the `DataAsCode` exporter from [raylib](github.com/raysan5/raylib)
    * Added writing to a file using a string, and getting file names

* Conversion from `Color` to `vec4` and vice-versa in the `Color` class of `AuroraUtils`

* Added context and event processing to `agl_ext_imgui`

* Added center property to `Collider` and it's subclasses for offseting the returned positions

* Added `GetTransformationMatrix` method to `Transform` class for ease of matrix calculations
    * Added `GetForwardVector` method to `Transform` & `Camera`


### Changed

* Window creation backend changed to SDL from GLFW

* Added Plane & Capsule shape to `agl_ext_primitives` using `par_shape` and `par_octasphere`
    * Replaced predefined sphere model with procedurally generated sphere model

* AuroraCore now builds to a library and not an executable file
    * #### Things the DLL Update changed
        * Uniform buffers no longer use a template to prevent explicit instantiation

* `Ref<T>` classes cannot directly be initialized using pointers

### Deprecated

### Removed

* OpenGL Support
    * I'm working on experimental features currently, and I don't want to have to change structures between libraries. Deko3D, Citro3D, and Metal are somewhat related to the structure of Vulkan, meaning there won't be much difference between the two.
    * **OpenGL support will come back at a later date** when the essential features of the graphics library are completed.

* Redundant `agl_ext` and old `UniformBuffer` of AGL removed
    * Nothing was using the old `agl_ext` in `ext.hpp`
    * `UniformBuffer<T>` was replaced with `aglUniformBuffer` (no template) in `agl.hpp`

### Fixed

* Window being unable to close with X button using GLFW

* `BoxCollider` not returning shape when requested

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
