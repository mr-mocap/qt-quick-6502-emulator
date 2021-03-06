#include "addressing_mode_helpers.hpp"



struct AND_ZeroPage_Expectations
{
    uint8_t a;
    NZFlags flags;

    uint8_t operand;
};

using ANDZeroPage     = AND<ZeroPage, AND_ZeroPage_Expectations, 3>;
using ANDZeroPageMode = ParameterizedInstructionExecutorTestFixture<ANDZeroPage>;


static void StoreTestValueAtEffectiveAddress(InstructionExecutorTestFixture &fixture, const ANDZeroPage &instruction_param)
{
    fixture.fakeMemory[instruction_param.address.zero_page_address] = instruction_param.requirements.initial.operand;
}

static void SetupAffectedOrUsedRegisters(InstructionExecutorTestFixture &fixture, const ANDZeroPage &instruction_param)
{
    fixture.r.a = instruction_param.requirements.initial.a;
    fixture.r.SetFlag(FLAGS6502::N, instruction_param.requirements.initial.flags.n_value.expected_value);
    fixture.r.SetFlag(FLAGS6502::Z, instruction_param.requirements.initial.flags.z_value.expected_value);
}

template<>
void LoadInstructionIntoMemoryAndSetRegistersToInitialState(      InstructionExecutorTestFixture &fixture,
                                                            const ANDZeroPage                    &instruction_param)
{
    SetupRAMForInstructionsThatHaveAnEffectiveAddress(fixture, instruction_param);
    SetupAffectedOrUsedRegisters(fixture, instruction_param);
}

template<>
void RegistersAreInExpectedState(const Registers &registers,
                                 const AND_ZeroPage_Expectations &expectations)
{
    EXPECT_THAT(registers.a, Eq(expectations.a));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::N), Eq(expectations.flags.n_value.expected_value));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::Z), Eq(expectations.flags.z_value.expected_value));
}

template<>
void MemoryContainsInstruction(const InstructionExecutorTestFixture &fixture,
                               const Instruction<AbstractInstruction_e::AND, ZeroPage> &instruction)
{
    EXPECT_THAT(fixture.fakeMemory.at( fixture.executor.registers().program_counter ), Eq( OpcodeFor(AbstractInstruction_e::AND, AddressMode_e::ZeroPage) ));
    EXPECT_THAT(fixture.fakeMemory.at( fixture.executor.registers().program_counter + 1), Eq(instruction.address.zero_page_address));
}

template<>
void MemoryContainsExpectedComputation(const InstructionExecutorTestFixture &fixture,
                                       const ANDZeroPage                    &instruction)
{
    EXPECT_THAT(fixture.fakeMemory.at( instruction.address.zero_page_address ), Eq(instruction.requirements.initial.operand));
}


static const std::vector<ANDZeroPage> ANDZeroPageModeTestValues {
ANDZeroPage{
    // Beginning of a page
    ZeroPage().address(0x1000).zp_address(6),
    ANDZeroPage::Requirements{
        .initial = {
            .a = 0,
            .flags = { },
            .operand = 0x00 },
        .final = {
            .a = 0,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true } },
            .operand = 0x00
        }}
},
ANDZeroPage{
    // One before the end of a page
    ZeroPage().address(0x10FE).zp_address(6),
    ANDZeroPage::Requirements{
        .initial = {
            .a = 0,
            .flags = { },
            .operand = 0xFF },
        .final = {
            .a = 0,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true } },
            .operand = 0xFF
        }}
},
ANDZeroPage{
    // Crossing a page boundary
    ZeroPage().address(0x10FF).zp_address(6),
    ANDZeroPage::Requirements{
        .initial = {
            .a = 0xFF,
            .flags = { },
            .operand = 0 },
        .final = {
            .a = 0,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true } },
            .operand = 0
        }}
},
ANDZeroPage{
    // Loading a zero affects the Z flag
    ZeroPage().address(0x8000).zp_address(16),
    ANDZeroPage::Requirements{
        .initial = {
            .a = 0xFF,
            .flags = { },
            .operand = 0xFF },
        .final = {
            .a = 0xFF,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false } },
            .operand = 0xFF
        }}
},
ANDZeroPage{
    // Check for masking out the high bit
    ZeroPage().address(0x8000).zp_address(0xFF),
    ANDZeroPage::Requirements{
        .initial = {
            .a = 0xFF,
            .flags = { },
            .operand = 0x80 },
        .final = {
            .a = 0x80,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false } },
            .operand = 0x80
        }}
},
ANDZeroPage{
    // Use alternating bits for a zero result
    ZeroPage().address(0x8000).zp_address(0x0F),
    ANDZeroPage::Requirements{
        .initial = {
            .a = 0b10101010,
            .flags = { },
            .operand = 0b01010101 },
        .final = {
            .a = 0x00,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true } },
            .operand = 0b01010101
        }}
},
ANDZeroPage{
    // Use the same bits for the same result
    ZeroPage().address(0x8000).zp_address(0x0F),
    ANDZeroPage::Requirements{
        .initial = {
            .a = 0b10101010,
            .flags = { },
            .operand = 0b10101010 },
        .final = {
            .a = 0b10101010,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false } },
            .operand = 0b10101010
        }}
},
ANDZeroPage{
    // Use the same bits for the same result (not the same pattern as before)
    ZeroPage().address(0x8000).zp_address(0x0F),
    ANDZeroPage::Requirements{
        .initial = {
            .a = 0b01010101,
            .flags = { },
            .operand = 0b01010101 },
        .final = {
            .a = 0b01010101,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = false } },
            .operand = 0b01010101
        }}
}
};

TEST_P(ANDZeroPageMode, TypicalInstructionExecution)
{
    TypicalInstructionExecution(*this, GetParam());
}

INSTANTIATE_TEST_SUITE_P(AndZeroPageAtVariousAddresses,
                         ANDZeroPageMode,
                         testing::ValuesIn(ANDZeroPageModeTestValues) );
