load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "platform",
    srcs = ["platform.cc"],
    hdrs = ["platform.h"],
    visibility = ["//main:__pkg__", "//test:__pkg__"],
)

cc_library(
    name = "logger",
    srcs = ["logger.cc"],
    hdrs = ["logger.h"],
    # Use select() to conditionally define ENABLE_VERBOSE_LOGGING
    defines = select({
        "//:verbose_mode": ["ENABLE_VERBOSE_LOGGING"],
        "//conditions:default": [],
    }),
    visibility = ["//main:__pkg__", "//test:__pkg__"],
)
