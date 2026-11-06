# Expressions

Expressions are a sequence of *Stack Operations* that end when all of the Operands are popped off the Expression Stack. Each Operand may have its own instruction that pops one Operand off the stack or pops all the Operands off performing a Closure Operation.

Everything in Script is a real-time **A** = **A** * **B** **Abstract Algebra Expression** where B is a B-Stream, or Byte Stream. Expression are a sequence of **Operations** performed on **Operand Objects** on a **Operand Stack**. *Operations* are *abstract function calls* that shall be able to take one or more input and output parameters in the form of **B-Sequences** (**BSQ** or *Byte Sequence*). A BSQ only the description of the data format packed that may be word aligned, thus a BSQ is describes the parameters not the arguments and a B-Stream describes the arguments. *Operations* shall be performed using a UTF-8 Index followed by the BSQ input arguments for that Op. When an *Operand* is pushed onto the stack, all Unicode indexes shall be mapped to that Operand until it is popped off the stack and is no longer the **Current Operand**.

Once a valid Expression has been received, a room may then **Evaluate** that expression from one of potentially many asynchronous BIn in the program's main loop in any order.

## Hyphenated Script Expression Example

```Crabs
/* typedef IUA UIT;
   static const UIT in_params[]  = { 1, kUI4, kUI8 },
                       out_params[] = { 1, NILL, kNIL };
    static const Op kOp= { “Example-Function_with_hypens_and_underscores”,
                          rxHeader, txHeader,
                          “Description”, '(', ')', ' ', "-_", _NilP };
                                          ^    ^    ^    ^
                                          |    |    |    |
                Pop One Operation --------+    |    |    |
                Pop All Operation -------------+    |    |
                Default Operation ------------------+    |
                Chars '-' and '_'                        |
                allowed in Tokens -----------------------+    */

Example-Function_with_hypens_and_underscores {
    1 2     // Index 64 is the first user-definable function index.
    A 1 2;  // Index 64 is the first user-definable function index.
}
```

## Operations

**Script Operations** are **Abstract Stack Machine Functions** implemented as either *Local Procedure Calls*  or *Remote Procedure Calls* that are indexed by a single *UTF-8 Character*. *Script Operations* may take a *Packed B-Stream* or *Word-Aligned B-Stream* of parameter arguments.

### Operation Header

```C
struct Op {
    const char   * name;          //< Op name.
    const UIT * in,               //< Input BSQ params or OpFirst.
                 * out;           //< Output BSQ params or OpLast.
    const char   * description;   //< Op description.
    wchar_t        pop,           //< Index of the Pop Operation.
                   close,         //< Index of the Close Operation.
                   default_op;    //< Index of the Default Operation.
    const char   * allowed_chars; //< String of allowed symbols in keys.
    const Slot   * evaluation;    //< Evaluated expression Slot.
};
```

### C++ Header Examples

```C++
static const UIT rx_header_without_strings[] = { 2, kUI1, kSI2 },   
                    rx_header_with_strings[]    = { 2, kUI1, kSTR, 32 },
                    //< We need to specify the max string length.
                    rx_header_with_array[]      = { 2, kUI1, Array (kUI1, 32) },
                    //< We need to specify the exact number of bytes.
                    tx_header_with_strings[]    = { 2, kUI1, kSTR };
                    //< We don’t specify the max length for tx output.
```

### Operation C++ Example

```C++
#include <crabs/global.h>
using namespace _;

const Op* Star (wchar_t index, Expression* crabs)  {
    void* params[2];

    if (index != 'A') return 0;  //< switch statements are normally use used.

    static const unsigned int params[] = { 2, kADR, kUI4, kSTR, 32 },
                              result[] = { 2, kUI4, kSTR };
    static const Operation OpExample= { “expression-name”,
                                         params, result,
                                         “Description”, 0 };
    // Script uses the a nil crabs pointer as a flag to get the Operation header.
    if (IsError(crabs)) return &OpExample;

    // Example RPC variables.
    IUC input_a,
        input_b,
        output_a = 1,
        output_b = 2;

    if (Read (crabs, OpExample, Args (params, &input_a, &input_b)))
         return crabs->result;

    // Operation logic here.

    return Write (crabs, OpExample, Args (params, &output_a, &output_b));
}
```

### Operation Call Format

#### UTF-8 Format

| Byte 1   | Byte 2   | Byte 3   | Byte 4   | UTF-32 Result         |
|:--------:|:--------:|:--------:|:--------:|:---------------------:|
| 0aaaaaaa |          |          |          | 00000000000000aaaaaaa |
| 110aaaaa | 10bbbbbb |          |          | 0000000000aaaaabbbbbb |
| 1110aaaa | 10bbbbbb | 10cccccc |          | 00000aaaabbbbbbcccccc |
| 11110aaa | 10bbbbbb | 10cccccc | 10dddddd | aaabbbbbbccccccdddddd |

#### UTF-16 Format

| Bytes {4N, 4N+ 1} | Bytes {4N + 2, 4N+ 3} | UTF-32 Result        |
|:-----------------:|:---------------------:|:--------------------:|
| 000000aaaaaaaaaa  |                       | 0000000000aaaaaaaaaa |
| 110110aaaaaaaaaa  | 110111bbbbbbbbbb      | aaaaaaaaaabbbbbbbbbb |

### Crabs Operations

#### Default Operation

The Default Operation shall be set by each Operand and when the Interpreter or Compiler is resolving keys and sees a number, that Operand's Default Operation shall be called.

#### Cache Search Operation

A *Cache Search Operation* is an operation that searches for a locally cached version of an object before consulting the ICAN3 DNS authority.

#### Clear Expression Operation

Clears Expression Operation clears the current Expression Slots without deleting it.

#### Open Expression Operation

*ASCII SI Operation* is used to open a *Expression Stack Level*. The operation may be hooked using the *ASCII '(' Operation* before calling SI.

#### Close Expression Operation

**ASCII SO Operation** is used to open a *Expression Stack Level*. The operation may be hooked using the *ASCII '(' Operation* before calling SI.

#### Importance Operation

**Importance Operation** performs  Raises the *Importance Level* for each consecutive ASCII '!' IUA.

### Division Operation

*Division Operation* by default performs a mathematical division of the current object and *B-Sequence Operand*. The operation may be overwritten in non-root context.

#### Multiply Operation

*Multiply Operation* by default performs a mathematical multiplication of the current object and *B-Sequence Operand*. The operation may be overwritten in non-root context.

#### Plus Operation

*Plus Operation* by default performs a mathematical addition of the current object and *B-Sequence Operand*. The operation may be overwritten in non-root context.

#### Subtraction Operation

*Subtraction Operation* by default performs a mathematical subtraction of the current object and *B-Sequence Operand*. The operation may be overwritten in non-root context.

#### Closure Operation

Signals the current automaton to finish processing a word.

#### Separation Operator

*Separation Operator* does nothing by default but may be overridden to allow for faster parsing of *Separation of Ideas*.

#### Exponent Operation

**Exponent Operations** (**Exp Op** or **Exp Ops**) work similarly to math exponents only with A*B operations. **Exp Ops** work by specifying the number of loops **Varint**. If the value of the varint is **NaN** then it is a **Infinite Exponential Operations** (**IEO**). **IEOs** are functionally identical to while loops. **Loops** are exited prematurely using the **Clear Operation**.

#### Delete Operation

*Delete Operation* deletes the previous char in the ring buffer.

#### Whitespace Operations

Script is whitespace delimited spaces may be skipped by the interpreter or interpreted as a Operating with index 64 (ASCII ' '). For real-time streaming natural language processing Whitespace Operations are used to identify pauses in speech (i.e. dead audio).

### Interrupt Escape Sequence

Each Room has a single **Interrupt Expression** in the index '/'. When an ESC interrupt IUA is received the *Interrupt Expression* is pushed onto the stack. If this ESC returns a Result this Result must be inserted into the location of the ESC in the B-Stream.

#### Mandatory Single Byte Interrupt Operations

| ASCII |    Formal Name      | Description |
|:-----:|:-------------------:|:-----------:|
|  EOT  | End of transmission | Disconnects from the host. |
|  CAN  | Cancel              | Cancels the current Expression being scanned. |

### Device Controls 1-4

Device Controls 1 through 4 are the ASCII DC1, DC2, DC3, and DC4 Operations. These Operations may be directly mapped the VK_SHIFT, VK_CONTROL, VK_ALT, and VK_PAUSE buttons on the keyboard. DC1, DC2, and DC3 may be used with momentary switch where that Device Control Operation is called when the button is pressed and depressed. DC4 is may be used with a momentary switch where the Device Control Operation is called when the button is pressed or depressed.

#### DC1 - XON -VK_SHIFT

DC4 must be an <kNIL>:<kNIL> Operand that does nothing by default and uses ASCII DC4 for it's Closure Operation. DC4 may be used for Shift Keyboard Shortcuts the system and may be overridden by the user for application specific purposes.

#### DC2 - VK_CONTROL

DC4 must be an <kNIL>:<kNIL> Operand that does nothing by default and uses ASCII DC4 for it's Closure Operation. DC4 may be used for Control Keyboard Shortcuts the system and may be overridden by the user for application specific purposes.

#### DC3 - XOFF - VK_ALT

DC4 must be an <kNIL>:<kNIL> Operand that does nothing by default and uses ASCII DC4 for it's Closure Operation. DC4 may be used for Alt Keyboard Shortcuts the system and may be overridden by the user for application specific purposes.

DC3 must be a device that does nothing by default and uses DC3 for a Closure Operation. XOFF may be overridden by the user for application specific purposes.

#### DC4 - App - VK_PAUSE

DC4 must be an <kNIL>:<kNIL> Operand that does nothing by default and uses ASCII DC4 for it's Closure Operation. DC4 may be used to pause the system and may be overridden by the user for application specific purposes.

### Reset Operations

Reset Operations are operations that required the slot in the Chinese Door to be closed. This is typically something like remote firmware updating. Each implementation is responsible for initializing the system and proving The Chinese Room Group Automata Theorem.

### Return Values

Return values just need an address of an Operation to send the return value too. Return values shall be written to the BOut that leads to the initiating Operation's BIn in an automated process. In the case that a different Operation's return address is needed Rooms may pass the return address of the as an input parameter as a kTKN type.
