@echo off
REM ============================================================================
REM Single-cycle CPU Compile and Simulation Script (Windows)
REM ============================================================================

echo ========================================
echo Compiling Verilog code...
echo ========================================

REM Use iverilog to compile all Verilog files
iverilog -o cpu_sim.vvp ^
    pc_register.v ^
    instruction_memory.v ^
    control_unit.v ^
    register_file.v ^
    alu_control.v ^
    alu.v ^
    data_memory.v ^
    cpu_top.v ^
    testbench.v

if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b 1
)

echo ========================================
echo Compilation succeeded! Starting simulation...
echo ========================================

REM Run simulation
vvp cpu_sim.vvp

if %errorlevel% neq 0 (
    echo Simulation failed!
    pause
    exit /b 1
)

echo ========================================
echo Simulation complete! Waveform file generated: cpu_test.vcd
echo ========================================
echo To view the waveform, use:
echo gtkwave cpu_test.vcd
echo ========================================

pause
