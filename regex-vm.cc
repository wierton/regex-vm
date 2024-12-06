#include <assert.h>
#include <iostream>
#include <list>
#include <set>
#include <stdio.h>
#include <vector>
using namespace std;

// #define DEBUG
#ifdef DEBUG
#  define ONDBG(...) __VA_ARGS__
#else
#  define ONDBG(...)
#endif

enum vop_t {
  vop_split,
  vop_jump,
  vop_match,
  vop_accept,
};

struct vinstr_t {
  vop_t op;
  int t1, t2;
};

ostream &operator<<(ostream &os, vinstr_t vinstr) {
  auto [op, t1, t2] = vinstr;
  switch (op) {
  case vop_split: os << "split " << t1 << "," << t2; break;
  case vop_jump: os << "jump " << t1; break;
  case vop_match: os << "match " << (char)t1; break;
  case vop_accept: os << "accept"; break;
  default: assert(0);
  }
  return os;
}

template <template <class, class...> class C>
ostream &operator<<(
    ostream &os, const C<vinstr_t> &vinstrs) {
  os << "[";
  for (auto it = vinstrs.begin(); it != vinstrs.end();) {
    os << *it;
    ++it;
    if (it != vinstrs.end()) os << ";";
  }
  os << "]";
  return os;
}

bool match(
    const vector<vinstr_t> &vops, string_view input) {
  vector<pair<int, int>> stack;
  stack.emplace_back(0, 0);
  set<pair<int, int>> visited;
  while (stack.size()) {
    auto [idx, off] = stack.back();
    stack.pop_back();

    if (visited.find(pair<int, int>{idx, off}) !=
        visited.end())
      continue;
    visited.insert(pair<int, int>{idx, off});

    while (true) {
      auto [op, t1, t2] = vops[idx];
      switch (op) {
      case vop_split:
        stack.emplace_back(idx + t2, off);
        idx += t1;
        break;
      case vop_jump: idx += t1; break;
      case vop_match:
        if (off >= input.size() || (char)t1 != input[off])
          goto fail;
        off++, idx++;
        break;
      case vop_accept:
        if (off == input.size()) return true;
        goto fail;
      }
    }
  fail:;
  }
  return false;
}

vector<vinstr_t> parse(string_view input) {
  ONDBG(cout << "\n--- parsing " << input << endl);
  vector<char> ops;
  vector<list<vinstr_t>> instrs;

  auto pop = [&]() {
    ONDBG(cout << "+-- ");
    char op = ops.back();
    ops.pop_back();
    switch (op) {
    case '@': {
      assert(instrs.size() >= 2);
      auto rhs = move(instrs.back());
      instrs.pop_back();
      auto lhs = move(instrs.back());
      instrs.pop_back();
      ONDBG(cout << lhs << " .@. " << rhs << " -> ");
      lhs.splice(lhs.end(), rhs);
      ONDBG(cout << lhs << endl);
      instrs.push_back(move(lhs));
    } break;
    case '|': {
      assert(instrs.size() >= 2);
      auto rhs = move(instrs.back());
      instrs.pop_back();
      auto lhs = move(instrs.back());
      instrs.pop_back();
      ONDBG(cout << lhs << " .|. " << rhs << " -> ");
      list<vinstr_t> result;
      result.emplace_back(
          vinstr_t{vop_split, 1, (int)lhs.size() + 2});
      result.splice(result.end(), lhs);
      result.emplace_back(
          vinstr_t{vop_jump, (int)rhs.size() + 1});
      result.splice(result.end(), rhs);
      ONDBG(cout << result << endl);
      instrs.push_back(move(result));
    } break;
    case '+': {
      assert(instrs.size() >= 1);
      auto result = move(instrs.back());
      instrs.pop_back();
      ONDBG(cout << result << " .+ -> ");
      result.emplace_back(
          vinstr_t{vop_split, 1, -(int)result.size()});
      ONDBG(cout << result << endl);
      instrs.push_back(move(result));
    } break;
    case '*': {
      assert(instrs.size() >= 1);
      auto sub = move(instrs.back());
      instrs.pop_back();
      ONDBG(cout << sub << " .* -> ");
      auto subsize = sub.size();
      list<vinstr_t> result;
      result.emplace_back(
          vinstr_t{vop_split, 1, (int)sub.size() + 1});
      result.splice(result.end(), sub);
      result.emplace_back(
          vinstr_t{vop_split, 1, -(int)subsize});
      ONDBG(cout << result << endl);
      instrs.push_back(move(result));
    } break;
    }
  };

  auto is_normal = [](char ch) {
    return ch != '|' && ch != '(' && ch != ')' &&
           ch != '+' && ch != '*';
  };

  char prevch = 0;
  for (char ch : input) {
    /* needs glue check */
    if (prevch != 0) {
      if ((is_normal(prevch) && is_normal(ch)) ||
          (is_normal(prevch) && ch == '(') ||
          (prevch == '+' && is_normal(ch)) ||
          (prevch == '+' && ch == '(') ||
          (prevch == '*' && is_normal(ch)) ||
          (prevch == '*' && ch == '(') ||
          (prevch == ')' && ch == '(') ||
          (prevch == ')' && is_normal(ch))) {
        while (ops.size() && ops.back() == '@') pop();
        ops.push_back('@');
      } else if ((prevch == '(' && ch == '|') ||
                 (prevch == '|' && ch == '|') ||
                 (prevch == '|' && ch == ')') ||
                 (prevch == '(' && ch == ')')) {
        instrs.emplace_back(); // epsilon
      }
    }

    ONDBG(cout << "^ processing " << ch << endl);

    if (ch == '(') {
      ops.push_back(ch);
    } else if (ch == ')') {
      while (ops.size() && ops.back() != '(') pop();
      if (ops.size() && ops.back() == '(') ops.pop_back();
    } else if (ch == '|') {
      while (ops.size() &&
             (ops.back() == '@' || ops.back() == '|') &&
             ops.back() != '(')
        pop();
      ops.push_back(ch);
    } else if (ch == '*' || ch == '+') {
      ops.push_back(ch);
      pop();
    } else {
      instrs.emplace_back();
      instrs.back().emplace_back(vinstr_t{vop_match, ch});
    }

#ifdef DEBUG
    cout << "+-- ops:";
    for (auto &op : ops) cout << " " << (char)op;
    cout << endl;

    cout << "`-- instrs:";
    for (auto &il : instrs) cout << " " << il;
    cout << endl;
#endif

    prevch = ch;
  }

  while (ops.size()) pop();

  assert(instrs.size() == 1);

  vector<vinstr_t> ret;
  for (auto &instr : instrs[0]) {
    ret.push_back(move(instr));
  }
  ret.emplace_back(vinstr_t{vop_accept});
  return ret;
}

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
