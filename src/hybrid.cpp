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

#include "hybrid.h"

using namespace std;

// Init static variables:

// face data
pargrid::DataID Hybrid::dataFaceBID;
pargrid::DataID Hybrid::dataFaceJID;

// cell data
pargrid::DataID Hybrid::dataCellRhoQiID;
pargrid::DataID Hybrid::dataCellBID;
pargrid::DataID Hybrid::dataCellJID;
pargrid::DataID Hybrid::dataCellUeID;
pargrid::DataID Hybrid::dataCellJiID;
pargrid::DataID Hybrid::dataCellMaxUeID;
pargrid::DataID Hybrid::dataCellMaxViID;
pargrid::DataID Hybrid::dataCellMinRhoQiID;
pargrid::DataID Hybrid::dataCellIonosphereID;
pargrid::DataID Hybrid::dataCellExosphereID;

// node data
pargrid::DataID Hybrid::dataNodeRhoQiID;
pargrid::DataID Hybrid::dataNodeEID;
pargrid::DataID Hybrid::dataNodeBID;
pargrid::DataID Hybrid::dataNodeJID;
pargrid::DataID Hybrid::dataNodeUeID;
pargrid::DataID Hybrid::dataNodeJiID;

// stencils
pargrid::StencilID Hybrid::accumulationStencilID;

// flags
pargrid::DataID Hybrid::dataInnerFlagFieldID;
pargrid::DataID Hybrid::dataInnerFlagNodeID;
pargrid::DataID Hybrid::dataInnerFlagParticleID;
#ifdef USE_XMIN_BOUNDARY
pargrid::DataID Hybrid::dataXminFlagID;
#endif
#ifdef ION_SPECTRA_ALONG_ORBIT
pargrid::DataID Hybrid::dataSpectraFlagID;
pargrid::DataID Hybrid::dataSpectraID;
#endif

// bit masks to check the existence of +x, -x, +y, -y, +z, -z neighbour cell
uint32_t Hybrid::X_POS_EXISTS;   
uint32_t Hybrid::X_NEG_EXISTS;   
uint32_t Hybrid::Y_POS_EXISTS;
uint32_t Hybrid::Y_NEG_EXISTS;
uint32_t Hybrid::Z_POS_EXISTS;
uint32_t Hybrid::Z_NEG_EXISTS;

int Hybrid::logInterval;
Real Hybrid::dx;
Real Hybrid::dV;
Real Hybrid::R_object;
Real Hybrid::R2_fieldObstacle;
Real Hybrid::R2_particleObstacle;
#ifdef USE_XMIN_BOUNDARY
Real Hybrid::xMinBoundary;
#endif
Real Hybrid::M_object;
Real Hybrid::maxUe2;
Real Hybrid::maxVi2;
Real Hybrid::minRhoQi;
Real Hybrid::eta;
bool Hybrid::useHallElectricField;
Real Hybrid::swMacroParticlesCellPerDt;
int Hybrid::Efilter;

Real Hybrid::IMFBx;
Real Hybrid::IMFBy;
Real Hybrid::IMFBz;
#if defined(USE_B_INITIAL) || defined(USE_B_CONSTANT)
Real Hybrid::laminarR2;
Real Hybrid::laminarR3;
Real Hybrid::coeffDip;
Real Hybrid::coeffQuad;
Real Hybrid::dipSurfB;
Real Hybrid::dipSurfR;
Real Hybrid::dipMinR2;
Real Hybrid::dipMomCoeff;
Real Hybrid::xDip;
Real Hybrid::yDip;
Real Hybrid::zDip;
Real Hybrid::thetaDip;
Real Hybrid::phiDip;
void (*Hybrid::magneticFieldProfilePtr)(const Real x,const Real y,const Real z,Real B[3]);
#endif
// total number of particle populations
unsigned int Hybrid::N_populations;
// number of ionospheric particle populations
unsigned int Hybrid::N_ionospherePopulations;
// number of exospheric particle populations
unsigned int Hybrid::N_exospherePopulations;
// names of particle populations
vector<string> Hybrid::populationNames;
// number of output particle variables
unsigned int Hybrid::N_outputPopVars;
// names of output particle variables
vector<string> Hybrid::outputPopVarStr;
// id of output particle variables for each population (<0 means no output)
vector<int> Hybrid::outputPopVarId;
// popids of particle populations included in each output particle variables
vector< vector<unsigned int> > Hybrid::outputPopVarIdVector;
// popids of particle populations included in the total plasma variables
vector<unsigned int> Hybrid::outputPlasmaPopId;
// output cell variables
map<string,bool> Hybrid::outputCellParams;

vector<ofstream*> Hybrid::plog;
ofstream Hybrid::flog;

vector<Real> Hybrid::particleCounterEscape;
vector<Real> Hybrid::particleCounterImpact;
vector<Real> Hybrid::particleCounterInject;
vector<Real> Hybrid::particleCounterInjectMacroparticles;
Real Hybrid::particleCounterTimeStart;

#ifdef WRITE_POPULATION_AVERAGES
pargrid::DataID Hybrid::dataCellAverageBID;
vector<pargrid::DataID> Hybrid::dataCellAverageDensityID;
vector<pargrid::DataID> Hybrid::dataCellAverageVelocityID;
int Hybrid::averageCounter;
#endif


