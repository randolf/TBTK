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
 *  @file Diagonalizater.h
 *  @brief Solves a block diagonal Model using diagonalization.
 *
 *  @author Kristofer Björnson
 */

#ifndef COM_DAFER45_TBTK_SOLVER_BLOCK_DIAGONALIZER
#define COM_DAFER45_TBTK_SOLVER_BLOCK_DIAGONALIZER

#include "TBTK/Communicator.h"
#include "TBTK/Model.h"
#include "TBTK/Solver/Solver.h"
#include "TBTK/Timer.h"

#include <complex>

namespace TBTK{
namespace Solver{

/** @brief Solves a block diagonal Model using diagonalization.
 *
 *  Solves a given model by Diagonalizing the Hamiltonian. The eigenvalues and
 *  eigenvectors can then either be directly extracted and used to calculate
 *  custom physical quantities, or the PropertyExtractor can be used to extract
 *  common properties. Scales as \f$O(n^3)\f$ with the dimension of the Hilbert
 *  space. */
class BlockDiagonalizer : public Solver, public Communicator{
public:
	/** Constructor */
	BlockDiagonalizer();

	/** Destructor. */
	virtual ~BlockDiagonalizer();

	/** Set self-consistency callback. If set to NULL or never called, the
	 *  self-consistency loop will not be run. */
	void setSelfConsistencyCallback(
		bool (*selfConsistencyCallback)(
			BlockDiagonalizer &blockDiagonalizer
		)
	);

	/** Set maximum number of iterations for the self-consistency loop. */
	void setMaxIterations(int maxIterations);

	/** Run calculations. Diagonalizes ones if no self-consistency callback
	 *  have been set, or otherwise multiple times until slef-consistencey
	 *  or maximum number of iterations has been reached. */
	void run();

	/** Get eigenvalue. */
	const double getEigenValue(int state);

	/** Get eigenvalue for specific block. Note that in contrast to
	 *  getEigenValue(int state), 'state' here is relative to the first
	 *  state of the block. */
	const double getEigenValue(const Index &blockIndex, int state);

	/** Get amplitude for given eigenvector \f$n\f$ and physical index
	 * \f$x\f$: \f$\Psi_{n}(x)\f$.
	 *  @param state Eigenstate number \f$n\f$.
	 *  @param index Physical index \f$x\f$.
	 */
	const std::complex<double> getAmplitude(int state, const Index &index);

	/** Same as getAmplitude(int state, const Index &index), but the
	 *  amplitude is accessed by first identifying the block using the
	 *  blockIndex, then using 'state', which here is relative to the first
	 *  state in the block, and finally accessing the amplitude
	 *  {blockIndex, intraBlockIndex}. */
	const std::complex<double> getAmplitude(
		const Index &blockIndex,
		int state,
		const Index &intraBlockIndex
	);

	/** Get first state in the block corresponding to the given index. */
	unsigned int getFirstStateInBlock(const Index &index) const;

	/** Get last state in the block corresponding to the given index. */
	unsigned int getLastStateInBlock(const Index &index) const;

	/** Set whether parallel execution is enabled or not. */
	void setParallelExecution(bool parallelExecution);
private:
	/** pointer to array containing Hamiltonian. */
	std::complex<double> *hamiltonian;

	/** Pointer to array containing eigenvalues.*/
	double *eigenValues;

	/** Pointer to array containing eigenvectors. */
	std::complex<double> *eigenVectors;

	/** Number of states per block. */
	std::vector<unsigned int> numStatesPerBlock;

	/** Block indices for give state. */
	std::vector<unsigned int> stateToBlockMap;

	/** The first state index in given block. */
	std::vector<unsigned int> blockToStateMap;

	/** Block sizes. */
	std::vector<unsigned int> blockSizes;

	/** Block offsets. */
	std::vector<unsigned int> blockOffsets;

	/** Eigen vector sizes. */
	std::vector<unsigned int> eigenVectorSizes;

	/** Eigen vector offsets. */
	std::vector<unsigned int> eigenVectorOffsets;

	/** Number of blocks in the Hamiltonian. */
	int numBlocks;

	/** Maximum number of iterations in the self-consistency loop. */
	int maxIterations;

	/** Flag indicating wether to enable parallel execution. */
	bool parallelExecution;

	/** Callback function to call each time a diagonalization has been
	 *  completed. */
	bool (*selfConsistencyCallback)(BlockDiagonalizer &blockDiagonalizer);

	/** Allocates space for Hamiltonian etc. */
	void init();

	/** Updates Hamiltonian. */
	void update();

	/** Diagonalizes the Hamiltonian. */
	void solve();
};

inline void BlockDiagonalizer::setSelfConsistencyCallback(
	bool (*selfConsistencyCallback)(
		BlockDiagonalizer &blockDiagonalizer
	)
){
	this->selfConsistencyCallback = selfConsistencyCallback;
}

inline void BlockDiagonalizer::setMaxIterations(int maxIterations){
	this->maxIterations = maxIterations;
}

inline const std::complex<double> BlockDiagonalizer::getAmplitude(
	int state,
	const Index &index
){
	const Model &model = getModel();
	unsigned int block = stateToBlockMap.at(state);
	unsigned int offset = eigenVectorOffsets.at(block);
	unsigned int linearIndex = model.getBasisIndex(index);
	unsigned int firstStateInBlock = blockToStateMap.at(block);
	unsigned int lastStateInBlock = firstStateInBlock + numStatesPerBlock.at(block)-1;
	offset += (state - firstStateInBlock)*numStatesPerBlock.at(block);
	if(linearIndex >= firstStateInBlock && linearIndex <= lastStateInBlock)
		return eigenVectors[offset + (linearIndex - firstStateInBlock)];
	else
		return 0;
}

inline const std::complex<double> BlockDiagonalizer::getAmplitude(
	const Index &blockIndex,
	int state,
	const Index &intraBlockIndex
){
	int firstStateInBlock = getModel().getHoppingAmplitudeSet()->getFirstIndexInBlock(
		blockIndex
	);
	unsigned int block = stateToBlockMap.at(firstStateInBlock);
	TBTKAssert(
		state >= 0 && state < (int)numStatesPerBlock.at(block),
		"BlockDiagonalizer::getAmplitude()",
		"Out of bound error. The block with block Index "
		<< blockIndex.toString() << " has "
		<< numStatesPerBlock.at(block) << " states, but state "
		<< state << " was requested.",
		""
	);
	unsigned int offset = eigenVectorOffsets.at(block) + state*numStatesPerBlock.at(block);
	unsigned int linearIndex = getModel().getBasisIndex(
		Index(blockIndex, intraBlockIndex)
	);
//	Streams::out << linearIndex << "\t" << Index(blockIndex, intraBlockIndex).toString() << "\n";

	return eigenVectors[offset + (linearIndex - firstStateInBlock)];
}

inline const double BlockDiagonalizer::getEigenValue(int state){
	return eigenValues[state];
}

inline const double BlockDiagonalizer::getEigenValue(
	const Index &blockIndex,
	int state
){
	int offset = getModel().getHoppingAmplitudeSet()->getFirstIndexInBlock(
		blockIndex
	);

	return eigenValues[offset + state];
}

inline unsigned int BlockDiagonalizer::getFirstStateInBlock(
	const Index &index
) const{
	unsigned int linearIndex = getModel().getBasisIndex(index);
	unsigned int block = stateToBlockMap.at(linearIndex);

	return blockToStateMap.at(block);
}

inline unsigned int BlockDiagonalizer::getLastStateInBlock(
	const Index &index
) const{
	unsigned int linearIndex = getModel().getBasisIndex(index);
	unsigned int block = stateToBlockMap.at(linearIndex);

	return getFirstStateInBlock(index) + numStatesPerBlock.at(block)-1;
}

inline void BlockDiagonalizer::setParallelExecution(
	bool parallelExecution
){
	this->parallelExecution = parallelExecution;
}

};	//End of namespace Solver
};	//End of namespace TBTK

#endif
