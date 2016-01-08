/** This file is part of the RHybrid simulation.
 *
 *  Copyright 2015- Finnish Meteorological Institute
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HYBRID_PROPAGATOR_H
#define HYBRID_PROPAGATOR_H

#include <particle_list_base.h>
#include <simulation.h>
#include <simulationclasses.h>

bool propagateB(Simulation& sim,SimulationClasses& simClasses,std::vector<ParticleListBase*>& particleLists);
void neumannCell(Real* cellData,Simulation& sim,SimulationClasses& simClasses,const std::vector<pargrid::CellID>& exteriorBlocks,const int vectorDim);
void neumannFace(Real* faceData,Simulation& sim,SimulationClasses& simClasses,const std::vector<pargrid::CellID>& exteriorBlocks);
void setIMF(Real* cellB,Simulation& sim,SimulationClasses& simClasses,const std::vector<pargrid::CellID>& exteriorBlocks);
void face2Cell(Real* faceData,Real* cellData,Simulation& sim,SimulationClasses& simClasses,pargrid::CellID blockID);
void cell2Node(Real* celldata,Real* nodeData,Simulation& sim,SimulationClasses& simClasses,pargrid::CellID blockID,const int vectorDim = 3);
void node2Cell(Real* nodeData,Real* cellData,Simulation& sim,SimulationClasses& simClasses,pargrid::CellID blockID);
void upwindNodeB(Real* cellB,Real* nodeUe,Real* nodeB,Simulation& sim,SimulationClasses& simClasses,pargrid::CellID blockID);
void calcCellUe(Real* cellJ,Real* cellJi,Real* cellRhoQi,Real* cellUe,bool* innerFlag,Real* cellMaxUe,Simulation& sim,SimulationClasses& simClasses,pargrid::CellID blockID);
void calcNodeE(Real* nodeUe,Real* nodeB,Real* nodeJ,Real* nodeE,bool* innerFlag,Simulation& sim,SimulationClasses& simClasses,pargrid::CellID blockID);
void calcNodeJ(Real* faceB,Real* nodeJ,Simulation& sim,SimulationClasses& simClasses,pargrid::CellID blockID);
void calcNodeUe(Real* nodeRhoQi,Real* nodeJi,Real* nodeJ,Real* nodeUe,bool* innerFlag,Real* cellMaxUe,Simulation& sim,SimulationClasses& simClasses,pargrid::CellID blockID);
void faceCurl(Real* nodeData,Real* faceData,bool doFaraday,Simulation& sim,SimulationClasses& simClasses,pargrid::CellID blockID);
void face2r(Real* r,Real* faceData,Simulation& sim,SimulationClasses& simClasses,pargrid::CellID blockID,Real* result);
void node2r(Real* r,Real* nodeData,Simulation& sim,SimulationClasses& simClasses,pargrid::CellID blockID,Real* result);
void cell2r(Real* r,Real* cellData,Simulation& sim,SimulationClasses& simClasses,pargrid::CellID blockID,Real* result);
void setupGetFields(Simulation& sim,SimulationClasses& simClasses);
void getFields(Real* r,Real* B,Real* Ue,Simulation& sim,SimulationClasses& simClasses,pargrid::CellID blockID);
void fetchData(Real* data,Real* array,SimulationClasses& simClasses,pargrid::CellID blockID,int vectorDim);

#endif
