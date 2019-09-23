///////////////////////////////////////////////////////////////////////////////
// gnomes_algs.hpp
//
// Algorithms that solve the greedy gnomes problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on gnomes_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <vector>

#include "gnomes_types.hpp"






namespace gnomes {

  struct Path_Status {
    path candidate_path; // Stores object of class "path"
    bool is_None; // True if path does not exist; otherwise, false

    
    Path_Status(const grid& setting)
      :candidate_path(setting), is_None(true) {}
    
    // Path_Ptr(const grid& setting) {
    //   candidate_path(setting);
    // }

  };

  // Solve the greedy gnomes problem for the given grid (which is called "setting"
  // in this case), using an exhaustive search algorithm.
  //
  // This algorithm is expected to run in exponential time, so the grid's
  // width+height must be small enough to fit in a 64-bit int; this is enforced
  // with an assertion.
  //
  // The grid must be non-empty.
  path greedy_gnomes_exhaustive(const grid& setting) {

    // grid must be non-empty.
    assert(setting.rows() > 0);
    assert(setting.columns() > 0);

    // Compute maximum path length, and check that it is legal (maxlen = r + c - 2)
    const size_t max_steps = setting.rows() + setting.columns() - 2;
    assert(max_steps < 64);

    // TODO: implement the exhaustive search algorithm, then delete this
    // comment.
    path best(setting); // best = None

    bool bit = 0; 

    //std::vector<size_t> stack(max_steps+1, 0); // allocate space for subset and initialize values to 0

    for (size_t len = 0; len <= max_steps; len++) {
      for (size_t bits = 0; bits <= (pow(2.0, len)-1); bits++) {
        path candidate(setting); // candidate solution with an empty path with only 1 step

        for (size_t k = 0; k <= len-1; k++) {

          // Generates candidate solution using bitwise operation
          bit = (bits >> k) & 1;
          if (bit == 1) {
            // Checks if step to right is valid
            if (candidate.is_step_valid(STEP_DIRECTION_RIGHT)) {
              // Moves to the right
              candidate.add_step(STEP_DIRECTION_RIGHT);
            }
            else {
              // Invalid move, invalid path
              break;
            }
          }
          else {
            // Checks if step downward is valid
            if (candidate.is_step_valid(STEP_DIRECTION_DOWN)) {
              // Moves down
              candidate.add_step(STEP_DIRECTION_DOWN);
            }
            else {
              // Invalid move, invalid path
              break;
            }
          }
        } // End of innermost for-loop


        // if candidate stays inside the grid and never crosses a CELL_ROCK (X)
        // (NOTE: this already got taken cared of by the is_step_valid(STEP_DIRECTION__)
        // after the bitwise operation)
          // if best = None or if candidate harvests more gold than best
          if (best.steps().size() == 1 || 
              candidate.total_gold() > best.total_gold())  {
                best = candidate;
              } 
        
      } // End of 2nd for-loop
    }

    return best;
  }



  // Solve the greedy gnomes problem for the given grid, using a dynamic
  // programming algorithm.
  //
  // The grid must be non-empty.
  path greedy_gnomes_dyn_prog(const grid& setting) {

    // grid must be non-empty.
    assert(setting.rows() > 0);
    assert(setting.columns() > 0);

    // TODO: implement the dynamic programming algorithm, then delete this
    // comment.
    path best(setting);

    // row x column matrix for storing paths
    // Path_Status arr[setting.rows()][setting.columns()];


    // FOR 2D ARRAY
    /*
    for (size_t row = 0; row < setting.rows(); ++row) {
      for (size_t col = 0; col < setting.columns(); ++col) {
        arr[row][col].candidate
      }
    }
    */

    // 2D vector of data type Path_Status 
     std::vector<std::vector<Path_Status>> vec(setting.rows(), 
        std::vector<Path_Status>(setting.columns(), Path_Status(setting)));

  //  std::vector<size_t> vec_ptrs(max_steps+1, 0); // allocate space for subset and initialize values to 0

    // Base case
    vec[0][0].candidate_path.add_step(STEP_DIRECTION_START); 
    //arr[0][0].candidate_path.add_step(STEP_DIRECTION_START); 

    // General case
    for (size_t i = 0; i < setting.rows(); ++i) {
      for (size_t j = 0; j < setting.columns(); ++j) {

        // if rock (X) is not encountered
        if (!setting.may_step(i, j)) {
          // Continue moving
          vec[i][j].is_None = true;
          continue;          
        }
        //Path_Status from_above = Path_Status(setting);
        //Path_Status from_left = Path_Status(setting);
        path from_above(setting);
        path from_left(setting);


        // When we are not on the top row, move down
        if (i > 0 && vec[i - 1][j].is_None == false) {
          // Move down
          from_above = vec[i - 1][j].candidate_path.add_step(STEP_DIRECTION_DOWN);
        }
        
        // When we are not at the leftmost column, move right
        if (j > 0 && vec[i][j - 1].is_None == false) {
          // Move right
          from_left = vec[i][j - 1].candidate_path.add_step(STEP_DIRECTION_RIGHT);
        }

      }
    }

    // Post-processing step

    return best;
  }

}