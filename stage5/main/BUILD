load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_binary(
    name = "app",
    srcs = ["main.cc"],
    defines = select({
        "//:verbose_mode": ["ENABLE_VERBOSE_LOGGING"],
        "//conditions:default": [],
    }),
    deps = [
        "//lib:logger",
        "//lib:platform",
    ],
)
