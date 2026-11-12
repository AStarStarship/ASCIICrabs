# Slots

A Socket is defined a block of memory used to transceived data, and is typically implemented as ring buffer. There are two types of slots, a B-Input (BIn) and B-Output (BOut). A **Slot** is defined as a Ring Buffer Socket. BIn and BOut implementations written in a Hardware Development Language may not strictly comply with the contiguous memory operation but shall operate functionally equivalent to the specified register stack machine implementation. For register stack machines with contiguous memory. Each Slot shall begin a 64-bit aligned memory address and shall have a 64-bit aligned end address.

## Slot Data Structure in C

```C++
struct Slot {
    char* begin, //< Beginning of the ring buffer.
        * start, //< Start of the data in the ring buffer.
        * stop,  //< Stop of the data in the ring buffer.
        * end;   //< End of the ring buffer.
};
```

## B-Input Slot

A B-Input, BIn, is a type of Slot for incoming Messages that uses offsets from the beginning of the data structure in memory.

### BIn C Data Structure

```C++
typedef unsigned int IUT;
struct BIn {
    IUT          size,   //< The size of the buffer.
                    start;  //< The starting index of the ring buffer data.
    volatile IUT stop;   //< The stopping index of the ring buffer data.
    IUT          read;   //< The read variable.
};
```

### B-Input Slot Memory Layout

```None
+--------------------+
|     Ring Buffer    |
|------------------- |  +
| BIn Data Structure |  |
+--------------------+ 0xN
```

### B-Input Compliant Finite State Machine Example

Finite State Machine uses a stack of state indexes for all states that have bidirectional arrows. Once the Crabs Message has been interpreted and the hash is verified, the data has thought to be in the correct format but has not been verified to be bound into determinant ranges (@see Bounding Escape Sequences).

![B-Input Interpreter FSM](https://github.com/AStarStarship/KabukiToolkit/wiki/expression-scanner-fsm.jpg)

## B-Output Slot

A B-Output Slot, BOut, almost works similarly to the BIn except the BOut reserves N bytes at the end of the ring buffer, where N equals 0, 8, 16, or 24, for writing integers and floating-point numbers to string. If an integer type is written to the buffer and the string overflows the buffer, the implementation may copy the overflowed string to the beginning of the slot. The number N shall be chosen such that the desired number of bytes that can be overflowed without overflowing the buffer. When a number is written to a BOut in RAM, the BOut shall be required to contain enough free space to write the maximum length of the number string.

### BOut C Data Structure

```C++
struct API BOut {
    IUT          size;  //< Size of the B-Output.
    volatile IUT start; //< Starting index of the ring-buffer data.
    IUC          stop,  //< Stopping index of the ring-buffer data.
                 read;  //< Address that the BOut device is reading from.
};
```

### B-Output Memory Layout

```AsciiArt
+------------------------------------+
| Slot number string overflow buffer |
|------------------------------------|
|            Ring Buffer             |
|------------------------------------|
|        BOut Data Structure         |
+------------------------------------+
```

## Windows

Windows combine a BIn and BOut in contiguous memory. The ring buffers function similar to BIn and BOut except that the Slot.stop offset in the B-Input Window gets swapped with duplex B-Output Window. This is because one is a read-only and one is a write only that may occur in parallel as opposed to a strictly serial Slot operation where the room may both read and write to the same Slot.

### Window Memory Layout

```AsciiArt
+------+
| BOut |
|------|   ^
| BIn  |   |
+------+  0xN
```

## Default In Slot and Out Slot

Script implementations may implement a default BIn Slot named In (**in UpperCaseCamel**)
 and default BOut Slot named Out (**in UpperCaseCamel**). In shall stream bytes from a Text keyboard, BOut Slot or Text display input. Out stream bytes to a Abstract text display, BIn Slot, or Abstract serial output.
