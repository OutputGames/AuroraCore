
project "AuroraCore"
   kind "SharedLib"
   language "C++"
   cppdialect "C++17"
   location "generated\\"
   compileas "C++"
   targetdir "bin/%{cfg.buildcfg}"

   defines {"_CRT_SECURE_NO_WARNINGS"}

   files {
       "src/**",
        "include/**",
         ".editorconfig",
         "%{_OPTIONS['utilslocation']}/include/**" ,
         "%{_OPTIONS['utilslocation']}/vendor/glm/glm/**" ,
         "testresources/**",
         "%{_OPTIONS['utilslocation']}".."/vendor/sdl2/include/**" , 
         "vendor/jolt/include/**",
         --"vendor/enet/*.c",
         "vendor/enet/include/**",
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
       "%{_OPTIONS['utilslocation']}".."/vendor/sdl2/include/",
       "vendor/jolt/include/",
       "vendor/enet/include/",
      }
    libdirs {
      "%{_OPTIONS['agllocation']}/vendor/glfw/lib-vc2022/",
      "%{_OPTIONS['agllocation']}/vendor/assimp/lib/Release/", 
      "%{_OPTIONS['utilslocation']}".."/vendor/sdl2/lib",
      "vendor/jolt/lib/",
      "vendor/enet/"
    }

    
    links {"vulkan-1.lib", "enet64"}
    libdirs {"C:/VulkanSDK/1.3.250.0/Lib"}
    includedirs {"C:/VulkanSDK/1.3.250.0/Include"}
    --files {"%{_OPTIONS['agllocation']}/vendor/imgui/backends/imgui_impl_vulkan.*"}

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      debugdir "./"
      runtime "Debug"
      optimize "Off"
      links {"SDL2d", "SDL2maind"}

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      runtime "Release"
      links {"SDL2", "SDL2main"}

   filter "action:vs*" 
      defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS"}
      dependson {"AuroraAudioLibrary", "AuroraGraphicsLibrary"}
      links {"AuroraAudioLibrary.lib", "AuroraGraphicsLibrary.lib"}
      characterset ("MBCS")

   filter "system:windows"
      defines {"_WIN32"}
      libdirs {"bin/%{cfg.buildcfg}/"}
      links {"winmm", "ws2_32", "kernel32"}