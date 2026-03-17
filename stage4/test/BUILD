load("@rules_cc//cc:cc_test.bzl", "cc_test")

cc_test(
    name = "calculator_test",
    srcs = ["calculator_test.cc"],
    deps = [
        "//lib:calculator",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "greeter_test",
    srcs = ["greeter_test.cc"],
    deps = [
        "//lib:greeter",
        "@googletest//:gtest_main",
    ],
)
