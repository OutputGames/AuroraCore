
project "AuroraCore"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   location "generated\\"
   compileas "C++"
   targetdir "bin/%{cfg.buildcfg}"

   defines {"AURORA_DLLBUILD"}

   files {
       "src/**",
        "include/**",
         ".editorconfig",
         "%{_OPTIONS['utilslocation']}/include/**" ,
         "%{_OPTIONS['utilslocation']}/vendor/glm/glm/**" ,
         "testresources/**"
      }
   includedirs {
       "include/", 
       "%{_OPTIONS['utilslocation']}".."/include", 
       "%{_OPTIONS['utilslocation']}".."/vendor/glm",
       "%{_OPTIONS['sunriselocation']}".."/vendor/", 
       "%{_OPTIONS['sunriselocation']}".."/vendor/alsoft/include", 
       "%{_OPTIONS['sunriselocation']}".."/include/", 
       "%{_OPTIONS['agllocation']}".."/include/",
       "%{_OPTIONS['agllocation']}".."/vendor/glfw/include",
       "%{_OPTIONS['utilslocation']}".."/vendor/glm", 
      }
    libdirs {"%{_OPTIONS['agllocation']}/vendor/glfw/lib-vc2022/","%{_OPTIONS['agllocation']}/vendor/assimp/lib/Release/"}
    links {"glfw3.lib", "assimp-vc143-mt.lib"}

    filter {"options:gfxapi=vulkan"}
      defines {"GRAPHICS_VULKAN"}
      links {"vulkan-1.lib"}
      libdirs {"C:/VulkanSDK/1.3.250.0/Lib"}
      includedirs {"C:/VulkanSDK/1.3.250.0/Include"}
      removefiles {"include/GL/**"}
      files {"vendor/imgui/backends/imgui_impl_vulkan.*"}

   filter {"options:gfxapi=opengl"}
      defines {"GRAPHICS_OPENGL"}
      libdirs {"lib/glew"}
      links {"glew32.lib", "opengl32.lib"}
      removefiles {"include/spirv_reflect/**"}
      files {"vendor/imgui/backends/imgui_impl_opengl3.*"}

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      debugdir "./"
      runtime "Debug"
      optimize "Off"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      runtime "Release"

   filter "action:vs*" 
      defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS"}
      dependson {"AuroraAudioLibrary", "AuroraGraphicsLibrary"}
      links {"AuroraAudioLibrary.lib", "AuroraGraphicsLibrary.lib"}
      characterset ("MBCS")

   filter "system:windows"
      defines {"_WIN32"}
      libdirs {"bin/%{cfg.buildcfg}/"}
      links {"winmm", "kernel32"}