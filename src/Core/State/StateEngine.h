/*
 For more information, please see: http://software.sci.utah.edu

 The MIT License

 Copyright (c) 2009 Scientific Computing and Imaging Institute,
 University of Utah.


 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
 */

#ifndef CORE_STATE_STATEENGINE_H
#define CORE_STATE_STATEENGINE_H

// STL includes
#include <map>
#include <set>

// boost includes
#include <boost/unordered_map.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include <Core/Utils/Log.h>
#include <Core/Utils/Exception.h>
#include <Core/Utils/Singleton.h>
#include <Core/Utils/Lockable.h>

#include <Core/State/StateBase.h>

namespace Core
{

// CLASS STATEENGINE

// Forward decclaration
class StateEngine;
class StateEnginePrivate;
class StateHandler;

// Class definition
class StateEngine : public Core::RecursiveLockable
{
  CORE_SINGLETON( StateEngine );
  
  // -- Constructor/destructor --
private:
  StateEngine();
  virtual ~StateEngine();

  // -- Interface for accessing state variables --
public:
  // GET_STATE:
  // Get pointer to the state variable based on the unique state tag
  bool get_state( const std::string& stateid, StateBaseHandle& state );

  // GET_STATE:
  // Get the state variable by index
  bool get_state( const size_t idx, StateBaseHandle& state);

  // NUMBER_OF_STATES:
  // The number of state variables in the system
  size_t number_of_states();
  
  // -- Interface for accounting stateids --
private:
  friend class StateHandler;
  
  //
  std::string register_state_handler( const std::string& type_str, 
    StateHandler* state_handler, bool auto_id );

  void remove_state_handler( const std::string& handler_id );
  
  // -- Signals --
public:
  typedef boost::signals2::signal<void ()> state_changed_signal_type;
  state_changed_signal_type state_changed_signal_;

  // -- Implementation details --
private:
  StateEnginePrivate* private_;

  // -- Static convenience functions --
public:

  // GETMUTEX
  // Get the mutex of the state engine
  static mutex_type& GetMutex()
  {
    return Instance()->get_mutex();
  }
  
};

} // end namespace Core

#endif