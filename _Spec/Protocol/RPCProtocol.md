# RPC Protocol Specification

The SCRIPT (Serial Chinese Room, Interprocess, and Telemetry) RPC Protocol defines a data-driven, transport-agnostic remote procedure call framework for the Crabs machine. It supports agentic systems and agent swarms while remaining compatible with constrained embedded systems.

## 1. RPC Frame Format

### 1.1 Frame Structure

Every RPC frame follows this layout:

```
+-------------------+-------------------+-------------------+-------------------+
| Frame Header      | Payload           | CRC               | Frame Trailer     |
| 16 bytes          | variable length   | 2 or 4 bytes      | 1 byte            |
+-------------------+-------------------+-------------------+-------------------+
```

### 1.2 Frame Header (16 bytes)

```
Offset  Size  Field            Description
------  ----  -----            -----------
  0      1    Magic            Frame magic: 0xA7 (ASCII Crabs)
  1      1    Version          Protocol version (currently 1)
  2      1    Frame Type       See §1.3
  3      1    Flags            Bit flags (see §1.4)
  4      4    Request ID       32-bit request identifier
  8      4    Payload Length   32-bit unsigned varint (VUC) of payload bytes
 12      2    CRC Type         0=none, 1=CRC-16, 2=CRC-32
 14      1    Compression      0=none, 1=lz4, 2=zlib (reserved for future)
 15      1    Reserved         Must be zero
```

### 1.3 Frame Types

| Value | Name          | Description                          |
|-------|---------------|--------------------------------------|
|   0   | REQ           | Request (caller → callee)            |
|   1   | RESP          | Response (callee → caller)           |
|   2   | ERROR         | Error result                         |
|   3   | CANCEL        | Cancellation request                 |
|   4   | HEARTBEAT     | Keepalive / progress indicator       |
|   5   | DISCOVER      | Capability discovery query           |
|   6   | DISCOVER_RESP | Capability discovery response        |
|   7   | STREAM_REQ    | Streaming request                    |
|   8   | STREAM_DATA   | Streaming data chunk                 |
|   9   | STREAM_END    | End of stream                        |
|  10   | SUBSCRIBE     | Subscribe to operation results       |
|  11   | UNSUBSCRIBE   | Unsubscribe from operation results   |
|  12   | PUB           | Publish event (pub/sub)              |

### 1.4 Flags

| Bit | Name      | Description                                          |
|-----|-----------|------------------------------------------------------|
|   0 | RESPONSE  | Set if this is a response to a prior request         |
|   1 | ERROR     | Set if frame carries an error                        |
|   2 | STREAMING | Set if this is part of a streaming exchange          |
|   3 | CHUNKED   | Set if payload is a fragment of a larger message     |
|   4 | COMPRESSED| Payload is compressed                                |
|   5 | ENCRYPTED | Payload is encrypted (handled at transport layer)    |
|   6 | RESERVED  | Reserved for future use                              |
|   7 | RESERVED  | Reserved for future use                              |

## 2. RPC Semantics

### 2.1 Request/Response

The default RPC mode. Caller sends REQ, callee sends RESP.

```
Caller                          Callee
  |--- REQ (request ID=N) -------->|
  |                               |
  |<-- RESP (request ID=N) --------|
```

**Guarantees**: At-least-once delivery by default. Exactly-once emulation via §8 deduplication.

### 2.2 Fire-and-Forget

Caller sends REQ with flag RESPONSE=0. No response expected.

### 2.3 Streaming

Caller sends STREAM_REQ, callee sends multiple STREAM_DATA frames, then STREAM_END.

```
Caller                          Callee
  |--- STREAM_REQ ---------------->|
  |<-- STREAM_DATA ----------------|
  |<-- STREAM_DATA ----------------|
  |<-- STREAM_DATA ----------------|
  |<-- STREAM_END -----------------|
```

### 2.4 Bidirectional Stream

Both parties send STREAM_DATA frames independently until either sends STREAM_END.

### 2.5 Publish/Subscribe

```
Publisher                    Broker                    Subscriber
  |--- PUB (topic=T) -------->|                           |
  |                            |--- dispatch to topic T -->|
  |                            |                           |
  |<-- ACK (optional) ---------|<-- SUBSCRIBE (topic=T) --|
```

Subscribers use SUBSCRIBE/UNSUBSCRIBE frames. The broker routes PUB frames to matching subscribers.

## 3. Operation Identity and Versioning

### 3.1 Operation ID

Each RPC operation has a stable 64-bit Operation ID (OpID):

```
+-------------------+-------------------+
| Namespace Hash    | Operation Name Hash|
| 32 bits           | 32 bits            |
+-------------------+-------------------+
```

- **Namespace Hash**: SipHash-2-4 of the namespace string (e.g., "system", "agent", "crabs")
- **Operation Name Hash**: SipHash-2-4 of the operation name within the namespace

The OpID is constant across protocol versions, enabling stable references.

### 3.2 Operation Names and Namespaces

Operation names follow the pattern: `namespace.operation.suboperation`

Examples:
- `crabs.room.create`
- `crabs.slot.read`
- `crabs.slot.write`
- `agent.task.submit`
- `agent.task.result`
- `system.health.ping`

Namespaces are hierarchical, separated by `.`. A namespace implicitly includes all sub-namespaces.

### 3.3 Versioning

Each operation supports a version number encoded in the Flags field (bits 6-7 for future use, or as a separate version byte in the payload header).

**Compatibility Rules**:
- **Backward compatible**: New optional fields, new operations in same namespace
- **Forward compatible**: Remove optional fields, deprecate operations (mark as OBSOLETE)
- **Breaking change**: Increment version number, provide migration path in OBSOLETE operation

### 3.4 Capability Negotiation (§6)

Peers discover each other's supported operations via DISCOVER frames. See §6.

## 4. Request/Response Envelope

### 4.1 Request Envelope

```
+-------------------+-------------------+-------------------+
| OpID (8 bytes)    | Request ID (4)    | Flags (1)         |
+-------------------+-------------------+-------------------+
| Parent Request ID | Trace ID          | Span ID           |
| (4 bytes)         | (8 bytes)         | (8 bytes)         |
+-------------------+-------------------+-------------------+
| Deadline (4)      | Timeout (4)       | Payload BSQ Header|
+-------------------+-------------------+-------------------+
| BSQ Parameters (variable)
+-------------------+
```

**Fields**:
- **OpID**: 8-byte operation identifier (§3.1)
- **Request ID**: 32-bit identifier for correlating request with response
- **Flags**: Per-request flags (retryable, etc.)
- **Parent Request ID**: 4 bytes, zero if root request
- **Trace ID**: 8-byte distributed trace identifier
- **Span ID**: 8-byte span/call identifier within the trace
- **Deadline**: Unix timestamp (seconds) when request expires, zero = no deadline
- **Timeout**: Milliseconds before caller gives up, zero = use deadline
- **BSQ Header**: B-Sequence header describing the payload (§5)
- **BSQ Parameters**: The actual arguments

### 4.2 Response Envelope

```
+-------------------+-------------------+-------------------+
| Request ID (4)    | Status (1)        | Payload Length (4)|
+-------------------+-------------------+-------------------+
| Trace ID (8)      | Span ID (8)       | BSQ Header        |
+-------------------+-------------------+-------------------+
| BSQ Results (variable)
+-------------------+
```

**Status Codes**:
| Value | Name      | Description                        |
|-------|-----------|------------------------------------|
|   0   | OK        | Success                            |
|   1   | INPROGRESS | Still processing (streaming)      |
|   2   | CANCELLED | Request was cancelled              |
|   3   | TIMEOUT   | Deadline or timeout exceeded       |
|   4   | ERROR     | Operation-specific error           |

### 4.3 Error Envelope

```
+-------------------+-------------------+-------------------+
| Request ID (4)    | Error Code (2)    | Error Class (1)   |
+-------------------+-------------------+-------------------+
| Retryable (1)     | Faulting OpID (8) | Message Length (4)|
+-------------------+-------------------+-------------------+
| Message (UTF-8)   | Details BSQ (variable) |
+-------------------+-------------------+
```

**Error Class**:
| Value | Name        | Description                          |
|-------|-------------|--------------------------------------|
|   0   | SYSTEM      | Protocol/system-level error          |
|   1   | NETWORK     | Network/connectivity error           |
|   2   | AUTH        | Authentication/authorization error   |
|   3   | VALIDATION  | Input validation error               |
|   4   | OPERATION   | Operation-specific error             |
|   5   | RESOURCE    | Resource exhaustion                  |
|   6   | TIMEOUT     | Timeout/error                        |
|   7   | UNKNOWN     | Unclassified error                   |

**Retryable**: 1 = caller may retry, 0 = do not retry.

## 5. B-Sequence Encoding for RPC

### 5.1 BSQ Header Format

All BSQ parameters in RPC frames follow the format defined in [BSequences.md](../Data/BSequences.md).

**Header**: `{ n, p_1, p_2, ..., p_n }` where `n` is a VUC (32-bit unsigned varint) and each `p_i` is a type descriptor.

**Type Descriptors**:
- For fixed-size POD types: a single byte (the POD type code)
- For variable-size types: two bytes — type code followed by size parameter
- For maps: the decomposed 3-byte format (§6 of this document)

### 5.2 Map Type Decomposition

Composite map types (BO0-BO8, DI0-DI5, TB0-TB3, LS0-LS2) are transmitted using the decomposed fundamental type format:

```
[MapKind:2][KeyType:5][ValueType:5][SizeType:5][DataType:5] = 22 bits in 3 bytes (big-endian)
```

**Wire Format Table**:

```
BO0: 0x01 0x98 0x55   Book<CHA=3, ISB=6, ISA=2, DTB=21>
BO1: 0x01 0x98 0xD5   Book<CHA=3, ISB=6, ISB=6, DTB=21>
BO2: 0x01 0x99 0xD5   Book<CHA=3, ISB=6, ISD=14, DTB=21>
BO3: 0x03 0x98 0x55   Book<CHB=7, ISB=6, ISA=2, DTB=21>
BO4: 0x03 0x98 0xD5   Book<CHB=7, ISB=6, ISB=6, DTB=21>
BO5: 0x03 0x99 0xD5   Book<CHB=7, ISB=6, ISD=14, DTB=21>
BO6: 0x05 0x98 0x55   Book<CHC=11, ISB=6, ISA=2, DTB=21>
BO7: 0x05 0x98 0xD5   Book<CHC=11, ISB=6, ISB=6, DTB=21>
BO8: 0x05 0x99 0xD5   Book<CHC=11, ISB=6, ISD=14, DTB=21>
DI0: 0x11 0xA8 0xC9   Dictionary<CHA=3, ISC=10, ISB=6, IUC=9, DTB=21>
DI1: 0x11 0xB8 0xCD   Dictionary<CHA=3, ISD=14, ISB=6, IUD=13, DTB=21>
DI2: 0x13 0xA8 0xC9   Dictionary<CHB=7, ISC=10, ISB=6, IUC=9, DTB=21>
DI3: 0x13 0xB9 0x4D   Dictionary<CHB=7, ISD=14, ISC=10, IUD=13, DTB=21>
DI4: 0x15 0xA8 0xC9   Dictionary<CHC=11, ISC=10, ISB=6, IUC=9, DTB=21>
DI5: 0x15 0xB9 0x4D   Dictionary<CHC=11, ISD=14, ISC=10, IUD=13, DTB=21>
TB0: 0x35 0x19 0x20   Table<ISC=10, ISB=6, IUC=9>
TB1: 0x37 0x29 0x20   Table<ISD=14, ISC=10, IUC=9>
TB2: 0x35 0x19 0xA0   Table<ISC=10, ISB=6, IUD=13>
TB3: 0x37 0x29 0xA0   Table<ISD=14, ISC=10, IUD=13>
LS0: 0x23 0x08 0x15   List<ISB=6, ISA=2, DTB=21>
LS1: 0x25 0x18 0x15   List<ISC=10, ISB=6, DTB=21>
LS2: 0x27 0x28 0x15   List<ISD=14, ISC=10, DTB=21>
```

**Map Kind Values**:
| Value | Kind       |
|-------|------------|
|   0   | Book       |
|   1   | Dictionary |
|   2   | List       |
|   3   | Table      |

**Fundamental Type Codes**:
| Code | Type | Code | Type | Code | Type |
|------|------|------|------|------|------|
|   0  | NIL  |   1  | IUA  |   2  | ISA  |
|   3  | CHA  |   4  | FPB  |   5  | IUB  |
|   6  | ISB  |   7  | CHB  |   8  | FPC  |
|   9  | IUC  |  10  | ISC  |  11  | CHC  |
|  12  | FPD  |  13  | IUD  |  14  | ISD  |
|  20  | PCa  |  21  | DTB  |  22  | DTC  |
|  23  | DTD  |      |      |      |      |

### 5.3 Validation Rules

1. **Integer bounds**: Value must fit within declared size type's range.
2. **Offset validation**: All offsets must be less than socket/buffer size.
3. **MapKind consistency**: Map kind determines which slots are mandatory.
4. **Type compatibility**: KeyType and ValueType must be compatible with MapKind.

## 6. Capability Negotiation and Discovery

### 6.1 Discovery Request

```
+-------------------+-------------------+
| Namespace (var)   | Version (1 byte)  |
+-------------------+-------------------+
```

- **Namespace**: UTF-8 string, empty = all namespaces
- **Version**: Client's maximum supported protocol version

### 6.2 Discovery Response

```
+-------------------+-------------------+-------------------+
| Operation Count (VUC)| OpID (8 bytes)  | Version (1)       |
+-------------------+-------------------+-------------------+
| BSQ In Header     | BSQ Out Header    | Capabilities      |
+-------------------+-------------------+-------------------+
```

**Capabilities flags** (per operation):
- `R` = Readable (can be called)
- `W` = Writable (can receive updates)
- `S` = Streamable (supports streaming)
- `P` = Pub/Sub participant
- `OBSOLETE` = Deprecated, still functional

### 6.3 Handshake Capability Exchange

During the generic handshake (§7), peers exchange a CAPABILITIES frame containing:
- Supported protocol versions
- Supported transport bindings
- Supported compression algorithms
- Supported CRC types
- Supported character widths
- Supported ASCII data type ranges
- Maximum frame size
- List of registered operations with their BSQ signatures

## 7. Handshake Protocol

### 7.1 Protocol Version Negotiation

The handshake begins with version negotiation:

```
Client                          Server
  |--- HELLO (version, caps) ---->|
  |<-- HELLO_ACK (version, caps) --|
  |--- HELLO_OK ------------------>|
  |<-- HELLO_OK_ACK --------------|
```

**HELLO frame**:
```
+-------------------+-------------------+-------------------+
| Magic (1)         | Version (1)       | Max Frame Size (4)|
+-------------------+-------------------+-------------------+
| CRC Type (1)      | Compression (1)   | Char Widths (1)   |
+-------------------+-------------------+-------------------+
| Data Type Range   | Reserved (2)      |
+-------------------+-------------------+
```

**HELLO_ACK frame** — Server's response with its capabilities.

**HELLO_OK** — Client confirms selected parameters.

**HELLO_OK_ACK** — Server acknowledges, connection established.

### 7.2 Auth Binding

If authentication is required, the auth token is bound to the handshake transcript:

```
AuthHash = HMAC-SHA256(key, handshake_transcript)
```

The auth token is sent as the first RPC frame after handshake completion.

### 7.3 Session Parameters

After handshake, the following parameters are fixed for the session:
- Protocol version
- Maximum frame size
- CRC type
- Compression algorithm (if any)
- Character width support
- ASCII data type ranges

These parameters cannot change mid-session.

## 8. QoS and Reliability

### 8.1 Delivery Guarantees

| Mode              | Description                                    |
|-------------------|------------------------------------------------|
| Best Effort       | No guarantee, fastest path                     |
| At-Most-Once      | May lose frames, never duplicates              |
| At-Least-Once     | May duplicate frames, never loses              |
| Exactly-Once      | Emulated via deduplication (§8.2)              |

### 8.2 Deduplication

At-least-once and exactly-once modes use request ID deduplication:

```
Sender                      Receiver
  |--- REQ (ID=N) ---------->|
  |                           | Stores ID=N in dedup window
  |<-- RESP (ID=N) -----------|
  |--- REQ (ID=N, retry) ---->|
  |                           | Sees ID=N in window, returns cached RESP
  |<-- RESP (ID=N, cached) ---|
```

**Dedup window**: Configurable (default 1024 entries), sliding time window (default 30 seconds).

### 8.3 Retry Rules

| Operation Type    | Retryable | Rationale                              |
|-------------------|-----------|----------------------------------------|
| Read operations   | Yes       | Idempotent — safe to repeat            |
| Write operations  | Conditional | Only if operation declares idempotent |
| Delete operations | Yes       | Idempotent — safe to repeat            |
| Stream operations | No        | Stateful — retry breaks sequencing     |
| Auth operations   | Limited   | Max 3 retries, then fail               |

### 8.4 Acknowledgment Modes

| Mode      | Scope     | Description                          |
|-----------|-----------|--------------------------------------|
| Per-slot  | Slot      | Ack per slot                         |
| Per-peer  | Peer      | Ack per peer connection              |
| Per-session| Session  | Ack per session                      |
| None      | —         | No acknowledgments                   |

### 8.5 State Machine

```
           +--------+
      +----| PENDING  |----+
      |    +--------+    |
      |         |        v
      |    +----+----+  +----------+
      |    | WAIT_ACK |<->| COMPLETE |
      |    +---------+  +----------+
      |         |
      |    +----+-----+
      |    | TIMEOUT   |----> RESEND or FAIL
      |    +----------+
      |
      +<-- CANCELLED
```

## 9. Cancellation and Deadlines

### 9.1 Cancellation

Caller sends CANCEL frame with the original Request ID:

```
+-------------------+-------------------+-------------------+
| Request ID (4)    | Reason (1)        | Reserved (3)      |
+-------------------+-------------------+-------------------+
```

**Reason codes**:
| Value | Name       | Description                        |
|-------|------------|------------------------------------|
|   0   | USER       | User-initiated cancellation        |
|   1   | TIMEOUT    | Deadline exceeded                  |
|   2   | ERROR      | Error condition                    |
|   3   | PREEMPT    | Preempted by higher priority       |

### 9.2 Deadline Propagation

Deadlines propagate through the call chain:

```
Deadline = min(request_deadline, parent_deadline - overhead)
```

Each hop subtracts estimated network overhead (configurable, default 10ms).

### 9.3 Timeout Hierarchy

```
Caller Timeout > Callee Deadline > Network Timeout
```

- **Caller Timeout**: Maximum time the caller waits (enforced by caller)
- **Callee Deadline**: Maximum time the callee works (enforced by callee)
- **Network Timeout**: Maximum time a frame can be in transit (enforced by transport)

## 10. Progress and Heartbeat

### 10.1 Progress States

| State     | Description                          |
|-----------|--------------------------------------|
| ACCEPT    | Request received, processing started |
| PROGRESS  | Intermediate progress update         |
| HEARTBEAT | Keepalive, no state change           |
| CHECKPOINT| Checkpoint saved                     |
| RESULT    | Operation complete, result available |
| FAILURE   | Operation failed                     |

### 10.2 Heartbeat Interval

Default: 5 seconds. Configurable per-peer. Minimum: 1 second. Maximum: 60 seconds.

### 10.3 Progress Frames

```
+-------------------+-------------------+-------------------+
| Request ID (4)    | State (1)         | Progress (1)      |
+-------------------+-------------------+-------------------+
| Checkpoint (8)    | ETA (4)           | Payload (variable)|
+-------------------+-------------------+-------------------+
```

- **Progress**: 0-100 percentage
- **Checkpoint**: 64-bit checkpoint identifier
- **ETA**: Estimated time to completion in milliseconds

## 11. Error Model

### 11.1 Error Code Space

Error codes are 16-bit values structured as:

```
+-------------------+-------------------+
| Class (8 bits)    | Code (8 bits)     |
+-------------------+-------------------+
```

### 11.2 Standard Error Codes

**SYSTEM (class 0)**:
| Code | Name        | Description                          |
|------|-------------|--------------------------------------|
|    1 | BAD_MAGIC   | Invalid frame magic                  |
|    2 | BAD_VERSION | Unsupported protocol version         |
|    3 | BAD_HEADER  | Malformed frame header               |
|    4 | BAD_CRC     | CRC check failure                    |
|    5 | OVERRUN     | Buffer overflow                      |
|    6 | UNDERRUN    | Buffer underflow                     |
|    7 | BAD_LENGTH  | Invalid payload length               |

**NETWORK (class 1)**:
| Code | Name        | Description                          |
|------|-------------|--------------------------------------|
|    1 | DISCONNECT  | Connection lost                      |
|    2 | TIMEOUT     | Network timeout                      |
|    3 | RESET       | Connection reset                     |
|    4 | REFUSED     | Connection refused                   |
|    5 | UNREACHABLE | Destination unreachable              |

**AUTH (class 2)**:
| Code | Name        | Description                          |
|------|-------------|--------------------------------------|
|    1 | UNAUTHENTICATED | No auth token provided            |
|    2 | UNAUTHORIZED  | Invalid or expired token            |
|    3 | FORBIDDEN     | Permission denied                  |
|    4 | EXPIRED       | Session expired                     |

**VALIDATION (class 3)**:
| Code | Name        | Description                          |
|------|-------------|--------------------------------------|
|    1 | BAD_TYPE    | Invalid data type                   |
|    2 | BAD_VALUE   | Value out of range                   |
|    3 | BAD_SIZE    | Size exceeds limits                  |
|    4 | BAD_FORMAT  | Malformed input                      |
|    5 | MISSING     | Required field missing               |

**OPERATION (class 4)**:
| Code | Name        | Description                          |
|------|-------------|--------------------------------------|
|    1 | NOT_FOUND   | Operation not registered             |
|    2 | BAD_ARGS    | Invalid arguments                   |
|    3 | EXEC_FAILED | Execution error                      |
|    4 | NOT_IMPL    | Operation not implemented            |
|    5 | DEPRECATED  | Operation deprecated                 |

**RESOURCE (class 5)**:
| Code | Name        | Description                          |
|------|-------------|--------------------------------------|
|    1 | OOM         | Out of memory                        |
|    2 | NO_SLOTS    | No available slots                   |
|    3 | NO_ROOMS    | No available rooms                   |
|    4 | FULL        | Buffer full                          |

**TIMEOUT (class 6)**:
| Code | Name        | Description                          |
|------|-------------|--------------------------------------|
|    1 | CALLER      | Caller timeout                       |
|    2 | CALLEE      | Callee deadline exceeded             |
|    3 | NETWORK     | Network timeout                      |

### 11.3 Error Details Payload

Errors include a BSQ details payload containing:
- Faulting operation OpID
- Input parameter snapshot (for debugging)
- Internal state snapshot (if applicable)
- Stack trace (in debug builds)

## 12. Security Model

### 12.1 Frame-Level Binding

Each frame's CRC and encryption scope is determined by flags:

| Scope        | Frames Covered by Auth              |
|-------------|-------------------------------------|
| Per-room    | All frames within a room session    |
| Per-op      | Individual operation frames         |
| Per-resource| Frames targeting specific resources |
| Per-role    | Frames based on caller's role       |

### 12.2 Encryption

Encryption is handled at the transport layer. The RPC protocol passes encrypted payloads transparently. Supported algorithms:
- AES-128-GCM (required)
- AES-256-GCM (recommended)
- ChaCha20-Poly1305 (optional)

### 12.3 Auth Token

Auth tokens are HMAC-SHA256 signed, bound to the handshake transcript:

```
Token = HMAC-SHA256(secret, handshake_transcript || nonce)
```

Tokens expire after configurable duration (default: 3600 seconds).

## 13. Transport Bindings

### 13.1 UART

- Baud rate: Configurable (115200 recommended)
- Framing: 8N1 (8 data bits, no parity, 1 stop bit)
- Flow control: XON/XOFF or hardware RTS/CTS
- Frame delimiter: FRAME_SEP (0x18)

### 13.2 SPI

- Mode: SPI Mode 0 or Mode 3
- Clock: Up to 10 MHz
- Frame delimiter: CS assertion/deassertion
- Word size: 8 bits

### 13.3 I2C

- Address: 7-bit or 10-bit
- Speed: Standard (100 kHz) or Fast (400 kHz)
- Frame delimiter: START/STOP conditions
- Bus arbitration: Built-in I2C arbitration

### 13.4 CAN

- Format: CAN 2.0B (29-bit ID)
- Bit rate: Up to 1 Mbps
- Frame delimiter: SOF/EOF
- Priority: CAN ID determines priority

### 13.5 Bluetooth

- Profile: BLE GATT
- MTU: Negotiated (default 23 bytes, up to 517 bytes)
- Frame delimiter: GATT characteristic writes
- Connection: Point-to-point or mesh

### 13.6 UDP

- Port: Configurable (default 9000)
- Framing: Length-prefixed frames
- Reliability: Application-level (RPC protocol §8)
- Multicast: Supported for pub/sub

### 13.7 Interprocess

- Unix Domain Sockets (Linux)
- Named Pipes (Windows)
- Shared Memory (all platforms)
- Frame delimiter: Length-prefixed

### 13.8 Windows Mirrors

- Windows named pipes with mirror mode
- Frame delimiter: Length-prefixed
- Duplex: Full-duplex by default

## 14. Schema Evolution

### 14.1 Additive Fields

New optional fields may be added to any frame:
- Receiver must ignore fields it does not recognize
- Field position is stable (no reordering)
- New fields must have default values

### 14.2 Removed Fields

Fields marked OBSOLETE:
- Sender must not transmit removed fields
- Receiver must ignore removed fields
- Deprecation period: minimum 2 protocol versions

### 14.3 Renamed Fields

Field renaming requires:
1. Add new field name (backward compatible)
2. Mark old field as OBSOLETE
3. Wait deprecation period
4. Remove old field

### 14.4 Type Widening/Narrowing

- **Widening** (e.g., IUA → IUB): Safe, backward compatible
- **Narrowing** (e.g., IUB → IUA): Breaking change, requires version bump

## 15. Agentic Framework

### 15.1 Agent Identity

Each agent has a unique Agent ID (64-bit):

```
+-------------------+-------------------+
| Node ID (32 bits) | Agent Index (32)  |
+-------------------+-------------------+
```

- **Node ID**: Identifies the physical/logical node
- **Agent Index**: Identifies the agent on the node

### 15.2 Room and Session IDs

- **Room ID**: 64-bit identifier for the Chinese Room
- **Session ID**: 64-bit identifier for the communication session
- **Role**: Agent role (COORDINATOR, WORKER, OBSERVER, ROUTER)

### 15.3 Swarm Topologies

| Topology     | Description                          |
|-------------|--------------------------------------|
| Hub/Spoke   | Central coordinator, spoke workers   |
| Mesh        | All-to-all communication             |
| Hierarchical| Nested rooms with parent/child       |
| Federated   | Independent clusters with sync       |

### 15.4 Agent Task Envelope

```
+-------------------+-------------------+-------------------+
| Task ID (8)       | Parent Task (8)   | Predecessor Event (8) |
+-------------------+-------------------+-------------------+
| Goal (var)        | Plan (var)        | Tool Invocations (var) |
+-------------------+-------------------+-------------------+
| Observation (var) | Result (var)      | Error (var)         |
+-------------------+-------------------+-------------------+
| Review (var)      | Timeout (4)       | Budget (var)        |
+-------------------+-------------------+-------------------+
```

### 15.5 Resource Budgeting

Resources tracked per-agent:
- **Time**: Wall-clock seconds
- **Tokens**: Computational tokens (implementation-defined)
- **Bytes**: Memory/network bytes
- **Compute**: CPU cycles or equivalent

### 15.6 Membership Protocol

| Event     | Description                          |
|-----------|--------------------------------------|
| JOIN      | Agent joins swarm                    |
| LEAVE     | Agent voluntarily leaves             |
| EVICTION  | Agent removed by coordinator         |
| FAILURE   | Agent detected as failed             |

### 15.7 Causal Ordering

Events maintain causal order:
- **Parent task**: Direct child of parent
- **Predecessor event**: Event that must complete first
- **Causal chain**: Full ancestry of the task

## 16. Conformance Profiles

### 16.1 Profile: Tiny Embedded

Minimal implementation for resource-constrained systems:
- Frame types: REQ, RESP, ERROR, CANCEL
- QoS: Best effort only
- No streaming
- No encryption
- CRC-16 only
- UDP or UART transport only

### 16.2 Profile: Full Room

Complete Chinese Room implementation:
- All frame types
- All QoS modes
- Full streaming support
- Optional encryption
- All CRC types
- All transport bindings

### 16.3 Profile: Router

Routing infrastructure node:
- Discovery and capability negotiation
- Multi-transport bridging
- Pub/sub routing
- Traffic shaping and QoS enforcement
- No room operations

### 16.4 Profile: Agent Node

Full agentic system implementation:
- All Room features
- Agent task envelope
- Swarm membership protocol
- Resource budgeting
- Causal event ordering

### 16.5 Profile: Swarm Coordinator

Swarm orchestration node:
- All Agent Node features
- Topology management
- Task distribution and load balancing
- Failure detection and recovery
- Cross-cluster federation

## 17. Terminals and Operators

### 17.1 Terminal Operations

| Op    | Description                          |
|-------|--------------------------------------|
| OPEN  | Open a terminal session              |
| CLOSE | Close a terminal session             |
| RESIZE| Resize terminal dimensions           |
| INT   | Interrupt current operation          |
| UPLOAD| Upload artifact to remote            |
| DOWNLOAD| Download artifact from remote      |

### 17.2 Control Plane

The control plane manages room and slot lifecycle:
- Room creation and destruction
- Slot allocation and deallocation
- Agent registration and deregistration
- Resource monitoring

## 18. New C0 Codes for RPC

The following ASCII Device Control characters are formalized for RPC use:

| C0    | Crabs Meaning  | Purpose                                    |
|-------|---------------|--------------------------------------------|
| NUL   | NOP           | No-op / padding / idle                     |
| SOH   | BEGIN         | Begin message, list, frame, or expression  |
| STX   | BEGIN_DATA    | Begin data payload / list load             |
| ETX   | END           | End current expression/list/frame          |
| EOT   | CLOSE         | Close room/session/stream                  |
| ENQ   | QUERY         | Ask for op/signature/schema/state          |
| ACK   | ACK           | Success transport/control response         |
| BEL   | SIGNAL        | Notify/ring/call attention                 |
| BS    | POP           | Pop one operand/scope                      |
| HT    | DUP           | Duplicate top stack item                   |
| LF    | EVAL          | Evaluate/step current expression           |
| VT    | PUSH_SCOPE    | Push current scope/parent/object           |
| FF    | CLEAR         | Clear current stack/list/result            |
| CR    | CALL          | Call current op/path with args             |
| SO    | ENTER         | Enter child scope/path segment             |
| SI    | LEAVE         | Leave scope/path segment                   |
| DLE   | LITERAL       | Next item is literal data/control escaped  |
| DC1   | REF           | Push reference to List item/index          |
| DC2   | SET           | Assign/write slot/key/index                |
| DC3   | GET           | Read slot/key/index                        |
| DC4   | RESULT        | Begin/write result values                  |
| NAK   | ERROR         | Error result/control failure               |
| SYN   | SYNC          | Resync frame/stream/scanner                |
| ETB   | COMMIT        | Commit transaction/frame/list append       |
| CAN   | CANCEL        | Cancel/rollback current expression         |
| EM    | END_MESSAGE   | End message/result batch                   |
| SUB   | SUBSTITUTE    | Substitute ESC output for expected BSQ     |
| ESC   | ESCAPE        | Begin executable replacement sequence      |
| FS    | FRAME_SEP     | Frame/list field separator                 |
| GS    | GROUP_SEP     | Group/arg separator                        |
| RS    | RECORD_SEP    | Record/op separator                        |
| US    | UNIT_SEP      | Unit/value separator                       |

## 19. Race Conditions

The following race conditions must be handled by RPC implementations:

| #  | Race Condition                              | Mitigation                         |
|----|---------------------------------------------|------------------------------------|
| 1  | Writer publishes stop before payload fully written | Atomic frame commit (§19.1)    |
| 2  | Reader reads frame while writer patches prefix/suffix | Lock-free ring buffer with memory barriers |
| 3  | Ring buffer wrap causes reader to see old bytes | Monotonic sequence numbers       |
| 4  | ABA: same offset reused, mistaken for same transaction | Monotonic transaction IDs      |
| 5  | Torn multi-byte header/footer write         | Atomic writes or spinlock          |
| 6  | CPU reorder: payload writes visible after commit pointer | Memory barriers (mfence)    |
| 7  | Reader validates CRC/length over mutable bytes | Copy-on-read or dual-buffer       |
| 8  | Producer overwrites unread frame            | Full/done flags, consumer drains  |
| 9  | Multiple writers reserve overlapping ranges | Per-slot mutex or lock-free queue |
| 10 | Lost update between concurrent transactions | Optimistic concurrency control    |
| 11 | Dirty read of uncommitted transaction       | Read-committed isolation level     |
| 12 | Non-repeatable read during multi-step exec  | Snapshot isolation                 |
| 13 | Phantom read while scanning index/table     | Serializable isolation             |
| 14 | Deadlock between round-robin tasks          | Lock ordering / timeout            |
| 15 | Starvation when one hot producer wins scheduler | Priority inheritance           |
| 16 | Timeout races: valid slow frame discarded   | Adaptive timeout (min/max bounds)  |
| 17 | Crash after payload write, before commit    | Write-ahead log (WAL)              |
| 18 | Crash after commit, before fsync            | Durable commit with fsync/fdatasync|

### 19.1 Atomic Frame Commit

Frames are committed atomically using a two-phase approach:
1. **Write**: Payload bytes written to ring buffer
2. **Commit**: Frame length written last, making frame visible

Readers validate: if length is non-zero, the full frame must be present.

## 20. Suggested Implementation Order

For incremental development, implement in this order:

1. **RPC frame format** (§1-2) — Core wire format and semantics
2. **CRC specification** (§1.2, §11) — Frame integrity
3. **Request/response/error/cancel envelopes** (§4, §11) — Basic RPC
4. **Capability negotiation and handshake** (§6-7) — Connection setup
5. **Transport bindings** (§13) — UART, UDP, interprocess first
6. **Discovery/introspection schema** (§6) — Service discovery
7. **Agent task envelope and swarm membership** (§15) — Agentic features
8. **QoS retry/ack/dedup rules** (§8) — Reliability
9. **Security binding to RPC envelope** (§12) — Encryption and auth
10. **Conformance profiles and interop tests** (§16) — Verification

## 21. Spec Consistency

### 21.1 Terminology Resolution

| Term          | Definition                                      |
|---------------|--------------------------------------------------|
| Object        | A Crabs data structure with type metadata        |
| Message       | A complete unit of data exchanged between rooms  |
| Packet        | A transport-layer unit (may contain multiple messages) |
| Datagram      | A connectionless message (UDP)                   |
| B-Stream      | A byte stream of Crabs data                      |
| BSQ           | A B-Sequence — metadata describing byte layout   |
| Slot          | A ring buffer for BIn/BOut communication         |
| Socket        | A logical connection endpoint (composed of slots)|
| Portal        | An external interface to a room                  |
| Room          | A Crabs instance with slots and operations       |

### 21.2 Header and Size Field Naming

- **Frame header**: The 16-byte prefix of every RPC frame
- **BSQ header**: The type descriptor array in a B-Sequence
- **Op header**: The Operation registration structure
- **Size fields**: Always 32-bit unsigned (IUC) unless specified otherwise
- **Hash types**: SipHash-2-4 for OpID, HMAC-SHA256 for auth
- **Pointer naming**: ISW (signed) and IUW (unsigned) for word-sized pointers
- **Address naming**: Universal Address Format (§13 of Addressing spec)

## Appendix A: Microframework API

The RPC protocol supports the following core operations for a C++ microframework:

| Operation        | Description                              |
|------------------|------------------------------------------|
| `rpc_encode_frame`    | Encode data into an RPC frame         |
| `rpc_decode_frame`    | Decode an RPC frame into data         |
| `rpc_register_op`     | Register an operation with its BSQ    |
| `rpc_dispatch_op`     | Dispatch a received request to handler|
| `rpc_write_slot`      | Write data to a BOut slot             |
| `rpc_read_slot`       | Read data from a BIn slot             |
| `rpc_tick`            | Process pending frames and timeouts   |
| `rpc_heartbeat`       | Send periodic heartbeat               |
| `rpc_discover`        | Query peer capabilities               |
| `rpc_subscribe`       | Subscribe to operation results        |
| `rpc_cancel`          | Cancel an in-flight request           |
| `rpc_stream_send`     | Send a streaming data chunk           |

## Appendix B: CRC Specification

### B.1 CRC-16

- Polynomial: 0x8005 (reflected, init 0x0000)
- Input: Frame header bytes (excluding CRC field) + payload
- Output: 16-bit value appended after payload

### B.2 CRC-32

- Polynomial: 0xEDB88320 (reflected Ethernet CRC-32)
- Input: Frame header bytes (excluding CRC field) + payload
- Output: 32-bit value appended after payload

### B.3 CRC Vectors (Test Cases)

```
Input: 0x00 (single zero byte)
CRC-16: 0x0000
CRC-32: 0x00000000

Input: 0xFF (single 0xFF byte)
CRC-16: 0x8408
CRC-32: 0xD90A56C3

Input: "123456789" (ASCII)
CRC-16: 0xBF05
CRC-32: 0xCBF43926
```
