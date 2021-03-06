[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 32
  ny = 32
  xmax = 16
  ymax = 16
  distribution = serial
[]

[Variables]
  [./u]
  [../]
[]

[AuxVariables]
  [./sub]
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = u
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = u
    boundary = left
    value = 0
  [../]
  [./right]
    type = DirichletBC
    variable = u
    boundary = right
    value = 1
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 1
  dt = 0.3

  # Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]

[MultiApps]
  [./sub]
    type = TransientMultiApp
    app_type = MooseTestApp
    positions = '4 4 0 8 8 0'
    input_files = multilevel_sub.i
  [../]
[]

[Transfers]
  [./from_sub]
    type = MultiAppDTKInterpolationTransfer
    direction = from_multiapp
    multi_app = sub
    source_variable = subsub
    variable = sub
  [../]
[]
