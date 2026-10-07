|# SCRIPT Protocol

The Serial Chinese Room, Interprocess, and Telemetry (SCRIPT) Specification consists of the Automaton Standard Code for Information Interchange Data Specification, the Universal Addressing Specification, and the Chinese Room Abstract Stack Machine (Crabs) Specification.

In the John Searle's famous Chinese Room Thought Experiment users may talk to the Chinese Room (i.e. end-point) by writing Messages (i.e. Datagrams) on a piece of Paper (B-Input) and passing them through a Slot (i.e. Socket) in a Door (Group of communication links leading to the same room) in the room. An Agent then reads the message and uses them to operate the Crabs, and replies by writing a Message on a piece of Paper and passing it back through the Slot.

## Content Table

1. [ASCII Data Specification](../Data/)
1. [Universal Addressing Specification](../Addressing/)
1. [Crabs Specification](../Crabs/)
1. [RPC Protocol Specification](RPCProtocol.md) — Complete RPC wire format, semantics, envelopes, QoS, security, transport bindings, and agentic framework
