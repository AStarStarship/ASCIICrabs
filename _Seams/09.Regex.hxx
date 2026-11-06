// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsRegEx
#include "../Regex.hpp"
using namespace ::_;
#if _Seam == _CrabsRegEx
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
namespace CRTest {
using namespace ::_;

#if _Seam >= _CrabsRegEx
static const CHA* Regex(const CHA* args) {
  _ATestBegin;
  _DROut("Testing Regex parser...\n");

  // Test 1: Simple literal
  {
    TRegex regex;
    RegexInit(&regex);
    RegexCompiled* compiled = RegexParse(&regex, "abc");
    _AAssert(compiled != _NilP);
    _AAssert(regex.parser.error == false);
    _AAssert(regex.compiled->node_count > 0);
    _AAssert(regex.compiled->group_count == 0);
    _DROut("Test 1 (literal 'abc'): nodes=" << compiled->node_count << " OK\n");
  }

  // Test 2: Dot and literal
  {
    TRegex regex;
    RegexInit(&regex);
    RegexCompiled* compiled = RegexParse(&regex, "a.c");
    _AAssert(compiled != _NilP);
    _AAssert(regex.parser.error == false);
    _DROut("Test 2 (dot 'a.c'): nodes=" << compiled->node_count << " OK\n");
  }

  // Test 3: Character class
  {
    TRegex regex;
    RegexInit(&regex);
    RegexCompiled* compiled = RegexParse(&regex, "[abc]");
    _AAssert(compiled != _NilP);
    _AAssert(regex.parser.error == false);
    _DROut("Test 3 (class '[abc]'): nodes=" << compiled->node_count << " OK\n");
  }

  // Test 4: Group
  {
    TRegex regex;
    RegexInit(&regex);
    RegexCompiled* compiled = RegexParse(&regex, "(abc)");
    _AAssert(compiled != _NilP);
    _AAssert(regex.parser.error == false);
    _AAssert(regex.compiled->group_count == 1);
    _DROut("Test 4 (group '(abc)'): nodes=" << compiled->node_count
           << " groups=" << compiled->group_count << " OK\n");
  }

  // Test 5: Alternation
  {
    TRegex regex;
    RegexInit(&regex);
    RegexCompiled* compiled = RegexParse(&regex, "ab|cd");
    _AAssert(compiled != _NilP);
    _AAssert(regex.parser.error == false);
    _DROut("Test 5 (alt 'ab|cd'): nodes=" << compiled->node_count << " OK\n");
  }

  // Test 6: Invalid pattern (unbalanced group)
  {
    TRegex regex;
    RegexInit(&regex);
    RegexCompiled* compiled = RegexParse(&regex, "(abc");
    _AAssert(compiled == _NilP);
    _AAssert(regex.parser.error == true);
    _DROut("Test 6 (invalid '(abc'): error=" << regex.parser.error << " OK\n");
  }

  _DROut("Regex tests complete.\n");
  return _NilP;
}
#endif

}  //< namespace CRTest
