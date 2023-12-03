
project "AuroraCore"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   location "generated\\"
   compileas "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files {
       "src/**",
        "include/**",
         ".editorconfig",
         "%{_OPTIONS['utilslocation']}/include/**" ,
         "%{_OPTIONS['utilslocation']}/vendor/glm/glm/**" ,
      }
   includedirs {
       "include/", 
       "%{_OPTIONS['utilslocation']}".."/include", 
       "%{_OPTIONS['sunriselocation']}".."/vendor/", 
       "%{_OPTIONS['agllocation']}".."/include/", 
       "%{_OPTIONS['sunriselocation']}".."/vendor/alsoft/include", 
       "%{_OPTIONS['utilslocation']}".."/vendor/glm" 
      }
    --libdirs {"vendor/glfw/lib-vc2022/","vendor/assimp/lib/Release/"}


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