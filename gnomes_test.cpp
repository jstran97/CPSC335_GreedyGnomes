///////////////////////////////////////////////////////////////////////////////
// subsequence_test.cpp
//
// Unit tests for gnomes_algs.hpp
//
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <random>

#include "rubrictest.hpp"

#include "gnomes_types.hpp"
#include "gnomes_algs.hpp"

int main() {

  Rubric rubric;

  const gnomes::step_direction R = gnomes::STEP_DIRECTION_RIGHT,
                               D = gnomes::STEP_DIRECTION_DOWN;

  gnomes::grid empty2(2, 2), empty4(4, 4);
  gnomes::path empty2_solution(empty2), empty4_solution(empty4);

  gnomes::grid horizontal(4, 4);
  horizontal.set(0, 3, gnomes::CELL_GOLD);
  gnomes::path horizontal_solution(horizontal, {R, R, R});

  gnomes::grid vertical(4, 4);
  vertical.set(3, 0, gnomes::CELL_GOLD);
  gnomes::path vertical_solution(vertical, {D, D, D});

  gnomes::grid all_gold(4, 4);
  for (gnomes::coordinate r = 0; r < 4; ++r) {
    for (gnomes::coordinate c = 0; c < 4; ++c) {
      if (!(r == 0 && c == 0)) {
        all_gold.set(r, c, gnomes::CELL_GOLD);
      }
    }
  }

  gnomes::grid maze(4, 4);
  /*    ..XX
        X..X
        XX..
        XXXG   */
  maze.set(0, 2, gnomes::CELL_ROCK);
  maze.set(0, 3, gnomes::CELL_ROCK);
  maze.set(1, 0, gnomes::CELL_ROCK);
  maze.set(1, 3, gnomes::CELL_ROCK);
  maze.set(2, 0, gnomes::CELL_ROCK);
  maze.set(2, 1, gnomes::CELL_ROCK);
  maze.set(3, 0, gnomes::CELL_ROCK);
  maze.set(3, 1, gnomes::CELL_ROCK);
  maze.set(3, 2, gnomes::CELL_ROCK);
  maze.set(3, 3, gnomes::CELL_GOLD);

  gnomes::path maze_solution(maze, {R, D, R, D, R, D});

  std::mt19937 gen;
  gnomes::grid small_random =  gnomes::grid::random(4, 5, 6, 4, gen),
               medium_random = gnomes::grid::random(12, 24, 20, 20, gen),
               large_random =  gnomes::grid::random(20, 79, 30, 70, gen);

  rubric.criterion("exhaustive search - simple cases", 4,
		   [&]() {
         TEST_EQUAL("empty2", empty2_solution, greedy_gnomes_exhaustive(empty2));
         TEST_EQUAL("empty4", empty4_solution, greedy_gnomes_exhaustive(empty4));
         TEST_EQUAL("horizontal", horizontal_solution, greedy_gnomes_exhaustive(horizontal));
         TEST_EQUAL("vertical", vertical_solution, greedy_gnomes_exhaustive(vertical));
         auto output = greedy_gnomes_exhaustive(all_gold);
         TEST_EQUAL("all_gold path length", 7, output.steps().size());
         TEST_EQUAL("all_gold total gold", 6, output.total_gold());
		   });

  rubric.criterion("exhaustive search - maze", 1,
		   [&]() {
         TEST_EQUAL("correct", maze_solution, greedy_gnomes_exhaustive(maze));
		   });

  rubric.criterion("dynamic programming - simple cases", 4,
		   [&]() {
         TEST_EQUAL("empty2", empty2_solution, greedy_gnomes_dyn_prog(empty2));
         TEST_EQUAL("empty4", empty4_solution, greedy_gnomes_dyn_prog(empty4));
         TEST_EQUAL("horizontal", horizontal_solution, greedy_gnomes_dyn_prog(horizontal));
         TEST_EQUAL("vertical", vertical_solution, greedy_gnomes_dyn_prog(vertical));
         auto output = greedy_gnomes_dyn_prog(all_gold);
         TEST_EQUAL("all_gold path length", 7, output.steps().size());
         TEST_EQUAL("all_gold total gold", 6, output.total_gold());
		   });

  rubric.criterion("dynamic programming - maze", 1,
		   [&]() {
         TEST_EQUAL("correct", maze_solution, greedy_gnomes_dyn_prog(maze));
		   });

  rubric.criterion("dynamic programming - random instances", 1,
		   [&]() {
         std::cout << std::endl;

         auto small_output = greedy_gnomes_dyn_prog(small_random);
         TEST_EQUAL("small", 3, small_output.total_gold());

         auto medium_output = greedy_gnomes_dyn_prog(medium_random);
         TEST_EQUAL("medium", 7, medium_output.total_gold());

         auto large_output = greedy_gnomes_dyn_prog(large_random);
         TEST_EQUAL("large", 9, large_output.total_gold());
		   });

  rubric.criterion("stress test", 2,
		   [&]() {
         const gnomes::coordinate ROWS = 5,
                                  MAX_COLUMNS = 15;
         const unsigned SEED = 20181130;

         std::mt19937 gen(SEED);

         for (gnomes::coordinate columns = 1; columns <= MAX_COLUMNS; ++columns) {
           auto area = ROWS * columns,
                gold = area / 5,
                rocks = area / 10;
           gnomes::grid setting = gnomes::grid::random(ROWS, columns, gold, rocks, gen);
           TEST_EQUAL("random grid with " + std::to_string(columns) + " columns",
                      gnomes::greedy_gnomes_exhaustive(setting).total_gold(),
                      gnomes::greedy_gnomes_dyn_prog(setting).total_gold());
         }
		   });

  return rubric.run();
	
}
