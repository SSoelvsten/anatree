/* Anatree : A Data Structure for Anagrams.
 *
 * Copyright (C) 2023  Steffan Christ Sølvsten
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesse General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef ANATREE_H
#define ANATREE_H

// 'assert(...)'
#include<assert.h>

// 'std::sort(...)'
#include<algorithm>

// 'std::unique_ptr' and 'std::stringstream' for 'anatree<>::node'
#include<memory>
#include<sstream>

// 'std::string', 'std::unordered_set', and 'std::unordered_map' for
// 'anatree<...>' template parameters
#include<string>
#include<unordered_set>
#include<unordered_map>

// C++20 concepts
#include <concepts>
#include <iterator>

////////////////////////////////////////////////////////////////////////////////
/// \brief A data structure capable of storing a set of 'std::string' (or
///        similar) data structures, enabling quick access to all 'anagrams' of
///        each word (within or not).
///
/// \tparam word_t      Type for words, i.e. lists of elemnts. This type should
///                     provide the interface of `std::string`, such as
///                     `std::basic_string<T>` and `std::vector<T>`.
///
/// \tparam char_comp_t Ordering of the symbols within each word. The order has
///                     a major impact on the size.
///
/// \tparam word_set_t  Type to be used for storing and returning sets of words.
////////////////////////////////////////////////////////////////////////////////
template<typename word_t      = std::string,
         typename char_comp_t = std::less<typename word_t::value_type>,
         typename word_set_t  = std::unordered_set<word_t>,
         typename word_map_t  = std::unordered_map<word_t, word_t>>
requires std::copyable<word_t>
      && std::sortable<typename word_t::iterator, char_comp_t>
      && std::equality_comparable<typename word_t::value_type>
      && std::totally_ordered<typename word_t::value_type>
class anatree
{
private:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Type of each individual character.
  //////////////////////////////////////////////////////////////////////////////
  using char_t = typename word_t::value_type;

private:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Individual node of the Anatree.
  //////////////////////////////////////////////////////////////////////////////
  class node
  {
  public:
    using ptr = std::unique_ptr<node>;

    // TODO: derive a non-useful value as 'NIL'.
    static constexpr char_t NIL = 0;

  public:
    ////////////////////////////////////////////////////////////////////////////
    /// \brief Character in this node
    ////////////////////////////////////////////////////////////////////////////
    char_t m_char = NIL;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Binary choice on children.
    ///
    /// Follow the 'false' pointer, if 'm_char' does not occur in the word.
    /// Otherwise follow the 'true' pointer, if it does.
    ////////////////////////////////////////////////////////////////////////////
    ptr m_children[2] = { nullptr, nullptr };

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Set of words that are anagrams of the path up to this node.
    ////////////////////////////////////////////////////////////////////////////
    word_set_t m_words;

  public:
    ////////////////////////////////////////////////////////////////////////////
    /// \brief Initialize a NIL node
    ////////////////////////////////////////////////////////////////////////////
    void init(char_t c, ptr f_ptr = nullptr, ptr t_ptr = nullptr)
    {
      assert(m_char == NIL && c != NIL);
      m_char = c;
      m_children[false] = f_ptr ? std::move(f_ptr) : make_node();
      m_children[true]  = t_ptr ? std::move(t_ptr) : make_node();
    }

  public:
    ////////////////////////////////////////////////////////////////////////////
    /// \brief Empty (NIL) constructor
    ////////////////////////////////////////////////////////////////////////////
    node() = default;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Empty (NIL) ptr constructor
    ////////////////////////////////////////////////////////////////////////////
    static ptr make_node()
    { return std::make_unique<node>(); }

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ////////////////////////////////////////////////////////////////////////////
    node(const node&) = default;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Non-nil ptr constructor
    ////////////////////////////////////////////////////////////////////////////
    node(const char_t c, ptr f_ptr = make_node(), ptr t_ptr = make_node())
    { init(c, std::move(f_ptr), std::move(t_ptr)); };

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Non-nil constructor
    ////////////////////////////////////////////////////////////////////////////
    static ptr make_node(char_t c, ptr f_ptr = make_node(), ptr t_ptr = make_node())
    { return std::make_unique<node>(c, std::move(f_ptr), std::move(t_ptr)); }

  public:
    ////////////////////////////////////////////////////////////////////////////
    std::string to_string()
    {
      std::stringstream ss;
      ss << "{ char: " << m_char
         << ", children: { " << m_children[false] << ", " << m_children[true]
         << " }, words [ ";
      for (const word_t &w : m_words) {
        ss << w << " ";
      }
      ss << "] }";
      return ss.str();
    }
  };

private:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Alias for 'node::ptr' that allows omitting the 'typename' keyword.
  //////////////////////////////////////////////////////////////////////////////
  using node_ptr = typename node::ptr;

private:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Comparator for characters.
  //////////////////////////////////////////////////////////////////////////////
  char_comp_t m_char_comp = char_comp_t();

  //////////////////////////////////////////////////////////////////////////////
  /// \brief Root of the anatree (initially a NIL node).
  //////////////////////////////////////////////////////////////////////////////
  node_ptr m_root = node::make_node();

  //////////////////////////////////////////////////////////////////////////////
  /// \brief Number of words stored within this tree.
  //////////////////////////////////////////////////////////////////////////////
  size_t m_size = 0u;

  //////////////////////////////////////////////////////////////////////////////
  /// \brief Number of nodes (including NIL) in the tree.
  //////////////////////////////////////////////////////////////////////////////
  size_t m_tree_size = 1u;

  //////////////////////////////////////////////////////////////////////////////
  /// \brief Unique `null` pointer to have a *no* return value.
  //////////////////////////////////////////////////////////////////////////////
  const node_ptr m_null = nullptr;

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Constructor of an empty Anatree.
  //////////////////////////////////////////////////////////////////////////////
  anatree(char_comp_t char_comp = char_comp_t())
    : m_char_comp(char_comp)
  { }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Copy-constructor, creating a deep copy of another Anatree.
  ///
  /// \warning Requires \f$O(N)\f$ time.
  //////////////////////////////////////////////////////////////////////////////
  anatree(const anatree &a)
    : m_char_comp(a.m_char_comp)
    , m_root(deep_copy(a.m_root))
    , m_size(a.m_size)
    , m_tree_size(a.m_tree_size)
  { }

private:
  static
  node_ptr
  deep_copy(const node_ptr &p)
  {
    node_ptr np = node::make_node();

    // Case: NIL
    // -> Leave node as-is
    if (p->m_char == node::NIL) {
      assert(p->m_children[false] == nullptr && p->m_children[true] == nullptr);
    }

    // Case: not NIL
    // -> Obtain recursively deep copy of children and copy over 'm_char'
    else {
      np->m_char = p->m_char;
      np->m_children[false] = deep_copy(p->m_children[false]);
      np->m_children[true]  = deep_copy(p->m_children[true]);
    }

    // Copy over the set of words stored within this node.
    np->m_words = p->m_words;
    return std::move(np);
  }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Move-constructor, taking over ownership of the other's tree.
  //////////////////////////////////////////////////////////////////////////////
  anatree(anatree&&) = default;

  // TODO with iterator:
  // template<class InputIt> anatree(InputIt first, InputIt last)

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Adds the word 'w' to the anatree.
  //////////////////////////////////////////////////////////////////////////////
  void
  insert(const word_t &w)
  {
    word_t key = sorted_word(w);
    m_root = insert__rec(std::move(m_root), w, key.begin(), key.end());
  }

private:
  node_ptr
  insert__rec(node_ptr &&p,
              const word_t &w,
              typename word_t::iterator curr,
              const typename word_t::iterator end)
  {
    assert(p != nullptr);

    // Case: Iterator done
    // -> Insert word
    if (curr == end) {
      if (!p->m_words.contains(w)) {
        m_size++;
        p->m_words.insert(w);
      }
      return p;
    }

    // Case: NIL
    // -> Turn into non-NIL node
    if (p->m_char == node::NIL) {
      assert(p->m_children[false] == nullptr && p->m_children[true] == nullptr);
      p->init(*curr);
      m_tree_size += 2; // <- NIL 'false' and 'true' children
      p->m_children[true] = insert__rec(std::move(p->m_children[true]), w, ++curr, end);
      return p;
    }

    // Case: Iterator behind
    // -> Insert new node in-between
    if (m_char_comp(*curr, p->m_char)) {
      // Hand over ownership of 'p' to a new node 'np'. The original node 'p' is
      // now available as its 'false' child.
      node_ptr np = node::make_node(*curr, std::move(p), node::make_node());
      np->m_words = np->m_children[false]->m_words;
      np->m_children[false]->m_words = word_set_t();
      m_tree_size += 2; // <- new node and its NIL 'false' child
      np->m_children[true]  = insert__rec(std::move(np->m_children[true]), w, ++curr, end);
      return std::move(np);
    }

    // Case: Iterator ahead
    // -> Follow 'false' child
    if (m_char_comp(p->m_char, *curr)) {
      p->m_children[false] = insert__rec(std::move(p->m_children[false]), w, curr, end);
      return p;
    }

    // Case: Iterator and node matches
    // -> Follow 'true' child
    p->m_children[true] = insert__rec(std::move(p->m_children[true]), w, ++curr, end);
    return p;
  }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Adds the words to the anatree as per the iterator.
  //////////////////////////////////////////////////////////////////////////////
  template<typename IT>
  void
  insert(IT begin, IT end)
  {
    while (begin != end) { insert(*(begin++)); }
  }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Remove all nodes/anagrams.
  //////////////////////////////////////////////////////////////////////////////
  void
  clear()
  {
    m_root = node::make_node();
    m_size = 0u;
    m_tree_size = 1u;
  }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Obtain all words in the Anatree, excluding words that are a
  ///        subanagram of another one returned.
  //////////////////////////////////////////////////////////////////////////////
  word_set_t
  keys() const
  {
    const auto rec_result = keys__rec(m_root);
    word_set_t ret;
    for (const auto [_, v] : rec_result) {
      ret.insert(v);
    }
    return ret;
  }

private:
  word_map_t
  keys__rec(const node_ptr &p) const
  {
    // Case: Leaf of Tree
    // -> Add a word, if any.
    if (p->m_char == node::NIL) {
      word_map_t ret;
      if (p->m_words.size() > 0) {
        ret[""] = *p->m_words.begin();
      }
      return ret;
    }

    // Case: Internal Node
    // -> Recurse for words with and without this character
    auto rec_true = keys__rec(p->m_children[true]);
    auto rec_false = keys__rec(p->m_children[false]);

    word_map_t ret;

    // -> Copy over words excluding current node's character (except for ones
    //    that are superseeded)
    for (const auto [rec_false_k, rec_false_v] : rec_false) {
      // Check whether the word is superseeded by any word in 'rec_true'.
      // - (a) Simple O(1) case
      if (rec_true.contains(rec_false_v)) { continue; }

      // - (b) Expensive O(T) case
      bool superseeded = false;
      for (const auto [rec_true_k, _] : rec_true) {
        int false_idx = rec_false_k.length() - 1;
        int true_idx  = rec_true_k.length()  - 1;

        while (0 <= false_idx && 0 <= true_idx) {
          const char_t false_char = rec_false_k[false_idx];
          const char_t true_char  = rec_true_k[true_idx];

          if (m_char_comp(true_char, false_char)) {
            true_idx--;
            continue;
          } else if (false_char == true_char) {
            false_idx--;
            true_idx--;
            continue;
          } else { // False includes character not in True path.
            break;
          }
        }
        if (false_idx < 0 && true_idx < 0) {
          superseeded = true;
          break;
        }
      }
      if (superseeded) { continue; }

      // Add word
      std::string curr_key(rec_false_k);
      curr_key.push_back(p->m_char);

      assert(!ret.contains(curr_key));
      ret[curr_key] = rec_false_v;
    }
    rec_false.clear(); // <-- Save on memory

    // -> Copy over words including current node's character.
    for (const auto [rec_k, rec_v] : rec_true) {
      std::string curr_key(rec_k);
      curr_key.push_back(p->m_char);

      ret[curr_key] = rec_v;
    }
    // rec_true.clear(); // <-- Save on memory (unecessary)

    return ret;
  }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Obtain all words in the Anatree of a certain length, excluding
  ///        words that are an anagram of another one returned.
  ///
  /// \param word_length The length of the desired words.
  //////////////////////////////////////////////////////////////////////////////
  word_set_t
  keys(const size_t word_length) const
  {
    return keys__rec(word_length, m_root, 0);
  }

private:
  word_set_t
  keys__rec(const size_t word_length,
            const node_ptr &p,
            const size_t true_edges) const
  {
    assert(true_edges <= word_length);


    // Case: Found word of 'word_length'
    // -> Search succesful (no need to keep on searching deeper)
    if (word_length == true_edges) {
      word_set_t ret;
      if (p->m_words.size() > 0) {
        ret.insert(*p->m_words.begin());
      }
      return ret;
    }

    // Case: Tree stopped early
    // -> Abandon subtree
    if (p->m_char == node::NIL) {
      return word_set_t();
    }

    // Case: Missing characters
    // -> Merge recursively from false and true subtrees
    auto rec_true = keys__rec(word_length, p->m_children[true], true_edges+1);
    auto rec_false = keys__rec(word_length, p->m_children[false], true_edges);
    rec_true.insert(rec_false.begin(), rec_false.end());

    return rec_true;
  }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Obtain all words that are anagrams of 'w'.
  ///
  /// \details An anagram is a word that can be created from (all) the letters
  ///          of 'w'.
  //////////////////////////////////////////////////////////////////////////////
  word_set_t
  anagrams_of(const word_t &w) const
  {
    const node_ptr& n = find_node(w);
    return n ? n->m_words : word_set_t();
  }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Obtain all words that are subanagrams of 'w'.
  ///
  /// \details A subanagram is a word that can be created from some (but not
  ///          necessarily all) letters of 'w'.
  //////////////////////////////////////////////////////////////////////////////
  word_set_t
  subanagrams_of(const word_t &w) const
  {
    word_t key = sorted_word(w);
    return subanagrams_of__rec(m_root, key.begin(), key.end());
  }

private:
  word_set_t
  subanagrams_of__rec(const node_ptr &p,
                      typename word_t::iterator curr,
                      const typename word_t::iterator end) const
  {
    // Case: Iterator or Anatree is done
    if (curr == end || p->m_char == node::NIL) {
      return p->m_words;
    }

    // Case: Iterator behind
    // -> Skip missing characters
    if (m_char_comp(*curr, p->m_char)) {
      while (m_char_comp(*curr, p->m_char) && curr != end) { ++curr; }
      return subanagrams_of__rec(p, curr, end);
    }

    // Case: Iterator ahead
    // -> Follow 'false' child
    if (m_char_comp(p->m_char, *curr)) {
      word_set_t ret(p->m_words);
      const auto rec_false = subanagrams_of__rec(p->m_children[false], curr, end);
      ret.insert(rec_false.begin(), rec_false.end());
      return ret;
    }

    // Case: Iterator and node matches
    // -> Follow both children, merge results and add words on current node
    ++curr;

    word_set_t ret(p->m_words);
    const auto rec_false = subanagrams_of__rec(p->m_children[false], curr, end);
    const auto rec_true = subanagrams_of__rec(p->m_children[true], curr, end);
    ret.insert(rec_false.begin(), rec_false.end());
    ret.insert(rec_true.begin(), rec_true.end());
    return ret;
  }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Whether the Anatree includes the word 'w'.
  //////////////////////////////////////////////////////////////////////////////
  bool
  contains(const word_t &w) const
  {
    const node_ptr &n = find_node(w);
    return n && n->m_words.contains(w);
  }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Number of stored words.
  //////////////////////////////////////////////////////////////////////////////
  size_t
  size() const
  {
    return m_size;
  }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Whether the Anatree is empty.
  //////////////////////////////////////////////////////////////////////////////
  bool
  empty() const
  {
    return size() == 0u;
  }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Number of nodes within the tree.
  //////////////////////////////////////////////////////////////////////////////
  size_t
  tree_size() const
  {
    return m_tree_size;
  }

private:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Creates a copy of the word 'w' with its characters sorted.
  //////////////////////////////////////////////////////////////////////////////
  word_t sorted_word(const word_t &w) const
  {
    word_t ret(w);
    std::sort(ret.begin(), ret.end(), m_char_comp);
    return ret;
  }

  //////////////////////////////////////////////////////////////////////////////
  /// \brief Traverses the anatree in search of the node corresponding to `w`.
  ///
  /// \returns Immutable reference to the node in the tree. If there is none,
  ///          then it returns `m_null`.
  /////////////////////////////////////////////////////////////////////////////
  const node_ptr& find_node(const word_t w) const
  {
    word_t key = sorted_word(w);
    return find_node__rec(m_root, key.begin(), key.end());
  }

  const node_ptr& find_node__rec(const node_ptr &p,
                                 typename word_t::iterator curr,
                                 const typename word_t::iterator end) const
  {
    // Case: Iterator is done
    if (curr == end) {
      return p;
    }

    // Case: Iterator behind or tree is done
    // -> No words with all letters exist, return Ø .
    if (m_char_comp(*curr, p->m_char) || p->m_char == node::NIL) {
      return m_null;
    }

    // Case: Iterator ahead
    // -> Follow 'false' child
    if (m_char_comp(p->m_char, *curr)) {
      return find_node__rec(p->m_children[false], curr, end);
    }

    // Case: Iterator and node matches
    // -> Follow 'true' child
    return find_node__rec(p->m_children[true], ++curr, end);
  }
};

#endif // ANATREE_H
