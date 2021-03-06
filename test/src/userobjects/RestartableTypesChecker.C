/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "RestartableTypesChecker.h"

template<>
InputParameters validParams<RestartableTypesChecker>()
{
  InputParameters params = validParams<RestartableTypes>();
  return params;
}


RestartableTypesChecker::RestartableTypesChecker(const InputParameters & parameters) :
    RestartableTypes(parameters)
{
}

RestartableTypesChecker::~RestartableTypesChecker()
{
}

void RestartableTypesChecker::initialSetup()
{
}

void RestartableTypesChecker::timestepSetup()
{
}

void
RestartableTypesChecker::execute()
{
  if (_real_data != 3)
    mooseError("Error reading restartable Real expected 3 got " << _real_data);

  if (_vector_data.size() != 4)
    mooseError("Error reading restartable std::vector<Real> expected size 4 got " << _vector_data.size());

  for (unsigned int i=0; i<_vector_data.size(); i++)
    if (_vector_data[i] != 3)
      mooseError("Error reading restartable std::vector<Real> expected 3 got " << _vector_data[i]);

  if (_vector_vector_data.size() != 4)
    mooseError("Error reading restartable std::vector<std::vector<Real> > expected size 4 got " << _vector_data.size());

  for (unsigned int i=0; i<_vector_vector_data.size(); i++)
  {
    for (unsigned int j=0; j<_vector_vector_data[i].size(); j++)
      if (_vector_vector_data[i][j] != 3)
        mooseError("Error reading restartable std::vector<std::vector<Real> > expected 3 got " << _vector_vector_data[i][j]);
  }

  if (_pointer_data->_i != 3)
    mooseError("Error reading restartable pointer data!");

  if (_custom_data._i != 3)
    mooseError("Error reading restartable custom data!");

  if (_custom_with_context._i != 3)
    mooseError("Error reading restartable custom data with context!");

  if (_set_data.size() != 2)
    mooseError("Error reading restartable std::set expected size 2 got " << _set_data.size());

  for (std::set<Real>::iterator it = _set_data.begin(); it != _set_data.end(); ++it)
    if (*it != 1 && *it != 2)
      mooseError("Error reading restartable set data!");

  if (_map_data.size() != 2)
    mooseError("Error reading restartable std::map expected size 2 got " << _map_data.size());

  if (_map_data[1] != 2.2)
    mooseError("Error reading restartable map data!");

  if (_map_data[2] != 3.4)
    mooseError("Error reading restartable map data!");

  if (_dense_vector_data.size() != 3)
    mooseError("Error reading restartable DenseVector size");
  for (unsigned int i = 0; i < _dense_vector_data.size(); i++)
    if (_dense_vector_data(i) != static_cast<Real>(i+1))
      mooseError("Error reading restartable DenseMatrix data!");

  if (_dense_matrix_data.m() != 2 || _dense_matrix_data.n() != 3)
    mooseError("Error reading restartable DenseMatrix size");
  for (unsigned int i = 0; i < _dense_matrix_data.m(); i++)
    for (unsigned int j = 0; j < _dense_matrix_data.n(); j++)
      if (_dense_matrix_data(i,j) != static_cast<Real>(i + j + 1))
        mooseError("Error reading restartable DenseMatrix data!");
}
