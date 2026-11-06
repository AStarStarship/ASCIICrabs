// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsMorseCodeHXX
#define _CrabsMorseCodeHXX
#include <_Config.h>
namespace _ {
/* This code doesn't really seam to match with Crabs's mission other than a
universal Chinese Room Langauge. The problem is that it doesn't seem to feit
anywhere in Kabuki Toolkit right now so it sits here. It's useful for
transeiving debug messages via LED lights. */

const CHA* ToMorseCode(CHA code) {
  static const CHA space[] = {'0', NIL};  //<

  static const CHA cypher[91][10] = {
      {'.', '-', '.', '-', '.', NIL, NIL, NIL, NIL,
       NIL},  //< ASCII , Morse code: End of message.
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII SOH.
      {'-', '.', '-', '.', '-', NIL, NIL, NIL, NIL,
       NIL},  //< ASCII STX, Morse code: Start copying.
      {'-', '.', '-', '.', '.', '-', '.', '.', NIL,
       NIL},  //< ASCII ETX, Morse code: Going off the air("clear").
      {'.', '.', '.', '-', '.', '-', NIL, NIL, NIL,
       NIL},  //< ASCII EOT, Morse code: End of transmission.
      {'-', '.', '-', '-', '.', NIL, NIL, NIL, NIL,
       NIL},  //< ASCII ENQ, Morse code: Invite a specific station to transmit.
      {'.', '.', '.', '-', '.', NIL, NIL, NIL, NIL,
       NIL},  //< ASCII ACK, Morse code: Understood.
      {'.', '.', '.', '-', '-', '-', '.', '.', '.',
       NIL},  //< ASCII BEL, Morse code: SOS distress signal.
      {'.', '.', '.', '.', '.', '.', '.', '.', NIL,
       NIL},  //< ASCII BS,  Morse code: Prosign error.
      {'0', NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL,
       NIL},  //< ASCII HT/Morse code:Space.
      {'.', '-', '.', '-', NIL, NIL, NIL, NIL, NIL,
       NIL},  //< ASCII LF,  Morse code: TNew Line
      {'-', '.', '.', '.', '-', NIL, NIL, NIL, NIL,
       NIL},  //< ASCII VT,  Morse code: TNew paragraph.
      {'.', '-', '.', '-', '.', NIL, NIL, NIL, NIL,
       NIL},  //< ASCII FF,  Morse code: TNew Page
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII CR
      {'-', '.', '.', '-', '-', '-', NIL, NIL, NIL,
       NIL},  //< ASCII SO,  Morse code: Change to Wabun Mores code.
      {'.', '-', '-', '.', '.', '.', NIL, NIL, NIL,
       NIL},  //< ASCII IS,  Morse code: Non-standard return to Western Mores
              // code
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII DLE
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII DC1
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII DC2
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII DC3
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII DC4
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII NAK
      {'.', '-', '.', '.', '.', NIL, NIL, NIL, NIL,
       NIL},  //< ASCII SYN, Morse code: AS, Wait.
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII ETB
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII CAN
      {'-', '.', '.', '.', '-', '.', '-', NIL, NIL,
       NIL},  //< ASCII EM,  Morse code: Break/BRB.
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII SUB
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII ESC
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII FS
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII GS
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII RS
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII US
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII ' '
      {'.', '-', '.', '.', '-', '.', NIL, NIL, NIL, NIL},  //< ASCII '!'
      {'.', '-', '.', '.', '-', '.', NIL, NIL, NIL, NIL},  //< ASCII '\"'
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII '#'
      {'.', '.', '.', '_', '.', '-', '.', '.', NIL, NIL},  //< ASCII '$'
      {'.', '.', '.', '.', '.', '_', '.', '.', NIL, NIL},  //< ASCII '%'
      {'.', '_', '.', '.', '.', NIL, NIL, NIL, NIL, NIL},  //< ASCII '&'
      {'.', '-', '-', '-', '-', '.', NIL, NIL, NIL, NIL},  //< ASCII '\''
      {'-', '.', '-', '-', '.', '-', NIL, NIL, NIL, NIL},  //< ASCII '('
      {'.', '-', '.', '.', '-', '.', NIL, NIL, NIL,
       NIL},  //< ASCII ')', Nonstandard, inverse of '('
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII '*'
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII '+'
      {'-', '-', '.', '.', '-', '-', NIL, NIL, NIL, NIL},  //< ASCII ','
      {'-', '.', '.', '.', '.', '-', NIL, NIL, NIL, NIL},  //< ASCII '-'
      {'.', '-', '.', '-', '.', '-', NIL, NIL, NIL, NIL},  //< ASCII '.'
      {'-', '.', '.', '-', '.', NIL, NIL, NIL, NIL, NIL},  //< ASCII '/'
      {'-', '-', '-', '-', '-', NIL, NIL, NIL, NIL, NIL},  //< ASCII '0'
      {'.', '-', '-', '-', NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII '1'
      {'.', '.', '-', '-', '-', NIL, NIL, NIL, NIL, NIL},  //< ASCII '2'
      {'.', '.', '.', '-', '-', NIL, NIL, NIL, NIL, NIL},  //< ASCII '3'
      {'.', '.', '.', '.', '-', NIL, NIL, NIL, NIL, NIL},  //< ASCII '4'
      {'.', '.', '.', '.', '.', NIL, NIL, NIL, NIL, NIL},  //< ASCII '5'
      {'-', '.', '.', '.', NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII '6'
      {'-', '-', '.', '.', '.', NIL, NIL, NIL, NIL, NIL},  //< ASCII '7'
      {'-', '-', '-', '.', '.', NIL, NIL, NIL, NIL, NIL},  //< ASCII '8'
      {'-', '-', '-', '-', '.', NIL, NIL, NIL, NIL, NIL},  //< ASCII '9'
      {'-', '-', '-', '.', '.', '.', NIL, NIL, NIL, NIL},  //< ASCII ':'
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII ','
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII '<'
      {'-', '.', '.', '.', '-', NIL, NIL, NIL, NIL, NIL},  //< ASCII '='
      {NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII '>'
      {'.', '.', '-', '-', '.', '.', NIL, NIL, NIL, NIL},  //< ASCII '?'
      {'.', '-', '-', '.', '-', '.', NIL, NIL, NIL, NIL},  //< ASCII '@'
      {'.', '-', NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'A'
      {'-', '.', '.', '.', NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'B'
      {'-', '.', '-', '.', NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'C'
      {'-', '.', '.', NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'D'
      {'.', NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'E'
      {'.', '.', '-', '.', NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'F'
      {'-', '-', '.', NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'G'
      {'.', '.', '.', '.', NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'H'
      {'.', '.', NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'I'
      {'.', '-', '-', '-', NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'J'
      {'-', '.', '-', NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'K'
      {'.', '-', '.', '.', NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'L'
      {'-', '-', NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'M'
      {'-', '.', NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'N'
      {'-', '-', '-', NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'O'
      {'.', '-', '-', '.', NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'P'
      {'-', '-', '.', '-', NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'Q'
      {'.', '-', '.', NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'R'
      {'.', '.', '.', NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'S'
      {'-', NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'T'
      {'.', '.', '-', NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'u'
      {'.', '.', '.', '-', NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'V'
      {'.', '-', '-', NIL, NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'W'
      {'-', '.', '.', '-', NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'X'
      {'-', '.', '-', '-', NIL, NIL, NIL, NIL, NIL, NIL},  //< ASCII 'Y'
      {'-', '-', '.', '.', NIL, NIL, NIL, NIL, NIL, NIL}   //< ASCII 'Z'
  };

  if (code < 0) return 0;
  if (code >= 'a' && code <= 'z')
    code -= 'a' - 'A';  //< Covert from lowercase to upper case if need be.
  if (code > 'Z') {
    switch (code) {
      case -4:
        return ".-.-";  //< 132
      case -5:
        return ".--.-";  //< 133
      case -6:
        return ".--.-";  //< 134
      case -16:
        return "..-..";  //< 144
      case -37:
        return "--.--";  //< 165
      case -25:
        return "---.";  //< 153
      case -26:
        return "..--";  //< 154
      default:
        return 0;
    }
  }
  return &cypher[code][0];
}

}  //< namespace _

#endif  //< _CrabsMorseCodeHXX
