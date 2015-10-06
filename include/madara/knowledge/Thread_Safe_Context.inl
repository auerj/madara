#ifndef _MADARA_THREADSAFECONTEXT_INL_
#define _MADARA_THREADSAFECONTEXT_INL_

/**
 * @file Thread_Safe_Context.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains inline functions for a thread safe context
 */

#include "ace/Guard_T.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Condition_Recursive_Thread_Mutex.h"
#include "ace/Synch.h"

#include "madara/expression/Interpreter.h"

#include <sstream>


inline madara::Knowledge_Record
madara::knowledge::Thread_Safe_Context::get (
  const std::string & key,
  const Knowledge_Reference_Settings & settings) const
{
  std::string cur_key;
  Knowledge_Record result;

  if (settings.expand_variables)
  {
    cur_key = expand_statement (key);
  }
  else
  {
    cur_key = key;
  }

  Knowledge_Map::const_iterator found = map_.find (cur_key);

  if (found != map_.end ())
  {
    return found->second;
  }

  return result;
}


inline int
madara::knowledge::Thread_Safe_Context::read_file (
  const std::string & key,
  const std::string & filename,
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return read_file (variable, filename, settings);
}

// return the value of a variable
inline madara::Knowledge_Record
madara::knowledge::Thread_Safe_Context::get (
  const Variable_Reference & variable,
  const Knowledge_Reference_Settings &) const
{
  Context_Guard guard (mutex_);

  if (variable.record_)
    return *variable.record_;
  else
    return Knowledge_Record ();
}

// return the value of a variable
inline bool
madara::knowledge::Thread_Safe_Context::exists (
  const Variable_Reference & variable,
  const Knowledge_Reference_Settings &) const
{
  Context_Guard guard (mutex_);

  if (variable.record_)
    return variable.record_->exists ();
  else
    return false;
}

// return the value of a variable
inline madara::Knowledge_Record
madara::knowledge::Thread_Safe_Context::retrieve_index (
  const Variable_Reference & variable,
  size_t index,
  const Knowledge_Reference_Settings &)
{
  Context_Guard guard (mutex_);

  if (variable.record_)
    return variable.record_->retrieve_index (index);
  else
    return Knowledge_Record ();
}

inline madara::Knowledge_Record
madara::knowledge::Thread_Safe_Context::retrieve_index (
  const std::string & key,
  size_t index,
  const Knowledge_Reference_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return retrieve_index (variable, index, settings);
}

inline int
madara::knowledge::Thread_Safe_Context::set (
  const std::string & key,
  const madara::Knowledge_Record & value,
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return set (variable, value, settings);
}


inline int
madara::knowledge::Thread_Safe_Context::set_xml (
  const std::string & key,
  const char * value, size_t size, 
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return set_xml (variable, value, size, settings);
}

inline int
madara::knowledge::Thread_Safe_Context::set_text (
  const std::string & key,
  const char * value, size_t size, 
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return set_text (variable, value, size, settings);
}

inline int
madara::knowledge::Thread_Safe_Context::set_jpeg (
  const std::string & key,
  const unsigned char * value, size_t size, 
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return set_jpeg (variable, value, size, settings);
}

inline int
madara::knowledge::Thread_Safe_Context::set_file (
  const std::string & key,
  const unsigned char * value, size_t size, 
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return set_file (variable, value, size, settings);
}

inline int
madara::knowledge::Thread_Safe_Context::set (
  const std::string & key,
  madara::Knowledge_Record::Integer value, 
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return set (variable, value, settings);
}

inline int
madara::knowledge::Thread_Safe_Context::set (
  const std::string & key,
  double value, 
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return set (variable, value, settings);
}

inline int
madara::knowledge::Thread_Safe_Context::set (
  const std::string & key,
  const std::string & value, 
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return set (variable, value, settings);
}

inline int
madara::knowledge::Thread_Safe_Context::set_index (
  const std::string & key,
  size_t index, Knowledge_Record::Integer value, 
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return set_index (variable, index, value, settings);
}

inline int
madara::knowledge::Thread_Safe_Context::set_index (
  const std::string & key,
  size_t index, double value, 
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return set_index (variable, index, value, settings);
}


inline int
madara::knowledge::Thread_Safe_Context::set (const std::string & key,
  const madara::Knowledge_Record::Integer * value,
  uint32_t size,
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return set (variable, value, size, settings);
}

inline int
madara::knowledge::Thread_Safe_Context::set (const std::string & key,
  const double * value,
  uint32_t size,
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return set (variable, value, size, settings);
}


inline int
madara::knowledge::Thread_Safe_Context::set (const std::string & key,
  const std::vector <Knowledge_Record::Integer> & value,
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return set (variable, value, settings);
}

inline int
madara::knowledge::Thread_Safe_Context::set (const std::string & key,
  const std::vector <double> & value,
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return set (variable, value, settings);
}


inline  madara::Knowledge_Record
madara::knowledge::Thread_Safe_Context::inc (
  const std::string & key,
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return inc (variable, settings);
}

inline  madara::Knowledge_Record
madara::knowledge::Thread_Safe_Context::dec (
  const std::string & key,
  const Knowledge_Update_Settings & settings)
{
  Variable_Reference variable = get_ref (key, settings);
  return dec (variable, settings);
}

inline  madara::Knowledge_Record
madara::knowledge::Thread_Safe_Context::inc (
  const Variable_Reference & variable,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (settings.always_overwrite ||
        variable.record_->write_quality >= variable.record_->quality)
    {
      ++(*variable.record_);
      variable.record_->quality = variable.record_->write_quality;
  
      mark_and_signal (variable.name_.get_ptr (), variable.record_, settings);
    }

    return *variable.record_;
  }

  return Knowledge_Record ();
}

#ifndef _MADARA_NO_KARL_

// return whether or not the key exists
inline bool
madara::knowledge::Thread_Safe_Context::delete_expression (
  const std::string & expression)
{
  Context_Guard guard (mutex_);

  return interpreter_->delete_expression (expression);
}

#endif // _MADARA_NO_KARL_

// return whether or not the key exists
inline bool
madara::knowledge::Thread_Safe_Context::delete_variable (
  const std::string & key,
  const Knowledge_Reference_Settings & settings)
{
  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;

  return map_.erase (*key_ptr) == 1;
}

// return whether or not the key exists
inline bool
madara::knowledge::Thread_Safe_Context::exists (
  const std::string & key,
  const Knowledge_Reference_Settings & settings) const
{
  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;

  // if key is not null
  if (*key_ptr != "")
  {
    // find the key in the knowledge base
    Knowledge_Map::const_iterator found = map_.find (*key_ptr);

    // if it's found, then return the value
    if (found != map_.end ())
      return found->second.status () != Knowledge_Record::UNCREATED;
  }

  // if no match, return empty (0)
  return false;
}

// Atomically decrement a stored value. Only reason we are inlining this function
// is because it is called by only one function, and we can save a bit of
// execution time via expansion into that function call.
inline madara::Knowledge_Record
madara::knowledge::Thread_Safe_Context::dec (
  const Variable_Reference & variable,
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  if (variable.record_)
  {
    // check if we have the appropriate write quality
    if (settings.always_overwrite ||
        variable.record_->write_quality >= variable.record_->quality)
    {
      --(*variable.record_);
      variable.record_->quality = variable.record_->write_quality;
  
      mark_and_signal (variable.name_.get_ptr (), variable.record_, settings);
    }

    return *variable.record_;
  }

  return Knowledge_Record ();
}

/// set the lamport clock (updates with lamport clocks lower
/// than our current clock get discarded)
inline uint64_t
madara::knowledge::Thread_Safe_Context::set_clock (
  uint64_t clock)
{
  Context_Guard guard (mutex_);

  // clock_ is always increasing. We never reset it to a lower clock value
  // user can check return value to see if the clock was set.
  if (clock_ < clock)
    clock_ = clock;

  return clock_;
}

/// set the lamport clock (updates with lamport clocks lower
/// than our current clock get discarded)
inline uint64_t 
madara::knowledge::Thread_Safe_Context::set_clock (
  const std::string & key, uint64_t clock,
  const Knowledge_Reference_Settings & settings)
{
  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;
  
  // check for null key
  if (*key_ptr == "")
    return 0;

  // create the key if it didn't exist
  Knowledge_Record & record = map_[*key_ptr];

  // check for value already set
  if (record.clock < clock)
  {
    record.clock = clock;

    // try to update the global clock as well
    this->set_clock (clock);
  }

  return record.clock;
}

/// set the lamport clock for a particular variable (updates with 
/// lamport clocks lower than our current clock get discarded)
inline uint64_t
madara::knowledge::Thread_Safe_Context::inc_clock (
  const std::string & key,
  const Knowledge_Update_Settings & settings)
{
  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;
  
  // check for null key
  if (*key_ptr == "")
    return 0;

  // create the key if it didn't exist
  Knowledge_Record & record = map_[*key_ptr];

  return record.clock += settings.clock_increment;
}

/// increment the process lamport clock
inline uint64_t
madara::knowledge::Thread_Safe_Context::inc_clock (
  const Knowledge_Update_Settings & settings)
{
  Context_Guard guard (mutex_);
  return clock_ += settings.clock_increment;
}

/// get the lamport clock (updates with lamport clocks lower
/// than our current clock get discarded)
inline uint64_t
madara::knowledge::Thread_Safe_Context::get_clock (void) const
{
  Context_Guard guard (mutex_);
  return clock_;
}

inline madara::logger::Logger &
madara::knowledge::Thread_Safe_Context::get_logger (void) const
{
  Context_Guard guard (mutex_);
  return *logger_;
}

inline void
madara::knowledge::Thread_Safe_Context::attach_logger (
  logger::Logger & logger) const
{
  Context_Guard guard (mutex_);
  logger_ = &logger;
}

/// get the lamport clock for a particular variable
inline uint64_t
madara::knowledge::Thread_Safe_Context::get_clock (
  const std::string & key,
  const Knowledge_Reference_Settings & settings) const
{
  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;
  
  // check for null key
  if (*key_ptr == "")
    return 0;

  // find the key in the knowledge base
  Knowledge_Map::const_iterator found = map_.find (*key_ptr);

  // if it's found, then compare the value
  if (found != map_.end ())
  {
    return found->second.clock;
  }
  else
    // key does not exist
    return 0;
}

/// Lock the mutex on this context. Warning: this will cause
/// all operations to block until the unlock call is made.
inline void
madara::knowledge::Thread_Safe_Context::lock (void) const
{
  mutex_.acquire ();
}

/// Unlock the mutex on this context.
inline void
madara::knowledge::Thread_Safe_Context::unlock (void) const
{
  mutex_.release ();
}

/// Print a statement, similar to printf (variable expressions allowed)
/// e.g. input = "MyVar{.id} = {MyVar{.id}}\n";
inline void 
madara::knowledge::Thread_Safe_Context::print (
  const std::string & statement, unsigned int level) const
{
  madara_logger_ptr_log (logger_, (int)level,
    this->expand_statement (statement).c_str ());
}

// clear all variables and their values
inline void
madara::knowledge::Thread_Safe_Context::clear (bool erase)
{
  // enter the mutex
  Context_Guard guard (mutex_);

  if (erase)
  {
    map_.clear ();
  }

  else
  {
    for (madara::Knowledge_Map::iterator i = map_.begin ();
      i != map_.end (); ++i)
    {
      i->second.reset_value ();
    }
  }

  changed_map_.clear ();
  changed_.signal ();
}


/// Make the current thread of execution wait for a change on the
/// context
inline void 
madara::knowledge::Thread_Safe_Context::wait_for_change (
  bool extra_release)
{
  // enter the mutex
  Context_Guard guard (mutex_);

  // if the caller is relying on a recursive call (e.g. Knowlege_Base::wait),
  // we'll need to call an extra release for this to work. Otherwise, the 
  // context would remain locked to the calling thread - even though it will
  // now be put to sleep
  if (extra_release)
    mutex_.release ();

  changed_.wait ();

  //if (extra_release)
  //  mutex_.lock ();
}

inline void
madara::knowledge::Thread_Safe_Context::mark_local_modified (
  const std::string & key, madara::Knowledge_Record & record,
  const Knowledge_Reference_Settings & settings
  )
{
  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;

  if (*key_ptr != "")
  {
    local_changed_map_[*key_ptr] = &record;

    if (record.status () != madara::Knowledge_Record::MODIFIED)
      record.set_modified ();
  }
}

inline void
madara::knowledge::Thread_Safe_Context::mark_modified (
  const Variable_Reference & variable)
{
  Context_Guard guard (mutex_);
  
  // if the record is valid
  if (variable.record_ != 0)
  {
    // mark the changed map with the variable information
    changed_map_[variable.name_.get_ptr ()] = variable.record_;

    // and set its status to modified
    if (variable.record_->status () != madara::Knowledge_Record::MODIFIED)
      variable.record_->set_modified ();
  }
}

inline std::string
madara::knowledge::Thread_Safe_Context::debug_modifieds (void) const
{
  Context_Guard guard (mutex_);
  std::stringstream result;

  result << changed_map_.size () << " modifications ready to send:\n";

  for (Knowledge_Records::const_iterator i = changed_map_.begin ();
    i != changed_map_.end (); ++i)
  {
    if (i->second->is_binary_file_type ())
    {
      result << "File: ";
    }
    else if (i->second->type () == Knowledge_Record::DOUBLE)
    {
      result << "Double: ";
    }
    else if (i->second->type () == Knowledge_Record::DOUBLE_ARRAY)
    {
      result << "Double array: ";
    }
    else if (i->second->type () == Knowledge_Record::INTEGER)
    {
      result << "Integer: ";
    }
    else if (i->second->type () == Knowledge_Record::INTEGER_ARRAY)
    {
      result << "Integer array: ";
    }
    else if (i->second->is_string_type ())
    {
      result << "String: ";
    }
    else
    {
      result << "Unknown: ";
    }

    result << i->first << " = " << i->second->to_string () << "\n";
  }

  return result.str ();
}


inline void
madara::knowledge::Thread_Safe_Context::mark_modified (
  const std::string & key, madara::Knowledge_Record & record,
  const Knowledge_Reference_Settings & settings)
{
  // enter the mutex
  std::string key_actual;
  const std::string * key_ptr;
  Context_Guard guard (mutex_);
  
  // if the user needs variable expansion, do so
  if (settings.expand_variables)
  {
    key_actual = expand_statement (key);
    key_ptr = &key_actual;
  }
  else
    key_ptr = &key;

  // if the variable name is valid
  if (*key_ptr != "")
  {
    // mark the changed map with the variable information
    changed_map_[*key_ptr] = &record;
    
    // and set its status to modified
    if (record.status () != madara::Knowledge_Record::MODIFIED)
      record.set_modified ();
  }
}

/// Return list of variables that have been modified
inline const madara::Knowledge_Records &
madara::knowledge::Thread_Safe_Context::get_modifieds (void) const
{
  Context_Guard guard (mutex_);

  return changed_map_;
}

/// Return list of variables that have been modified
inline const madara::Knowledge_Records &
madara::knowledge::Thread_Safe_Context::get_local_modified (void) const
{
  Context_Guard guard (mutex_);

  return local_changed_map_;
}

/// Reset all variables to unmodified
inline void 
madara::knowledge::Thread_Safe_Context::reset_modified (void)
{
  Context_Guard guard (mutex_);

  changed_map_.clear ();
  local_changed_map_.clear ();
}

/// Changes all global variables to modified at current time
inline void 
madara::knowledge::Thread_Safe_Context::apply_modified (void)
{
  Context_Guard guard (mutex_);

  // each synchronization counts as an event, since this is a
  // pretty important networking event

  //++this->clock_;

  for (madara::Knowledge_Map::iterator i = map_.begin ();
       i != map_.end (); 
       ++i)
  {
    if (i->first.size () > 0 && i->first[0] != '.')
    {
      // local or global doesn't matter. Clock and modification
      // aren't really a part of local variable checking anyway
      //i->second.status = madara::Knowledge_Record::MODIFIED;

      if (i->second.status () != Knowledge_Record::UNCREATED)
        mark_modified (i->first, i->second, Knowledge_Update_Settings ());
      else
        i->second.set_value (Knowledge_Record::Integer (0));

      //i->second.clock = this->clock_;
    }
  }
}

/// Reset a variable to unmodified
inline void 
madara::knowledge::Thread_Safe_Context::reset_modified (
  const std::string & variable)
{
  Context_Guard guard (mutex_);

  changed_map_.erase (variable);
}


/// Signal the condition that it can wake up someone else on changed data.
inline void
madara::knowledge::Thread_Safe_Context::signal (bool lock) const
{
  if (lock)
  {
    Context_Guard guard (mutex_);
    changed_.signal ();
  }
  else
    changed_.signal ();
}

inline void
madara::knowledge::Thread_Safe_Context::add_logger (
  const std::string & filename)
{
  logger_->add_file (filename);
}

inline int
madara::knowledge::Thread_Safe_Context::get_log_level (void)
{
  return logger_->get_level ();
}

inline void
madara::knowledge::Thread_Safe_Context::set_log_level (int level)
{
  logger_->set_level (level);
}



#endif // _MADARA_THREADSAFECONTEXT_INL_