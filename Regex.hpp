// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsRegexHPP
#define _CrabsRegexHPP
#include "String.hpp"
#if _Seam >= _CrabsRegEx
#include "AType.h"
#if _Seam == _CrabsRegEx
#include "_Debug.h"
#else
#include "_Release.h"
#endif
/* @defgroup Regex
ASCII Regex parser
A regex pattern compiler that parses patterns into an AST
of RegexNode items. Uses free-function API (no member
functions) with TRegex as the POD wrapper. The parser is
mutually recursive: ParseExpression -> ParseSequence ->
ParseAtom -> ParseClass. Uses ISN for indices (ISZ is
template-param only).
@{ */

namespace _ {

enum {
  RegexNodeCountMax = 1024,   //< Max AST nodes.
  RegexClassCharMax = 64,     //< Max chars in a character class.
  RegexGroupCountMax = 32,    //< Max capture groups.
  RegexPatternLengthMax = 256 //< Max regex pattern length.
};

// Regex AST node types
enum RegexNodeType : DTB {
  REGEX_NODE_LITERAL = 0,   //< Single character
  REGEX_NODE_DOT,           //< . - any character
  REGEX_NODE_CLASS,         //< [abc] - character class
  REGEX_NODE_GROUP,         //< (...) - grouping
  REGEX_NODE_QUANTIFIER,    //< *, +, ?
  REGEX_NODE_ANCHOR,        //< ^ or $
  REGEX_NODE_ALTERNATION,   //< |
  REGEX_NODE_SEQUENCE,      //< Concatenation of nodes
  REGEX_NODE_EMPTY,         //< Empty match
  REGEX_NODE_END,           //< End of pattern
  RegexNodeTypeCount
};

// Regex options
enum RegexOptions : DTB {
  REGEX_OPT_CASE_INSENSITIVE = 0x01,
  REGEX_OPT_MULTILINE        = 0x02,
  REGEX_OPT_DOTALL           = 0x04,
  REGEX_OPT_EXTENDED         = 0x08
};

// Character class
struct RegexCharClass {
  CHA chars[RegexClassCharMax];
  ISN charc;
  BOL negated;
};

// Regex AST node
struct RegexNode {
  RegexNodeType type;
  CHA value;
  RegexCharClass class_;
  ISN child_count;
  ISN children[4];
  ISN quant_min;
  ISN quant_max;
  ISN group_index;
};

// Compiled regex pattern
struct RegexCompiled {
  RegexNode nodes[RegexNodeCountMax];
  ISN node_count;
  ISN root_index;
  ISN group_count;
  RegexCharClass groups[RegexGroupCountMax];
  DTB options;
};

// Regex parser state
struct RegexParser {
  const CHA* pattern;
  ISN pattern_len;
  ISN pos;
  RegexCompiled compiled;
  ISN error_pos;
  BOL error;
};

// Regex wrapper
struct TRegex {
  CHA pattern[RegexPatternLengthMax];
  DTB options;
  RegexCompiled* compiled;
  RegexParser parser;
};

/* Forward declarations for mutually recursive parse functions. */
inline ISN RegexParseExpression(RegexParser* parser);
inline ISN RegexParseSequence(RegexParser* parser);
inline ISN RegexParseAtom(RegexParser* parser);
inline ISN RegexParseClass(RegexParser* parser);

/* Zeros out the given bytes. */
inline void RegexZero(void* pointer, ISN bytes) {
  IUA* p = TRef<IUA>(pointer);
  for (ISN i = 0; i < bytes; ++i) p[i] = 0;
}

/* Initializes the parser. */
inline void RegexParserInit(RegexParser* parser) {
  parser->pattern     = _NilP;
  parser->pattern_len = 0;
  parser->pos         = 0;
  parser->error_pos   = 0;
  parser->error       = false;
  RegexZero(&parser->compiled, sizeof(RegexCompiled));
}

/* Initializes the Regex wrapper. */
inline void RegexInit(TRegex* regex) {
  regex->options  = 0;
  regex->compiled = _NilP;
  RegexZero(regex->pattern, sizeof(regex->pattern));
  RegexParserInit(&regex->parser);
}

/* Gets a pointer to the nodes array. */
inline RegexNode* RegexNodes(RegexCompiled* compiled) {
  return compiled->nodes;
}

/* Adds a node to the compiled pattern. */
inline ISN RegexAddNode(RegexParser* parser, RegexNodeType type,
                        CHA value = 0, ISN quant_min = 0,
                        ISN quant_max = 0, ISN class_type = 0,
                        ISN anchor_type = 0) {
  ISN idx = parser->compiled.node_count;
  if (idx >= RegexNodeCountMax) {
    parser->error     = true;
    parser->error_pos = parser->pos;
    return -1;
  }

  RegexNode* nodes = RegexNodes(&parser->compiled);
  nodes[idx].type         = type;
  nodes[idx].value        = value;
  nodes[idx].child_count  = 0;
  nodes[idx].quant_min    = quant_min;
  nodes[idx].quant_max    = quant_max;
  nodes[idx].group_index  = 0;
  nodes[idx].class_.charc = 0;
  nodes[idx].class_.negated = false;

  if (class_type) {
    nodes[idx].class_.negated = true;
  }
  if (anchor_type) {
    nodes[idx].value = (CHA)anchor_type;
  }

  parser->compiled.node_count++;
  return idx;
}

/* Parses a character class. */
inline ISN RegexParseClass(RegexParser* parser) {
  parser->pos++;
  ISN class_idx = RegexAddNode(parser, REGEX_NODE_CLASS);
  RegexNode* nodes = RegexNodes(&parser->compiled);
  RegexCharClass* cls = &nodes[class_idx].class_;
  cls->charc   = 0;
  cls->negated = false;

  if (parser->pos < parser->pattern_len &&
      parser->pattern[parser->pos] == '^') {
    cls->negated = true;
    parser->pos++;
  }

  CHA last_char = 0;
  BOL in_range  = false;

  while (parser->pos < parser->pattern_len &&
         parser->pattern[parser->pos] != ']') {
    CHA c = parser->pattern[parser->pos];

    if (c == '\\' && parser->pos + 1 < parser->pattern_len) {
      parser->pos++;
      c = parser->pattern[parser->pos];
      if (c == 'd') c = '0';
      else if (c == 'w') c = 'a';
      else if (c == 's') c = ' ';
      parser->pos++;
    }

    if (in_range && parser->pos + 2 < parser->pattern_len &&
        parser->pattern[parser->pos + 1] == '-' &&
        parser->pattern[parser->pos + 2] != ']') {
      parser->pos += 2;
      CHA range_end = parser->pattern[parser->pos];
      parser->pos++;
      for (CHA ch = last_char; ch <= range_end &&
           cls->charc < RegexClassCharMax; ch++) {
        cls->chars[cls->charc++] = ch;
      }
      in_range = false;
    } else {
      if (cls->charc < RegexClassCharMax) {
        cls->chars[cls->charc++] = c;
      }
      last_char = c;
      in_range  = true;
    }

    parser->pos++;
  }

  if (parser->pos >= parser->pattern_len) {
    parser->error     = true;
    parser->error_pos = parser->pos;
    return -1;
  }

  parser->pos++;
  return class_idx;
}

/* Parses an atom. */
inline ISN RegexParseAtom(RegexParser* parser) {
  if (parser->pos >= parser->pattern_len) {
    parser->error     = true;
    parser->error_pos = parser->pos;
    return -1;
  }

  CHA c = parser->pattern[parser->pos];

  if (c == '(') {
    parser->pos++;
    ISN group_idx = RegexParseExpression(parser);
    if (parser->error) return -1;

    if (parser->pos >= parser->pattern_len ||
        parser->pattern[parser->pos] != ')') {
      parser->error     = true;
      parser->error_pos = parser->pos;
      return -1;
    }
    parser->pos++;

    ISN group_node_idx = RegexAddNode(parser, REGEX_NODE_GROUP);
    RegexNode* nodes   = RegexNodes(&parser->compiled);
    nodes[group_node_idx].children[0] = group_idx;
    nodes[group_node_idx].child_count = 1;
    nodes[group_node_idx].group_index = parser->compiled.group_count;
    parser->compiled.group_count++;
    return group_node_idx;
  }

  if (c == '[') {
    return RegexParseClass(parser);
  }

  if (c == '^') {
    parser->pos++;
    return RegexAddNode(parser, REGEX_NODE_ANCHOR, 0, 0, 0, 0, 1);
  }

  if (c == '$') {
    parser->pos++;
    return RegexAddNode(parser, REGEX_NODE_ANCHOR, 0, 0, 0, 0, 2);
  }

  if (c == '\\') {
    parser->pos++;
    if (parser->pos >= parser->pattern_len) {
      parser->error     = true;
      parser->error_pos = parser->pos;
      return -1;
    }
    CHA escaped = parser->pattern[parser->pos];
    parser->pos++;

    CHA value = 0;
    switch (escaped) {
      case 'n':  value = '\n'; break;
      case 't':  value = '\t'; break;
      case 'r':  value = '\r'; break;
      case '\\': value = '\\'; break;
      case 'd':  return RegexAddNode(parser, REGEX_NODE_CLASS, 0, 0, 0, 1, 0);
      case 'w':  return RegexAddNode(parser, REGEX_NODE_CLASS, 0, 0, 0, 2, 0);
      case 's':  return RegexAddNode(parser, REGEX_NODE_CLASS, 0, 0, 0, 3, 0);
      default:   value = escaped; break;
    }

    return RegexAddNode(parser, REGEX_NODE_LITERAL, value);
  }

  if (c == '.') {
    parser->pos++;
    return RegexAddNode(parser, REGEX_NODE_DOT);
  }

  parser->pos++;
  return RegexAddNode(parser, REGEX_NODE_LITERAL, c);
}

/* Parses a sequence. */
inline ISN RegexParseSequence(RegexParser* parser) {
  ISN first = -1;
  ISN last  = -1;

  while (parser->pos < parser->pattern_len &&
         parser->pattern[parser->pos] != ')' &&
         parser->pattern[parser->pos] != '|') {
    ISN node = RegexParseAtom(parser);
    if (parser->error) return -1;

    if (first == -1) {
      first = node;
      last  = node;
    } else {
      ISN seq_idx = RegexAddNode(parser, REGEX_NODE_SEQUENCE);
      RegexNode* nodes = RegexNodes(&parser->compiled);
      nodes[seq_idx].children[0]   = last;
      nodes[seq_idx].children[1]   = node;
      nodes[seq_idx].child_count   = 2;
      last = seq_idx;
    }
  }

  return first;
}

/* Parses an expression. */
inline ISN RegexParseExpression(RegexParser* parser) {
  ISN left = RegexParseSequence(parser);
  if (parser->error) return -1;

  if (parser->pos < parser->pattern_len &&
      parser->pattern[parser->pos] == '|') {
    parser->pos++;
    ISN right = RegexParseExpression(parser);
    if (parser->error) return -1;

    ISN alt_idx = RegexAddNode(parser, REGEX_NODE_ALTERNATION);
    RegexNode* nodes = RegexNodes(&parser->compiled);
    nodes[alt_idx].children[0]   = left;
    nodes[alt_idx].children[1]   = right;
    nodes[alt_idx].child_count   = 2;
    return alt_idx;
  }

  return left;
}

/* Parses the pattern. */
inline RegexCompiled* RegexParserParse(RegexParser* parser,
                                       const CHA* pattern_str,
                                       DTB opts = 0) {
  if (IsError(pattern_str)) {
    parser->error     = true;
    parser->error_pos = 0;
    return _NilP;
  }

  parser->pattern     = pattern_str;
  parser->pattern_len = 0;
  while (pattern_str[parser->pattern_len]) parser->pattern_len++;
  parser->pos         = 0;
  parser->error       = false;
  parser->error_pos   = 0;
  parser->compiled.node_count  = 0;
  parser->compiled.group_count = 0;
  parser->compiled.options     = opts;

  ISN root = RegexParseExpression(parser);
  if (parser->error) return _NilP;
  parser->compiled.root_index = root;

  if (parser->pos < parser->pattern_len) {
    parser->error     = true;
    parser->error_pos = parser->pos;
    return _NilP;
  }

  return &parser->compiled;
}

/* Parses the pattern string into the Regex wrapper. */
inline RegexCompiled* RegexParse(TRegex* regex, const CHA* pattern_str,
                                 DTB opts = 0) {
  if (IsError(pattern_str)) return _NilP;

  ISN len = 0;
  while (pattern_str[len] && len < RegexPatternLengthMax - 1) {
    regex->pattern[len] = pattern_str[len];
    len++;
  }
  regex->pattern[len] = 0;
  regex->options      = opts;

  regex->compiled = RegexParserParse(&regex->parser, regex->pattern, opts);
  return regex->compiled;
}

/* Matches the pattern against the text. */
inline BOL RegexMatch(TRegex* regex, const CHA* text,
                      const CHA** match_end = _NilP) {
  if (IsError(regex->compiled)) return false;
  return false;
}

}  //< namespace _

/* @} */  // End of Regex group.
#endif  //< _Seam >= _CrabsRegEx
#endif  //< _CrabsRegexHPP
