workspace "OpenGLPlayground"
   configurations { "Debug", "Release" }

project "OpenGLPlayground"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "bin/%{cfg.buildcfg}"

   defines {"SPDLOG_COMPILED_LIB"}

   pchheader "src/pch.hpp"

   includedirs {"external/include"}
   libdirs {"external/lib"}
   links {"glfw3", "soil2", "spdlog", "user32", "gdi32", "opengl32"}

   files { "src/**.hpp", "src/**.cpp", "external/src/**.c" }

   filter "configurations:Debug"
	  defines { "DEBUG" }
	  symbols "On"

   filter "configurations:Release"
	  defines { "NDEBUG" }
	  optimize "On"
