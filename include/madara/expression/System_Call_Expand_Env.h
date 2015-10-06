/* -*- C++ -*- */
#ifndef _MADARA_SYSTEM_CALL_EXPAND_ENV_H_
#define _MADARA_SYSTEM_CALL_EXPAND_ENV_H_

#ifndef _MADARA_NO_KARL_

#include <string>
#include <stdexcept>
#include "madara/utility/stdint.h"
#include "madara/expression/System_Call_Node.h"


namespace madara
{
  namespace expression
  {
    // Forward declaration.
    class Visitor;

    /**
     * @class System_Call_Expand_Env
     * @brief Returns the expansion of a statement with environment vars.
     *        For instance, VAR=$(VAR) would expand to VAR=MY VAR CONTENTS
     *        if VAR had been set to MY VAR CONTENTS in the operating system
     */
    class System_Call_Expand_Env : public System_Call_Node
    {
    public:
      /**
       * Constructor
       **/
      System_Call_Expand_Env (
        madara::knowledge::Thread_Safe_Context & context,
        const Component_Nodes & nodes);
      
      /**
       * Destructor
       **/
      virtual ~System_Call_Expand_Env (void);

      /**
       * Returns the value of the node
       * @return    value of the node
       **/
      virtual madara::Knowledge_Record item (void) const;

      /** 
       * Prunes the expression tree of unnecessary nodes. 
       * @param     can_change   set to true if variable nodes are contained
       * @return    value of current contained expression tree
       **/
      virtual madara::Knowledge_Record prune (bool & can_change);

      /** 
       * Evaluates the expression tree. 
       * @return    value of current contained expression tree
       **/
      virtual madara::Knowledge_Record evaluate (
        const madara::knowledge::Knowledge_Update_Settings & settings);

      /** 
       * Accepts a visitor subclassed from the Visitor class
       * @param    visitor   visitor instance to use
       **/
      virtual void accept (Visitor &visitor) const;
    };
  }
}

#endif // _MADARA_NO_KARL_

#endif /* _MADARA_SYSTEM_CALL_EXPAND_ENV_H_ */