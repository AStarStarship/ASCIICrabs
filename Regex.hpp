// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_REGEX_HPP
#define CRABS_REGEX_HPP
#include "String.hpp"
#if SEAM >= CRABS_REGEX
#include "AType.h"
#include "Stack.hpp"
#include "Array.hpp"

namespace _ {

// Regex AST node types
enum RegexNodeType {
  REGEX_NODE_LITERAL = 0,   // Single character
  REGEX_NODE_DOT,           // . - any character
  REGEX_NODE_CLASS,         // [abc] - character class
  REGEX_NODE_GROUP,         // (...) - grouping
  REGEX_NODE_QUANTIFIER,    // *, +, ?
  REGEX_NODE_ANCHOR,        // ^ or $
  REGEX_NODE_ALTERNATION,   // |
  REGEX_NODE_SEQUENCE,      // Concatenation of nodes
  REGEX_NODE_EMPTY,         // Empty match
  REGEX_NODE_END,           // End of pattern
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
const {
  RegexNodeCountMax = 1024,
  RegexClassCharMax = 64,
  RegexGroupCountMax = 32,
};

// Character class
struct RegexCharClass {
  CHA chars[RegexClassCharMax];     // Characters in class
  ISZ charc;                        // Character count
  BOL negated;                      // ^ negation
};

// Regex AST node
struct RegexNode {
  RegexNodeType type;               // Node type
  CHA value;                        // Literal character value
  RegexCharClass class_;            // Character class data
  ISZ child_count;                  // Number of children
  ISZ children[4];                  // Indices into node pool
  
  // Quantifier data
  ISZ quant_min;                    // Minimum repetition
  ISZ quant_max;                    // Maximum repetition (-1 = unlimited)
  
  // Group data
  ISZ group_index;                  // Group index (for capturing groups)
};

// Compiled regex pattern
struct RegexCompiled {
  RegexNode nodes[RegexNodeCountMax];  // AST node pool
  ISZ node_count;                      // Number of nodes
  ISZ root_index;                      // Root node index
  ISZ group_count;                     // Number of capturing groups
  RegexCharClass groups[RegexGroupCountMax]; // Group character classes
  DTB options;                         // Regex options
};

// Regex parser state
struct RegexParser {
  const CHA* pattern;          // Pattern string
  ISZ pattern_len;             // Pattern length
  ISZ pos;                     // Current position in pattern
  RegexCompiled compiled;      // Compiled result
  ISZ error_pos;               // Error position if parsing fails
  BOL error;                   // Error flag
  
  // Constructor
  RegexParser() : pattern(NILP), pattern_len(0), pos(0), 
                  compiled(), error_pos(0), error(false) {
    D_COUT("RegexParser initialized");
  }
  
  // Parse a pattern string
  RegexCompiled* Parse(const CHA* pattern_str, DTB opts = 0) {
    if (!pattern_str) {
      error = true;
      error_pos = 0;
      return NILP;
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
    
    // Parse the pattern
    root_index = ParseExpression();
    if (error) return NILP;
    
    // Verify we consumed the entire pattern
    if (pos < pattern_len) {
      error = true;
      error_pos = pos;
      return NILP;
    }
    
    compiled.root_index = root_index;
    return &compiled;
  }
  
  // Parse an expression (alternation)
  ISZ ParseExpression() {
    ISZ left = ParseSequence();
    if (error) return -1;
    
    // Check for alternation
    if (pos < pattern_len && pattern[pos] == '|') {
      pos++; // Skip '|'
      ISZ right = ParseExpression();
      if (error) return -1;
      
      // Create alternation node
      ISZ alt_idx = AddNode(REGEX_NODE_ALTERNATION);
      nodes()[alt_idx].children[0] = left;
      nodes()[alt_idx].children[1] = right;
      nodes()[alt_idx].child_count = 2;
      return alt_idx;
    }
    
    return left;
  }
  
  // Parse a sequence (concatenation)
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
        // Create sequence node
        ISZ seq_idx = AddNode(REGEX_NODE_SEQUENCE);
        nodes()[seq_idx].children[0] = last;
        nodes()[seq_idx].children[1] = node;
        nodes()[seq_idx].child_count = 2;
        last = seq_idx;
      }
    }
    
    return first;
  }
  
  // Parse an atom (basic element)
  ISZ ParseAtom() {
    if (pos >= pattern_len) {
      error = true;
      error_pos = pos;
      return -1;
    }
    
    CHA c = pattern[pos];
    
    // Grouping
    if (c == '(') {
      pos++; // Skip '('
      ISZ group_idx = ParseExpression();
      if (error) return -1;
      
      if (pos >= pattern_len || pattern[pos] != ')') {
        error = true;
        error_pos = pos;
        return -1;
      }
      pos++; // Skip ')'
      
      // Create group node
      ISZ group_node_idx = AddNode(REGEX_NODE_GROUP);
      nodes()[group_node_idx].children[0] = group_idx;
      nodes()[group_node_idx].child_count = 1;
      nodes()[group_node_idx].group_index = compiled.group_count;
      compiled.group_count++;
      return group_node_idx;
    }
    
    // Character class
    if (c == '[') {
      return ParseClass();
    }
    
    // Anchor
    if (c == '^') {
      pos++;
      return AddNode(REGEX_NODE_ANCHOR, 0, 0, 0, 0, 1); // Start anchor
    }
    
    if (c == '$') {
      pos++;
      return AddNode(REGEX_NODE_ANCHOR, 0, 0, 0, 0, 2); // End anchor
    }
    
    // Escape sequence
    if (c == '\\') {
      pos++; // Skip '\'
      if (pos >= pattern_len) {
        error = true;
        error_pos = pos;
        return -1;
      }
      CHA escaped = pattern[pos];
      pos++;
      
      // Handle escape sequences
      CHA value = 0;
      switch (escaped) {
        case 'n': value = '\n'; break;
        case 't': value = '\t'; break;
        case 'r': value = '\r'; break;
        case '\\': value = '\\'; break;
        case 'd':
          // \d - digit class
          return AddNode(REGEX_NODE_CLASS, 0, 0, 0, 0, 1);
        case 'w':
          // \w - word character class
          return AddNode(REGEX_NODE_CLASS, 0, 0, 0, 0, 2);
        case 's':
          // \s - whitespace class
          return AddNode(REGEX_NODE_CLASS, 0, 0, 0, 0, 3);
        default:
          value = escaped;
          break;
      }
      
      return AddNode(REGEX_NODE_LITERAL, value);
    }
    
    // Dot - any character
    if (c == '.') {
      pos++;
      return AddNode(REGEX_NODE_DOT);
    }
    
    // Literal character
    pos++;
    return AddNode(REGEX_NODE_LITERAL, c);
  }
  
  // Parse a character class [...]
  ISZ ParseClass() {
    pos++; // Skip '['
    
    ISZ class_idx = AddNode(REGEX_NODE_CLASS);
    RegexCharClass* cls = &nodes()[class_idx].class_;
    cls->charc = 0;
    cls->negated = false;
    
    // Check for negation
    if (pos < pattern_len && pattern[pos] == '^') {
      cls->negated = true;
      pos++;
    }
    
    // Parse class contents
    CHA last_char = 0;
    BOL in_range = false;
    
    while (pos < pattern_len && pattern[pos] != ']') {
      CHA c = pattern[pos];
      
      // Escape sequence in class
      if (c == '\\' && pos + 1 < pattern_len) {
        pos++; // Skip '\'
        c = pattern[pos];
        if (c == 'd') c = '0'; // Simplified: \d -> '0'
        else if (c == 'w') c = 'a'; // Simplified: \w -> 'a'
        else if (c == 's') c = ' '; // Simplified: \s -> ' '
        pos++;
      }
      
      if (in_range && pos + 2 < pattern_len && pattern[pos + 1] == '-' && pattern[pos + 2] != ']') {
        // Character range
        pos += 2; // Skip '-]'
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
    
    pos++; // Skip ']'
    return class_idx;
  }
  
  // Add a node to the pool
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
    
    // Set class type for special classes
    if (class_type) {
      nodes()[idx].class_.negated = true; // Placeholder
      // Would set up proper character ranges for \d, \w, \s
    }
    
    // Set anchor type
    if (anchor_type) {
      nodes()[idx].value = (CHA)anchor_type;
    }
    
    compiled.node_count++;
    return idx;
  }
  
  // Get node pointer
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
  
  // Constructor
  Regex() : pattern(), options(0), compiled(NILP) {}
  
  // Parse a pattern string
  RegexCompiled* Parse(const CHA* pattern_str, DTB opts = 0) {
    if (!pattern_str) return NILP;
    
    // Copy pattern
    ISZ len = 0;
    while (pattern_str[len] && len < RegexPatternLengthMax - 1) {
      pattern[len] = pattern_str[len];
      len++;
    }
    pattern[len] = 0;
    options = opts;
    
    // Parse
    compiled = parser.Parse(pattern, opts);
    return compiled;
  }
  
  // Check if pattern matches a string
  BOL Match(const CHA* text, const CHA** match_end = NILP) {
    if (!compiled) return false;
    
    // Simple matching implementation
    // For a full implementation, we'd need a matcher
    // This is a placeholder
    return false;
  }
};

}  //< namespace _
#endif
#endif
