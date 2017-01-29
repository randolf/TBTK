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
 *  @file DPropertyExtractor.h
 *  @brief Extracts physical properties from the DiagonalizationSolver
 *
 *  @author Kristofer Björnson
 */

#ifndef COM_DAFER45_TBTK_D_PROPERTY_EXTRACTOR
#define COM_DAFER45_TBTK_D_PROPERTY_EXTRACTOR

#include "DiagonalizationSolver.h"
#include "DOS.h"
#include "Density.h"
#include "EigenValues.h"
#include "LDOS.h"
#include "Magnetization.h"
#include "PropertyExtractor.h"
#include "SpinPolarizedLDOS.h"

#include <complex>

namespace TBTK{

/** The DPropertyExtractor extracts common physical properties such as DOS,
 *  Density, LDOS, etc. from a DiagonalizationSolver. These can then be written
 *  to file using the FileWriter.*/
class DPropertyExtractor : public PropertyExtractor{
public:
	/** Constructor. */
	DPropertyExtractor(DiagonalizationSolver *dSolver);

	/** Destructor. */
	~DPropertyExtractor();

	/** Legacy. */
	void saveEigenValues(
		std::string path = "./",
		std::string filename = "EV.dat"
	);

	/** Experimental. Extracts a tabulated version of the AmplitudeSet. */
	void getTabulatedAmplitudeSet(
		std::complex<double> **amplitudes,
		int **indices,
		int *numHoppingAmplitudes,
		int *maxIndexSize
	);

	/** Get eigenvalues. */
	Property::EigenValues* getEigenValues();

	/** Get eigenvalue. */
	double getEigenValue(int state);

	/** Get amplitude for given eigenvector \f$n\f$ and physical index
	 *  \f$x\f$: \f$\Psi_{n}(x)\f$.
	 *  @param state Eigenstate number \f$n\f$
	 *  @param index Physical index \f$x\f$. */
	const std::complex<double> getAmplitude(int state, const Index &index);

	/** Overrides PropertyExtractor::calculateDOS(). */
	virtual Property::DOS* calculateDOS();

	/** Calculate expectation value. */
	virtual std::complex<double> calculateExpectationValue(Index to, Index from);

	/** Overrides PropertyExtractor::calculateDensity(). */
	virtual Property::Density* calculateDensity(Index pattern, Index ranges);

	/** Overrides PropertyExtractor::calculateMagnetization(). */
	virtual Property::Magnetization* calculateMagnetization(
		Index pattern,
		Index ranges
	);

	/** Overrides PropertyExtractor::calculateLDOS(). */
	virtual Property::LDOS* calculateLDOS(
		Index pattern,
		Index ranges
	);

	/** Overrides PropertyExtractor::calculateSpinPolarizedLDOS(). */
	virtual Property::SpinPolarizedLDOS* calculateSpinPolarizedLDOS(
		Index pattern,
		Index ranges
	);
private:
	/** Callback for calculating density. Used by calculateDensity. */
	static void calculateDensityCallback(
		PropertyExtractor *cb_this,
		void *density,
		const Index &index,
		int offset
	);

	/** Callback for calculating magnetization. Used by calculateMAG. */
	static void calculateMAGCallback(
		PropertyExtractor *cb_this,
		void *mag,
		const Index &index,
		int offset
	);

	/** Calback for callculating local density of states. Used by
	 *  calculateLDOS. */
	static void calculateLDOSCallback(
		PropertyExtractor *cb_this,
		void *ldos,
		const Index &index,
		int offset
	);

	/** Callback for calculating spin-polarized local density of states.
	 *  Used by calculateSP_LDOS. */
	static void calculateSP_LDOSCallback(
		PropertyExtractor *cb_this,
		void *sp_ldos,
		const Index &index,
		int offset
	);

	/** DiagonalizationSolver to work on. */
	DiagonalizationSolver *dSolver;
};

inline double DPropertyExtractor::getEigenValue(int state){
	return dSolver->getEigenValue(state);
}

inline const std::complex<double> DPropertyExtractor::getAmplitude(
	int state,
	const Index &index
){
	return dSolver->getAmplitude(state, index);
}

};	//End of namespace TBTK

#endif