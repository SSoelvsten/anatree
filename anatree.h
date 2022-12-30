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

// 'std::shared_ptr' and 'std::stringstream' for 'anatree<>::node'
#include<memory>
#include<sstream>

// 'std::string' and 'std::unordered_set' for 'anatree<...>' template parameters
#include<string>
#include<unordered_set>

////////////////////////////////////////////////////////////////////////////////
/// \brief A data structure capable of storing a set of 'std::string' (or
///        similar) data structures, enabling quick access to all 'anagrams' of
///        each word (within or not).
///
/// \tparam word_t Type for words, i.e. lists of elemnts. This type should have
///                an interface similar to `std::string` / `std::vector<T>`.
///
/// \todo Add 'ordering' tparam to something else than `std::less`.
////////////////////////////////////////////////////////////////////////////////
template<typename word_t      = std::string,
         typename char_comp_t = std::less<typename word_t::value_type>,
         typename word_set_t  = std::unordered_set<word_t>>
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
    using ptr = std::shared_ptr<node>;

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
      m_children[false] = f_ptr ? f_ptr : std::make_shared<node>();
      m_children[true]  = t_ptr ? t_ptr : std::make_shared<node>();
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
    { return std::make_shared<node>(); }

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ////////////////////////////////////////////////////////////////////////////
    node(const node&) = default;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Non-nil ptr constructor
    ////////////////////////////////////////////////////////////////////////////
    node(const char_t c, ptr f_ptr = make_node(), ptr t_ptr = make_node())
    { init(c, f_ptr, t_ptr); };

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Non-nil constructor
    ////////////////////////////////////////////////////////////////////////////
    static ptr make_node(char_t c, ptr f_ptr = make_node(), ptr t_ptr = make_node())
    { return std::make_shared<node>(c, f_ptr, t_ptr); }

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
    const auto np = node::make_node();

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
    return np;
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
    m_root = insert__rec(m_root, w, key.begin(), key.end());
  }

private:
  node_ptr
  insert__rec(const node_ptr &p,
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
      p->m_children[true] = insert__rec(p->m_children[true], w, ++curr, end);
      return p;
    }

    // Case: Iterator behind
    // -> Insert new node in-between
    if (m_char_comp(*curr, p->m_char)) {
      const auto np = node::make_node(*curr, p, node::make_node());
      np->m_words = p->m_words;
      p->m_words = word_set_t();
      m_tree_size += 2; // <- new node and its NIL 'false' child
      np->m_children[true]  = insert__rec(np->m_children[true], w, ++curr, end);
      return np;
    }

    // Case: Iterator ahead
    // -> Follow 'false' child
    if (m_char_comp(p->m_char, *curr)) {
      p->m_children[false] = insert__rec(p->m_children[false], w, curr, end);
      return p;
    }

    // Case: Iterator and node matches
    // -> Follow 'true' child
    p->m_children[true] = insert__rec(p->m_children[true], w, ++curr, end);
    return p;
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
  ///
  /// \warning No functioning implementation is available!
  //////////////////////////////////////////////////////////////////////////////
  word_set_t
  keys() const
  {
    word_set_t filter;
    return word_set_t(); // <-- TODO
  }

private:
  // TODO
  word_set_t
  keys__rec(const node_ptr p,
            const word_t &path
            /*word_set_t filter*/) const
  {
    word_set_t ret;
    return ret;
  }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Obtain all words in the Anatree of a certain length, excluding
  ///        words that are a anagram of another one returned.
  ///
  /// \param word_length The length of the desired words
  //////////////////////////////////////////////////////////////////////////////
  word_set_t
  keys(const size_t word_length) const
  {
    return keys__rec(word_length, m_root, 0);
  }

private:
  word_set_t
  keys__rec(const size_t word_length,
            const node_ptr p,
            const size_t true_edges) const
  {
    assert(true_edges <= word_length);

    word_set_t ret;

    // Case: Found word of 'word_length'
    // -> Search succesful (no need to keep on searching deeper)
    if (word_length == true_edges) {
      if (p->m_words.size() > 0) {
        ret.insert(*p->m_words.begin());
      }
      return ret;
    }

    // Case: Tree stopped early
    // -> Abandon subtree
    if (p->m_char == node::NIL) {
      return ret;
    }

    // Case: Missing characters
    // -> Merge recursively from false and true subtrees
    const auto rec_true = keys__rec(word_length, p->m_children[true], true_edges+1);
    ret.insert(rec_true.begin(), rec_true.end());

    const auto rec_false = keys__rec(word_length, p->m_children[false], true_edges);
    ret.insert(rec_false.begin(), rec_false.end());

    return ret;
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
    word_t key = sorted_word(w);
    return anagrams_of__rec(m_root, key.begin(), key.end());
  }

private:
  word_set_t
  anagrams_of__rec(const node_ptr p,
                   typename word_t::iterator curr,
                   const typename word_t::iterator end) const
  {
    // Case: Iterator is done
    if (curr == end) {
      return p->m_words;
    }

    // Case: Iterator behind or tree is done
    // -> No words with all letters exist, return Ø .
    if (m_char_comp(*curr, p->m_char) || p->m_char == node::NIL) {
      return word_set_t();
    }

    // Case: Iterator ahead
    // -> Follow 'false' child
    if (m_char_comp(p->m_char, *curr)) {
      return anagrams_of__rec(p->m_children[false], curr, end);
    }

    // Case: Iterator and node matches
    // -> Follow 'true' child
    return anagrams_of__rec(p->m_children[true], ++curr, end);
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
  subanagrams_of__rec(const node_ptr p,
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
    return anagrams_of(w).contains(w);
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
};

#endif // ANATREE_H
