add_rules("mode.debug", "mode.release")

-- key: option name
-- value: function to convert option value to config value
local opts = {}
local opt

opt = "input filename"
table.insert(opts, opt)
option(opt)
do
    set_default("")
    set_description("input filename")
end

opt = "bin2c"
table.insert(opts, opt)
option(opt)
do
    set_default(false)
    set_description("use bin2c")
end

opt = "dry run"
table.insert(opts, opt)
option(opt)
do
    set_default(false)
    set_description("do not write any file")
end

opt = "have tic6x"
table.insert(opts, opt)
option(opt)
do
    set_default(true)
    set_description("enable TI C6X asm")
end

opt = "downsample"
table.insert(opts, opt)
option(opt)
do
    set_default("bilinear")
    set_values("bilinear", "bicubic")
    after_check(
        function(option)
            for i, value in ipairs(option:get("values")) do
                if option:value() == value then
                    option:set_value(i)
                end
            end
        end
    )
    set_description("downsample from 720p to 360p")
end

opt = "padding"
table.insert(opts, opt)
option(opt)
do
    set_default("symmetric")
    set_values("edge", "reflect", "symmetric")
    after_check(
        function(option)
            for i, value in ipairs(option:get("values")) do
                if option:value() == value then
                    option:set_value(i)
                end
            end
        end
    )
    set_description("padding method")
end

opt = "scale"
table.insert(opts, opt)
option(opt)
do
    set_default(2)
    set_values(1, 2, 4)
    set_description("SCALE scale")
end

opt = "x264 bit depth"
table.insert(opts, opt)
option(opt)
do
    set_default(8)
    set_values(8, 10)
    set_description("bit depth")
end

opt = "x264 chroma format"
table.insert(opts, opt)
option(opt)
do
    set_default(420)
    set_values(400, 420, 422, 444)
    after_check(
        function(option)
            for i, value in ipairs(option:get("values")) do
                if option:value() == value then
                    option:set_value(i - 1)
                end
            end
        end
    )
    set_description("chroma format")
end

opt = "x264 log level"
table.insert(opts, opt)
option(opt)
do
    set_default("info")
    set_values("error", "warning", "info", "debug")
    after_check(
        function(option)
            for i, value in ipairs(option:get("values")) do
                if option:value() == value then
                    option:set_value(i - 1)
                end
            end
        end
    )
    set_description("log level")
end

target("x264-dsp")
do
    set_kind("binary")
    for _, name in ipairs(opts) do
        set_configvar(
            name:upper():gsub(" ", "_"),
            get_config(name),
            { quote = name == "input filename" }
        )
    end
    add_configfiles("xmake/config.h.in")
    if get_config("bin2c") then
        add_rules("utils.bin2c", { extensions = { ".yuv" } })
        add_configfiles("xmake/yuv.h.in")
        set_configvar("HEADER_FILENAME", get_config("input filename"):match("[^/]*$"))
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
