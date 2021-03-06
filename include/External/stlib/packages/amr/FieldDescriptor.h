// -*- C++ -*-

/*!
  \file amr/FieldDescriptor.h
  \brief Describe a data field.
*/

#if !defined(__amr_FieldDescriptor_h__)
#define __amr_FieldDescriptor_h__

#include <string>

namespace amr {

//! Describe a data field.
class FieldDescriptor {

   //
   // Member data.
   //
private:

   int _numberOfComponents;
   std::string _name;

   //--------------------------------------------------------------------------
   /*! \name Constructors etc.
    Use the synthesized copy constructor, assignment operator, and destructor.*/
   //@{
public:

   //! Construct from the number of components and the name.
   FieldDescriptor(const int numberOfComponents, std::string name) :
      _numberOfComponents(numberOfComponents),
      _name(name) {
   }

   //@}
   //--------------------------------------------------------------------------
   //! \name Accessors.
   //@{
public:

   //! Get the number of components.
   int
   getNumberOfComponents() const {
      return _numberOfComponents;
   }

   //! Get the name.
   const std::string&
   getName() const {
      return _name;
   }

   //@}
   //--------------------------------------------------------------------------
   //! \name Equality.
   //@{
public:

   //! Return true if the data structures are equal.
   bool
   operator==(const FieldDescriptor& other) {
      return _numberOfComponents == other._numberOfComponents &&
             _name == other._name;
   }

   //@}
};

} // namespace amr

#endif
