// Import Unit Testing framework, Bandit
#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

// Import Anatree
#include <anatree.h>

// -------------------------------------------------------------------------- //
// Define tests

go_bandit([]() {
  describe("insert(), tree_size(), size(), empty()", []() {
    it("can report size of Ø", []() {
      anatree<> a;

      AssertThat(a.size(), Is().EqualTo(0u));
      AssertThat(a.empty(), Is().True());

      AssertThat(a.tree_size(), Is().EqualTo(1u));
    });

    it("can report size of { '' }", []() {
      anatree<> a;
      a.insert("");

      AssertThat(a.size(), Is().EqualTo(1u));
      AssertThat(a.empty(), Is().False());

      AssertThat(a.tree_size(), Is().EqualTo(1u));
    });

    it("can report size of { 'a' }", []() {
      anatree<> a;
      a.insert("a");

      AssertThat(a.size(), Is().EqualTo(1u));
      AssertThat(a.empty(), Is().False());

      AssertThat(a.tree_size(), Is().EqualTo(3u));
    });

    it("can report size of { 'b' }", []() {
      anatree<> a;
      a.insert("b");

      AssertThat(a.size(), Is().EqualTo(1u));
      AssertThat(a.empty(), Is().False());

      AssertThat(a.tree_size(), Is().EqualTo(3u));
    });

    it("can report size of { 'a', 'b' }", []() {
      anatree<> a;
      a.insert("a");
      a.insert("b");

      AssertThat(a.size(), Is().EqualTo(2u));
      AssertThat(a.empty(), Is().False());

      AssertThat(a.tree_size(), Is().EqualTo(5u));
    });

    it("can report size of { 'b', '', 'a' }", []() {
      anatree<> a;
      a.insert("b");
      a.insert("");
      a.insert("a");

      AssertThat(a.size(), Is().EqualTo(3u));
      AssertThat(a.empty(), Is().False());

      AssertThat(a.tree_size(), Is().EqualTo(5u));
    });

    it("can report size of { 'a', '', 'b' }", []() {
      anatree<> a;
      a.insert("a");
      a.insert("");
      a.insert("b");

      AssertThat(a.size(), Is().EqualTo(3u));
      AssertThat(a.empty(), Is().False());

      AssertThat(a.tree_size(), Is().EqualTo(5u));
    });

    it("can report size of { '', 'b', 'ab', 'a' }", []() {
      anatree<> a;
      a.insert("");
      a.insert("b");
      a.insert("ab");
      a.insert("a");

      AssertThat(a.size(), Is().EqualTo(4u));
      AssertThat(a.empty(), Is().False());

      AssertThat(a.tree_size(), Is().EqualTo(7u));
    });

    it("does not increment size when inserting duplicate 'ab'", []() {
      anatree<> a;
      a.insert("a");
      a.insert("ab");
      a.insert("a");

      AssertThat(a.size(), Is().EqualTo(2u));
      AssertThat(a.empty(), Is().False());

      AssertThat(a.tree_size(), Is().EqualTo(5u));
    });

    it("does not increment size when inserting duplicate 'a'", []() {
      anatree<> a;
      a.insert("b");
      a.insert("a");
      a.insert("ab");
      a.insert("a");

      AssertThat(a.size(), Is().EqualTo(3u));
      AssertThat(a.empty(), Is().False());

      AssertThat(a.tree_size(), Is().EqualTo(7u));
    });
  });

  describe("insert(), contains()", []() {
    it("can insert { '' }", []() {
      anatree<> a;
      AssertThat(a.contains(""), Is().False());

      a.insert("");
      AssertThat(a.contains(""), Is().True());
    });

    it("can insert { 'a' }", []() {
      anatree<> a;
      AssertThat(a.contains("a"), Is().False());

      a.insert("a");
      AssertThat(a.contains("a"), Is().True());
    });

    it("does not contain '' after insert of 'a'", []() {
      anatree<> a;
      AssertThat(a.contains(""), Is().False());

      a.insert("a");
      AssertThat(a.contains(""), Is().False());
    });

    it("can insert { 'b' }", []() {
      anatree<> a;
      AssertThat(a.contains("b"), Is().False());

      a.insert("b");
      AssertThat(a.contains("b"), Is().True());
    });

    it("does not contain 'a' after insert of 'b'", []() {
      anatree<> a;
      AssertThat(a.contains("a"), Is().False());

      a.insert("b");
      AssertThat(a.contains("a"), Is().False());
    });

    it("can insert { 'a', 'b' }", []() {
      anatree<> a;
      AssertThat(a.contains("a"), Is().False());
      AssertThat(a.contains("b"), Is().False());

      a.insert("a");
      AssertThat(a.contains("a"), Is().True());
      AssertThat(a.contains("b"), Is().False());

      a.insert("b");
      AssertThat(a.contains("a"), Is().True());
      AssertThat(a.contains("b"), Is().True());
    });

    it("can insert { 'ab', 'ba' }", []() {
      anatree<> a;
      AssertThat(a.contains("ab"), Is().False());
      AssertThat(a.contains("ba"), Is().False());

      a.insert("ab");
      AssertThat(a.contains("ab"), Is().True());
      AssertThat(a.contains("ba"), Is().False());

      a.insert("ba");
      AssertThat(a.contains("ab"), Is().True());
      AssertThat(a.contains("ba"), Is().True());
    });

    it("can insert { 'ac', 'ba' }", []() {
      anatree<> a;
      AssertThat(a.contains("ac"), Is().False());
      AssertThat(a.contains("ba"), Is().False());

      a.insert("ac");
      AssertThat(a.contains("ac"), Is().True());
      AssertThat(a.contains("ba"), Is().False());

      a.insert("ba");
      AssertThat(a.contains("ac"), Is().True());
      AssertThat(a.contains("ba"), Is().True());
    });

    it("can insert { 'b', '', 'a' }", []() {
      anatree<> a;
      AssertThat(a.contains(""), Is().False());
      AssertThat(a.contains("a"), Is().False());
      AssertThat(a.contains("b"), Is().False());

      a.insert("b");
      AssertThat(a.contains(""), Is().False());
      AssertThat(a.contains("a"), Is().False());
      AssertThat(a.contains("b"), Is().True());

      a.insert("");
      AssertThat(a.contains(""), Is().True());
      AssertThat(a.contains("a"), Is().False());
      AssertThat(a.contains("b"), Is().True());

      a.insert("a");
      AssertThat(a.contains(""), Is().True());
      AssertThat(a.contains("a"), Is().True());
      AssertThat(a.contains("b"), Is().True());
    });

    it("can reinsert 'a' in { 'a', 'b' }", []() {
      anatree<> a;
      AssertThat(a.contains("a"), Is().False());
      AssertThat(a.contains("b"), Is().False());

      a.insert("a");
      AssertThat(a.contains("a"), Is().True());
      AssertThat(a.contains("b"), Is().False());

      a.insert("b");
      AssertThat(a.contains("a"), Is().True());
      AssertThat(a.contains("b"), Is().True());

      a.insert("a");
      AssertThat(a.contains("a"), Is().True());
      AssertThat(a.contains("b"), Is().True());
    });
  });

  describe("insert(), anagrams_of()", []() {
    it("can identify anagrams of '' in Ø", []() {
      anatree<> a;

      const auto res = a.anagrams_of("");
      AssertThat(res.size(), Is().EqualTo(0u));
    });

    it("can identify anagrams of '' in { 'a' }", []() {
      anatree<> a;
      a.insert("a");

      const auto res = a.anagrams_of("");
      AssertThat(res.size(), Is().EqualTo(0u));
    });

    it("can identify anagrams of '' in { 'a', '' }", []() {
      anatree<> a;
      a.insert("a");
      a.insert("");

      const auto res = a.anagrams_of("");
      AssertThat(res.size(), Is().EqualTo(1u));
      AssertThat(res.contains(""), Is().True());
    });

    it("can identify anagrams of 'ab' in { 'a', 'ab', 'ba' }", []() {
      anatree<> a;
      a.insert("a");
      a.insert("ab");
      a.insert("ba");

      const auto res = a.anagrams_of("ab");
      AssertThat(res.size(), Is().EqualTo(2u));
      AssertThat(res.contains("ab"), Is().True());
      AssertThat(res.contains("ba"), Is().True());
    });

    it("can identify anagrams of 'ab' in { 'a', 'ab', 'aa', 'ba', 'b', 'aba' }", []() {
      anatree<> a;
      a.insert("a");
      a.insert("ab");
      a.insert("aa");
      a.insert("ba");
      a.insert("b");
      a.insert("aba");

      const auto res = a.anagrams_of("ab");
      AssertThat(res.size(), Is().EqualTo(2u));
      AssertThat(res.contains("ab"), Is().True());
      AssertThat(res.contains("ba"), Is().True());
    });

    it("does not have multiple copies of 'ab' in { 'ab', 'a', 'ab' }", []() {
      anatree<> a;
      a.insert("ab");
      a.insert("a");
      a.insert("ab");

      const auto res = a.anagrams_of("ab");
      AssertThat(res.size(), Is().EqualTo(1u));
      AssertThat(res.contains("ab"), Is().True());
    });

    it("can identify anagrams of 'ab' in { 'ba', 'a' }", []() {
      anatree<> a;
      a.insert("ba");
      a.insert("a");

      const auto res = a.anagrams_of("ab");
      AssertThat(res.size(), Is().EqualTo(1u));
      AssertThat(res.contains("ba"), Is().True());
    });

    it("can identify anagrams of 'ab' in { 'ab', 'ba', 'a' }", []() {
      anatree<> a;
      a.insert("ab");
      a.insert("ba");
      a.insert("a");

      const auto res = a.anagrams_of("ab");
      AssertThat(res.size(), Is().EqualTo(2u));
      AssertThat(res.contains("ab"), Is().True());
      AssertThat(res.contains("ba"), Is().True());
    });

    it("can identify anagrams of 'a' in { 'ab', 'ba', 'a' }", []() {
      anatree<> a;
      a.insert("ab");
      a.insert("ba");
      a.insert("a");

      const auto res = a.anagrams_of("a");
      AssertThat(res.size(), Is().EqualTo(1u));
      AssertThat(res.contains("a"), Is().True());
    });
  });
 });

// -------------------------------------------------------------------------- //
// Run tests
int main(int argc, char* argv[])
{
#ifdef NDEBUG
  std::cerr << "Warning: Internal assertions are not enabled!"
            << std::endl << std::endl;
#endif
  return bandit::run(argc, argv);
}
