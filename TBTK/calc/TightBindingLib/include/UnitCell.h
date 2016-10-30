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
 *  @file UnitCell.h
 *  @brief Unit cell that act as container of States.
 *
 *  @author Kristofer Björnson
 */

#ifndef COM_DAFER45_TBTK_UNIT_CELL
#define COM_DAFER45_TBTK_UNIT_CELL

#include "AbstractState.h"

namespace TBTK{

class UnitCell{
public:
	/** Constructor. */
	UnitCell();

	/** Destructor. */
	~UnitCell();

	/** Add state. Note: The UnitCell assumes ownership of the pointers and
	 *  will delete them when it is destroyed. */
	void addState(AbstractState *state);

	/** Get states. */
	const std::vector<AbstractState*> getStates() const;
private:
	/** Pointers to states. */
	std::vector<AbstractState*> states;
};

inline void UnitCell::addState(AbstractState *state){
	states.push_back(state);
}

inline const std::vector<AbstractState*> UnitCell::getStates() const{
	return states;
}

};	//End of namespace TBTK

#endif
