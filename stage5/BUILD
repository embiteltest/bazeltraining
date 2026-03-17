# Root BUILD file for config_setting definitions

load("@bazel_skylib//rules:common_settings.bzl", "bool_flag")

# Custom build flag: --//:verbose
bool_flag(
    name = "verbose",
    build_setting_default = False,
)

config_setting(
    name = "verbose_mode",
    flag_values = {
        "//:verbose": "True",
    },
)

config_setting(
    name = "is_linux",
    constraint_values = [
        "@platforms//os:linux",
    ],
)

config_setting(
    name = "is_windows",
    constraint_values = [
        "@platforms//os:windows",
    ],
)

config_setting(
    name = "is_macos",
    constraint_values = [
        "@platforms//os:macos",
    ],
)

config_setting(
    name = "opt_build",
    values = {
        "compilation_mode": "opt",
    },
)

config_setting(
    name = "dbg_build",
    values = {
        "compilation_mode": "dbg",
    },
)
