#include "regex-vm.hpp"

using namespace regvm;

void run_tests() {
  // Basic character matching
  assert(match(parse("a"), "a") == true);
  assert(match(parse("a"), "b") == false);
  assert(match(parse("abc"), "abc") == true);
  assert(match(parse("abc"), "abd") == false);

  // Alternation tests
  assert(match(parse("a|b"), "a") == true);
  assert(match(parse("a|b"), "b") == true);
  assert(match(parse("a|b"), "c") == false);
  assert(match(parse("cat|dog"), "cat") == true);
  assert(match(parse("cat|dog"), "dog") == true);
  assert(match(parse("cat|dog"), "rat") == false);

  // Kleene star tests
  assert(match(parse("a*"), "") == true);
  assert(match(parse("a*"), "a") == true);
  assert(match(parse("a*"), "aaa") == true);
  assert(match(parse("a*"), "b") == false);
  assert(match(parse("(ab)*"), "") == true);
  assert(match(parse("(ab)*"), "ab") == true);
  assert(match(parse("(ab)*"), "abab") == true);
  assert(match(parse("(ab)*"), "aba") == false);

  // Plus operator tests
  assert(match(parse("a+"), "") == false);
  assert(match(parse("a+"), "a") == true);
  assert(match(parse("a+"), "aaa") == true);
  assert(match(parse("(ab)+"), "") == false);
  assert(match(parse("(ab)+"), "ab") == true);
  assert(match(parse("(ab)+"), "abab") == true);

  // Complex patterns
  assert(match(parse("(a|b)*abb"), "abb") == true);
  assert(match(parse("(a|b)*abb"), "aabb") == true);
  assert(match(parse("(a|b)*abb"), "babb") == true);
  assert(match(parse("(a|b)*abb"), "ababb") == true);
  assert(match(parse("(a|b)*abb"), "abc") == false);

  // Nested groups
  assert(match(parse("((a|b)+)(c|d)"), "ac") == true);
  assert(match(parse("((a|b)+)(c|d)"), "bd") == true);
  assert(match(parse("((a|b)+)(c|d)"), "aaac") == true);
  assert(match(parse("((a|b)+)(c|d)"), "bbbd") == true);
  assert(match(parse("((a|b)+)(c|d)"), "e") == false);

  // Mixed operators
  assert(match(parse("a+(b|c)*d"), "ad") == true);
  assert(match(parse("a+(b|c)*d"), "abd") == true);
  assert(match(parse("a+(b|c)*d"), "abcd") == true);
  assert(match(parse("a+(b|c)*d"), "aaaabbbcd") == true);
  assert(match(parse("a+(b|c)*d"), "bcd") == false);

  // Edge cases
  assert(match(parse("(a|)*"), "") == true);
  assert(match(parse("(a|)*"), "a") == true);
  assert(match(parse("((a)|)*"), "aaa") == true);
  assert(match(parse("()*"), "") == true);

  // Long patterns
  assert(
      match(parse("(ab|cd)*(ef|gh)+"), "abcdefgh") == true);
  assert(
      match(parse("(ab|cd)*(ef|gh)+"), "ababefef") == true);
  assert(
      match(parse("(ab|cd)*(ef|gh)+"), "cdcdghgh") == true);
  assert(match(parse("(ab|cd)*(ef|gh)+"), "abcd") == false);

  cout << "All tests passed!" << endl;
}

int main() {
  run_tests();
  return 0;
}
