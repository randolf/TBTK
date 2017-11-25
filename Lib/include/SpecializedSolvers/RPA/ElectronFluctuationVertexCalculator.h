/* Copyright 2017 Kristofer Björnson
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
 *  @file SelfEnergyCalculator.h
 *  @brief Calculates the self-energy using the RPA approximation.
 *
 *  @author Kristofer Björnson
 */

#ifndef COM_DAFER45_TBTK_ELECTRON_FLUCTUATION_VERTEX_CALCULATOR
#define COM_DAFER45_TBTK_ELECTRON_FLUCTUATION_VERTEX_CALCULATOR

#include "BrillouinZone.h"
#include "BlockDiagonalizationSolver.h"
#include "BPropertyExtractor.h"
#include "IndexedDataTree.h"
#include "SusceptibilityCalculator.h"

namespace TBTK{

class ElectronFluctuationVertexCalculator{
public:
	/** Constructor. */
	ElectronFluctuationVertexCalculator(
		const MomentumSpaceContext &momentumSpaceContext
	);

	/** Destructor. */
	~ElectronFluctuationVertexCalculator();

	/** Create slave ElectronFluctuationVertexCalculator. The slave reuses
	 *  internal lookup tables used to speed up the calculations and should
	 *  not be used after the generating master have been destructed. */
	ElectronFluctuationVertexCalculator* createSlave();

	/** Get momentum cpsace context. */
	const MomentumSpaceContext& getMomentumSpaceContext() const;

	/** Initialize the SelfEnergyCalculator. */
//	void init();

	/** Enum class for indicating whether the energy is an arbitrary comlex
	 *  number, or if it is restricted to the real or imaginary axis. */
	enum class EnergyType {Real, Imaginary, Complex};

	/** Set self-energy energy type. */
	void setEnergyType(EnergyType energyType);

	/** Get energy type. */
	EnergyType getEnergyType() const;

	/** Set the energies for which the self-energy should be
	 *  calculated. */
	void setEnergies(
		const std::vector<std::complex<double>> &energies
	);

	/** Set to true if the energies can be assumed to be inversion
	 *  symmetric in the complex plane.
	 *
	 *  Important note:
	 *  Only set this to true if the energies passed to setEnergies() are
	 *  on the form (-E_n, -E_{n-1}, ..., E_{n-1}, E_{n}. Setting this flag
	 *  to true without fullfilling this condition will result in undefined
	 *  behavior. */
	void setEnergiesAreInversionSymmetric(
		bool energiesAreInversionSymmetric
	);

	/** Get wheter the energies are inversion symmetric. */
	bool getEnergiesAreInversionSymmetric() const;

	/** Calculate self-energy vertex. */
	std::vector<std::complex<double>> calculateSelfEnergyVertex(
		const std::vector<double> &k,
		const std::vector<int> &orbitalIndices
	);

	/** Set U. */
	void setU(std::complex<double> U);

	/** Set Up. */
	void setUp(std::complex<double> Up);

	/** Set J. */
	void setJ(std::complex<double> J);

	/** Set Jp. */
	void setJp(std::complex<double> Jp);

	/** Generate interaction amplitudes. Can be called multiple times and
	 *  will only regenerate the interaction amplitudes when needed. */
	void generateInteractionAmplitudes();

	/** Save susceptibilities. */
	void saveSusceptibilities(const std::string &filename) const;

	/** Load susceptibilities. */
	void loadSusceptibilities(const std::string &filename);
private:
	/** SusceptibilityCalculator. */
	SusceptibilityCalculator *susceptibilityCalculator;

	/** Lookup table for calculating k+q. */
//	int *kMinusQLookupTable;

	/** Generate lookup table for the k-q linear index. Can be called
	 *  repeatedly, and the lookup table is only generated once. */
//	void generateKMinusQLookupTable();

	/** Returns the linear index for k+q. */
/*	template<bool useKPlusKLookupTable>
	int getKMinusQLinearIndex(
		unsigned int meshIndex,
		const std::vector<double> &k,
		int kLinearIndex
	) const;*/

	/** Number of energies to sum over. */
//	unsigned int numSummationEnergies;

	/** Energies to sum over. */
//	std::vector<std::complex<double>> summationEnergies;

	/** Energy type. */
	EnergyType energyType;

	/** Energies to calculate the vertex for. */
	std::vector<std::complex<double>> energies;

	/** Flag indicating whether the SelfEnergyCalculator is initialized. */
	bool isInitialized;

	/** IndexedDataTree storing the vertex. */
	IndexedDataTree<SerializeableVector<std::complex<double>>> vertexTree;

	/** Invert matix. */
/*	void invertMatrix(
		std::complex<double> *matrix,
		unsigned int dimensions
	);*/

	/** Multiply matrices. */
/*	void multiplyMatrices(
		std::complex<double> *matrix1,
		std::complex<double> *matrix2,
		std::complex<double> *result,
		unsigned int dimensions
	);*/

	/** Print matrix. */
/*	void printMatrix(
		std::complex<double> *matrix,
		unsigned int dimensions
	);*/

	/** Self-energy main loop. */
/*	template<bool singleSelfEnergyEnergy>
	void selfEnergyMainLoop(
		const std::vector<double> &k,
		const std::vector<int> &orbitalIndices,
		std::vector<std::complex<double>> &result
	);*/

	/** Interaction parameters. */
	std::complex<double> U, Up, J, Jp;

	/** Interaction amplitudes used to calculate the self-energy vertex. */
	std::vector<InteractionAmplitude> u1;
	std::vector<InteractionAmplitude> u2;
	std::vector<InteractionAmplitude> u3;

	/** Flag indicating whether the interaction amplitudes are initialized.
	 */
	bool interactionAmplitudesAreGenerated;
};

inline const MomentumSpaceContext& ElectronFluctuationVertexCalculator::getMomentumSpaceContext(
) const{
	return susceptibilityCalculator->getMomentumSpaceContext();
}

/*inline void SelfEnergyCalculator::setNumSummationEnergies(
	unsigned int numSummationEnergies
){
	this->numSummationEnergies = numSummationEnergies;
}*/

inline void ElectronFluctuationVertexCalculator::setEnergyType(
	EnergyType energyType
){
	this->energyType = energyType;
	switch(energyType){
	case EnergyType::Real:
		susceptibilityCalculator->setSusceptibilityEnergyType(
			SusceptibilityCalculator::EnergyType::Real
		);
		break;
	case EnergyType::Imaginary:
		susceptibilityCalculator->setSusceptibilityEnergyType(
			SusceptibilityCalculator::EnergyType::Imaginary
		);
		break;
	case EnergyType::Complex:
		susceptibilityCalculator->setSusceptibilityEnergyType(
			SusceptibilityCalculator::EnergyType::Complex
		);
		break;
	default:
		TBTKExit(
			"ElectronFluctuationVertexCalculator::setEnergyType()",
			"Unknown energy type.",
			"This should never happen, contact the developer."
		);
	}
}

inline ElectronFluctuationVertexCalculator::EnergyType ElectronFluctuationVertexCalculator::getEnergyType(
) const{
	return energyType;
}

inline void ElectronFluctuationVertexCalculator::setEnergies(
	const std::vector<std::complex<double>> &energies
){
	this->energies = energies;
	vertexTree.clear();

	susceptibilityCalculator->setSusceptibilityEnergies(energies);
}

inline void ElectronFluctuationVertexCalculator::setEnergiesAreInversionSymmetric(
	bool energiesAreInversionSymmetric
){
	susceptibilityCalculator->setSusceptibilityEnergiesAreInversionSymmetric(
		energiesAreInversionSymmetric
	);
}

inline bool ElectronFluctuationVertexCalculator::getEnergiesAreInversionSymmetric(
) const{
	return susceptibilityCalculator->getSusceptibilityEnergiesAreInversionSymmetric();
}

inline void ElectronFluctuationVertexCalculator::setU(std::complex<double> U){
	this->U = U;
	susceptibilityCalculator->setU(U);
	vertexTree.clear();
	interactionAmplitudesAreGenerated = false;
}

inline void ElectronFluctuationVertexCalculator::setUp(std::complex<double> Up){
	this->Up = Up;
	susceptibilityCalculator->setUp(Up);
	vertexTree.clear();
	interactionAmplitudesAreGenerated = false;
}

inline void ElectronFluctuationVertexCalculator::setJ(std::complex<double> J){
	this->J = J;
	susceptibilityCalculator->setJ(J);
	vertexTree.clear();
	interactionAmplitudesAreGenerated = false;
}

inline void ElectronFluctuationVertexCalculator::setJp(std::complex<double> Jp){
	this->Jp = Jp;
	susceptibilityCalculator->setJp(Jp);
	vertexTree.clear();
	interactionAmplitudesAreGenerated = false;
}

/*inline void SelfEnergyCalculator::precomputeSusceptibilities(
	unsigned int numWorkers
){
	susceptibilityCalculator.precompute(numWorkers);
}*/

inline void ElectronFluctuationVertexCalculator::saveSusceptibilities(
	const std::string &filename
) const{
	susceptibilityCalculator->saveSusceptibilities(filename);
}

inline void ElectronFluctuationVertexCalculator::loadSusceptibilities(
	const std::string &filename
){
	susceptibilityCalculator->loadSusceptibilities(filename);
}

};	//End of namespace TBTK

#endif