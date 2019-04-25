{
  "targets": [
    {
      "target_name": "cryptonight",
      "include_dirs": [
	"<!(node -e \"require('nan')\")"
      ],
      "xcode_settings": {
        "OTHER_CFLAGS": [
          "-maes",
          "-Ofast",
          "-fexceptions"
        ]
      },
      "cflags": [
        "-maes",
        "-Ofast",
        "-fexceptions"
      ],
      "sources": [
        "lib/cryptonight.cc",
        "lib/vendor/aesb.c",
        "lib/vendor/blake256.c",
        "lib/vendor/chacha.c",
        "lib/vendor/crypto-ops-data.c",
        "lib/vendor/crypto-ops.c",
        "lib/vendor/groestl.c",
        "lib/vendor/hash-extra-blake.c",
        "lib/vendor/hash-extra-groestl.c",
        "lib/vendor/hash-extra-jh.c",
        "lib/vendor/hash-extra-skein.c",
        "lib/vendor/hash.c",
        "lib/vendor/hex.cpp",
        "lib/vendor/jh.c",
        "lib/vendor/keccak.c",
        "lib/vendor/skein.c",
        "lib/vendor/slow-hash.c",
        "lib/vendor/CryptonightR_JIT.c",
        "lib/vendor/CryptonightR_template.S"
      ]
    }
  ]
}
