cc_library(
    name = "soa",
    hdrs = ["include/mapped_soa.h", "include/soa.h" ],
    includes = [
        "include",
    ],
    copts = [ "--std=c++14"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "example",
    srcs = ["soa_example.cc"],
    visibility = ["//visibility:public"],
    copts = [ "--std=c++14"],
    deps = [
        ":soa"
    ]
)