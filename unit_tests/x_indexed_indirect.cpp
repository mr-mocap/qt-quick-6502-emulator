#include <gmock/gmock.h>
#include "instruction_checks.hpp"

using namespace testing;

struct LDA_XIndexedIndirect_Expectations
{
    uint16_t address_to_indirect_to;
    uint8_t  a;
    uint8_t  x;
    NZFlags  flags;
};

using LDAXIndexedIndirect = LDA<XIndexedIndirect, LDA_XIndexedIndirect_Expectations, 6>;

class LDAXIndexedIndirectMode : public InstructionExecutorTestFixture,
                                public WithParamInterface<LDAXIndexedIndirect>
{
public:
};

template<>
void LoadInstructionIntoMemoryAndSetRegistersToInitialState(      InstructionExecutorTestFixture &fixture,
                                                            const LDAXIndexedIndirect            &instruction_param)
{
    fixture.loadOpcodeIntoMemory(instruction_param.operation,
                                 AddressMode_e::XIndexedIndirect,
                                 instruction_param.address.instruction_address);
    fixture.fakeMemory[instruction_param.address.instruction_address + 1] = instruction_param.address.zero_page_address;
    fixture.fakeMemory[instruction_param.address.zero_page_address + instruction_param.requirements.initial.x    ] = fixture.loByteOf(instruction_param.requirements.initial.address_to_indirect_to);
    fixture.fakeMemory[instruction_param.address.zero_page_address + instruction_param.requirements.initial.x + 1] = fixture.hiByteOf(instruction_param.requirements.initial.address_to_indirect_to);
    fixture.fakeMemory[instruction_param.requirements.initial.address_to_indirect_to] = instruction_param.requirements.final.a;

    // Load appropriate registers
    fixture.r.a = instruction_param.requirements.initial.a;
    fixture.r.x = instruction_param.requirements.initial.x;
    fixture.r.SetFlag(FLAGS6502::N, instruction_param.requirements.initial.flags.n_value.expected_value);
    fixture.r.SetFlag(FLAGS6502::Z, instruction_param.requirements.initial.flags.z_value.expected_value);
}

template<>
void RegistersAreInExpectedState(const Registers &registers,
                                 const LDA_XIndexedIndirect_Expectations &expectations)
{
    EXPECT_THAT(registers.a, Eq(expectations.a));
    EXPECT_THAT(registers.x, Eq(expectations.x));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::N), Eq(expectations.flags.n_value.expected_value));
    EXPECT_THAT(registers.GetFlag(FLAGS6502::Z), Eq(expectations.flags.z_value.expected_value));
}

template<>
void MemoryContainsInstruction(const InstructionExecutorTestFixture &fixture,
                               const Instruction<AbstractInstruction_e::LDA, XIndexedIndirect> &instruction)
{
    EXPECT_THAT(fixture.fakeMemory.at( fixture.executor.registers().program_counter ),     Eq( OpcodeFor(AbstractInstruction_e::LDA, AddressMode_e::XIndexedIndirect) ));
    EXPECT_THAT(fixture.fakeMemory.at( fixture.executor.registers().program_counter + 1 ), Eq(instruction.address.zero_page_address));
}

template<>
void MemoryContainsExpectedComputation(const InstructionExecutorTestFixture &fixture,
                                       const LDAXIndexedIndirect            &instruction)
{
    const auto    address_stored_in_zero_page    = instruction.requirements.initial.address_to_indirect_to;
    const uint8_t zero_page_address_to_load_from = instruction.address.zero_page_address;
    const uint8_t value_to_load = instruction.requirements.final.a;
    const uint8_t x_register    = instruction.requirements.initial.x;

    EXPECT_THAT(fixture.fakeMemory.at( zero_page_address_to_load_from + x_register),     Eq( fixture.loByteOf(address_stored_in_zero_page) ));
    EXPECT_THAT(fixture.fakeMemory.at( zero_page_address_to_load_from + x_register + 1), Eq( fixture.hiByteOf(address_stored_in_zero_page) ));
    EXPECT_THAT(fixture.fakeMemory.at( address_stored_in_zero_page ), Eq(value_to_load));
}


static const std::vector<LDAXIndexedIndirect> LDAXIndexedIndirectModeTestValues {
LDAXIndexedIndirect{
    // Beginning of a page
    XIndexedIndirect().address(0x8000).zp_address(0xA0),
    LDAXIndexedIndirect::Requirements{
        .initial = {
            .address_to_indirect_to = 0xC000,
            .a = 0,
            .x = 12,
            .flags = { }},
        .final = {
            .address_to_indirect_to = 0xC000,
            .a = 6,
            .x = 12,
            .flags = { }
        }}
}
};

TEST_P(LDAXIndexedIndirectMode, TypicalInstructionExecution)
{
    TypicalInstructionExecution(*this, GetParam());
}

INSTANTIATE_TEST_SUITE_P(LoadXIndexedIndirectAtVariousAddresses,
                         LDAXIndexedIndirectMode,
                         testing::ValuesIn(LDAXIndexedIndirectModeTestValues) );
