// -*- C++ -*-

#if !defined(__stochastic_Reaction_ipp__)
#error This file is an implementation detail of Reaction.
#endif

namespace stochastic {


// 3.7
// Compute the binomial coefficient for the case that n >= 0 and k > 0.
// This is a little faster than the version in the numerical package.
// Storing the inverse factorial does not pay off. This function is fast
// because most of the time k = 1 and the loop is skipped.
template<typename _Result, typename _Argument>
inline
_Result
_computeBinomialCoefficient(_Argument n, const std::size_t k) {
#ifdef DEBUG_stlib
   assert(n >= 0 && k > 0);
#endif
   _Result result = n;
   for (std::size_t i = 1; i != k; ++i) {
      result *= --n;
      result /= (i + 1);
   }
   return result;
}

#if 0
template<typename _Result, typename _Argument>
inline
_Result
_computeBinomialCoefficient(const _Argument n, const std::size_t k) {
   if (k == 1) {
      return n;
   }
   else {
      return 0.5 * n *(n - 1);
   }
}
#endif

// Compute the discrete mass action propensity function given the species
// populations.
template<bool _IsDiscrete>
template<typename Container>
inline
double
Reaction<_IsDiscrete>::
_computePropensityFunction(Loki::Int2Type<true> /*Discrete*/,
                           const Container& populations) const {
   double result = _scaledRateConstant;

   for (typename SparseVectorSizeType::const_iterator i = _reactants.begin();
         i != _reactants.end(); ++i) {
#ifdef DEBUG_stlib
      assert(i->first < populations.size());
#endif
      double p = populations[i->first];
      for (std::size_t j = 0; j != i->second; ++j) {
         result *= p--;
      }
   }
   return result;
}


// Compute the continuous mass action propensity function given the species
// populations.
template<bool _IsDiscrete>
template<typename Container>
inline
double
Reaction<_IsDiscrete>::
_computePropensityFunction(Loki::Int2Type<false> /*Continuous*/,
                           const Container& populations) const {
   double result = _scaledRateConstant;

   for (typename SparseVectorSizeType::const_iterator i = _reactants.begin();
         i != _reactants.end(); ++i) {
#ifdef DEBUG_stlib
      assert(i->first < populations.size());
#endif
      for (std::size_t j = 0; j != i->second; ++j) {
         result *= populations[i->first];
      }
   }
   return result;
}


// Compute the propensity function derivatives given the the species
// populations.
template<bool _IsDiscrete>
template<typename Container>
inline
void
Reaction<_IsDiscrete>::
_computePropensityFunctionDerivatives
(Loki::Int2Type<true> /*Discrete*/, const double propensityFunction,
 const Container& populations, array::SparseVector<double>* derivatives) const {
   // Compute the derivatives.
   derivatives->clear();
   double value;
   for (typename SparseVectorSizeType::const_iterator i = _reactants.begin();
         i != _reactants.end(); ++i) {
      // If there will be a singularity in evaluating the difference of harmonic
      // numbers.
      if (populations[i->first] >= 0 && populations[i->first] <= i->second &&
            populations[i->first] == std::size_t(populations[i->first])) {
         // Singular case.
         // Perturb the populations to avoid the singularity.
         Container perturbed(populations);
         perturbed[i->first] += (1. + perturbed[i->first]) *
                                std::numeric_limits<double>::epsilon();
         value = computeDifferenceOfHarmonicNumbers
                 (perturbed[i->first], i->second) * computePropensityFunction(perturbed);
      }
      else {
         // Regular case.
         value = computeDifferenceOfHarmonicNumbers
                 (populations[i->first], i->second) * propensityFunction;
      }
      // Record the non-zero values.
      if (value != 0) {
         derivatives->append(i->first, value);
      }
   }
}

// Compute the difference of two harmonic numbers: H_x - H_{x-n}.
template<bool _IsDiscrete>
inline
double
Reaction<_IsDiscrete>::
computeDifferenceOfHarmonicNumbers(double x, std::size_t n) const {
   double result = 0;
   while (n) {
#ifdef DEBUG_stlib
      assert(x != 0);
#endif
      result += 1. / x;
      --x;
      --n;
   }
   return result;
}

//--------------------------------------------------------------------------
// Free functions.
//--------------------------------------------------------------------------

// Return true if the reaction is valid.
template<bool _IsDiscrete>
inline
bool
isValid(const Reaction<_IsDiscrete>& reaction,
        const std::size_t numberOfSpecies) {
   typedef typename Reaction<_IsDiscrete>::SparseVectorSizeType
   SparseVectorSizeType;
   typedef typename SparseVectorSizeType::const_iterator const_iterator;

   // The reactants and products may not both be empty.
   if (reaction.getReactants().empty() && reaction.getProducts().empty()) {
      return false;
   }

   // Check the reactant indices and stoichiometries.
   for (const_iterator i = reaction.getReactants().begin();
         i != reaction.getReactants().end(); ++i) {
      // Species index.
      if (i->first >= numberOfSpecies) {
         return false;
      }
      // Stoichiometry.
      if (i->second <= 0) {
         return false;
      }
   }

   // Check the product indices and stoichiometries.
   for (const_iterator i = reaction.getProducts().begin();
         i != reaction.getProducts().end(); ++i) {
      // Species index.
      if (i->first >= numberOfSpecies) {
         return false;
      }
      // Stoichiometry.
      if (i->second <= 0) {
         return false;
      }
   }

   // Check the rate constant.
   if (reaction.getScaledRateConstant() < 0) {
      return false;
   }

   return true;
}

// Read the reaction in ascii format.
template<bool _IsDiscrete>
inline
void
readAscii(std::istream& in, Reaction<_IsDiscrete>* x) {
   typename Reaction<_IsDiscrete>::SparseVectorSizeType reactants, products;
   double rateConstant;
   in >> reactants >> products >> rateConstant;
   x->rebuild(reactants, products, rateConstant);
}


// Read the reaction in ascii format.
template<bool _IsDiscrete>
inline
void
readReactantsAscii(std::istream& in, Reaction<_IsDiscrete>* x) {
   typename Reaction<_IsDiscrete>::SparseVectorSizeType reactants;
   in >> reactants;
   x->setReactants(reactants);
}


// Read the products in ascii format.
template<bool _IsDiscrete>
inline
void
readProductsAscii(std::istream& in, Reaction<_IsDiscrete>* x) {
   typename Reaction<_IsDiscrete>::SparseVectorSizeType products;
   in >> products;
   x->setProducts(products);
}

} // namespace stochastic
