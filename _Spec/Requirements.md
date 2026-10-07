# Requirements

SCRIPT devices shall be required to:

## 1. Overview Requirements

1. **Reduce RAM**: Minimize memory usage through efficient data structures and memory management techniques.
2. **Reduce ROM**: Optimize code size and data storage to fit within limited flash memory.
3. **Reduce Complexity**: Simplify the architecture and implementation to improve maintainability and reliability.
4. **Be Portable**: Support multiple platforms and architectures without significant code changes.
5. **Maximize Code Reuse**: Implement modular design patterns and reusable components.
6. **Minimize Code Surface Area**: Reduce the amount of code that needs to be executed for common operations.
7. **Have International Language Support**: Support Unicode and multiple character encodings (UTF-8, UTF-16, UTF-32).
8. **Operate in a Safe Manner**: Implement proper error handling, bounds checking, and memory safety mechanisms.
9. **Be Useful in Multiple Contexts**: Support various use cases including embedded systems, IoT devices, and educational environments.
10. **Decode, Encode, Load, Store, and Transceive Data Quickly**: Optimize data processing pipelines for high-performance operation.
11. **Support Real-time Operation**: Provide deterministic timing for critical operations.
12. **Enable Interoperability**: Support standard protocols and interfaces for integration with other systems.
13. **Provide Extensibility**: Allow for future enhancements and feature additions without breaking compatibility.
14. **Ensure Reliability**: Implement robust error recovery and fault tolerance mechanisms.
15. **Optimize Power Consumption**: Support low-power operation for battery-powered devices.

## 2. ASCII Data Specification Requirements

16. Use 3-letter all-caps ASCII C0 code mimicry abbreviations for data types (e.g., IUA, ISA, CHA, FPB).
17. Support Plain Old Data (POD) types with bit patterns supporting 8, 16, 32, 64, and 128-bit widths.
18. Support Vector Types: VHT (Vector of Homotuples), ARY (Array), SCK (Stack), MAT (Matrix).
19. Support variable byte length types from 1 to 2048 bytes using Variable Byte Length (VBL) encoding.
20. Support Plain Context Types (PCa through PCl) ordered by descending width.
21. Support Extended Mappable Types in range 0-511 and Extended Block Types.
22. Support Unicode strings using ASCII Strings (without character count tracking) or ASCII Ropes (String Rope with character count).
23. Support B-Sequences for describing byte stream layouts with pre-specified types at runtime.
24. Support Clock formats: TMC (32-bit Unix timestamp), TMS (64-bit signed seconds), TMT (subsecond), SSD (64-bit Subsecond Id), SSE (128-bit Subsecond Id UUIDs).
25. Ensure all ASCII Data Types are memory aligned on 32-bit systems.

## 3. Addressing Requirements

26. Use 96-bit IP Address format with longitude, latitude, and height above/below sea level.
27. Implement Most Longitudinal bit (MLb) metric unit for precise location encoding.
28. Support MSb Variant Encoding (Varint) for coordinate compression in Universal Address Format.
29. Support polar coordinates for Star Configuration routing in 2D polar subnets.
30. Reserve address ranges with longitude >±180° or latitude >±90° for authoritative endpoints.
31. Support off-planet addressing with height values of -32 and height_varint_bit.
32. Support 64800 possible directions for off-planet signals (360° longitude × 180° latitude).
33. Ensure minimum varint byte requirements for Interstellar Internet Routes.

## 4. Protocol Requirements

34. Implement Group Automata theorem with total, associative, identity, and inverse properties (non-commutative group structure).
35. Support Expressions as sequences of Stack Operations ending with empty Operand Stack.
36. Support Script Operations indexed by single UTF-8 Characters with kBSQ parameter descriptions.
37. Support BIn and BOut Slots implemented as ring buffers with 64-bit memory alignment.
38. Support interrupt escape sequences (ESC) for Interrupt Expressions at index '/'.
39. Implement mandatory single byte interrupt operations: EOT (End of Transmission) and CAN (Cancel).
40. Support Device Controls DC1-DC4 mapped to VK_SHIFT, VK_CONTROL, VK_ALT, VK_PAUSE.
41. Support Closure Operations, Separation Operators, and Default Operations for key resolution.
42. Support Exponent Operations including Infinite Exponential Operations (IEOs) for loop constructs.
43. Ensure all Unicode Operation indexes are valid and all B-Stream lengths are valid.

## 5. Crabs Specification Requirements

44. Implement Chinese Room Abstract Stack Machine (Crabs) capable of running Group Automata using SCRIPT Protocol and ASCII Data Types.
45. Provide standardized Crabs API for interacting with Chinese Rooms via text terminals over Script.
46. Support multiple terminal interfaces including PC keyboard, app GUI, and other serial B-Streams.
47. Support ASCII C0 Codes and VK Keyboard Codes for keyboard input.
48. Support alt, shift, and control keys for copy, paste, and Script Macros.
49. Minimize RAM, ROM, and complexity while maximizing code reuse and minimizing code surface area.
50. Be portable to almost any microprocessor and pen and paper computers.
51. Operate safely with proper error handling and recovery mechanisms.

## 6. Authentication Requirements

52. Implement authentication using sequences of Crabs Operations.
53. Support token verification using shared secrets or public key infrastructure.
54. Require cryptographically secure tokens with maximum 24-hour lifetime.
55. Use 16-bit nonces for replay attack prevention (unique and non-repeating per attempt).
56. Implement rate-limiting for failed authentication attempts.
57. Support session key exchange using secure key exchange protocols.

## 7. Portal Requirements

58. Support Room-to-Room Portals (e.g., SPI, UART, I2C, CAN, Bluetooth).
59. Support Hallway Portals broadcasting messages to multiple Slots in Doors.
60. Handle imperfect transmission lines with appropriate interrupt systems for each protocol.
61. Support UDP connections with data loss recovery mechanisms.

## 8. Message Requirements

62. Define Crabs Messages as set of stack operations starting and ending with empty Operand Stack.
63. Support IUA-encoded B-Streams for non-printable Unicode characters.
64. Support UTF-8, UTF-16, and UTF-32 encoding for messages.
65. Ensure all keys resolve correctly and B-Stream lengths are valid for each B-Sequence argument.

## 9. Autoject Requirements

66. Implement Autojects as word-aligned memory managed objects using Socket Abstract Data Type and RAMFactory.
67. Support RAMFactory operations: creating, deleting, returning heap pointer, and returning data type.
68. Use multiples of CPU word size for memory allocation (no 8-bit support).
69. Support stack-to-heap auto-growth functionality.

## 10. Map Types Requirements

70. Implement contiguous memory layouts with 16/32/64-bit signed integer size headers at beginning.
71. Support standard Object types: Array, Stack, Matrix, Loom, Book, Dictionary, Map, Socket.
72. Support various hash table implementations optimized for microcontrollers and embedded systems.

## 11. Integrity Requirements

73. **SHA256 (or equivalent) as a first-class Crabs Machine operation.**
    The Crabs Machine shall provide tamper-evident hashing as a built-in
    operation, exposed as a type-value entry on the contiguous stack.
    This is required by the Freedom Voting Machine (FVM), which needs
    hash-chain integrity at each stage of the voting pipeline (ballot
    cast, ledger line, block commit, count agreement). The hash
    operation belongs in the core so all Crabs applications benefit,
    not in individual applications.

    **Status (2026-09-30):** Deferred. The FVM uses FNV-1a as a
    SLOP-SHORTCUT placeholder until this requirement is implemented.
    The FVM shall NOT implement SHA256 directly — it shall consume the
    Crabs Machine's hash operation once available.

    **Application:** Freedom Voting Machine (FVM), blockchain
    hash-chain integrity, ballot tamper detection, audit trail
    verification.
