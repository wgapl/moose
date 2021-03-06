/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "NSSUPGMass.h"

template<>
InputParameters validParams<NSSUPGMass>()
{
  // Initialize the params object from the base class
  InputParameters params = validParams<NSSUPGBase>();

  return params;
}



NSSUPGMass::NSSUPGMass(const InputParameters & parameters)
    : NSSUPGBase(parameters)
{
}



Real NSSUPGMass::computeQpResidual()
{
  // From "Component SUPG contributions" section of the notes,
  // the mass equation is stabilized by taum and the gradient of
  // phi_i dotted with the momentum equation strong residuals.
  // Note that the momentum equation strong residuals are stored
  // in entries 1,2,3 of the "_strong_residuals" vector, regardless
  // of what dimension we're solving in.
  RealVectorValue Ru (_strong_residuals[_qp][1],
                      _strong_residuals[_qp][2],
                      _strong_residuals[_qp][3]);

  // Separate variable just for printing purposes...
  Real result = _taum[_qp] * (Ru * _grad_test[_i][_qp]);

  // Debugging: Print results only if they are nonzero
//  if (std::abs(result) > 0.)
//    Moose::out << "SUPGMass[" << _qp << "]=" << result << std::endl;

  return result;
}


Real NSSUPGMass::computeQpJacobian()
{
  // This is the density equation, so pass the on-diagonal variable number
  return this->compute_jacobian(_rho_var_number);
}


Real NSSUPGMass::computeQpOffDiagJacobian(unsigned int jvar)
{
  return this->compute_jacobian(jvar);
}



Real NSSUPGMass::compute_jacobian(unsigned var)
{
  // Convert the Moose numbering to canonical NS variable numbering.
  unsigned m = this->map_var_number(var);

  // Time derivative contributions only for momentum
  Real time_part = 0.;

  // The derivative of "udot" wrt u for each of the momentum variables.
  // This is always 1/dt unless you are using BDF2...
  Real d_udot_du[3] = {_d_rhoudot_du[_qp], _d_rhovdot_du[_qp], _d_rhowdot_du[_qp]};

  switch ( m )
  {
  case 1:
  case 2:
  case 3:
    // time_part = _grad_test[_i][_qp](m-1) * (_phi[_j][_qp]/_subproblem.parent()->dt());
    time_part = _grad_test[_i][_qp](m-1) * (_phi[_j][_qp] * d_udot_du[m-1]);
    break;
  }

  // Debugging
  // if (time_part != 0.0)
  //   Moose::out << "time_part=" << time_part << std::endl;

  // Store result so we can print it before returning
  Real result = _taum[_qp] * (time_part + _grad_test[_i][_qp] * (_calA[_qp][m] * _grad_phi[_j][_qp]));

  // Debugging
  // if (std::abs(result) > 0.0)
  //   Moose::out << "SUPG mass jacobian[qp=" << _qp << "] = " << result << std::endl;

  return result;
}

