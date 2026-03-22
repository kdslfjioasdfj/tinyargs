function concat_arrays(arr1, arr2)
	local result = {}
	local n = 0
	for i = 1, #arr1 do
		n = n + 1
		result[n] = arr1[i]
	end
	for i = 1, #arr2 do
		n = n + 1
		result[n] = arr2[i]
	end

	return result
end

local coreheaders = { "include/tinyargs.h" }
local coresrcs = { "src/tinyargs.c" }

local exampleheaders = concat_arrays(coreheaders, {})
local examplesrcs = { "example/example.c" }

workspace("tinyargs")
language("C")
cdialect("C11")
includedirs({ "include" })
configurations({ "Debug", "Release" })
defines({ "tinyargs_CONFIG_BUILDING" })
filter({ "toolset:gcc or toolset:clang" })
buildoptions({ "-Wall", "-Wextra", "-Wpedantic" })
filter({}) -- Clear filters
filter({ "toolset:msc" })
buildoptions({ "/W4" })
defines({ "_CRT_SECURE_NO_WARNINGS" })
filter({}) -- Clear filters
filter({ "configurations:Release" })
defines({ "NDEBUG" })
symbols("Off")
optimize("On")
filter({}) -- Clear filters
filter({ "configurations:Debug" })
defines({ "DEBUG" })
symbols("On")
optimize("Off")

newoption({
	trigger = "shared",
	description = "Make a shared library instead of the a static one.",
})

project("tinyargs")
if _OPTIONS["shared"] then
	kind("SharedLib")
	defines({ "TINYARGS_CONFIG_SHARED" })
	filter({ "system:not windows" })
	buildoptions({ "-fPIC" })
	filter({}) -- Clear filters
else
	kind("StaticLib")
end
files(concat_arrays(coreheaders, coresrcs))

project("tinyargs-example")
kind("ConsoleApp")
files(concat_arrays(exampleheaders, examplesrcs))
links({ "tinyargs" })
