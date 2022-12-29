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

#include<algorithm>
#include<assert.h>
#include<memory>
#include<sstream>
#include<string>
#include<unordered_set>

////////////////////////////////////////////////////////////////////////////////
/// \brief A data structure capable of storing a set of 'std::string' (or
/// similar) data structures, enabling quick access to all 'anagrams' of each
/// word (within or not).
////////////////////////////////////////////////////////////////////////////////
template<typename word_t = std::string, typename char_t = std::string::value_type>
class anatree
{
private:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Individual node of the Anatree.
  ///////////////////////////////////////////////////////////////////////////////
  class node
  {
  public:
    typedef std::shared_ptr<node> ptr;

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
    std::unordered_set<word_t> m_words;

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
  /// \brief Root of the anatree (initially a NIL node).
  //////////////////////////////////////////////////////////////////////////////
  typename node::ptr m_root = node::make_node();
  size_t m_tree_size = 1u;

public:
  anatree() = default;
  anatree(const anatree&) = default;

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Adds the word 'w' to the anatree.
  //////////////////////////////////////////////////////////////////////////////
  void
  insert(const word_t& w)
  {
    word_t key = sorted_word(w);
    m_root = insert__rec(m_root, w, key.begin(), key.end());
  }

private:
  typename node::ptr
  insert__rec(const typename node::ptr& p,
              const word_t& w,
              typename word_t::iterator curr,
              const typename word_t::iterator end)
  {
    assert(p != nullptr);

    // Case: Iterator done
    // -> Insert word
    if (curr == end) {
      p->m_words.insert(w);
      return p;
    }

    // Case: NIL
    // -> Turn into non-NIL node
    if (p->m_char == node::NIL) {
      assert(p->m_children[false] == nullptr && p->m_children[true] == nullptr);
      p->init(*curr);
      m_tree_size += 2;
      p->m_children[true] = insert__rec(p->m_children[true], w, ++curr, end);
      return p;
    }

    // Case: Iterator behind
    // -> Insert new node in-between
    if (*curr < p->m_char) {
      const typename node::ptr np = node::make_node(*curr, p, node::make_node());
      np->m_words = p->m_words;
      p->m_words = std::unordered_set<word_t>();
      m_tree_size += 1;
      np->m_children[true]  = insert__rec(np->m_children[true], w, ++curr, end);
      return np;
    }

    // Case: Iterator ahead
    // -> Follow 'false' child
    if (p->m_char < *curr) {
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
  /// \brief Obtain the minimal representative set words that cover all of the
  ///        words contained within the Anatree.
  ///
  /// \detail Each word in the Anatree is either in the returned result or it is
  ///         a subanagram of some other word in the result.
  //////////////////////////////////////////////////////////////////////////////
  typename std::unordered_set<word_t>
  keys() const
  {
    return keys__rec(m_root);
  }

private:
  typename std::unordered_set<word_t>
  keys__rec(const typename node::ptr p) const
  {
    std::unordered_set<word_t> ret;
    if (p->m_char == node::NIL) {
      if (p->m_words.size() > 0) {
        ret.insert(*p->m_words.begin());
      }
      return ret;
    }

    std::unordered_set<word_t> rec_false = keys__rec(p->m_children[false]);
    std::unordered_set<word_t> rec_true = keys__rec(p->m_children[true]);
    ret.insert(rec_false.begin(), rec_false.end());
    ret.insert(rec_true.begin(), rec_true.end());
    return ret;
  }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Obtain all words that are anagrams of 'w'.
  ///
  /// \details An anagram is a word that can be created from (all) the of 'w'.
  //////////////////////////////////////////////////////////////////////////////
  typename std::unordered_set<word_t>
  anagrams_of(const word_t& w) const
  {
    word_t key = sorted_word(w);
    return anagrams_of__rec(m_root, key.begin(), key.end());
  }

private:
  typename std::unordered_set<word_t>
  anagrams_of__rec(const typename node::ptr p,
                   typename word_t::iterator curr,
                   const typename word_t::iterator end) const
  {
    // Case: Iterator is done
    if (curr == end) {
      return p->m_words;
    }

    // Case: Iterator behind or tree is done
    // -> No words with all letters exist, return Ø .
    if (*curr < p->m_char || p->m_char == node::NIL) {
      return std::unordered_set<word_t>();
    }

    // Case: Iterator ahead
    // -> Follow 'false' child
    if (p->m_char < *curr) {
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
  typename std::unordered_set<word_t>
  subanagrams_of(const word_t& w) const
  {
    word_t key = sorted_word(w);
    return subanagrams_of__rec(m_root, key.begin(), key.end());
  }

private:
  typename std::unordered_set<word_t>
  subanagrams_of__rec(const typename node::ptr p,
                      typename word_t::iterator curr,
                      const typename word_t::iterator end) const
  {
    // Case: Iterator or Anatree is done
    if (curr == end || p->m_char == node::NIL) {
      return p->m_words;
    }

    // Case: Iterator behind
    // -> Skip missing characters
    if (*curr < p->m_char) {
      while (*curr < p->m_char && curr != end) { ++curr; }
      return subanagrams_of__rec(p, curr, end);
    }

    // Case: Iterator ahead
    // -> Follow 'false' child
    if (p->m_char < *curr) {
      std::unordered_set<word_t> ret(p->m_words);
      const std::unordered_set<word_t> rec_false = subanagrams_of__rec(p->m_children[false], curr, end);
      ret.insert(rec_false.begin(), rec_false.end());
      return ret;
    }

    // Case: Iterator and node matches
    // -> Follow both children, merge results and add words on current node
    ++curr;

    std::unordered_set<word_t> ret(p->m_words);
    const std::unordered_set<word_t> rec_false = subanagrams_of__rec(p->m_children[false], curr, end);
    const std::unordered_set<word_t> rec_true = subanagrams_of__rec(p->m_children[true], curr, end);
    ret.insert(rec_false.begin(), rec_false.end());
    ret.insert(rec_true.begin(), rec_true.end());
    return ret;
  }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Whether the Anatree includes the word 'w'.
  //////////////////////////////////////////////////////////////////////////////
  bool
  contains(const word_t& w) const
  {
    return anagrams_of(w).contains(w);
  }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Remove all nodes/anagrams.
  //////////////////////////////////////////////////////////////////////////////
  void
  clear()
  {
    m_root = node::make_node();
    m_tree_size = 1u;
  }

public:
  //////////////////////////////////////////////////////////////////////////////
  /// \brief Number of nodes.
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
  word_t sorted_word(const word_t& w) const
  {
    word_t ret(w);
    std::sort(ret.begin(), ret.end());
    return ret;
  }
};

#endif // ANATREE_H
