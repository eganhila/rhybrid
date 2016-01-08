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

#ifndef PARTICLE_BOUNDARY_COND_HYBRID_H
#define PARTICLE_BOUNDARY_COND_HYBRID_H

#include <base_class_particle_boundary_condition.h>

#include <hybrid.h>

template<class SPECIES,class PARTICLE>
class ParticleBoundaryCondHybrid: public ParticleBoundaryCondBase {
 public:
   ParticleBoundaryCondHybrid();
   ~ParticleBoundaryCondHybrid();
   
   bool addConfigFileItems(ConfigReader& cr,const std::string& regionName);
   bool apply(pargrid::DataID particleDataID,unsigned int* N_particles,const std::vector<pargrid::CellID>& exteriorBlocks);
   bool finalize();
   bool initialize(Simulation& sim,SimulationClasses& simClasses,ConfigReader& cr,
		   const std::string& regionName,const ParticleListBase* plist);
   
 private:
   SPECIES species;
};

template<class SPECIES,class PARTICLE> inline
ParticleBoundaryCondBase* HybridBoundaryCondMaker() {return new ParticleBoundaryCondHybrid<SPECIES,PARTICLE>();}

template<class SPECIES,class PARTICLE> inline
ParticleBoundaryCondHybrid<SPECIES,PARTICLE>::ParticleBoundaryCondHybrid() { }

template<class SPECIES,class PARTICLE> inline
ParticleBoundaryCondHybrid<SPECIES,PARTICLE>::~ParticleBoundaryCondHybrid() { }

template<class SPECIES,class PARTICLE> inline
bool ParticleBoundaryCondHybrid<SPECIES,PARTICLE>::addConfigFileItems(ConfigReader& cr,const std::string& regionName) {
   return true;
}

template<class SPECIES,class PARTICLE> inline
bool ParticleBoundaryCondHybrid<SPECIES,PARTICLE>::apply(pargrid::DataID particleDataID,unsigned int* N_particles,
							 const std::vector<pargrid::CellID>& exteriorBlocks) {
   pargrid::DataWrapper<PARTICLE> wrapper = simClasses->pargrid.getUserDataDynamic<PARTICLE>(particleDataID);
   
   // Remove particles on exterior cells:
   Real t_propag = 0.0;
   for (pargrid::CellID b=0; b<exteriorBlocks.size(); ++b) {
      // Measure computation time if we are testing for repartitioning:
      if (sim->countPropagTime == true) t_propag = MPI_Wtime();
      
      const pargrid::CellID blockLID = exteriorBlocks[b];
      PARTICLE* particles = wrapper.data()[blockLID];
      for(size_t p=0; p<N_particles[blockLID]; ++p) {
	 // escape counter
	 Hybrid::particleCounterEscape[species.popid-1] += particles[p].state[particle::WEIGHT];
      }
      N_particles[blockLID] = 0;
      wrapper.resize(blockLID,0);
      
      // Store block injection time:
      if (sim->countPropagTime == true) {
	 t_propag = std::max(0.0,MPI_Wtime() - t_propag);
	 simClasses->pargrid.getCellWeights()[blockLID] += t_propag;
      }
   }

   // Inner boundary
   bool* innerFlagParticle = simClasses->pargrid.getUserDataStatic<bool>(Hybrid::dataInnerFlagParticleID);
   const double* crd = getBlockCoordinateArray(*sim,*simClasses);
   for (pargrid::CellID b=0; b<simClasses->pargrid.getNumberOfLocalCells(); ++b) {
      if (innerFlagParticle[b] == false) { continue; }
      
      // Measure computation time if we are testing for repartitioning:
      if (sim->countPropagTime == true) t_propag = MPI_Wtime();
      
      PARTICLE* particles = wrapper.data()[b];
      const size_t b3 = 3*b;
      const Real xBlock = crd[b3+0];
      const Real yBlock = crd[b3+1];
      const Real zBlock = crd[b3+2];
      int current = 0;
      int end = N_particles[b]-1;
      while (current <= end) {
	 const Real r2 =  sqr(xBlock + particles[current].state[particle::X]) +
	   sqr(yBlock + particles[current].state[particle::Y]) +
	   sqr(zBlock + particles[current].state[particle::Z]);
	 //if (r2 < Hybrid::R2_particleObstacle) {
         if (r2 < this->species.R2_obstacle) {
	    // impact counter
	    Hybrid::particleCounterImpact[this->species.popid-1] += particles[current].state[particle::WEIGHT];
	    particles[current] = particles[end];
	    --end;
	    continue;
	 }
	 ++current;
      }
      wrapper.resize(b,current);
      N_particles[b] = current;
      
      // Store block injection time:
      if (sim->countPropagTime == true) {
	 t_propag = std::max(0.0,MPI_Wtime() - t_propag);
	 simClasses->pargrid.getCellWeights()[b] += t_propag;
      }
   }

   return true;
}

template<class SPECIES,class PARTICLE> inline
bool ParticleBoundaryCondHybrid<SPECIES,PARTICLE>::finalize() {
   return true;
}

template<class SPECIES,class PARTICLE> inline
bool ParticleBoundaryCondHybrid<SPECIES,PARTICLE>::initialize(Simulation& sim,SimulationClasses& simClasses,ConfigReader& cr,
							      const std::string& regionName,const ParticleListBase* plist) {
   bool success = ParticleBoundaryCondBase::initialize(sim,simClasses,cr,regionName,plist);
   species = *reinterpret_cast<const SPECIES*>(plist->getSpecies());
   return success;
}

#endif
