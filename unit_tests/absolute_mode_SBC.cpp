#include "addressing_mode_helpers.hpp"



struct SBC_Absolute_Expectations
{
    uint8_t a;
    NZCVFlags flags;

    uint8_t   operand;
};

struct SBC_Absolute_Expectations_BCD
{
    uint8_t a;
    NZCVFlags flags;

    uint8_t   operand;
};

using SBCAbsolute     = SBC<Absolute, SBC_Absolute_Expectations, 4>;
using SBCAbsoluteBCD  = SBC<Absolute, SBC_Absolute_Expectations_BCD, 5>;
using SBCAbsoluteMode = ParameterizedInstructionExecutorTestFixture<SBCAbsolute>;
using SBCAbsoluteModeBCD = ParameterizedInstructionExecutorTestFixture<SBCAbsoluteBCD>;


static void StoreTestValueAtEffectiveAddress(InstructionExecutorTestFixture &fixture, const SBCAbsolute &instruction_param)
{
    fixture.fakeMemory[instruction_param.address.absolute_address       ] = instruction_param.requirements.initial.operand;
}

static void SetupAffectedOrUsedRegisters(InstructionExecutorTestFixture &fixture, const SBCAbsolute &instruction_param)
{
    fixture.r.a = instruction_param.requirements.initial.a;
    fixture.r.SetFlag(FLAGS6502::N, instruction_param.requirements.initial.flags.n_value.expected_value);
    fixture.r.SetFlag(FLAGS6502::Z, instruction_param.requirements.initial.flags.z_value.expected_value);
    fixture.r.SetFlag(FLAGS6502::C, instruction_param.requirements.initial.flags.c_value.expected_value);
    fixture.r.SetFlag(FLAGS6502::V, instruction_param.requirements.initial.flags.v_value.expected_value);
}

template<>
void LoadInstructionIntoMemoryAndSetRegistersToInitialState(      InstructionExecutorTestFixture &fixture,
                                                            const SBCAbsolute                    &instruction_param)
{
    SetupRAMForInstructionsThatHaveAnEffectiveAddress(fixture, instruction_param);
    SetupAffectedOrUsedRegisters(fixture, instruction_param);
}

template<>
void RegistersAreInExpectedState(const Registers &registers,
                                 const SBC_Absolute_Expectations &expectations)
{
    EXPECT_THAT(registers.a, Eq(expectations.a));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::N), Eq(expectations.flags.n_value.expected_value));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::Z), Eq(expectations.flags.z_value.expected_value));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::C), Eq(expectations.flags.c_value.expected_value));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::V), Eq(expectations.flags.v_value.expected_value));
}

template<>
void MemoryContainsInstruction(const InstructionExecutorTestFixture &fixture,
                               const Instruction<AbstractInstruction_e::SBC, Absolute> &instruction)
{
    EXPECT_THAT(fixture.fakeMemory.at( fixture.executor.registers().program_counter ), Eq( OpcodeFor(AbstractInstruction_e::SBC, AddressMode_e::Absolute) ));
    EXPECT_THAT(fixture.fakeMemory.at( fixture.executor.registers().program_counter + 1), Eq( fixture.loByteOf(instruction.address.absolute_address) ));
    EXPECT_THAT(fixture.fakeMemory.at( fixture.executor.registers().program_counter + 2), Eq( fixture.hiByteOf(instruction.address.absolute_address) ));
}

template<>
void MemoryContainsExpectedComputation(const InstructionExecutorTestFixture &fixture,
                                       const SBCAbsolute                    &instruction)
{
    EXPECT_THAT(fixture.fakeMemory.at( instruction.address.absolute_address ), Eq( instruction.requirements.initial.operand ));
}

// BCD
static void StoreTestValueAtEffectiveAddress(InstructionExecutorTestFixture &fixture, const SBCAbsoluteBCD &instruction_param)
{
    fixture.fakeMemory[instruction_param.address.absolute_address       ] = instruction_param.requirements.initial.operand;
}

static void SetupAffectedOrUsedRegisters(InstructionExecutorTestFixture &fixture, const SBCAbsoluteBCD &instruction_param)
{
    // Let's just set the Decimal flag since this is a BCD mode test
    fixture.r.a = instruction_param.requirements.initial.a;
    fixture.r.SetFlag(FLAGS6502::N, instruction_param.requirements.initial.flags.n_value.expected_value);
    fixture.r.SetFlag(FLAGS6502::Z, instruction_param.requirements.initial.flags.z_value.expected_value);
    fixture.r.SetFlag(FLAGS6502::C, instruction_param.requirements.initial.flags.c_value.expected_value);
    fixture.r.SetFlag(FLAGS6502::V, instruction_param.requirements.initial.flags.v_value.expected_value);
    fixture.r.SetFlag(FLAGS6502::D, true);
}

template<>
void LoadInstructionIntoMemoryAndSetRegistersToInitialState(      InstructionExecutorTestFixture &fixture,
                                                            const SBCAbsoluteBCD                 &instruction_param)
{
    SetupRAMForInstructionsThatHaveAnEffectiveAddress(fixture, instruction_param);
    SetupAffectedOrUsedRegisters(fixture, instruction_param);
}

template<>
void RegistersAreInExpectedState(const Registers &registers,
                                 const SBC_Absolute_Expectations_BCD &expectations)
{
    EXPECT_THAT(registers.a, Eq(expectations.a));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::N), Eq(expectations.flags.n_value.expected_value));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::Z), Eq(expectations.flags.z_value.expected_value));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::C), Eq(expectations.flags.c_value.expected_value));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::V), Eq(expectations.flags.v_value.expected_value));
    EXPECT_TRUE(registers.GetFlag(FLAGS6502::D));
}

template<>
void MemoryContainsExpectedComputation(const InstructionExecutorTestFixture &fixture,
                                       const SBCAbsoluteBCD                 &instruction)
{
    EXPECT_THAT(fixture.fakeMemory.at( instruction.address.absolute_address ), Eq( instruction.requirements.initial.operand ));
}


static const std::vector<SBCAbsolute> SBCAbsoluteModeTestValues {
SBCAbsolute{
    // Beginning of a page
    Absolute().address(0x0000).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 6,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // Carry bit is clear on overflow. So, simulate no overflow.
                .v_value = { .expected_value = false } },
            .operand = 0 },
        .final = {
            .a = 6,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // No borrow!
                .v_value = { .expected_value = false } },
            .operand = 0
        }}
},
SBCAbsolute{
    // Middle of a page
    Absolute().address(0x0088).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 6,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // Carry bit is clear on overflow. So, simulate no overflow.
                .v_value = { .expected_value = false } },
            .operand = 0 },
        .final = {
            .a = 6,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // No borrow!
                .v_value = { .expected_value = false } },
            .operand = 0
        }}
},
SBCAbsolute{
    // End of a page
    Absolute().address(0x00FD).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 6,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // Carry bit is clear on overflow. So, simulate no overflow.
                .v_value = { .expected_value = false } },
            .operand = 0 },
        .final = {
            .a = 6,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // No borrow!
                .v_value = { .expected_value = false } },
            .operand = 0
        }}
},
SBCAbsolute{
    // Crossing a page (partial absolute address)
    Absolute().address(0x00FE).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 6,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // Carry bit is clear on overflow. So, simulate no overflow.
                .v_value = { .expected_value = false } },
            .operand = 0 },
        .final = {
            .a = 6,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // No borrow!
                .v_value = { .expected_value = false } },
            .operand = 0
        }}
},
SBCAbsolute{
    // Crossing a page (entire absolute address)
    Absolute().address(0x00FF).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 6,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // Carry bit is clear on overflow. So, simulate no overflow.
                .v_value = { .expected_value = false } },
            .operand = 0 },
        .final = {
            .a = 6,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // No borrow!
                .v_value = { .expected_value = false } },
            .operand = 0
        }}
},
SBCAbsolute{
    // Subtracting a zero does not affect the Z flag
    Absolute().address(0x8000).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 6,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false }, // Previous overflow occurred.  Simulate borrow.
                .v_value = { .expected_value = false } },
            .operand = 0 },
        .final = {
            .a = 5,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // No borrow generated
                .v_value = { .expected_value = false } },
            .operand = 0
        }}
},
SBCAbsolute{
    Absolute().address(0x8000).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 0,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true },
                .c_value = { .expected_value = true }, // No previous borrow
                .v_value = { .expected_value = false } },
            .operand = 0 },
        .final = {
            .a = 0,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true },
                .c_value = { .expected_value = true }, // No borrow occurred
                .v_value = { .expected_value = false } },
            .operand = 0
        }}
},
SBCAbsolute{
    Absolute().address(0x8000).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 0,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true },
                .c_value = { .expected_value = false }, // Previous borrow
                .v_value = { .expected_value = false } },
            .operand = 0x00 },
        .final = {
            .a = 0xFF,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false }, // Borrow occurred
                .v_value = { .expected_value = false } },
            .operand = 0x00
        }}
},
SBCAbsolute{
    Absolute().address(0x8000).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 0,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true },
                .c_value = { .expected_value = true }, // No previous borrow
                .v_value = { .expected_value = false } },
            .operand = 0x80 },
        .final = {
            .a = 0x80,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false }, // Borrow occurred
                .v_value = { .expected_value = true } },
            .operand = 0x80
        }}
},
// Carry flag
SBCAbsolute{
    // 2 - 1 = 1, C = 1, V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 0x02,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // No previous borrow
                .v_value = { .expected_value = false } },
            .operand = 0x01 },
        .final = {
            .a = 0x01,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // No borrow occurred
                .v_value = { .expected_value = false } },
            .operand = 0x01
        }}
},
SBCAbsolute{
    // 0 - 1 = FF, C = 1, V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 0x00,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // No previous borrow
                .v_value = { .expected_value = false } },
            .operand = 0x01 },
        .final = {
            .a = 0xFF,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false }, // Borrow occurred
                .v_value = { .expected_value = false } },
            .operand = 0x01
        }}
},
SBCAbsolute{
    // 0 - FF = 1, C = 1, V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 0x00,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // No previous borrow
                .v_value = { .expected_value = false } },
            .operand = 0xFF },
        .final = {
            .a = 0x01,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false }, // Borrow occurred
                .v_value = { .expected_value = false } },
            .operand = 0xFF
        }}
},
SBCAbsolute{
    // 80 - 1 = 7F, C = 1, V = 1
    Absolute().address(0x8000).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 0x80,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // No previous borrow
                .v_value = { .expected_value = false } },
            .operand = 0x01 },
        .final = {
            .a = 0x7F,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = true } },
            .operand = 0x01
        }}
},
SBCAbsolute{
    // 0x81 - 0x01 = 0x80 (-127 - 1 = -128), C = 1, V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 0x81,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // No previous borrow
                .v_value = { .expected_value = false } },
            .operand = 0x01 },
        .final = {
            .a = 0x80,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // No borrow occurred
                .v_value = { .expected_value = false } },
            .operand = 0x01
        }}
},
SBCAbsolute{
    // 0xFF - 0x7F = 0x80 (-1 - 127 = -128), C = 1, V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 0xFF,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // No previous borrow
                .v_value = { .expected_value = false } },
            .operand = 0x7F },
        .final = {
            .a = 0x80,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .operand = 0x7F
        }}
},
SBCAbsolute{
    // 0x80 - 0x80 = 0x00 (-128 - -128 = -256), C = 1, V = 1
    Absolute().address(0x8000).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 0x80,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // No previous borrow
                .v_value = { .expected_value = false } },
            .operand = 0x80 },
        .final = {
            .a = 0x00,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true },
                .c_value = { .expected_value = true }, // No borrow occurred
                .v_value = { .expected_value = false } },
            .operand = 0x80
        }}
},
SBCAbsolute{
    // 0xFF - 0x80 = -129,  V = 1
    Absolute().address(0x8000).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 0xFF,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // No previous borrow
                .v_value = { .expected_value = false } },
            .operand = 0x80 },
        .final = {
            .a = 0x7F,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .operand = 0x80
        }}
},
SBCAbsolute{
    // 0x03 - 0x02 - C = 0x00,
    Absolute().address(0x8000).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 0x03,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false }, // Previous borrow occurred
                .v_value = { .expected_value = false } },
            .operand = 0x02 },
        .final = {
            .a = 0x00,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true },
                .c_value = { .expected_value = true }, // No borrow occurred
                .v_value = { .expected_value = false } },
            .operand = 0x02
        }}
},
SBCAbsolute{
    // 0x01 - 0x01 - C = 0xFF,
    Absolute().address(0x8000).value(0xA000),
    SBCAbsolute::Requirements{
        .initial = {
            .a = 0x01,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false }, // Previous borrow occurred
                .v_value = { .expected_value = false } },
            .operand = 0x01 },
        .final = {
            .a = 0xFF,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false }, // Borrow occurred
                .v_value = { .expected_value = false } },
            .operand = 0x01
        }}
}
};

TEST_P(SBCAbsoluteMode, TypicalInstructionExecution)
{
    TypicalInstructionExecution(*this, GetParam());
}

INSTANTIATE_TEST_SUITE_P(SubtractAbsoluteAtVariousAddresses,
                         SBCAbsoluteMode,
                         testing::ValuesIn(SBCAbsoluteModeTestValues) );


static const std::vector<SBCAbsoluteBCD> SBCAbsoluteModeBCDTestValues {
SBCAbsoluteBCD{
    // 01 - 1 = 00, C = unchanged, V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x01,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true },
                .c_value = { .expected_value = true }, // Set the carry beforehand
                .v_value = { .expected_value = false } },
            .operand = 0x01},
        .final = {
            .a = 0x00,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .operand = 0x01
        }}
},
SBCAbsoluteBCD{
    // 09 - 1 = 08, C = unchanged, V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x09,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // Set the carry beforehand
                .v_value = { .expected_value = false } },
            .operand = 0x01},
        .final = {
            .a = 0x08,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .operand = 0x01
        }}
},
SBCAbsoluteBCD{
    // 10 - 1 = 9, C = unchanged, V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x10,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // Set the carry beforehand
                .v_value = { .expected_value = false } },
            .operand = 0x01},
        .final = {
            .a = 0x09,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .operand = 0x01
        }}
},
SBCAbsoluteBCD{
    // 20 - 1 = 19, C = unchanged, V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x19,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // Set the carry beforehand
                .v_value = { .expected_value = true } },
            .operand = 0x01},
        .final = {
            .a = 0x20,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .operand = 0x01
        }}
},
SBCAbsoluteBCD{
    // 30 - 1 = 29, C = unchanged, V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x30,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // Set the carry beforehand
                .v_value = { .expected_value = false } },
            .operand = 0x01},
        .final = {
            .a = 0x29,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .operand = 0x01
        }}
},
SBCAbsoluteBCD{
    // 40 - 1 = 39, C = unchanged, V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x40,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // Set the carry beforehand
                .v_value = { .expected_value = false } },
            .operand = 0x01},
        .final = {
            .a = 0x39,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .operand = 0x01
        }}
},
SBCAbsoluteBCD{
    // 50 - 1 = 49, C = unchanged, V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x50,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // Set the carry beforehand
                .v_value = { .expected_value = false } },
            .operand = 0x01},
        .final = {
            .a = 0x49,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .operand = 0x01
        }}
},
SBCAbsoluteBCD{
    // 60 - 1 = 59, C = unchanged, V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x60,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // Set the carry beforehand
                .v_value = { .expected_value = false } },
            .operand = 0x01},
        .final = {
            .a = 0x59,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .operand = 0x01
        }}
},
SBCAbsoluteBCD{
    // 70 - 1 = 69, C = unchanged, V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x70,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // Set the carry beforehand
                .v_value = { .expected_value = false } },
            .operand = 0x01},
        .final = {
            .a = 0x69,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .operand = 0x01
        }}
},
SBCAbsoluteBCD{
    // 80 - 1 = 79, C = unchanged, V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x80,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // Set the carry beforehand
                .v_value = { .expected_value = false } },
            .operand = 0x01},
        .final = {
            .a = 0x79,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .operand = 0x01
        }}
},
SBCAbsoluteBCD{
    // 90 - 1 = 89, C = unchanged, V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x90,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // Set the carry beforehand
                .v_value = { .expected_value = false } },
            .operand = 0x01},
        .final = {
            .a = 0x89,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .operand = 0x01
        }}
},
SBCAbsoluteBCD{
    // 0 - 1 = 99, C = 0 (wraparound), V = 0
    Absolute().address(0x8000).value(0xA000),
    SBCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x00,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true },
                .c_value = { .expected_value = true }, // Set the carry beforehand
                .v_value = { .expected_value = false } },
            .operand = 0x01},
        .final = {
            .a = 0x99,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .operand = 0x01
        }}
}
};

TEST_P(SBCAbsoluteModeBCD, SBCAbsoluteModeBCDTest)
{
    TypicalInstructionExecution(*this, GetParam());
}

INSTANTIATE_TEST_SUITE_P(SubtractAbsoluteAtVariousAddressesBCD,
                         SBCAbsoluteModeBCD,
                         testing::ValuesIn(SBCAbsoluteModeBCDTestValues) );
