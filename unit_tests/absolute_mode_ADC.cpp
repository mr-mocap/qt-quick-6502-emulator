#include "addressing_mode_helpers.hpp"



struct ADC_Absolute_Expectations
{
    constexpr ADC_Absolute_Expectations &accumulator(const uint8_t v) { a = v; return *this; }

    uint8_t a;
    NZCVFlags flags;

    uint8_t   addend;
};

struct ADC_Absolute_Expectations_BCD
{
    constexpr ADC_Absolute_Expectations_BCD &accumulator(const uint8_t v) { a = v; return *this; }

    uint8_t a;
    NZCVFlags flags;

    uint8_t   addend;
};

using ADCAbsolute     = ADC<Absolute, ADC_Absolute_Expectations, 4>;
using ADCAbsoluteBCD  = ADC<Absolute, ADC_Absolute_Expectations_BCD, 5>;
using ADCAbsoluteMode = ParameterizedInstructionExecutorTestFixture<ADCAbsolute>;
using ADCAbsoluteModeBCD = ParameterizedInstructionExecutorTestFixture<ADCAbsoluteBCD>;


static void StoreTestValueAtEffectiveAddress(InstructionExecutorTestFixture &fixture, const ADCAbsolute &instruction_param)
{
    fixture.fakeMemory[instruction_param.address.absolute_address       ] = instruction_param.requirements.initial.addend;
}

static void SetupAffectedOrUsedRegisters(InstructionExecutorTestFixture &fixture, const ADCAbsolute &instruction_param)
{
    fixture.r.a = instruction_param.requirements.initial.a;
    fixture.r.SetFlag(FLAGS6502::N, instruction_param.requirements.initial.flags.n_value.expected_value);
    fixture.r.SetFlag(FLAGS6502::Z, instruction_param.requirements.initial.flags.z_value.expected_value);
    fixture.r.SetFlag(FLAGS6502::C, instruction_param.requirements.initial.flags.c_value.expected_value);
    fixture.r.SetFlag(FLAGS6502::V, instruction_param.requirements.initial.flags.v_value.expected_value);
}

template<>
void LoadInstructionIntoMemoryAndSetRegistersToInitialState(      InstructionExecutorTestFixture &fixture,
                                                            const ADCAbsolute                    &instruction_param)
{
    SetupRAMForInstructionsThatHaveAnEffectiveAddress(fixture, instruction_param);
    SetupAffectedOrUsedRegisters(fixture, instruction_param);
}

template<>
void RegistersAreInExpectedState(const Registers &registers,
                                 const ADC_Absolute_Expectations &expectations)
{
    EXPECT_THAT(registers.a, Eq(expectations.a));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::N), Eq(expectations.flags.n_value.expected_value));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::Z), Eq(expectations.flags.z_value.expected_value));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::C), Eq(expectations.flags.c_value.expected_value));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::V), Eq(expectations.flags.v_value.expected_value));
}

template<>
void MemoryContainsInstruction(const InstructionExecutorTestFixture &fixture,
                               const Instruction<AbstractInstruction_e::ADC, Absolute> &instruction)
{
    EXPECT_THAT(fixture.fakeMemory.at( fixture.executor.registers().program_counter ), Eq( OpcodeFor(AbstractInstruction_e::ADC, AddressMode_e::Absolute) ));
    EXPECT_THAT(fixture.fakeMemory.at( fixture.executor.registers().program_counter + 1), Eq( fixture.loByteOf(instruction.address.absolute_address) ));
    EXPECT_THAT(fixture.fakeMemory.at( fixture.executor.registers().program_counter + 2), Eq( fixture.hiByteOf(instruction.address.absolute_address) ));
}

template<>
void MemoryContainsExpectedComputation(const InstructionExecutorTestFixture &fixture,
                                       const ADCAbsolute                    &instruction)
{
    EXPECT_THAT(fixture.fakeMemory.at( instruction.address.absolute_address ), Eq( instruction.requirements.initial.addend ));
}


// BCD
static void StoreTestValueAtEffectiveAddress(InstructionExecutorTestFixture &fixture, const ADCAbsoluteBCD &instruction_param)
{
    fixture.fakeMemory[instruction_param.address.absolute_address       ] = instruction_param.requirements.initial.addend;
}

static void SetupAffectedOrUsedRegisters(InstructionExecutorTestFixture &fixture, const ADCAbsoluteBCD &instruction_param)
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
                                                            const ADCAbsoluteBCD                 &instruction_param)
{
    SetupRAMForInstructionsThatHaveAnEffectiveAddress(fixture, instruction_param);
    SetupAffectedOrUsedRegisters(fixture, instruction_param);
}

template<>
void RegistersAreInExpectedState(const Registers &registers,
                                 const ADC_Absolute_Expectations_BCD &expectations)
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
                                       const ADCAbsoluteBCD                 &instruction)
{
    EXPECT_THAT(fixture.fakeMemory.at( instruction.address.absolute_address ), Eq( instruction.requirements.initial.addend ));
}

static const std::vector<ADCAbsolute> ADCAbsoluteModeTestValues {
ADCAbsolute{
    // Beginning of a page
    Absolute().address(0x0000).value(0xA000),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0,
            .flags = { },
            .addend = 6},
        .final = {
            .a = 6,
            .flags = { },
            .addend = 6
        }}
},
ADCAbsolute{
    // Middle of a page
    Absolute().address(0x0088).value(0xA000),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0,
            .flags = { },
            .addend = 6},
        .final = {
            .a = 6,
            .flags = { },
            .addend = 6
        }}
},
ADCAbsolute{
    // End of a page
    Absolute().address(0x00FD).value(0xA000),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0,
            .flags = { },
            .addend = 6},
        .final = {
            .a = 6,
            .flags = { },
            .addend = 6
        }}
},
ADCAbsolute{
    // Crossing a page (partial absolute address)
    Absolute().address(0x00FE).value(0xA000),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0,
            .flags = { },
            .addend = 6},
        .final = {
            .a = 6,
            .flags = { },
            .addend = 6
        }}
},
ADCAbsolute{
    // Crossing a page (entire absolute address)
    Absolute().address(0x00FF).value(0xA000),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0,
            .flags = { },
            .addend = 6},
        .final = {
            .a = 6,
            .flags = { },
            .addend = 6
        }}
},
ADCAbsolute{
    // Adding a zero affects the Z flag
    Absolute().address(0x8000).value(0xA000),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0x00,
            .flags = { },
            .addend = 0},
        .final = {
            .a = 0x00,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0
        }}
},
ADCAbsolute{
    // Adding a negative affects the N flag
    Absolute().address(0x8000).value(0xA000),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0x00,
            .flags = { },
            .addend = 0x80},
        .final = {
            .a = 0x80,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x80
        }}
},
// Carry flag
ADCAbsolute{
    // 1 + 1 = 2, C = 0, V=0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0x01,
            .flags = { },
            .addend = 0x01},
        .final = {
            .a = 0x02,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01
        }}
},
ADCAbsolute{
    // FF + 1 = 0, C = 1, V = 0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0xFF,
            .flags = { },
            .addend = 0x01},
        .final = {
            .a = 0x00,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .addend = 0x01
        }}
},
ADCAbsolute{
    // 1 + FF = 0, C = 1, V = 0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0x01,
            .flags = { },
            .addend = 0xFF},
        .final = {
            .a = 0x00,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .addend = 0xFF
        }}
},
ADCAbsolute{
    // 7F + 1 = 80, C = 0, V = 1
    Absolute().address(0x8000).value(0xA000),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0x7f,
            .flags = { },
            .addend = 0x01},
        .final = {
            .a = 0x80,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = true } },
            .addend = 0x01
        }}
},
ADCAbsolute{
    // 0x80 + 0x01 = 0x81 (-128 + 1 = -127), C = 0, V = 0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0x80,
            .flags = { },
            .addend = 0x01},
        .final = {
            .a = 0x81,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01
        }}
},
ADCAbsolute{
    // 0x80 + 0x7F = 0xFF (-128 + 127 = -1), C = 0, V = 0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0x80,
            .flags = { },
            .addend = 0x7F},
        .final = {
            .a = 0xFF,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x7F
        }}
},
ADCAbsolute{
    // 0x80 + 0x80 = 0x00 (-128 + -127 = -256), C = 1, V = 1
    Absolute().address(0x8000).value(0xA000),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0x80,
            .flags = { },
            .addend = 0x80},
        .final = {
            .a = 0x00,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = true } },
            .addend = 0x80
        }}
},
ADCAbsolute{
    // 0x80 + 0xFF = -129,  V = 1
    Absolute().address(0x8000).value(0xA000),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0x80,
            .flags = { },
            .addend = 0xFF},
        .final = {
            .a = 0x7F,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = true } },
            .addend = 0xFF
        }}
},
ADCAbsolute{
    // 0x00 + 0x02 + C = 0x03,
    Absolute().address(0x8000).value(0x02),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0x00,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .addend = 0x02},
        .final = {
            .a = 0x03,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0xFF
        }}
},
ADCAbsolute{
    // 0xFF + 0x01 + C = 0x01,
    Absolute().address(0x8000).value(0xA000),
    ADCAbsolute::Requirements{
        .initial = {
            .a = 0xFF,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .addend = 0x01},
        .final = {
            .a = 0x01,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = true }, // Carry should be set because we wrapped around
                .v_value = { .expected_value = false } },
            .addend = 0x01
        }}
}
};

TEST_P(ADCAbsoluteMode, TypicalInstructionExecution)
{
    TypicalInstructionExecution(*this, GetParam());
}

INSTANTIATE_TEST_SUITE_P(AddAbsoluteAtVariousAddresses,
                         ADCAbsoluteMode,
                         testing::ValuesIn(ADCAbsoluteModeTestValues) );

// Assume that the N, Z, and V flags are set according to regular binary values and NOT BCD values.
static const std::vector<ADCAbsoluteBCD> ADCAbsoluteModeBCDTestValues {
ADCAbsoluteBCD{
    // 00 + 1 = 01, C = 0, V = 0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x00,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01},
        .final = {
            .a = 0x01,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01
        }}
},
ADCAbsoluteBCD{
    // 08 + 1 = 09, C = 0, V = 0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x08,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01},
        .final = {
            .a = 0x09,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01
        }}
},
ADCAbsoluteBCD{
    // 09 + 1 = 10, C = 0, V = 0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x09,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01},
        .final = {
            .a = 0x10,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01
        }}
},
ADCAbsoluteBCD{
    // 19 + 1 = 20, C = 0, V = 0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x19,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = true } },
            .addend = 0x01},
        .final = {
            .a = 0x20,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01
        }}
},
ADCAbsoluteBCD{
    // 29 + 1 = 30, C = 0, V = 0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x29,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01},
        .final = {
            .a = 0x30,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01
        }}
},
ADCAbsoluteBCD{
    // 39 + 1 = 40, C = 0, V = 0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x39,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01},
        .final = {
            .a = 0x40,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01
        }}
},
ADCAbsoluteBCD{
    // 49 + 1 = 50, C = 0, V = 0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x49,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01},
        .final = {
            .a = 0x50,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01
        }}
},
ADCAbsoluteBCD{
    // 59 + 1 = 60, C = 0, V = 0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x59,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01},
        .final = {
            .a = 0x60,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01
        }}
},
ADCAbsoluteBCD{
    // 69 + 1 = 70, C = 0, V = 0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x69,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01},
        .final = {
            .a = 0x70,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01
        }}
},
ADCAbsoluteBCD{
    // 79 + 1 = 80, C = 0, V = 0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x79,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01},
        .final = {
            .a = 0x80,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = true } },
            .addend = 0x01
        }}
},
ADCAbsoluteBCD{
    // 89 + 1 = 90, C = 0, V = 0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x89,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01},
        .final = {
            .a = 0x90,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01
        }}
},
ADCAbsoluteBCD{
    // 99 + 1 = 00, C = 1, V = 0
    Absolute().address(0x8000).value(0xA000),
    ADCAbsoluteBCD::Requirements{
        .initial = {
            .a = 0x99,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false },
                .c_value = { .expected_value = false },
                .v_value = { .expected_value = false } },
            .addend = 0x01},
        .final = {
            .a = 0x00,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true },
                .c_value = { .expected_value = true },
                .v_value = { .expected_value = false } },
            .addend = 0x01
        }}
}
};

TEST_P(ADCAbsoluteModeBCD, ADCAbsoluteModeBCDTest)
{
    TypicalInstructionExecution(*this, GetParam());
}

INSTANTIATE_TEST_SUITE_P(AddAbsoluteAtVariousAddressesBCD,
                         ADCAbsoluteModeBCD,
                         testing::ValuesIn(ADCAbsoluteModeBCDTestValues) );
