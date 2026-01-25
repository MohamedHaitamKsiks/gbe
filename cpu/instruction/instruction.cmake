set (GBE_HEADERS ${GBE_HEADERS}
    ${CMAKE_CURRENT_LIST_DIR}/Instruction.h
    ${CMAKE_CURRENT_LIST_DIR}/InstructionType.h
    ${CMAKE_CURRENT_LIST_DIR}/InstructionDecoder.h
    ${CMAKE_CURRENT_LIST_DIR}/Operand.h
    ${CMAKE_CURRENT_LIST_DIR}/Assembly.h
    ${CMAKE_CURRENT_LIST_DIR}/Disassembler.h
)

set(GBE_SOURCES ${GBE_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/InstructionDecoder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Operand.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Instruction.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Assembly.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Disassembler.cpp
)
