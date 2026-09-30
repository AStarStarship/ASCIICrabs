// Standalone test for Regex.hpp - NO ASCIICrabs dependencies
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

// Minimal type definitions - self-contained, no ASCIICrabs includes
typedef char CHA;
typedef uint16_t CHB;
typedef uint32_t CHC;
typedef int8_t ISA;
typedef uint8_t IUA;
typedef int16_t ISB;
typedef uint16_t IUB;
typedef int32_t ISC;
typedef uint32_t IUC;
typedef int64_t ISD;
typedef uint64_t IUD;
typedef void* ISW;
typedef uint64_t IUW;
typedef uint64_t ISZ;
typedef int32_t ISY;
typedef uint16_t DTW;
typedef uint8_t DTB;
typedef int64_t DTD;
typedef uint8_t BOL;
#define NILP ((void*)0)
#define NILP_CONST ((const void*)0)

// Regex pattern length constant
const int RegexPatternLengthMax = 256;

// Now include the regex parser (it only needs the types above)
#pragma once
#ifndef CRABS_REGEX_HPP
#define CRABS_REGEX_HPP

namespace _ {

// Regex AST node types
enum RegexNodeType {
  REGEX_NODE_LITERAL = 0,
  REGEX_NODE_DOT,
  REGEX_NODE_CLASS,
  REGEX_NODE_GROUP,
  REGEX_NODE_QUANTIFIER,
  REGEX_NODE_ANCHOR,
  REGEX_NODE_ALTERNATION,
  REGEX_NODE_SEQUENCE,
  REGEX_NODE_EMPTY,
  REGEX_NODE_END,
  RegexNodeTypeCount
};

// Regex options
enum RegexOptions : DTB {
  REGEX_OPT_CASE_INSENSITIVE = 0x01,
  REGEX_OPT_MULTILINE = 0x02,
  REGEX_OPT_DOTALL = 0x04,
  REGEX_OPT_EXTENDED = 0x08
};

// Maximum AST nodes
const int RegexNodeCountMax = 1024;
const int RegexClassCharMax = 64;
const int RegexGroupCountMax = 32;

// Character class
struct RegexCharClass {
  CHA chars[RegexClassCharMax];
  ISZ charc;
  BOL negated;
};

// Regex AST node
struct RegexNode {
  RegexNodeType type;
  CHA value;
  RegexCharClass class_;
  ISZ child_count;
  ISZ children[4];
  ISZ quant_min;
  ISZ quant_max;
  ISZ group_index;
};

// Compiled regex pattern
struct RegexCompiled {
  RegexNode nodes[RegexNodeCountMax];
  ISZ node_count;
  ISZ root_index;
  ISZ group_count;
  RegexCharClass groups[RegexGroupCountMax];
  DTB options;
};

// Regex parser state
struct RegexParser {
  const CHA* pattern;
  ISZ pattern_len;
  ISZ pos;
  RegexCompiled compiled;
  ISZ error_pos;
  BOL error;
  
  RegexParser() : pattern((const CHA*)NILP), pattern_len(0), pos(0), 
                  error_pos(0), error(false) {
    memset(&compiled, 0, sizeof(compiled));
  }
  
  RegexCompiled* Parse(const CHA* pattern_str, DTB opts = 0) {
    if (!pattern_str) {
      error = true;
      error_pos = 0;
      return (RegexCompiled*)NILP;
    }
    
    pattern = pattern_str;
    pattern_len = 0;
    while (pattern[pattern_len]) pattern_len++;
    pos = 0;
    error = false;
    error_pos = 0;
    compiled.node_count = 0;
    compiled.group_count = 0;
    compiled.options = opts;
    
    compiled.root_index = ParseExpression();
    if (error) return (RegexCompiled*)NILP;
    
    if (pos < pattern_len) {
      error = true;
      error_pos = pos;
      return (RegexCompiled*)NILP;
    }
    
    return &compiled;
  }
  
  ISZ ParseExpression() {
    ISZ left = ParseSequence();
    if (error) return -1;
    
    if (pos < pattern_len && pattern[pos] == '|') {
      pos++;
      ISZ right = ParseExpression();
      if (error) return -1;
      
      ISZ alt_idx = AddNode(REGEX_NODE_ALTERNATION);
      nodes()[alt_idx].children[0] = left;
      nodes()[alt_idx].children[1] = right;
      nodes()[alt_idx].child_count = 2;
      return alt_idx;
    }
    
    return left;
  }
  
  ISZ ParseSequence() {
    ISZ first = -1;
    ISZ last = -1;
    
    while (pos < pattern_len && pattern[pos] != ')' && pattern[pos] != '|') {
      ISZ node = ParseAtom();
      if (error) return -1;
      
      if (first == -1) {
        first = node;
        last = node;
      } else {
        ISZ seq_idx = AddNode(REGEX_NODE_SEQUENCE);
        nodes()[seq_idx].children[0] = last;
        nodes()[seq_idx].children[1] = node;
        nodes()[seq_idx].child_count = 2;
        last = seq_idx;
      }
    }
    
    return first;
  }
  
  ISZ ParseAtom() {
    if (pos >= pattern_len) {
      error = true;
      error_pos = pos;
      return -1;
    }
    
    CHA c = pattern[pos];
    
    if (c == '(') {
      pos++;
      ISZ group_idx = ParseExpression();
      if (error) return -1;
      
      if (pos >= pattern_len || pattern[pos] != ')') {
        error = true;
        error_pos = pos;
        return -1;
      }
      pos++;
      
      ISZ group_node_idx = AddNode(REGEX_NODE_GROUP);
      nodes()[group_node_idx].children[0] = group_idx;
      nodes()[group_node_idx].child_count = 1;
      nodes()[group_node_idx].group_index = compiled.group_count;
      compiled.group_count++;
      return group_node_idx;
    }
    
    if (c == '[') {
      return ParseClass();
    }
    
    if (c == '^') {
      pos++;
      return AddNode(REGEX_NODE_ANCHOR, 0, 0, 0, 0, 1);
    }
    
    if (c == '$') {
      pos++;
      return AddNode(REGEX_NODE_ANCHOR, 0, 0, 0, 0, 2);
    }
    
    if (c == '\\') {
      pos++;
      if (pos >= pattern_len) {
        error = true;
        error_pos = pos;
        return -1;
      }
      CHA escaped = pattern[pos];
      pos++;
      
      CHA value = 0;
      switch (escaped) {
        case 'n': value = '\n'; break;
        case 't': value = '\t'; break;
        case 'r': value = '\r'; break;
        case '\\': value = '\\'; break;
        case 'd':
          return AddNode(REGEX_NODE_CLASS, 0, 0, 0, 0, 1);
        case 'w':
          return AddNode(REGEX_NODE_CLASS, 0, 0, 0, 0, 2);
        case 's':
          return AddNode(REGEX_NODE_CLASS, 0, 0, 0, 0, 3);
        default:
          value = escaped;
          break;
      }
      
      return AddNode(REGEX_NODE_LITERAL, value);
    }
    
    if (c == '.') {
      pos++;
      return AddNode(REGEX_NODE_DOT);
    }
    
    pos++;
    return AddNode(REGEX_NODE_LITERAL, c);
  }
  
  ISZ ParseClass() {
    pos++;
    
    ISZ class_idx = AddNode(REGEX_NODE_CLASS);
    RegexCharClass* cls = &nodes()[class_idx].class_;
    cls->charc = 0;
    cls->negated = false;
    
    if (pos < pattern_len && pattern[pos] == '^') {
      cls->negated = true;
      pos++;
    }
    
    CHA last_char = 0;
    BOL in_range = false;
    
    while (pos < pattern_len && pattern[pos] != ']') {
      CHA c = pattern[pos];
      
      if (c == '\\' && pos + 1 < pattern_len) {
        pos++;
        c = pattern[pos];
        if (c == 'd') c = '0';
        else if (c == 'w') c = 'a';
        else if (c == 's') c = ' ';
        pos++;
      }
      
      if (in_range && pos + 2 < pattern_len && pattern[pos + 1] == '-' && pattern[pos + 2] != ']') {
        pos += 2;
        CHA range_end = pattern[pos];
        pos++;
        
        for (CHA ch = last_char; ch <= range_end && cls->charc < RegexClassCharMax; ch++) {
          cls->chars[cls->charc++] = ch;
        }
        in_range = false;
      } else {
        if (cls->charc < RegexClassCharMax) {
          cls->chars[cls->charc++] = c;
        }
        last_char = c;
        in_range = true;
      }
      
      pos++;
    }
    
    if (pos >= pattern_len) {
      error = true;
      error_pos = pos;
      return -1;
    }
    
    pos++;
    return class_idx;
  }
  
  ISZ AddNode(RegexNodeType type, CHA value = 0, 
              ISZ quant_min = 0, ISZ quant_max = 0,
              ISZ class_type = 0, ISZ anchor_type = 0) {
    ISZ idx = compiled.node_count;
    if (idx >= RegexNodeCountMax) {
      error = true;
      error_pos = pos;
      return -1;
    }
    
    nodes()[idx].type = type;
    nodes()[idx].value = value;
    nodes()[idx].child_count = 0;
    nodes()[idx].quant_min = quant_min;
    nodes()[idx].quant_max = quant_max;
    nodes()[idx].group_index = 0;
    nodes()[idx].class_.charc = 0;
    nodes()[idx].class_.negated = false;
    
    if (class_type) {
      nodes()[idx].class_.negated = true;
    }
    
    if (anchor_type) {
      nodes()[idx].value = (CHA)anchor_type;
    }
    
    compiled.node_count++;
    return idx;
  }
  
  RegexNode* nodes() {
    return compiled.nodes;
  }
};

// Regex wrapper
struct Regex {
  CHA pattern[RegexPatternLengthMax];
  DTB options;
  RegexCompiled* compiled;
  RegexParser parser;
  
  Regex() : options(0), compiled((RegexCompiled*)NILP) {
    memset(pattern, 0, sizeof(pattern));
  }
  
  RegexCompiled* Parse(const CHA* pattern_str, DTB opts = 0) {
    if (!pattern_str) return (RegexCompiled*)NILP;
    
    ISZ len = 0;
    while (pattern_str[len] && len < RegexPatternLengthMax - 1) {
      pattern[len] = pattern_str[len];
      len++;
    }
    pattern[len] = 0;
    options = opts;
    
    compiled = parser.Parse(pattern, opts);
    return compiled;
  }
  
  BOL Match(const CHA* text, const CHA** match_end = (const CHA**)NILP) {
    if (!compiled) return false;
    return false;
  }
};

}  //< namespace _
#endif  // CRABS_REGEX_HPP

// Test helper
int test_count = 0;
int pass_count = 0;

void TEST(const char* name, BOL result) {
  test_count++;
  if (result) {
    pass_count++;
    printf("  PASS: %s\n", name);
  } else {
    printf("  FAIL: %s\n", name);
  }
}

void test_literal() {
  printf("\nTest: Literal characters\n");
  _::Regex regex;
  
  _::RegexCompiled* compiled = regex.Parse("hello");
  TEST("Parse 'hello'", compiled != NILP);
  if (compiled) {
    TEST("Node count == 5", compiled->node_count == 5);
    TEST("Root is LITERAL 'h'", compiled->nodes[compiled->root_index].type == _::REGEX_NODE_LITERAL && 
         compiled->nodes[compiled->root_index].value == 'h');
  }
}

void test_dot() {
  printf("\nTest: Dot wildcard\n");
  _::Regex regex;
  
  _::RegexCompiled* compiled = regex.Parse("a.b");
  TEST("Parse 'a.b'", compiled != NILP);
  if (compiled) {
    TEST("Node count == 3", compiled->node_count == 3);
    TEST("Middle node is DOT", compiled->nodes[compiled->root_index + 1].type == _::REGEX_NODE_DOT);
  }
}

void test_class() {
  printf("\nTest: Character classes\n");
  _::Regex regex;
  
  _::RegexCompiled* compiled = regex.Parse("[abc]");
  TEST("Parse '[abc]'", compiled != NILP);
  if (compiled) {
    TEST("Node is CLASS", compiled->nodes[compiled->root_index].type == _::REGEX_NODE_CLASS);
    TEST("Has 3 chars", compiled->nodes[compiled->root_index].class_.charc == 3);
  }
}

void test_class_range() {
  printf("\nTest: Character class ranges\n");
  _::Regex regex;
  
  _::RegexCompiled* compiled = regex.Parse("[a-z]");
  TEST("Parse '[a-z]'", compiled != NILP);
  if (compiled) {
    TEST("Node is CLASS", compiled->nodes[compiled->root_index].type == _::REGEX_NODE_CLASS);
    TEST("Has 26 chars (a-z)", compiled->nodes[compiled->root_index].class_.charc == 26);
  }
}

void test_group() {
  printf("\nTest: Grouping\n");
  _::Regex regex;
  
  _::RegexCompiled* compiled = regex.Parse("(abc)");
  TEST("Parse '(abc)'", compiled != NILP);
  if (compiled) {
    TEST("Node count == 4", compiled->node_count == 4);
    TEST("Root is GROUP", compiled->nodes[compiled->root_index].type == _::REGEX_NODE_GROUP);
    TEST("Group index == 0", compiled->nodes[compiled->root_index].group_index == 0);
    TEST("Group count == 1", compiled->group_count == 1);
  }
}

void test_alternation() {
  printf("\nTest: Alternation\n");
  _::Regex regex;
  
  _::RegexCompiled* compiled = regex.Parse("a|b");
  TEST("Parse 'a|b'", compiled != NILP);
  if (compiled) {
    TEST("Root is ALTERNATION", compiled->nodes[compiled->root_index].type == _::REGEX_NODE_ALTERNATION);
    TEST("Has 2 children", compiled->nodes[compiled->root_index].child_count == 2);
  }
}

void test_anchor_start() {
  printf("\nTest: Start anchor\n");
  _::Regex regex;
  
  _::RegexCompiled* compiled = regex.Parse("^hello");
  TEST("Parse '^hello'", compiled != NILP);
  if (compiled) {
    TEST("First node is ANCHOR", compiled->nodes[compiled->root_index].type == _::REGEX_NODE_ANCHOR);
  }
}

void test_anchor_end() {
  printf("\nTest: End anchor\n");
  _::Regex regex;
  
  _::RegexCompiled* compiled = regex.Parse("hello$");
  TEST("Parse 'hello$'", compiled != NILP);
  if (compiled) {
    TEST("Last node is ANCHOR", compiled->nodes[compiled->root_index].type == _::REGEX_NODE_ANCHOR);
  }
}

void test_escape() {
  printf("\nTest: Escape sequences\n");
  _::Regex regex;
  
  _::RegexCompiled* compiled = regex.Parse("a\\nb");
  TEST("Parse 'a\\nb'", compiled != NILP);
  if (compiled) {
    TEST("Node count == 3", compiled->node_count == 3);
    TEST("Middle is newline literal", compiled->nodes[compiled->root_index + 1].value == '\n');
  }
}

void test_escape_special() {
  printf("\nTest: Special escape sequences\n");
  _::Regex regex;
  
  _::RegexCompiled* compiled = regex.Parse("\\d\\w\\s");
  TEST("Parse '\\d\\w\\s'", compiled != NILP);
  if (compiled) {
    TEST("Node count == 3", compiled->node_count == 3);
    TEST("All are CLASS nodes", 
         compiled->nodes[compiled->root_index].type == _::REGEX_NODE_CLASS &&
         compiled->nodes[compiled->root_index + 1].type == _::REGEX_NODE_CLASS &&
         compiled->nodes[compiled->root_index + 2].type == _::REGEX_NODE_CLASS);
  }
}

void test_complex() {
  printf("\nTest: Complex pattern\n");
  _::Regex regex;
  
  _::RegexCompiled* compiled = regex.Parse("^([a-z]+)@(\\w+)\\.([a-z]{2,4})$");
  TEST("Parse complex email pattern", compiled != NILP);
  if (compiled) {
    TEST("Has 3 groups", compiled->group_count == 3);
    TEST("Root is ANCHOR (^)", compiled->nodes[compiled->root_index].type == _::REGEX_NODE_ANCHOR);
  }
}

void test_empty() {
  printf("\nTest: Empty pattern\n");
  _::Regex regex;
  
  _::RegexCompiled* compiled = regex.Parse("");
  TEST("Parse empty string", compiled != NILP);
  if (compiled) {
    TEST("Node count == 0", compiled->node_count == 0);
  }
}

void test_invalid() {
  printf("\nTest: Invalid patterns\n");
  _::Regex regex;
  
  _::RegexCompiled* compiled1 = regex.Parse("(");
  TEST("Reject unclosed group", compiled1 == NILP);
  
  _::RegexCompiled* compiled2 = regex.Parse("[");
  TEST("Reject unclosed class", compiled2 == NILP);
  
  _::RegexCompiled* compiled3 = regex.Parse("a\\");
  TEST("Reject trailing backslash", compiled3 == NILP);
}

int main() {
  printf("=== Regex Parser Tests ===\n");
  
  test_literal();
  test_dot();
  test_class();
  test_class_range();
  test_group();
  test_alternation();
  test_anchor_start();
  test_anchor_end();
  test_escape();
  test_escape_special();
  test_complex();
  test_empty();
  test_invalid();
  
  printf("\n=== Results: %d/%d tests passed ===\n", pass_count, test_count);
  
  return (pass_count == test_count) ? 0 : 1;
}
