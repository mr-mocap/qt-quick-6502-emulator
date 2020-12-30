#include "addressing_mode_helpers.hpp"



struct JMP_Indirect_Expectations
{
    uint16_t address_to_indirect_to;
    uint16_t program_counter;
};

using JMPIndirect     = JMP<Indirect, JMP_Indirect_Expectations, 5>;
using JMPIndirectMode = ParameterizedInstructionExecutorTestFixture<JMPIndirect>;


static void StoreTestValueAtEffectiveAddress(InstructionExecutorTestFixture &fixture, const JMPIndirect &instruction_param)
{
    fixture.fakeMemory[instruction_param.requirements.initial.address_to_indirect_to    ] = fixture.loByteOf(instruction_param.requirements.initial.address_to_indirect_to);
    fixture.fakeMemory[instruction_param.requirements.initial.address_to_indirect_to + 1] = fixture.hiByteOf(instruction_param.requirements.initial.address_to_indirect_to);
}

static void SetupAffectedOrUsedRegisters(InstructionExecutorTestFixture &fixture, const JMPIndirect &instruction_param)
{
    // NOTE: Intentionally left empty!
    fixture.r.program_counter = instruction_param.requirements.initial.program_counter;
}

template<>
bool ProgramCounterIsSetToCorrectValue(const InstructionExecutor &executor,
                                       const JMPIndirect         &instruction)
{
    // One past the instruction, for typical instructions.
    return executor.registers().program_counter == instruction.requirements.final.program_counter;
}

template<>
void LoadInstructionIntoMemoryAndSetRegistersToInitialState(      InstructionExecutorTestFixture &fixture,
                                                            const JMPIndirect            &instruction_param)
{
    SetupRAMForInstructionsThatHaveAnIndirectedEffectiveAddress(fixture, instruction_param);
    SetupAffectedOrUsedRegisters(fixture, instruction_param);
}

template<>
void RegistersAreInExpectedState(const Registers &registers,
                                 const JMP_Indirect_Expectations &expectations)
{
    EXPECT_THAT(registers.program_counter, Eq(expectations.program_counter));
}

template<>
void MemoryContainsInstruction(const InstructionExecutorTestFixture &fixture,
                               const Instruction<AbstractInstruction_e::JMP, Indirect> &instruction)
{
    EXPECT_THAT(fixture.fakeMemory.at( fixture.executor.registers().program_counter ),     Eq( OpcodeFor(AbstractInstruction_e::JMP, AddressMode_e::Indirect) ));
    EXPECT_THAT(fixture.fakeMemory.at( fixture.executor.registers().program_counter + 1 ), Eq( fixture.loByteOf(instruction.address.indirect_address) ) );
    EXPECT_THAT(fixture.fakeMemory.at( fixture.executor.registers().program_counter + 2 ), Eq( fixture.hiByteOf(instruction.address.indirect_address) ) );
}

template<>
void MemoryContainsExpectedComputation(const InstructionExecutorTestFixture &/* fixture */,
                                       const JMPIndirect            &/* instruction */)
{
    // NOTE: Intentionally left empty!
}

template<>
void InstructionExecutedInExpectedClockTicks(const InstructionExecutorTestFixture &fixture,
                                             const JMPIndirect            &instruction)
{
    EXPECT_THAT(fixture.executor.clock_ticks, Eq(instruction.requirements.cycle_count));
}


static const std::vector<JMPIndirect> JMPIndirectModeTestValues {
JMPIndirect{
    // Beginning of a page
    Indirect().address(0x1000).indirect_addr(0x00A0),
    JMPIndirect::Requirements{
        .initial = {
            .address_to_indirect_to = 0xA000,
            .program_counter = 0x1000 },
        .final = {
            .address_to_indirect_to = 0xA000,
            .program_counter = 0xA000
        }}
},
JMPIndirect{
    // Middle of a page
    Indirect().address(0x1088).indirect_addr(0x00FE),
    JMPIndirect::Requirements{
        .initial = {
            .address_to_indirect_to = 0xA000,
            .program_counter = 0x1088 },
        .final = {
            .address_to_indirect_to = 0xA000,
            .program_counter = 0xA000
        }}
},
#if 0
JMPIndirect{
    // Middle of a page
    // Expose an oddity.  The address is only read from a single page.
    // Thus, the data is at the end with the other half wrapped-around
    // to the beginning.
    Indirect().address(0x1088).indirect_addr(0x00FF),
    JMPIndirect::Requirements{
        .initial = {
            .address_to_indirect_to = 0xA000,
            .program_counter = 0x1088 },
        .final = {
            .address_to_indirect_to = 0xA000,
            .program_counter = 0xA000
        }}
},
#endif
JMPIndirect{
    // End of a page
    Indirect().address(0x10FD).indirect_addr(0xB000),
    JMPIndirect::Requirements{
        .initial = {
            .address_to_indirect_to = 0xA000,
            .program_counter = 0x10FD },
        .final = {
            .address_to_indirect_to = 0xA000,
            .program_counter = 0xA000
        }}
},
JMPIndirect{
    // Crossing a page boundary
    Indirect().address(0x10FE).indirect_addr(0xC07F),
    JMPIndirect::Requirements{
        .initial = {
            .address_to_indirect_to = 0xA000,
            .program_counter = 0x10FE },
        .final = {
            .address_to_indirect_to = 0xA000,
            .program_counter = 0xA000
        }}
},
JMPIndirect{
    // Crossing a page boundary
    Indirect().address(0x10FE).indirect_addr(0xAAAA),
    JMPIndirect::Requirements{
        .initial = {
            .address_to_indirect_to = 0xF000,
            .program_counter = 0x10FE },
        .final = {
            .address_to_indirect_to = 0xF000,
            .program_counter = 0xF000
        }}
}
};

TEST_P(JMPIndirectMode, TypicalInstructionExecution)
{
    TypicalInstructionExecution(*this, GetParam());
}

INSTANTIATE_TEST_SUITE_P(JumpIndirectAtVariousAddresses,
                         JMPIndirectMode,
                         testing::ValuesIn(JMPIndirectModeTestValues) );
