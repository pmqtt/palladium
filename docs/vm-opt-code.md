# Palladium Virtual Machine Instruction Set

## NAME

`palladium_vm` - Instruction Set for the Palladium Virtual Machine

## DESCRIPTION

The Palladium Virtual Machine (VM) is a stack- and register-based virtual machine providing a collection of instructions for data manipulation, program flow control, and arithmetic operations.

This document describes the instructions understood by the VM, including their syntax and functionality.

## COMMAND TABLE

```
| Command            | Arguments                      | Description                                | Byte  |
|--------------------|--------------------------------|--------------------------------------------|-------|
| Load              | Register Number               | c(0) = c(i)                               | 0x0010|
| CLoad             | VMType                        | c(0) = const                              | 0x0020|
| IndLoad           | Register Number               | c(0) = c(c(i))                            | 0x0030|
| SLoad             | Register Number               | c(i) = stack.pop()                        | 0x0040|
| Store             | Register Number               | c(i) = c(0)                               | 0x0050|
| INDStore          | Register Number               | c(c(i)) = c(0)                            | 0x0060|
| Add               | Register Number               | c(0) = c(0) + c(i)                        | 0x0070|
| CAdd              | VMType                        | c(0) = c(0) + const                       | 0x0080|
| INDAdd            | Register Number               | c(0) = c(0) + c(c(i))                     | 0x0090|
| If                | cond, VMType, Target Address  | Jump if condition is met                  | 0x00A0|
| Goto              | Target Address                | Unconditional jump                        | 0x00B0|
| Halt              | None                          | Stops VM execution                        | 0x00C0|
| Push              | VMType                        | stack.push(value)                         | 0x00D0|
| Pop               | None                          | stack.pop()                               | 0x00E0|
| stack_top         | None                          | Reads top stack value                     | 0x00F0|
| Print             | Register and Field Address    | Prints top stack value                    | 0x0100|
| PrintRegStructField| None                          | Prints the field in saved VMStruct        | 0x0101|
| Call              | Function Name                 | Jumps to function, saves state            | 0x0110|
| RetVoid           | None                          | Returns from function                     | 0x0120|
| Return            | Register Number               | Returns and pushes register to stack      | 0x0120|
| StructCreate      | Register Number, Field Count  | c(i) = Struct                             | 0x0130|
| AddField          | Register Number, VMStructType | c(i) = Struct.AddField                    | 0x0131|
| SetField          | Register, Field Address, Type | c(i).Field(addr) = VMStructType           | 0x0132|
| Allocate          | Bytes Constant                | c(9) = address                            | 0x0140|
| Deallocate        | None                          | free(c(9))                                | 0x0141|
| WriteMem          | Push(VMType)                  | mem[c(9)+0]...mem[c(9)+sizeof(VMType)]    | 0x0142|
| ReadMem           | Push(size), Push(type)        | VMType as type = mem[c(9)+0]...mem[size]  | 0x0142|
```

## INSTRUCTIONS

The instructions are divided into categories: Data Manipulation, Control Flow, Arithmetic, Stack Operations, and I/O.

### 1. Data Manipulation

#### `Load i`

Loads the value from register `c(i)` into `c(0)`.

**Example:**

```assembly
Load 2
```

Copies the value from register 2 into register 0.

#### `CLoad value`

Loads a constant value into `c(0)`.

**Example:**

```assembly
CLoad 42
```

Sets `c(0)` to 42.

#### `INDLoad i`

Loads the value from the register indexed by `c(i)` into `c(0)`.

**Example:**

```assembly
INDLoad 1
```

If `c(1) = 3`, loads the value from register 3 into `c(0)`.

#### `SLoad i`

Loads the top value of the stack into `c(i)`.

**Example:**

```assembly
SLoad 1
```

Stores the top stack value into `c(1)`.

#### `Store i`

Stores the value of `c(0)` into `c(i)`.

**Example:**

```assembly
Store 2
```

Stores the value of `c(0)` into `c(2)`.

#### `INDStore i`

Stores the value of `c(0)` into the register indexed by `c(i)`.

**Example:**

```assembly
INDStore 1
```

If `c(1) = 4`, stores the value of `c(0)` into `c(4)`.

### 2. Control Flow

#### `If cond value target`

Jumps to `target` if the condition `cond` is met.

**Conditions:**

- `0`: <
- `1`: >
- `2`: ==
- `3`: !=
- `4`: <=
- `5`: >=

**Example:**

```assembly
If 0 10 5
```

Jumps to address 5 if `c(0) < 10`.

#### `Goto i`

Unconditionally jumps to address `i`.

**Example:**

```assembly
Goto 10
```

Sets the program counter to 10.

#### `Halt`

Stops the execution of the VM.

**Example:**

```assembly
Halt
```

Ends the program.

### 3. Arithmetic

#### `Add i`

Adds the value in `c(i)` to `c(0)` and stores the result in `c(0)`.

**Example:**

```assembly
Add 3
```

Adds the value of `c(3)` to `c(0)`.

#### `CAdd value`

Adds a constant value to `c(0)`.

**Example:**

```assembly
CAdd 5
```

Adds 5 to `c(0)`.

#### `INDAdd i`

Adds the value in `c(c(i))` to `c(0)`.

**Example:**

```assembly
INDAdd 2
```

Adds the value from the register indexed by `c(2)` to `c(0)`.

### 4. Stack Operations

#### `Push value`

Pushes a value onto the stack.

**Example:**

```assembly
Push 10
```

Adds 10 to the stack.

#### `Pop`

Removes the top value from the stack.

**Example:**

```assembly
Pop
```

Removes the top stack value.

#### `stack_top`

Reads the top value of the stack without removing it.

**Example:**

```assembly
stack_top
```

Returns the current top stack value.

### 5. I/O

#### `Print`

Prints the top value of the stack.

**Example:**

```assembly
Print
```

Outputs the top stack value and removes it.

### 6. Function Calls

#### `Call fname`

Jumps to the address of function `fname` and saves the current state.

**Example:**

```assembly
Call "myFunction"
```

Calls the function `myFunction`.

#### `RetVoid`

Returns to the function caller.

**Example:**

```assembly
RetVoid
```

Restores the previous function state.

## EXAMPLE PROGRAM

A program that adds two numbers and outputs the result:

```assembly
CLoad 10        # Load 10 into c(0)
Push c(0)       # Push 10 onto the stack
CLoad 20        # Load 20 into c(0)
Push c(0)       # Push 20 onto the stack
Pop             # Remove 20 from the stack
Add 0           # Add c(0) to the stack value
Print           # Output the result
Halt            # End the VM
```

## ERRORS

Unknown instructions or invalid values cause the VM to halt with an error message.

## AUTHOR

Palladium Virtual Machine Development Team
