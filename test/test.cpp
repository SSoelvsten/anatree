#include <vector>

// Import Unit Testing framework, Bandit
#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

// Import Anatree
#include <anatree.h>

// -------------------------------------------------------------------------- //
// Define tests

go_bandit([]() {
  describe("anatree<std::string, std::less>", []() {
    // -------------------------------------------------------------------------
    describe("insert(w), tree_size(), size(), empty()", []() {
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

      it("can report size of { 'abc', 'bc' }", []() {
        anatree<> a;
        a.insert("bc");
        a.insert("abc");

        /*      _(a)_
        //     /     \
        //   (b)     (b)
        //  /   \   /   \
        //  .  (c)  .   (c)
        //     / \      / \
        //     . .[abc] . .[bc]
        */

        AssertThat(a.size(), Is().EqualTo(2u));
        AssertThat(a.empty(), Is().False());

        AssertThat(a.tree_size(), Is().EqualTo(11u));
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

    // -------------------------------------------------------------------------
    describe("anatree(...)", []() {
      it("can create a new and empty tree", []() {
        anatree<> a;

        AssertThat(a.size(), Is().EqualTo(0u));
        AssertThat(a.empty(), Is().True());

        AssertThat(a.tree_size(), Is().EqualTo(1u));
      });

      it("can (deep) copy-construct an empty Anatree", []() {
        anatree<> a1;
        anatree<> a2(a1);

        AssertThat(a2.size(), Is().EqualTo(0u));
        AssertThat(a2.empty(), Is().True());

        AssertThat(a2.tree_size(), Is().EqualTo(1u));

        // Check is an independent deep copy
        a2.insert("a");

        AssertThat(a1.contains("a"), Is().False());
      });

      it("can (deep) copy-construct a non-empty Anatree", []() {
        anatree<> a1;
        a1.insert("b");
        a1.insert("ab");

        anatree<> a2(a1);

        AssertThat(a2.size(), Is().EqualTo(2u));
        AssertThat(a2.empty(), Is().False());

        AssertThat(a2.tree_size(), Is().EqualTo(7u));

        AssertThat(a2.contains("b"), Is().True());
        AssertThat(a2.contains("ab"), Is().True());

        // Check is an independent deep copy
        a2.insert("a");

        AssertThat(a1.contains("a"), Is().False());
      });

      it("can move-construct an empty Anatree", []() {
        const auto a1_gen = []() {
          anatree<> a1;
          return a1;
        };

        anatree<> a2(a1_gen());

        AssertThat(a2.size(), Is().EqualTo(0u));
        AssertThat(a2.empty(), Is().True());

        AssertThat(a2.tree_size(), Is().EqualTo(1u));
      });

      it("can move-construct a non-empty Anatree", []() {
        const auto a1_gen = []() {
          anatree<> a1;
          a1.insert("b");
          a1.insert("ab");
          return a1;
        };

        anatree<> a2(a1_gen());

        AssertThat(a2.size(), Is().EqualTo(2u));
        AssertThat(a2.empty(), Is().False());

        AssertThat(a2.tree_size(), Is().EqualTo(7u));
      });

      it("can iterator-construct from {}", []() {
        std::vector<std::string> inputs = {};

        anatree<> a(inputs.begin(), inputs.end());

        AssertThat(a.size(), Is().EqualTo(0u));
        AssertThat(a.empty(), Is().True());

        AssertThat(a.tree_size(), Is().EqualTo(1u));
      });

      it("can iterator-construct from { 'a', '', 'b' }", []() {
        std::vector<std::string> inputs = { "a", "", "b" };

        anatree<> a(inputs.begin(), inputs.end());

        AssertThat(a.size(), Is().EqualTo(3u));
        AssertThat(a.empty(), Is().False());

        AssertThat(a.tree_size(), Is().EqualTo(5u));
      });

      it("can iterator-construct from { 'a', '', 'ab', 'b' }", []() {
        std::vector<std::string> inputs = { "a", "", "ab", "b" };

        anatree<> a(inputs.begin(), inputs.end());

        AssertThat(a.size(), Is().EqualTo(4u));
        AssertThat(a.empty(), Is().False());

        AssertThat(a.tree_size(), Is().EqualTo(7u));
      });

      it("can iterator-construct from { 'abc', 'bc' }", []() {
        std::vector<std::string> inputs = { "abc", "bc" };

        anatree<> a(inputs.begin(), inputs.end());

        AssertThat(a.size(), Is().EqualTo(2u));
        AssertThat(a.empty(), Is().False());

        AssertThat(a.tree_size(), Is().EqualTo(11u));
      });
    });

    // -------------------------------------------------------------------------
    describe("insert(w), contains()", []() {
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

    // -------------------------------------------------------------------------
    describe("insert(begin, end), size(), contains()", []() {
      it("can insert [] in Ø", []() {
        anatree<> a;

        std::vector<std::string> words = {};

        AssertThat(a.size(), Is().EqualTo(0u));
        a.insert(words.begin(), words.end());
        AssertThat(a.size(), Is().EqualTo(0u));
      });

      it("can insert [ 'a', 'ab', 'b' ] in Ø", []() {
        anatree<> a;

        std::vector<std::string> words = { "a", "ab", "b" };

        a.insert(words.begin(), words.end());

        AssertThat(a.size(), Is().EqualTo(3u));
        AssertThat(a.contains("a"), Is().True());
        AssertThat(a.contains("ab"), Is().True());
        AssertThat(a.contains("b"), Is().True());
      });

      it("can insert [] in { 'a' }", []() {
        anatree<> a;
        a.insert("a");

        std::vector<std::string> words = {};
        a.insert(words.begin(), words.end());

        AssertThat(a.size(), Is().EqualTo(1u));
        AssertThat(a.contains("a"), Is().True());
      });

      it("can insert ['b', 'a'] in { 'a' }", []() {
        anatree<> a;
        a.insert("a");

        std::vector<std::string> words = { "b", "a" };
        a.insert(words.begin(), words.end());

        AssertThat(a.size(), Is().EqualTo(2u));
        AssertThat(a.contains("a"), Is().True());
        AssertThat(a.contains("b"), Is().True());
      });

      it("can insert ['', 'b'] in { 'a' }", []() {
        anatree<> a;
        a.insert("a");

        std::vector<std::string> words = { "", "b" };
        a.insert(words.begin(), words.end());

        AssertThat(a.size(), Is().EqualTo(3u));
        AssertThat(a.contains(""), Is().True());
        AssertThat(a.contains("a"), Is().True());
        AssertThat(a.contains("b"), Is().True());
      });

      it("can insert ['', 'ab'] in { 'a', 'b' }", []() {
        anatree<> a;
        a.insert("b");
        a.insert("a");

        std::vector<std::string> words = { "", "ab" };
        a.insert(words.begin(), words.end());

        AssertThat(a.size(), Is().EqualTo(4u));
        AssertThat(a.contains(""), Is().True());
        AssertThat(a.contains("a"), Is().True());
        AssertThat(a.contains("b"), Is().True());
        AssertThat(a.contains("ab"), Is().True());
      });
    });

    // -------------------------------------------------------------------------
    describe("insert(w), has_anagram_of()", []() {
      it("does not contain an anagram of 'b' after insert of 'a'", []() {
        anatree<> a;
        AssertThat(a.has_anagram_of("b"), Is().False());

        a.insert("a");
        AssertThat(a.has_anagram_of("b"), Is().False());
      });

      it("contains an anagram of 'a' after insert of 'a'", []() {
        anatree<> a;
        AssertThat(a.has_anagram_of("a"), Is().False());

        a.insert("a");
        AssertThat(a.has_anagram_of("a"), Is().True());
      });

      it("contains an anagram of 'ab' after insert of 'ba'", []() {
        anatree<> a;
        AssertThat(a.has_anagram_of("ab"), Is().False());

        a.insert("ba");
        AssertThat(a.has_anagram_of("ab"), Is().True());
      });

      it("does not contain an anagram of 'a' after insert of 'ab'", []() {
        anatree<> a;
        AssertThat(a.has_anagram_of("a"), Is().False());

        a.insert("ab");
        AssertThat(a.has_anagram_of("a"), Is().False());
      });
    });

    // -------------------------------------------------------------------------
    describe("insert(w), anagrams_of()", []() {
      it("can find anagrams of '' in Ø", []() {
        anatree<> a;

        const auto res = a.anagrams_of("");
        AssertThat(res.empty(), Is().True());
      });

      it("can find anagrams of 'a' in Ø", []() {
        anatree<> a;

        const auto res = a.anagrams_of("a");
        AssertThat(res.empty(), Is().True());
      });

      it("can find anagrams of '' in { 'a' }", []() {
        anatree<> a;
        a.insert("a");

        const auto res = a.anagrams_of("");
        AssertThat(res.size(), Is().EqualTo(0u));
      });

      it("can find anagrams of '' in { 'a', '' }", []() {
        anatree<> a;
        a.insert("a");
        a.insert("");

        const auto res = a.anagrams_of("");
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains(""), Is().True());
      });

      it("can find anagrams of 'ab' in { 'a', 'ab', 'ba' }", []() {
        anatree<> a;
        a.insert("a");
        a.insert("ab");
        a.insert("ba");

        const auto res = a.anagrams_of("ab");
        AssertThat(res.size(), Is().EqualTo(2u));
        AssertThat(res.contains("ab"), Is().True());
        AssertThat(res.contains("ba"), Is().True());
      });

      it("can find anagrams of 'ab' in { 'a', 'ab', 'aa', 'ba', 'b', 'aba' }", []() {
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

      it("can find anagrams of 'ab' in { 'ba', 'a' }", []() {
        anatree<> a;
        a.insert("ba");
        a.insert("a");

        const auto res = a.anagrams_of("ab");
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains("ba"), Is().True());
      });

      it("can find anagrams of 'ab' in { 'ab', 'ba', 'a' }", []() {
        anatree<> a;
        a.insert("ab");
        a.insert("ba");
        a.insert("a");

        const auto res = a.anagrams_of("ab");
        AssertThat(res.size(), Is().EqualTo(2u));
        AssertThat(res.contains("ab"), Is().True());
        AssertThat(res.contains("ba"), Is().True());
      });

      it("can find anagrams of 'a' in { 'ab', 'ba', 'a' }", []() {
        anatree<> a;
        a.insert("ab");
        a.insert("ba");
        a.insert("a");

        const auto res = a.anagrams_of("a");
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains("a"), Is().True());
      });

      it("can find anagrams in { 'odd', 'dog', 'fog', 'loo', 'god' }", []() {
        anatree<> a;
        a.insert("odd");
        a.insert("dog");
        a.insert("fog");
        a.insert("loo");
        a.insert("god");

        {
          const auto res = a.anagrams_of("odd");
          AssertThat(res.size(), Is().EqualTo(1u));
          AssertThat(res.contains("odd"), Is().True());
        }

        {
          const auto res = a.anagrams_of("dog");
          AssertThat(res.size(), Is().EqualTo(2u));
          AssertThat(res.contains("dog"), Is().True());
          AssertThat(res.contains("god"), Is().True());
        }

        {
          const auto res = a.anagrams_of("fog");
          AssertThat(res.size(), Is().EqualTo(1u));
          AssertThat(res.contains("fog"), Is().True());
        }

        {
          const auto res = a.anagrams_of("loo");
          AssertThat(res.size(), Is().EqualTo(1u));
          AssertThat(res.contains("loo"), Is().True());
        }

        {
          const auto res = a.anagrams_of("god");
          AssertThat(res.size(), Is().EqualTo(2u));
          AssertThat(res.contains("dog"), Is().True());
          AssertThat(res.contains("god"), Is().True());
        }
      });

      it("can find anagrams in { 'do', 'dog', 'fog', 'god', 'gold', 'loo', 'odd', 'of', 'oo' }", []() {
        anatree<> a;
        a.insert("do");
        a.insert("dog");
        a.insert("fog");
        a.insert("god");
        a.insert("gold");
        a.insert("loo");
        a.insert("odd");
        a.insert("of");
        a.insert("oo");

        {
          const auto res = a.anagrams_of("loo");
          AssertThat(res.size(), Is().EqualTo(1u));
          AssertThat(res.contains("loo"), Is().True());
        }

        {
          const auto res = a.anagrams_of("dog");
          AssertThat(res.size(), Is().EqualTo(2u));
          AssertThat(res.contains("dog"), Is().True());
          AssertThat(res.contains("god"), Is().True());
        }

        {
          const auto res = a.anagrams_of("gold");
          AssertThat(res.size(), Is().EqualTo(1u));
          AssertThat(res.contains("gold"), Is().True());
        }
      });
    });

    // -------------------------------------------------------------------------
    describe("insert(w), keys()", []() {
      it("can find keys in Ø", []() {
        anatree<> a;

        const auto res = a.keys();
        AssertThat(res.empty(), Is().True());
      });

      it("can find keys in { '' }", []() {
        anatree<> a;
        a.insert("");

        const auto res = a.keys();
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains(""), Is().True());
      });

      it("can find keys in { 'a' }", []() {
        anatree<> a;
        a.insert("a");

        const auto res = a.keys();
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains("a"), Is().True());
      });

      it("can find keys in { '', 'a' }", []() {
        anatree<> a;
        a.insert("");
        a.insert("a");

        const auto res = a.keys();
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains("a"), Is().True());
      });

      it("can find keys in { 'a', 'b' }", []() {
        anatree<> a;
        a.insert("a");
        a.insert("b");

        const auto res = a.keys();
        AssertThat(res.size(), Is().EqualTo(2u));
        AssertThat(res.contains("a"), Is().True());
        AssertThat(res.contains("b"), Is().True());
      });

      it("can find keys in { 'a', 'ab' }", []() {
        anatree<> a;
        a.insert("a");
        a.insert("ab");

        const auto res = a.keys();
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains("ab"), Is().True());
      });

      it("can find keys in { 'a', 'b', 'ab' }", []() {
        anatree<> a;
        a.insert("a");
        a.insert("ab");
        a.insert("b");

        const auto res = a.keys();
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains("ab"), Is().True());
      });

      it("can find keys in { 'a', 'aab', 'b' }", []() {
        anatree<> a;
        a.insert("a");
        a.insert("aab");
        a.insert("b");

        const auto res = a.keys();
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains("aab"), Is().True());
      });

      it("can find keys in { 'ab', 'aab', 'b' }", []() {
        anatree<> a;
        a.insert("ab");
        a.insert("aab");
        a.insert("b");

        const auto res = a.keys();
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains("aab"), Is().True());
      });

      it("can find keys in { 'a', 'b', 'aba', 'ab', 'bb' }", []() {
        anatree<> a;
        a.insert("a");
        a.insert("b");
        a.insert("aba");
        a.insert("ab");
        a.insert("bb");

        const auto res = a.keys();
        AssertThat(res.size(), Is().EqualTo(2u));
        AssertThat(res.contains("bb"), Is().True());
        AssertThat(res.contains("aba"), Is().True());
      });

      it("can find keys in { 'do', 'dog', 'fog', 'god', 'gold', 'loo', 'odd', 'of', 'oo' }", []() {
        anatree<> a;
        a.insert("do");
        a.insert("dog");
        a.insert("fog");
        a.insert("god");
        a.insert("gold");
        a.insert("loo");
        a.insert("odd");
        a.insert("of");
        a.insert("oo");

        const auto res = a.keys();
        AssertThat(res.size(), Is().EqualTo(4u));
        AssertThat(res.contains("loo"), Is().True());
        AssertThat(res.contains("fog"), Is().True());
        AssertThat(res.contains("gold"), Is().True());
        AssertThat(res.contains("odd"), Is().True());
      });
    });

    // -------------------------------------------------------------------------
    describe("insert(w), keys(word_length)", []() {
      it("can find 0-keys in Ø", []() {
        anatree<> a;

        const auto res = a.keys(0);
        AssertThat(res.empty(), Is().True());
      });

      it("can find 1-keys in Ø", []() {
        anatree<> a;

        const auto res = a.keys(1);
        AssertThat(res.empty(), Is().True());
      });

      it("can find 0-keys in { '', 'a' }", []() {
        anatree<> a;
        a.insert("");
        a.insert("a");

        const auto res = a.keys(0);
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains(""), Is().True());
      });

      it("can find 1-keys in { '', 'a' }", []() {
        anatree<> a;
        a.insert("");
        a.insert("a");

        const auto res = a.keys(1);
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains("a"), Is().True());
      });

      it("can find 1-keys in { 'a', 'ab', 'b' }", []() {
        anatree<> a;
        a.insert("a");
        a.insert("ab");
        a.insert("b");

        const auto res = a.keys(1);
        AssertThat(res.size(), Is().EqualTo(2u));
        AssertThat(res.contains("a"), Is().True());
        AssertThat(res.contains("b"), Is().True());
      });

      it("can find 2-keys in { 'a', 'ab', 'b' }", []() {
        anatree<> a;
        a.insert("a");
        a.insert("ab");
        a.insert("b");

        const auto res = a.keys(2);
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains("ab"), Is().True());
      });

      it("can find 3-keys in { 'do', 'dog', 'fog', 'god', 'gold', 'loo', 'odd', 'of', 'oo' }", []() {
        anatree<> a;
        a.insert("do");
        a.insert("dog");
        a.insert("fog");
        a.insert("god");
        a.insert("gold");
        a.insert("loo");
        a.insert("odd");
        a.insert("of");
        a.insert("oo");

        const auto res = a.keys(3);
        AssertThat(res.size(), Is().EqualTo(4u));
        AssertThat(res.contains("loo"), Is().True());
        AssertThat(res.contains("fog"), Is().True());
        AssertThat(res.contains("dog") ^ res.contains("god"), Is().True());
        AssertThat(res.contains("odd"), Is().True());
      });

      it("can find 4-keys in { 'do', 'dog', 'fog', 'god', 'gold', 'loo', 'odd', 'of', 'oo' }", []() {
        anatree<> a;
        a.insert("do");
        a.insert("dog");
        a.insert("fog");
        a.insert("god");
        a.insert("gold");
        a.insert("loo");
        a.insert("odd");
        a.insert("of");
        a.insert("oo");

        const auto res = a.keys(4);
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains("gold"), Is().True());
      });
    });

    // -------------------------------------------------------------------------
    describe("insert(w), subanagrams_of()", []() {
      it("can find subanagrams of '' in Ø", []() {
        anatree<> a;

        const auto res = a.subanagrams_of("");
        AssertThat(res.empty(), Is().True());
      });

      it("can find subanagrams of 'a' in Ø", []() {
        anatree<> a;

        const auto res = a.subanagrams_of("a");
        AssertThat(res.empty(), Is().True());
      });

      it("can find subanagrams of '' in { '', 'a' }", []() {
        anatree<> a;
        a.insert("");
        a.insert("a");

        const auto res = a.subanagrams_of("");
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains(""), Is().True());
      });

      it("can find subanagrams of 'a' in { '', 'a' }", []() {
        anatree<> a;
        a.insert("");
        a.insert("a");

        const auto res = a.subanagrams_of("a");
        AssertThat(res.size(), Is().EqualTo(2u));
        AssertThat(res.contains("a"), Is().True());
        AssertThat(res.contains(""), Is().True());
      });

      it("can find subanagrams of 'a' in { 'a', 'b' }", []() {
        anatree<> a;
        a.insert("a");
        a.insert("b");

        const auto res = a.subanagrams_of("a");
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains("a"), Is().True());
      });

      it("can find subanagrams of 'b' in { 'a', 'b' }", []() {
        anatree<> a;
        a.insert("a");
        a.insert("b");

        const auto res = a.subanagrams_of("b");
        AssertThat(res.size(), Is().EqualTo(1u));
        AssertThat(res.contains("b"), Is().True());
      });

      it("can find subanagrams of 'ab' and 'ba' in { 'a', 'ab', 'b', 'ba' }", []() {
        anatree<> a;
        a.insert("a");
        a.insert("ab");
        a.insert("ba");
        a.insert("b");

        {
          const auto res = a.subanagrams_of("ab");
          AssertThat(res.size(), Is().EqualTo(4u));
          AssertThat(res.contains("a"), Is().True());
          AssertThat(res.contains("b"), Is().True());
          AssertThat(res.contains("ab"), Is().True());
          AssertThat(res.contains("ba"), Is().True());
        }

        {
          const auto res = a.subanagrams_of("ba");
          AssertThat(res.size(), Is().EqualTo(4u));
          AssertThat(res.contains("a"), Is().True());
          AssertThat(res.contains("b"), Is().True());
          AssertThat(res.contains("ab"), Is().True());
          AssertThat(res.contains("ba"), Is().True());
        }
      });

      it("can find subanagrams of 'ab' and 'ba' in { 'a', 'ab', 'c', 'ba' }", []() {
        anatree<> a;
        a.insert("a");
        a.insert("ab");
        a.insert("ba");
        a.insert("c");

        {
          const auto res = a.subanagrams_of("ab");
          AssertThat(res.size(), Is().EqualTo(3u));
          AssertThat(res.contains("a"), Is().True());
          AssertThat(res.contains("ab"), Is().True());
          AssertThat(res.contains("ba"), Is().True());
        }

        {
          const auto res = a.subanagrams_of("ba");
          AssertThat(res.size(), Is().EqualTo(3u));
          AssertThat(res.contains("a"), Is().True());
          AssertThat(res.contains("ab"), Is().True());
          AssertThat(res.contains("ba"), Is().True());
        }
      });

      it("does not have multiple copies of 'ab' in { 'a', 'ab', 'b', 'ab' }", []() {
        anatree<> a;
        a.insert("a");
        a.insert("ab");
        a.insert("b");
        a.insert("ab");

        const auto res = a.subanagrams_of("ab");
        AssertThat(res.size(), Is().EqualTo(3u));
        AssertThat(res.contains("a"), Is().True());
        AssertThat(res.contains("b"), Is().True());
        AssertThat(res.contains("ab"), Is().True());
      });

      it("can find subanagrams of 'ba' in { 'a', 'ab', 'b' }", []() {
        anatree<> a;
        a.insert("a");
        a.insert("ab");
        a.insert("b");

        const auto res = a.subanagrams_of("ba");
        AssertThat(res.size(), Is().EqualTo(3u));
        AssertThat(res.contains("a"), Is().True());
        AssertThat(res.contains("b"), Is().True());
        AssertThat(res.contains("ab"), Is().True());
      });

      it("can find subanagrams of 'abc' in { 'bc', 'c', 'b' }", []() {
        anatree<> a;
        a.insert("bc");
        a.insert("b");
        a.insert("c");

        const auto res = a.subanagrams_of("abc");
        AssertThat(res.size(), Is().EqualTo(3u));
        AssertThat(res.contains("bc"), Is().True());
        AssertThat(res.contains("b"), Is().True());
        AssertThat(res.contains("c"), Is().True());
      });

      it("can find subanagrams of 'acab' in { 'bbc', 'cc', 'bc', 'c' }", []() {
        anatree<> a;
        a.insert("bbc");
        a.insert("cc");
        a.insert("bc");
        a.insert("c");

        const auto res = a.subanagrams_of("acab");
        AssertThat(res.size(), Is().EqualTo(2u));
        AssertThat(res.contains("bc"), Is().True());
        AssertThat(res.contains("c"), Is().True());
      });

      it("does not crash on finding subanagrams of 'ab' in { 'c', 'cc' }", []() {
        anatree<> a;
        a.insert("c");
        a.insert("cc");

        const auto res = a.subanagrams_of("ab");
        AssertThat(res.size(), Is().EqualTo(0u));
      });

      it("can find subanagrams in { 'do', 'dog', 'fog', 'god', 'gold', 'loo', 'odd', 'of', 'oo' }", []() {
        anatree<> a;
        a.insert("do");
        a.insert("dog");
        a.insert("fog");
        a.insert("god");
        a.insert("gold");
        a.insert("loo");
        a.insert("odd");
        a.insert("of");
        a.insert("oo");

        {
          const auto res = a.subanagrams_of("loo");
          AssertThat(res.size(), Is().EqualTo(2u));
          AssertThat(res.contains("loo"), Is().True());
          AssertThat(res.contains("oo"), Is().True());
        }

        {
          const auto res = a.subanagrams_of("dog");
          AssertThat(res.size(), Is().EqualTo(3u));
          AssertThat(res.contains("dog"), Is().True());
          AssertThat(res.contains("god"), Is().True());
          AssertThat(res.contains("do"), Is().True());
        }

        {
          const auto res = a.subanagrams_of("god");
          AssertThat(res.size(), Is().EqualTo(3u));
          AssertThat(res.contains("dog"), Is().True());
          AssertThat(res.contains("god"), Is().True());
          AssertThat(res.contains("do"), Is().True());
        }

        {
          const auto res = a.subanagrams_of("gold");
          AssertThat(res.size(), Is().EqualTo(4u));
          AssertThat(res.contains("gold"), Is().True());
          AssertThat(res.contains("dog"), Is().True());
          AssertThat(res.contains("god"), Is().True());
          AssertThat(res.contains("do"), Is().True());
        }
      });
    });
  });

  describe("anatree<std::string, std::greater>", []() {
    describe("insert(w), tree_size(), size(), empty()", []() {
      it("can report size of { 'abc', 'bc' }", []() {
        anatree<std::string, std::greater<char>> a;
        a.insert("bc");
        a.insert("abc");

        /*      (c)
        //      / \
        //     . (b)
        //       / \
        //   [bc]. (a)
        //         / \
        //         . .[abc]
        */

        AssertThat(a.size(), Is().EqualTo(2u));
        AssertThat(a.empty(), Is().False());

        AssertThat(a.tree_size(), Is().EqualTo(7u));
      });
    });
  });

  describe("anatree<std::wstring, ...>", []() {
    anatree<std::wstring> a;

    it("is initially empty", [&a]() {
      AssertThat(a.empty(), Is().True());
    });

    it("can insert 'do', 'dog', 'fog', 'god', 'gold'", [&a]() {
      a.insert(L"do");
      a.insert(L"dog");
      a.insert(L"fog");
      a.insert(L"god");
      a.insert(L"gold");

      AssertThat(a.size(), Is().EqualTo(5u));
      AssertThat(a.empty(), Is().False());

      AssertThat(a.tree_size(), Is().EqualTo(19u));
    });

    it("can find anagrams of 'dog'", [&a]() {
      const auto res = a.anagrams_of(L"dog");
      AssertThat(res.size(), Is().EqualTo(2u));
      AssertThat(res.contains(L"dog"), Is().True());
      AssertThat(res.contains(L"god"), Is().True());
    });

    it("can find anagrams of 'god'", [&a]() {
      const auto res = a.anagrams_of(L"dog");
      AssertThat(res.size(), Is().EqualTo(2u));
      AssertThat(res.contains(L"dog"), Is().True());
      AssertThat(res.contains(L"god"), Is().True());
    });

    it("can find subanagrams of 'gold'", [&a]() {
      const auto res = a.subanagrams_of(L"gold");
      AssertThat(res.size(), Is().EqualTo(4u));
      AssertThat(res.contains(L"gold"), Is().True());
      AssertThat(res.contains(L"dog"), Is().True());
      AssertThat(res.contains(L"god"), Is().True());
      AssertThat(res.contains(L"do"), Is().True());
    });
  });

  describe("anatree<std::u8string, ...>", []() {
    anatree<std::u8string> a;

    it("it can construct { 'ø', 'øm', 'øse', 'søm', 'møs', 'søn' }", [&a]() {
      AssertThat(a.empty(), Is().True());

      a.insert(u8"ø");
      a.insert(u8"øm");
      a.insert(u8"øse");
      a.insert(u8"søm");
      a.insert(u8"møs");
      a.insert(u8"søn");
      a.insert(u8"mø");

      AssertThat(a.size(), Is().EqualTo(7u));
      AssertThat(a.empty(), Is().False());

      AssertThat(a.tree_size(), Is().EqualTo(33u));
    });

    it("it can find anagrams of 'søm'", [&a]() {
      const auto res = a.anagrams_of(u8"søm");
      AssertThat(res.size(), Is().EqualTo(2u));
      AssertThat(res.contains(u8"søm"), Is().True());
      AssertThat(res.contains(u8"møs"), Is().True());
    });

    it("it can find subanagrams of 'søm'", [&a]() {
      const auto res = a.subanagrams_of(u8"søm");
      AssertThat(res.size(), Is().EqualTo(5u));
      AssertThat(res.contains(u8"søm"), Is().True());
      AssertThat(res.contains(u8"møs"), Is().True());
      AssertThat(res.contains(u8"mø"), Is().True());
      AssertThat(res.contains(u8"øm"), Is().True());
      AssertThat(res.contains(u8"ø"), Is().True());
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
