/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   https://www.lammps.org/, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

#include "fix_wall_harmonic.h"
#include "atom.h"
#include "error.h"

using namespace LAMMPS_NS;
using namespace FixConst;

/* ---------------------------------------------------------------------- */

FixWallHarmonic::FixWallHarmonic(LAMMPS *lmp, int narg, char **arg) : FixWall(lmp, narg, arg)
{
  dynamic_group_allow = 1;
}

/* ----------------------------------------------------------------------
   interaction of all particles in group with a wall
   m = index of wall coeffs
   which = xlo,xhi,ylo,yhi,zlo,zhi (0,1,..,5)
   error if any particle is on or behind wall
   dim = 0,1,2 (x,y,z)

------------------------------------------------------------------------- */
/* NEW ONE */

void FixWallHarmonic::wall_particle(int m, int which, double coord)
{
  double dr, fwall;
  double vn;

  double **x = atom->x;
  double **f = atom->f;
  int *mask = atom->mask;
  int nlocal = atom->nlocal;

  /* The dim is the int part, so if its zhi (which=5) then dim = 2 ==> Z coordinate */
  int dim = which / 2;
  int side = which % 2;
  if (side == 0) side = -1;

  for (int i = 0; i < nlocal; i++)
    if (mask[i] & groupbit) {
      if (side < 0)
        dr = coord - x[i][dim];
      else
        dr = x[i][dim] - coord;
      if (dr >= cutoff[m]) continue;
      if (dr <= 0.0) {
        /* No force if the particle is inside the control volume */
        continue;
      }
      fwall = side * 2.0 * epsilon[m] * dr;
      f[i][dim] -= fwall;
      ewall[0] += epsilon[m] * dr * dr;
      ewall[m + 1] += fwall;

      if (evflag) {
        if (side < 0)
          vn = -fwall * dr;
        else
          vn = fwall * dr;
        v_tally(dim, i, vn);
      }
    }

}
