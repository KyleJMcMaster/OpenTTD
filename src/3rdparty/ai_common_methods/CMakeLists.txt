# Add your source files here
set(EXTRA_SOURCES
    test.cpp
    # cuda_logic.cu    If you have CUDA files
)

# Tell CMake to include these in the main game library
target_sources(openttd_lib PRIVATE ${EXTRA_SOURCES})

# If you use CUDA, tell the compiler to treat .cu files correctly
set_source_files_properties(cuda_logic.cu PROPERTIES LANGUAGE CUDA)

# Add your folder to the include path so you can use #include "my_ai_bridge.hpp" anywhere
target_include_directories(openttd_lib PRIVATE .)