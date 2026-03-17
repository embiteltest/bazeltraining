load("@rules_cc//cc:cc_test.bzl", "cc_test")

cc_test(
    name = "platform_test",
    srcs = ["platform_test.cc"],
    deps = [
        "//lib:platform",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "logger_test",
    srcs = ["logger_test.cc"],
    deps = [
        "//lib:logger",
        "@googletest//:gtest_main",
    ],
)
