#include "addressing_mode_helpers.hpp"



struct TXS_Implied_Expectations
{
    uint8_t s;
    uint8_t x;
};

using TXSImplied     = TXS<Implied, TXS_Implied_Expectations, 2>;
using TXSImpliedMode = ParameterizedInstructionExecutorTestFixture<TXSImplied>;


static void SetupAffectedOrUsedRegisters(InstructionExecutorTestFixture &fixture, const TXSImplied &instruction_param)
{
    fixture.r.stack_pointer = instruction_param.requirements.initial.s;
    fixture.r.x = instruction_param.requirements.initial.x;
}

template<>
void LoadInstructionIntoMemoryAndSetRegistersToInitialState(      InstructionExecutorTestFixture &fixture,
                                                            const TXSImplied                    &instruction_param)
{
    SetupRAMForInstructionsThatHaveImpliedValue(fixture, instruction_param);
    SetupAffectedOrUsedRegisters(fixture, instruction_param);
}

template<>
void RegistersAreInExpectedState(const Registers &registers,
                                 const TXS_Implied_Expectations &expectations)
{
    EXPECT_THAT(registers.stack_pointer, Eq(expectations.s));
    EXPECT_THAT(registers.x, Eq(expectations.x));
}

template<>
void MemoryContainsInstruction(const InstructionExecutorTestFixture &fixture,
                               const Instruction<AbstractInstruction_e::TXS, Implied> &)
{
    EXPECT_THAT(fixture.fakeMemory.at( fixture.executor.registers().program_counter ), Eq( OpcodeFor(AbstractInstruction_e::TXS, AddressMode_e::Implied) ));
}

template<>
void MemoryContainsExpectedComputation(const InstructionExecutorTestFixture &,
                                       const TXSImplied                    &)
{
    // No memory affected
}


static const std::vector<TXSImplied> TXSImpliedModeTestValues {
TXSImplied{
    Implied().address(0x1000),
    TXSImplied::Requirements{
        .initial = {
            .s = 0xFF,
            .x = 0
        },
        .final = {
            .s = 0,
            .x = 0
        }}
},
TXSImplied{
    Implied().address(0x1000),
    TXSImplied::Requirements{
        .initial = {
            .s = 0x00,
            .x = 0xFF
        },
        .final = {
            .s = 0xFF,
            .x = 0xFF
        }}
}
};

TEST_P(TXSImpliedMode, TypicalInstructionExecution)
{
    TypicalInstructionExecution(*this, GetParam());
}

INSTANTIATE_TEST_SUITE_P(TXSImpliedAtVariousAddresses,
                         TXSImpliedMode,
                         testing::ValuesIn(TXSImpliedModeTestValues) );
