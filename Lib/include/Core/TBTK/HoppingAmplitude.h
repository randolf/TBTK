/* Copyright 2016 Kristofer Björnson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @package TBTKcalc
 *  @file HoppingAmplitude.h
 *  @brief Hopping amplitude from state 'from' to state 'to'.
 *
 *  @author Kristofer Björnson
 */

#ifndef COM_DAFER45_TBTK_HOPPING_AMPLITUDE
#define COM_DAFER45_TBTK_HOPPING_AMPLITUDE

#include "TBTK/Index.h"
#include "TBTK/Serializeable.h"

#include <complex>
#include <initializer_list>
#include <tuple>
#include <vector>

namespace TBTK{

enum HermitianConjugate {HC};

/** @brief Hopping amplitude from state 'from' to state 'to'.
 *
 *  A hopping amplitude is a coefficeint \f$a_{ij}\f$ in a bilinear Hamiltonian
 *  \f$H = \sum_{ij}a_{ij}c_{i}^{\dagger}c_{j}\f$, where \f$i\f$ and \f$j\f$
 *  are reffered to using 'to' and 'from' respectively. The constructors can be
 *  called with the parameters either in the order (from, to, value) or the
 *  order (value, to, from). The former follows the order in which the process
 *  can be thought of as happening, while the later corresponds to the order in
 *  which values and operators stands in the Hamiltonian.
 */
class HoppingAmplitude{
public:
	/** Constructor. */
	HoppingAmplitude(
		std::complex<double> amplitude,
		Index toIndex,
		Index fromIndex
	);

	/** Constructor. Takes a callback function rather than a paramater
	 *  value. The callback function has to be defined such that it returns
	 * a value for the given indices when called at run time. */
	HoppingAmplitude(
		std::complex<double> (*amplitudeCallback)(
			const Index &to,
			const Index &from
		),
		Index toIndex,
		Index fromIndex
	);

	/** Copy constructor. */
	HoppingAmplitude(const HoppingAmplitude &ha);

	/** Constructor. Constructs the HoppingAmplitude from a serialization
	 *  string. */
	HoppingAmplitude(
		const std::string &serializeation,
		Serializeable::Mode mode
	);

	/** Get the Hermitian cojugate of the HoppingAmplitude. */
	HoppingAmplitude getHermitianConjugate() const;

	/** Print HoppingAmplitude. Mainly for debugging. */
	void print() const;

	/** Get the amplitude value \f$a_{ij}\f$. */
	std::complex<double> getAmplitude() const;

	/** Addition operator. */
	std::tuple<HoppingAmplitude, HoppingAmplitude> operator+(
		const HermitianConjugate hc
	);

	/** Get to index. */
	const Index& getToIndex() const;

	/** Get from index. */
	const Index& getFromIndex() const;

	/** Get string representation of the HoppingAmplitude. */
	std::string toString() const;

	/** Serialize HoppingAmplitude. Note that HoppingAmplitude is
	 *  pseudo-Serializeable in that it implements the Serializeable
	 * interface, but does so non-virtually. */
	std::string serialize(Serializeable::Mode mode) const;

	/** Get size in bytes. */
	unsigned int getSizeInBytes() const;
private:
	/** Amplitude \f$a_{ij}\f$. Will be used if amplitudeCallback is NULL.
	 */
	std::complex<double> amplitude;

	/** Callback function for runtime evaluation of amplitudes. Will be
	 *  called if not NULL. */
	std::complex<double> (*amplitudeCallback)(
		const Index &toIndex,
		const Index &fromIndex
	);

	/** Index to jump from (annihilate). */
	Index fromIndex;

	/** Index to jump to (create). */
	Index toIndex;

};

inline std::complex<double> HoppingAmplitude::getAmplitude() const{
	if(amplitudeCallback)
		return amplitudeCallback(toIndex, fromIndex);
	else
		return amplitude;
}

inline std::tuple<HoppingAmplitude, HoppingAmplitude> HoppingAmplitude::operator+(
	HermitianConjugate hc
){
	return std::make_tuple(*this, this->getHermitianConjugate());
}

inline const Index& HoppingAmplitude::getToIndex() const{
	return toIndex;
}

inline const Index& HoppingAmplitude::getFromIndex() const{
	return fromIndex;
}

inline std::string HoppingAmplitude::toString() const{
	std::string str;
	str += "("
			+ std::to_string(real(amplitude))
			+ ", " + std::to_string(imag(amplitude))
		+ ")"
		+ ", " + toIndex.toString()
		+ ", " + fromIndex.toString();

	return str;
}

inline unsigned int HoppingAmplitude::getSizeInBytes() const{
	return sizeof(HoppingAmplitude)
		- sizeof(fromIndex)
		- sizeof(toIndex)
		+ fromIndex.getSizeInBytes()
		+ toIndex.getSizeInBytes();
}

};	//End of namespace TBTK

#endif