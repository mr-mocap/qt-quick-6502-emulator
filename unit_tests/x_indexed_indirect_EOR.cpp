#include "addressing_mode_helpers.hpp"



struct EOR_XIndexedIndirect_Expectations
{
    uint16_t address_to_indirect_to;
    uint8_t  a;
    uint8_t  x;
    NZFlags  flags;

    uint8_t  operand;
};

using EORXIndexedIndirect     = EOR<XIndexedIndirect, EOR_XIndexedIndirect_Expectations, 6>;
using EORXIndexedIndirectMode = ParameterizedInstructionExecutorTestFixture<EORXIndexedIndirect>;


void StoreTestValueAtEffectiveAddress(InstructionExecutorTestFixture &fixture, const EORXIndexedIndirect &instruction_param)
{
    fixture.fakeMemory[instruction_param.requirements.initial.address_to_indirect_to] = instruction_param.requirements.initial.operand;
}

static void SetupAffectedOrUsedRegisters(InstructionExecutorTestFixture &fixture, const EORXIndexedIndirect &instruction_param)
{
    fixture.r.a = instruction_param.requirements.initial.a;
    fixture.r.x = instruction_param.requirements.initial.x;
    fixture.r.SetFlag(FLAGS6502::N, instruction_param.requirements.initial.flags.n_value.expected_value);
    fixture.r.SetFlag(FLAGS6502::Z, instruction_param.requirements.initial.flags.z_value.expected_value);
}

template<>
void LoadInstructionIntoMemoryAndSetRegistersToInitialState(      InstructionExecutorTestFixture &fixture,
                                                            const EORXIndexedIndirect            &instruction_param)
{
    SetupRAMForInstructionsThatHaveAnIndirectedEffectiveAddressWithNoCarry(fixture, instruction_param);
    SetupAffectedOrUsedRegisters(fixture, instruction_param);
}

template<>
void RegistersAreInExpectedState(const Registers &registers,
                                 const EOR_XIndexedIndirect_Expectations &expectations)
{
    EXPECT_THAT(registers.a, Eq(expectations.a));
    EXPECT_THAT(registers.x, Eq(expectations.x));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::N), Eq(expectations.flags.n_value.expected_value));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::Z), Eq(expectations.flags.z_value.expected_value));
}

template<>
void MemoryContainsInstruction(const InstructionExecutorTestFixture &fixture,
                               const Instruction<AbstractInstruction_e::EOR, XIndexedIndirect> &instruction)
{
    EXPECT_THAT(fixture.fakeMemory.at( fixture.executor.registers().program_counter ),     Eq( OpcodeFor(AbstractInstruction_e::EOR, AddressMode_e::XIndexedIndirect) ));
    EXPECT_THAT(fixture.fakeMemory.at( fixture.executor.registers().program_counter + 1 ), Eq(instruction.address.indirect_address));
}

template<>
void MemoryContainsExpectedComputation(const InstructionExecutorTestFixture &fixture,
                                       const EORXIndexedIndirect            &instruction)
{
    const auto    address_stored_in_zero_page    = instruction.requirements.initial.address_to_indirect_to;
    const uint8_t x_register    = instruction.requirements.initial.x;
    const uint8_t zero_page_address_to_load_from = fixture.calculateZeroPageIndexedAddress(instruction.address.indirect_address, x_register);
    const uint8_t value         = instruction.requirements.initial.operand;

    EXPECT_THAT(fixture.fakeMemory.at( zero_page_address_to_load_from     ), Eq( fixture.loByteOf(address_stored_in_zero_page) ));
    EXPECT_THAT(fixture.fakeMemory.at( zero_page_address_to_load_from + 1 ), Eq( fixture.hiByteOf(address_stored_in_zero_page) ));
    EXPECT_THAT(fixture.fakeMemory.at( address_stored_in_zero_page ), Eq(value));
}


static const std::vector<EORXIndexedIndirect> EORXIndexedIndirectModeTestValues {
EORXIndexedIndirect{
    // Beginning of a page
    XIndexedIndirect().address(0x8000).zp_address(0xA0),
    EORXIndexedIndirect::Requirements{
        .initial = {
            .address_to_indirect_to = 0xA000,
            .a = 0,
            .x = 0,
            .flags = { },
            .operand = 0x00 },
        .final = {
            .address_to_indirect_to = 0xA000,
            .a = 0,
            .x = 0,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true } },
            .operand = 0x00
        }}
},
EORXIndexedIndirect{
    // Middle of a page
    XIndexedIndirect().address(0x8080).zp_address(0xA0),
    EORXIndexedIndirect::Requirements{
        .initial = {
            .address_to_indirect_to = 0xA000,
            .a = 0,
            .x = 0,
            .flags = { },
            .operand = 0x00 },
        .final = {
            .address_to_indirect_to = 0xA000,
            .a = 0,
            .x = 0,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true } },
            .operand = 0x00
        }}
},
EORXIndexedIndirect{
    // End of a page
    XIndexedIndirect().address(0x80FE).zp_address(0xA0),
    EORXIndexedIndirect::Requirements{
        .initial = {
            .address_to_indirect_to = 0xA000,
            .a = 0xFF,
            .x = 0x0F,
            .flags = { },
            .operand = 0x00 },
        .final = {
            .address_to_indirect_to = 0xA000,
            .a = 0xFF,
            .x = 0x0F,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false } },
            .operand = 0x00
        }}
},
EORXIndexedIndirect{
    // Crossing a page
    XIndexedIndirect().address(0x80FF).zp_address(0xA0),
    EORXIndexedIndirect::Requirements{
        .initial = {
            .address_to_indirect_to = 0xA000,
            .a = 0xFF,
            .x = 0x0F,
            .flags = { },
            .operand = 0x00 },
        .final = {
            .address_to_indirect_to = 0xA000,
            .a = 0xFF,
            .x = 0x0F,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false } },
            .operand = 0x00
        }}
},
EORXIndexedIndirect{
    // Test individual bits are EOR'd.
    XIndexedIndirect().address(0x8000).zp_address(0xA0),
    EORXIndexedIndirect::Requirements{
        .initial = {
            .address_to_indirect_to = 0xA000,
            .a = 0b10101010,
            .x = 0x02,
            .flags = { },
            .operand = 0b10101010 },
        .final = {
            .address_to_indirect_to = 0xA000,
            .a = 0b00000000,
            .x = 0x02,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true } },
            .operand = 0b10101010
        }}
},
EORXIndexedIndirect{
    // Test individual bits are EOR'd.
    XIndexedIndirect().address(0x8000).zp_address(0xA0),
    EORXIndexedIndirect::Requirements{
        .initial = {
            .address_to_indirect_to = 0xA000,
            .a = 0b01010101,
            .x = 0xFF,
            .flags = { },
            .operand = 0b01010101 },
        .final = {
            .address_to_indirect_to = 0xA000,
            .a = 0b00000000,
            .x = 0xFF,
            .flags = {
                .n_value = { .expected_value = false },
                .z_value = { .expected_value = true } },
            .operand = 0b01010101
        }}
},
EORXIndexedIndirect{
    // Test individual bits are EOR'd.
    XIndexedIndirect().address(0x8000).zp_address(0xA0),
    EORXIndexedIndirect::Requirements{
        .initial = {
            .address_to_indirect_to = 0xA000,
            .a = 0b10101010,
            .x = 0x80,
            .flags = { },
            .operand = 0b01010101 },
        .final = {
            .address_to_indirect_to = 0xA000,
            .a = 0b11111111,
            .x = 0x80,
            .flags = {
                .n_value = { .expected_value = true },
                .z_value = { .expected_value = false } },
            .operand = 0b01010101
        }}
}
};

TEST_P(EORXIndexedIndirectMode, TypicalInstructionExecution)
{
    TypicalInstructionExecution(*this, GetParam());
}

INSTANTIATE_TEST_SUITE_P(ExclusiveOrXIndexedIndirectAtVariousAddresses,
                         EORXIndexedIndirectMode,
                         testing::ValuesIn(EORXIndexedIndirectModeTestValues) );
