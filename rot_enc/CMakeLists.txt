
add_library(rot_enc)

# invoke pio_asm to assemble state machine code
pico_generate_pio_header(rot_enc ${CMAKE_CURRENT_LIST_DIR}/rot_enc.pio)

target_link_libraries(rot_enc PRIVATE
        pico_stdlib
        hardware_pio
        )

target_include_directories (rot_enc PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)
