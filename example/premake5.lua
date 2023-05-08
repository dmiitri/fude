project "example"
    kind "ConsoleApp"
    language "C"
    targetdir "%{wks.location}/build/bin"
    objdir "%{wks.location}/build/bin-int"
    location "%{wks.location}/build/scripts"

    files {
        "./**.c",
    }

    includedirs {
        "../include",
    }

    links {
        "fude",
    }

    filter "system:linux"
        links {
            "m",
        }