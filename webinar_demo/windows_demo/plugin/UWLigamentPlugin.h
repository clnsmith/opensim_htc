#ifndef _OPENSIM_UWLIGAMENT_PLUGIN_H_
#define _OPENSIM_UWLIGAMENT_PLUGIN_H_
/* ---------------------------------------------------------------------------- *
 *                             UWLigament.h                                     *
 * ---------------------------------------------------------------------------- *
 * This ligament model was based on the ligament model found in 			    *
 * Blankevoort L and Huiskes R (1991). Ligament-Bone Interaction in a           * 
 * Three-Dimensional Model of the Knee. Journal of Biomechanical                *
 * Engineering. 113:263-269.                                                    *
 * 																				*
 * Important variables in the model: 											*
 * linear_stiffness: Stiffness representing the slope of the linear portion of  *
 * 				     the force-strain curve. 								    *
 * 																				*
 * ligament_transition_strain: Strain at which the force-strain relationship of	* 
 * 							   the ligament transitions from quadratic to 		*
 *							   linear. Typically defined to be 0.06 in the 		*
 *						 	   literature. 										*
 *																				*
 * reference_strain: Strain in the ligament when the joint is in the reference  *
 * 				     position. The reference position is full extension for the *
 * 				     knee. 														*
 * 																				*
 * reference_length: Length of the ligament when the joint is in the reference 	* 
 * 				     position. 													*
 * 																				*
 * normalized_damping_coefficient: Coefficient for normalized damping of the 	* 
 * 								 ligament. Be aware, this is not the same as a 	* 
 * 								 standard damping coefficient.  				*
 * 								 See UWLigament::computeForce in UWLigament.cpp *
 * 								 for the definition of the damping force. 		*
 *  																			*
 *                                                                              *
 * Author(s): Michael Vignos, Colin Smith, Rachel Lenhart, Darryl Thelen        *
 *                                                                              *
 *
 *																			    *	
 * Licensed under the Apache License, Version 2.0 (the "License"); you may      *
 * not use this file except in compliance with the License. You may obtain a    *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.           *
 *                                                                              *
 * Unless required by applicable law or agreed to in writing, software          *
 * distributed under the License is distributed on an "AS IS" BASIS,            *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.     *
 * See the License for the specific language governing permissions and          *
 * limitations under the License.                                               *
 *                                                                              *
 * ---------------------------------------------------------------------------- */

//=============================================================================
// INCLUDES
//=============================================================================
// Headers define the various property types that OpenSim objects can read 
#include <string>
#include "osimPluginDLL.h"
#include <OpenSim/Simulation/Model/Force.h>
#include <OpenSim/Simulation/Model/GeometryPath.h>
#include <OpenSim/Common/ScaleSet.h>
#include <OpenSim/Simulation/Model/Model.h>
#include <OpenSim/Actuators/osimActuatorsDLL.h>

//=============================================================================
//=============================================================================
/*
 * A class template for writing a custom Force plugin. 
 * Applies a body drag force to a given OpenSim Body at
 * it's center of mass location.
 *
 */
namespace OpenSim {


//=============================================================================
//=============================================================================
/**
 * A class implementing a ligament. The path of the ligament is
 * stored in a GeometryPath object.
 */
class OSIMPLUGIN_API UWLigament : public Force  {
OpenSim_DECLARE_CONCRETE_OBJECT(UWLigament, Force);
public:
//=============================================================================
// PROPERTIES
//=============================================================================
    /** @name Property declarations
    These are the serializable properties associated with this class. **/
    /**@{**/    OpenSim_DECLARE_UNNAMED_PROPERTY(GeometryPath, 
		"the set of points defining the path of the ligament");
	OpenSim_DECLARE_PROPERTY(linear_stiffness, double,
		"slope of the linear portion of the force-strain curve of ligament");
	OpenSim_DECLARE_PROPERTY(ligament_transition_strain, double,
		"strain at which ligament force-strain curve transitions from quadratic to linear. Commonly 0.06 in literature.");
	OpenSim_DECLARE_PROPERTY(reference_strain, double,
		"strain of ligament when joint is at full extension");
	OpenSim_DECLARE_PROPERTY(reference_length, double,
		"length of ligament when joint is at full extension");
	OpenSim_DECLARE_PROPERTY(normalized_damping_coefficient, double,
		"coefficient for normalized damping of ligament");



//=============================================================================
// METHODS
//=============================================================================
public:
	// Default Constructor
	UWLigament();
	virtual ~UWLigament();
	//--------------------------------------------------------------------------
	// GET
	//--------------------------------------------------------------------------
	// Properties
	const GeometryPath& getGeometryPath() const 
    {   return get_GeometryPath(); }
	GeometryPath& updGeometryPath() 
    {   return upd_GeometryPath(); }
	virtual bool hasGeometryPath() const { return true;};
	virtual double getLength(const SimTK::State& s) const;
	virtual double getLengtheningSpeed(const SimTK::State& s) const;
	virtual double getLinearStiffness() const 
    {   return get_linear_stiffness(); }
	virtual bool setLinearStiffness(double aLinearStiffness);
	virtual double getReferenceStrain() const 
    {   return get_reference_strain(); }
	virtual bool setReferenceStrain(double aReferenceStrain);
	virtual double getLigamentTransitionStrain() const 
    {   return get_ligament_transition_strain(); }
	virtual bool setLigamentTransitionStrain(double aLigamentTransitionStrain);
	virtual double getReferenceLength() const 
    {   return get_reference_length(); }
	virtual bool setReferenceLength(double aReferenceLength);
	virtual double getNormalizedDampingCoefficient() const 
    {   return get_normalized_damping_coefficient(); }
	virtual bool setNormalizedDampingCoefficient(double aNormalizedDampingCoefficient);

	// computed variables
	const double& getTension(const SimTK::State& s) const;


	//--------------------------------------------------------------------------
	// COMPUTATIONS
	//--------------------------------------------------------------------------
	virtual double computeMomentArm(const SimTK::State& s, Coordinate& aCoord) const;
	virtual void computeForce(const SimTK::State& s, 
							  SimTK::Vector_<SimTK::SpatialVec>& bodyForces, 
							  SimTK::Vector& generalizedForces) const;

	//--------------------------------------------------------------------------
	// SCALE
	//--------------------------------------------------------------------------
	virtual void preScale(const SimTK::State& s, const ScaleSet& aScaleSet);
	virtual void scale(const SimTK::State& s, const ScaleSet& aScaleSet);
	virtual void postScale(const SimTK::State& s, const ScaleSet& aScaleSet);


	//--------------------------------------------------------------------------
	// DISPLAY
	//--------------------------------------------------------------------------
	virtual const VisibleObject* getDisplayer() const;
	virtual void updateDisplayer(const SimTK::State& s) const;

	//-----------------------------------------------------------------------------
	// REPORTING
	//-----------------------------------------------------------------------------
	/** 
	 * Provide name(s) of the quantities (column labels) of the force value(s) to be reported
	 */
	virtual OpenSim::Array<std::string> getRecordLabels() const ;
	/**
	*  Provide the value(s) to be reported that correspond to the labels
	*/
	virtual OpenSim::Array<double> getRecordValues(const SimTK::State& state) const ;


protected:
	/** Override this method if you would like to calculate a color for use
    when the Ligament's path is displayed in the visualizer. You do not have 
    to invoke the base class ("Super") method, just replace it completely. This
    method will be invoked during realizeDynamics() so the supplied a state has 
    already been realized through Stage::Velocity and you can access time, 
    position, and velocity dependent quantities. You must not attempt to 
    realize the passed-in \a state any further since we are already in the 
    middle of realizing here. Return SimTK::Vec3(SimTK::NaN) if you want to 
    leave the color unchanged (that's what the base class implementation does).

    @param[in] state    
        A SimTK::State already realized through Stage::Velocity. Do not 
        attempt to realize it any further.
    @returns 
        The desired color for the path as an RGB vector with each
        component ranging from 0 to 1, or NaN to indicate that the color
        should not be changed. **/
    virtual SimTK::Vec3 computePathColor(const SimTK::State& state) const;

    // Implement ModelComponent interface.
    /** Extension of parent class method; derived classes may extend further. **/
	void connectToModel(Model& aModel) OVERRIDE_11;
    /** Extension of parent class method; derived classes may extend further. **/
	void addToSystem(SimTK::MultibodySystem& system) const OVERRIDE_11;
    /** Extension of parent class method; derived classes may extend further. **/
    void realizeDynamics(const SimTK::State& state) const OVERRIDE_11;

	


private:
	void setNull();
	//void constructProperties();
	void setupProperties();

	
//=============================================================================
};	// END of class UWLigament
//=============================================================================
//=============================================================================

} // end of namespace OpenSim

#endif // _OPENSIM_UWLIGAMENT_PLUGIN_H_


