{
    "targets": [{
        "target_name": "nakasendo",
        'cflags!': [ '-fno-exceptions' ],
        'cflags_cc!': [ '-fno-exceptions' ],
        'cflags_cc' : ['-std=c++14 -fPIC'],
        "sources": [
            "cppsrc/main.cpp",
	        "cppsrc/Nakasendo_Bindings/ecpointBindings.cpp",
	        "cppsrc/Nakasendo_Bindings/polynomialBindings.cpp",
	        "cppsrc/Nakasendo_Bindings/bignumBindings.cpp",
	        "cppsrc/Nakasendo_Bindings/messageHashBindings.cpp",
	        "cppsrc/Nakasendo_Bindings/SymEncDecBindings.cpp",
	        "cppsrc/Nakasendo_Bindings/AsymKeyBindings.cpp",
	        "cppsrc/Nakasendo_Bindings/BSVAddressBindings.cpp",
		    "cppsrc/Nakasendo_TS_Bindings/playerWrapper.cpp",
		    "cppsrc/Nakasendo_TS_Bindings/GroupMetaDataWrapper.cpp",
		    "cppsrc/Nakasendo_TS_Bindings/OrchestratorWrapper.cpp"
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")",
	    "./cppsrc/Nakasendo_Bindings",
	    "./cppsrc/Nakasendo_TS_Bindings",
	    "./naklibsrc/src/applications/TS_protobuf/cpp",
	    "./naklibsrc/src/core",
	    "./naklibsrc/src/generated/hpp"
        ],  
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'product_dir': '<(module_path)',
        'conditions': [
            ['OS=="mac"', {
                'xcode_settings': {
                'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                },
                'include_dirs': ["/usr/local/include"],
                'libraries': ['-Wl,/usr/local/lib/libcrypto.a -Wl,<(module_root_dir)/naklibsrc/lib/osx/libBigNumbers.a -Wl,<(module_root_dir)/naklibsrc/lib/osx/libAsymKey.a -Wl,<(module_root_dir)/naklibsrc/lib/osx/libBSVAddress.a -Wl,<(module_root_dir)/naklibsrc/lib/osx/libECPoint.a -Wl,<(module_root_dir)/naklibsrc/lib/osx/libMessageHash.a -Wl,<(module_root_dir)/naklibsrc/lib/osx/libPolynomial.a -Wl,<(module_root_dir)/naklibsrc/lib/osx/libSecretSplit.a -Wl,<(module_root_dir)/naklibsrc/lib/osx/libSymEncDec.a -Wl,<(module_root_dir)/naklibsrc/lib/osx/libTSCore.a']
            }]
        ],
        'defines': [ 'NAPI_CPP_EXCEPTIONS' ]
    }
    ]
}
