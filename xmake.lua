add_rules("mode.debug", "mode.release")

-- key: option name
-- value: function to convert option value to config value
local opts = {}
local values
local opt

opt = "input filename"
opts[opt] = ""
option(opt)
do
    set_default("")
    set_description("input filename")
end

opt = "bin2c"
opts[opt] = ""
option(opt)
do
    set_default(false)
    set_description("use bin2c")
end

opt = "dry run"
opts[opt] = ""
option(opt)
do
    set_default(false)
    set_description("do not write any file")
end

opt = "have tic6x"
opts[opt] = ""
option(opt)
do
    set_default(true)
    set_description("enable TI C6X asm")
end

opt = "downsample"
opts[opt] = { bilinear = 1, bicubic = 2 }
option(opt)
do
    set_default("bilinear")
    values = {}
    for value in pairs(opts[opt]) do
        table.insert(values, value)
    end
    set_values(values)
    set_description("downsample from 720p to 360p")
end

opt = "padding"
opts[opt] = { edge = 1, reflect = 2, symmetric = 3 }
option(opt)
do
    set_default("symmetric")
    values = {}
    for value in pairs(opts[opt]) do
        table.insert(values, value)
    end
    set_values(values)
    set_description("padding method")
end

opt = "scale"
opts[opt] = ""
option(opt)
do
    set_default(2)
    set_values(1, 2, 4)
    set_description("SCALE scale")
end

opt = "x264 bit depth"
opts[opt] = ""
option(opt)
do
    set_default(8)
    set_values(8, 10)
    set_description("bit depth")
end

opt = "x264 chroma format"
opts[opt] = { [400] = 0, [420] = 1, [422] = 2, [444] = 3 }
option(opt)
do
    set_default(420)
    values = {}
    for value in pairs(opts[opt]) do
        table.insert(values, value)
    end
    set_values(values)
    set_description("chroma format")
end

opt = "x264 log level"
opts[opt] = { error = 0, warning = 1, info = 2, debug = 3 }
option(opt)
do
    set_default("info")
    values = {}
    for value in pairs(opts[opt]) do
        table.insert(values, value)
    end
    set_values(values)
    set_description("log level")
end

target("x264-dsp")
do
    set_kind("binary")
    for name, values in pairs(opts) do
        local value = get_config(name)
        if values ~= "" then
            value = values[value]
        end
        set_configvar(
            name:upper():gsub(" ", "_"),
            value,
            { quote = name == "input filename" }
        )
    end
    add_configfiles("xmake/config.h.in")
    if get_config("bin2c") then
        add_rules("utils.bin2c", {extensions = {".yuv"}})
        add_configfiles("xmake/yuv.h.in")
        local value = get_config("input filename")
        set_configvar("HEADER_FILENAME", value:match("[^/]*$"))
        add_files(value)
    end
    add_includedirs("$(buildir)")
    add_includedirs("$(projectdir)")
    add_files(
        "downsample.c",
        "input.c",
        "output.c",
        "x264.c",
        "common/*.c",
        "encoder/*.c"
    )
end
