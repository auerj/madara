/* -*- C++ -*- */
#ifndef _EXPRESSION_TREE_CPP_
#define _EXPRESSION_TREE_CPP_

#ifndef _MADARA_NO_KARL_

#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include <string>
#include <map>
#include <stdexcept>

#include "madara/expression/Component_Node.h"
#include "madara/expression/Iterator.h"
#include "madara/expression/Iterator_Impl.h"
#include "madara/expression/Expression_Tree.h"
#include "madara/expression/Leaf_Node.h"

namespace madara
{
  namespace expression
  {
    /**
     * @class Expression_Tree_Iterator_Factory
     * @brief Implementation of a factory pattern that dynamically allocates
     *        the appropriate @a Expression_Tree_Iterator_Impl object.
     * 
     *        This is a variant of the Abstract Factory pattern that
     *        has a set of related factory methods but which doesn't use
     *        inheritance. 
     *
     * @see   Expression_Tree_Level_Order_Iterator_Impl,
     *        Expression_Tree_In_Order_Iterator_Impl, 
     *        Expression_Tree_Pre_Order_Iterator_Impl, and
     *        Expression_Tree_Post_Order_Iterator_Impl.
     */


    enum
    {
      EQUALITY_CHAR = 0-1,
      INEQUALITY_CHAR = 0-2,
      GREATER_THAN_EQUAL = 0-3,
      LESSER_THAN_EQUAL = 0-4,
    };


    class Expression_Tree_Iterator_Factory
    {
    public:
      /// Constructor.
      Expression_Tree_Iterator_Factory (void);

      /// Dynamically allocate a new @a Expression_Tree_Iterator_Impl
      /// object based on the designated @a traversal_order and @a end_iter.
      Expression_Tree_Iterator_Impl * make_tree_iterator (Expression_Tree &tree, 
                                                         const std::string &traversal_order,
                                                         bool end_iter);

    private:
      /// Dynamically allocate a new @a Expression_Tree_Level_Order_Iterator_Impl
      /// object based on the designated @a end_iter.
      Expression_Tree_Iterator_Impl *make_in_order_tree_iterator (Expression_Tree &tree,
                                                                  bool end_iter);

      /// Dynamically allocate a new @a Expression_Tree_Pre_Order_Iterator_Impl
      /// object based on the designated @a end_iter.
      Expression_Tree_Iterator_Impl *make_pre_order_tree_iterator (Expression_Tree &tree,
                                                                   bool end_iter);

      /// Dynamically allocate a new @a Expression_Tree_Post_Order_Iterator_Impl
      /// object based on the designated @a end_iter.
      Expression_Tree_Iterator_Impl *make_post_order_tree_iterator (Expression_Tree &tree,
                                                                    bool end_iter);

      /// Dynamically allocate a new @a Expression_Tree_Level_Order_Iterator_Impl
      /// object based on the designated @a end_iter.
      Expression_Tree_Iterator_Impl *make_level_order_tree_iterator (Expression_Tree &tree,
                                                                     bool end_iter);

      typedef Expression_Tree_Iterator_Impl *(Expression_Tree_Iterator_Factory::*TRAVERSAL_PTMF)
        (Expression_Tree &tree, bool end_iter);
      typedef ::std::map < std::string, TRAVERSAL_PTMF> TRAVERSAL_MAP;

      TRAVERSAL_MAP traversal_map_;
    };
  }
}

madara::expression::Expression_Tree_Iterator_Factory::Expression_Tree_Iterator_Factory (void)
{
  traversal_map_["in-order"] =
    &Expression_Tree_Iterator_Factory::make_in_order_tree_iterator;
  traversal_map_["pre-order"] =
    &Expression_Tree_Iterator_Factory::make_pre_order_tree_iterator;
  traversal_map_["post-order"] =
    &Expression_Tree_Iterator_Factory::make_post_order_tree_iterator;
  traversal_map_["level-order"] =
    &Expression_Tree_Iterator_Factory::make_level_order_tree_iterator;
}

madara::expression::Expression_Tree_Iterator_Impl *
madara::expression::Expression_Tree_Iterator_Factory::make_level_order_tree_iterator (
  madara::expression::Expression_Tree &tree, bool end_iter)
{ 
  return new Level_Order_Expression_Tree_Iterator_Impl (tree, end_iter);
}

madara::expression::Expression_Tree_Iterator_Impl *
madara::expression::Expression_Tree_Iterator_Factory::make_in_order_tree_iterator (
  madara::expression::Expression_Tree &tree, bool end_iter)
{ 
  return new In_Order_Iterator_Impl (tree, end_iter);
}

madara::expression::Expression_Tree_Iterator_Impl *
madara::expression::Expression_Tree_Iterator_Factory::make_pre_order_tree_iterator (
  madara::expression::Expression_Tree &tree, bool end_iter)
{ 
  return new Pre_Order_Iterator_Impl (tree, end_iter);
}

madara::expression::Expression_Tree_Iterator_Impl *
madara::expression::Expression_Tree_Iterator_Factory::make_post_order_tree_iterator (
  Expression_Tree &tree, bool end_iter)
{ 
  return new Post_Order_Iterator_Impl (tree, end_iter);
}

madara::expression::Expression_Tree_Iterator_Impl *
madara::expression::Expression_Tree_Iterator_Factory::make_tree_iterator (
  madara::expression::Expression_Tree &tree, const std::string &traversal_order,
  bool end_iter)
{  
  TRAVERSAL_MAP::iterator iter = traversal_map_.find (traversal_order);
  if (iter == traversal_map_.end ())
    {
      return 0;
    }
  else
    {
      Expression_Tree_Iterator_Factory::TRAVERSAL_PTMF ptmf = iter->second;
      return (this->*ptmf) (tree, end_iter);
    }
}

// Define a single instance of a factory that's local to this class.
static madara::expression::Expression_Tree_Iterator_Factory tree_iterator_factory;

// Default ctor

madara::expression::Expression_Tree::Expression_Tree (
  logger::Logger & logger)
  : logger_ (&logger), root_ (0)
{    
}

// Ctor take an underlying NODE*.

madara::expression::Expression_Tree::Expression_Tree (
  logger::Logger & logger,
  madara::expression::Component_Node *root, bool increase_count)
  : logger_ (&logger), root_ (root, increase_count)
{    
}

// Copy ctor

madara::expression::Expression_Tree::Expression_Tree (
  logger::Logger & logger,
  const madara::expression::Expression_Tree &t)
  : logger_ (&logger), root_ (t.root_)
{
}

// Assignment operator

void
madara::expression::Expression_Tree::operator= (
  const madara::expression::Expression_Tree &t)
{
  // Refcounter class takes care of the internal decrements and
  // increments.
  if (this != &t)
  {
    logger_ = t.logger_;
    root_ = t.root_;
  }
}

// Dtor

madara::expression::Expression_Tree::~Expression_Tree (void)
{
  // taken care of by Refcounter class
}

// Check if the tree is empty.

bool
madara::expression::Expression_Tree::is_null (void) const
{
  return root_.get_ptr () == 0;
}


/// Prune the tree of unnecessary nodes. 
/// Returns evaluation of the node and sets can_change appropriately.
/// if this node can be changed, that means it shouldn't be pruned.
madara::Knowledge_Record
madara::expression::Expression_Tree::prune (void)
{
  bool root_can_change = false;
  madara::Knowledge_Record root_value;

  if (this->root_.get_ptr ())
  {
    root_value = this->root_->prune (root_can_change);
    if (!root_can_change && 
      dynamic_cast <Leaf_Node *> (this->root_.get_ptr ()) == 0)
    {
      root_ = new Leaf_Node (*(this->logger_), root_value);
    }
  }

  return root_value;
}


/// Evaluates the node and its children. This does not prune any of
/// the expression tree, and is much faster than the prune function
madara::Knowledge_Record 
madara::expression::Expression_Tree::evaluate (
  const madara::knowledge::Knowledge_Update_Settings & settings)
{
  if (root_.get_ptr () != 0)
    return root_->evaluate (settings);
  else
    return madara::Knowledge_Record::Integer (0);
}


// return root pointer
madara::expression::Component_Node *
madara::expression::Expression_Tree::get_root (void) 
{
  return root_.get_ptr ();
}

// Return the stored item.

madara::Knowledge_Record 
madara::expression::Expression_Tree::item (void) const
{
  return root_->item ();
}

// Return the left branch.

madara::expression::Expression_Tree
madara::expression::Expression_Tree::left (void)
{
  return Expression_Tree (*logger_, root_->left (), true);
}

// Return the left branch.

madara::expression::Expression_Tree
madara::expression::Expression_Tree::right (void)
{
  return Expression_Tree (*logger_, root_->right (), true);
}

// Return a begin iterator of a specified type.

madara::expression::Expression_Tree::iterator
madara::expression::Expression_Tree::begin (const std::string &traversal_order)
{
  return Expression_Tree::iterator
    (tree_iterator_factory.make_tree_iterator (*this, 
                                               traversal_order, 
                                               false));
}

// Return an end iterator of a specified type.

madara::expression::Expression_Tree::iterator
madara::expression::Expression_Tree::end (const std::string &traversal_order)
{
  return Expression_Tree::iterator
  (tree_iterator_factory.make_tree_iterator (*this, 
                                               traversal_order, 
                                               true));
}

// Return a begin iterator of a specified type.

madara::expression::Expression_Tree_Const_Iterator
madara::expression::Expression_Tree::begin (const std::string &traversal_order) const
{
  Expression_Tree *non_const_this = const_cast <Expression_Tree *> (this);
  return Expression_Tree::const_iterator (tree_iterator_factory.make_tree_iterator (*non_const_this,
                                                                                    traversal_order,
                                                                                    false));
}

// Return an end iterator of a specified type.

madara::expression::Expression_Tree_Const_Iterator
madara::expression::Expression_Tree::end (const std::string &traversal_order) const
{
  Expression_Tree *non_const_this = const_cast <Expression_Tree *> (this);
  return Expression_Tree::const_iterator (tree_iterator_factory.make_tree_iterator (*non_const_this,
                                                                                    traversal_order,
                                                                                    true));
}

/// Accept a visitor to perform some action on the Expression_Tree.

void
madara::expression::Expression_Tree::accept (Visitor &visitor) const
{
  root_->accept (visitor);
}

#endif // _MADARA_NO_KARL_

#endif /* _EXPRESSION_TREE_CPP_ */